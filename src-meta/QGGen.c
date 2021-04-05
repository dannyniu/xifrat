/* DannyNiu/NJF, 2021-03-19. Public Domain. */
// QGGen - quasigroup generator for "restricted-commutative latin-squares".

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROG_PRINTF(...) fprintf(stderr, __VA_ARGS__)

#define USEFORK 1
#define FORK_TRSHD 4
#define CHLD_LIMIT 2

// roughly c*2^{fork_index} processes in parallel, some are sleeping.
int exit_trshd = 0;
int fork_index = 3;
int chld_count = 0;

#define P 16
#define P2 (P*P)

// convention-01: cell indicies use ``int'', cell values ``short''.
// convention-02: sets to -1, tests non-negativeness.

// row-major.
short SBox[P2];
#define sbox(a,b) (SBox[a*P+b])

struct { int a, b; } walk[P2];

struct pair { int u, v; };
struct pair pairs[P2*P2];
int pairs_stops[P2];
int pairs_stacktop;

struct rel { int cells[P]; };
struct rel relations[P];

struct shuffled { int i; short nibs[P]; };
struct shuffled shuffles[P2];

#undef P
#undef P2

int P = 16;
int P2 = 256;
int prog = 0;

void algo_init();
int pair_add(int u, int v);
void relations_recalculate();
int constrain_verify(short x, int a, int b);

void algo_init()
{
    int i, j;
    
    for(i=0; i<P2; i++)
    {
        SBox[i] = -1;
        walk[i].a = i / P;
        walk[i].b = i % P;
    }
    
    for(i=0; i<P2*P2; i++)
        pairs[i].u = pairs[i].v = -1;

    for(i=0; i<P2; i++) pairs_stops[i] = 0;
    pairs_stacktop = 0;
    
    for(i=0; i<P; i++)
    {
        for(j=0; j<P; j++)
            relations[i].cells[j] = -1;
    }

    for(i=0; i<P2; i++)
    {
        shuffles[i].i = 0;
        for(j=0; j<P; j++)
            shuffles[i].nibs[j] = j; // randomize this later.
    }
}

// The following functions (
//    pair_add,
//    relations_recalculate,
//    constrain_verify): 
// returns true if constrains are added successfully.
// returns false on constrain violations.

int pair_add(int u, int v)
{
    int i, j, k;
    int w;

    // 0. canonicalize the set {u,v}.
    if( u == v ) return true;
    if( u < 0 || v < 0 ) return true;
    if( u > v )
    {
        w = u;
        u = v;
        v = w;
    }
    
    // 1. check for existing conflict.
    // 1.1. the sbox.
    i = SBox[u];
    j = SBox[v];
    if( i >= 0 && j >= 0 && i != j ) return false;

    // 1.2. the relations.
    w = -1;
    
    for(i=0; i<P; i++)
    {
        for(j=0; j<P; j++)
            if( relations[i].cells[j] == u )
            {
                w = i;
                i = P; // prepare to ``break 2;''. P+1 shouldn't overflow.
                break;
            }
    }
    
    for(i=0; i<P; i++)
    {
        for(j=0; j<P; j++)
            if( relations[i].cells[j] == v )
            {
                if( w >= 0 && i != w )
                    return false;
                else if( w < 0 )
                {
                    w = i;
                    i = P; // see above "prepare to ...".
                    break;
                }
            }
    }

    // 2. if no existing relation, check if some cell has value.
    if( w < 0 ) w = SBox[u];
    if( w < 0 ) w = SBox[v];

    // 3. add relation if one can be determined.
    if( w >= 0 )
    {
        for(i=0; i<P; i++)
        {
            if( relations[w].cells[i] == u )
                break;
            else if( relations[w].cells[i] < 0 )
            {
                relations[w].cells[i] = u;
                break;
            }
        }
        
        for(i=0; i<P; i++)
        {
            if( relations[w].cells[i] == v )
                break;
            else if( relations[w].cells[i] < 0 )
            {
                relations[w].cells[i] = v;
                break;
            }
        }

        if( i >= P ) return false; // 1 relation with too many elements!

        // 2021-03-26:
        // Trying out whether aggregating unadded pairs improves performance.
        for(k=0; k<P; k++)
        {
            for(j=0; j<pairs_stops[pairs_stacktop]; j++)
                if( relations[w].cells[k] == pairs[j].u ||
                    relations[w].cells[k] == pairs[j].v )
                {
                    for(i=0; i<P; i++)
                    {
                        if( relations[w].cells[i] == pairs[j].u )
                            break;
                        else if( relations[w].cells[i] < 0 )
                        {
                            relations[w].cells[i] = pairs[j].u;
                            break;
                        }
                    }
                    
                    for(i=0; i<P; i++)
                    {
                        if( relations[w].cells[i] == pairs[j].v )
                            break;
                        else if( relations[w].cells[i] < 0 )
                        {
                            relations[w].cells[i] = pairs[j].v;
                            break;
                        }
                    }
                    
                    if( i >= P ) return false; // see above.
                }
        }

        // 2021-03-21:
        // the following constraint spawner hampered the performance for
        // some reason and it's disabled.
        // for(i=0; i<P; i++)
        // {
        //     int a, b;
        //     a = relations[w].cells[i] / P;
        //     b = relations[w].cells[i] % P;
        // 
        //     if( SBox[relations[w].cells[i]] == w )
        //         return true;
        //     if( !constrain_verify(w, a, b) )
        //         return false;
        // }
    }

    // 4. add the pair {u,v} lastly regardless.
    for(i=0; i<pairs_stops[pairs_stacktop]; i++)
    {
        if( pairs[i].u == u && pairs[i].v == v )
            break;
    }

    if( i >= pairs_stops[pairs_stacktop] )
    {
        pairs[i].u = u;
        pairs[i].v = v;
        pairs_stops[pairs_stacktop]++;
    }

    return true;
}

void relations_recalculate()
{
    int i, j;

    // 1. reset all relations.    
    for(i=0; i<P; i++)
    {
        for(j=0; j<P; j++)
            relations[i].cells[j] = -1;
    }

    for(i=0; i<P2*P2; i++)
        pair_add(pairs[i].u, pairs[i].v);
}

int constrain_verify(short x, int a, int b)
{
    int c, d;
    int i, j;
    
    //- if( x < 0 || x >= P ) return false; // pointless overhead?
    
    if( sbox(a,b) >= 0 && sbox(a,b) != x )
        return false;

    // added on 2021-04-01: avoid fixed-points.
    if( a == b && a == x )
        return false;

    // code path optimization.
    if( sbox(a,b) == x )
        return true;

    // 1. check if x conflicts with...
    // 1.1. ...with an existing equality equation set.
    c = a*P+b;
    for(i=0; i<P; i++)
    {
        for(j=0; j<P; j++)
            if( relations[i].cells[j] == c && i != x )
                return false;
    }

    // 1.2. ...with a row or a column;
    for(i=0; i<P; i++)
    {
        if( x == sbox(a,i) || x == sbox(i,b) )
            return false;
    }

    // # Before this point, return false on failure,
    // # After this point, ``goto violated;'' on failure.

    // if check passes, set the next empty cell to x.
    sbox(a,b) = x;

    // 2. add constraints.
    for(c=0; c<P; c++)
    {
        for(d=0; d<P; d++)
        {
            int g, h, u, v;

            u = v = -1;
            
            g = sbox(a,b);
            h = sbox(c,d);
            if( g >= 0 && h >= 0 ) u = g * P + h;

            g = sbox(a,c);
            h = sbox(b,d);
            if( g >= 0 && h >= 0 ) v = g * P + h;

            if( u >= 0 && v >= 0 )
                if( !pair_add(u, v) )
                    goto violated;
            
            u = v = -1;
            
            g = sbox(c,d);
            h = sbox(a,b);
            if( g >= 0 && h >= 0 ) u = g * P + h;

            g = sbox(c,a);
            h = sbox(d,b);
            if( g >= 0 && h >= 0 ) v = g * P + h;

            if( u >= 0 && v >= 0 )
                if( !pair_add(u, v) )
                    goto violated;
        }
    }

    // finally. one cell is set, complete and return.
    //- fulfilled:; // commented-out to silence the unused label warning.
    do // progress reporting.
    {
        if( ++prog < P2*P ) break;
        prog = 0;
        PROG_PRINTF("\n-- current progress: --\n");
        
        PROG_PRINTF("sbox: \n");
        for(i=0; i<P2; i++)
        {
            PROG_PRINTF("% 3d%c", SBox[i], P-i%P==1 ? '\n' : ' ');
        }

        PROG_PRINTF("shuffles: \n");
        for(i=0; i<P2; i++)
        {
            PROG_PRINTF("% 3d%c", shuffles[i].i, P-i%P==1 ? '\n' : ' ');
        }

        PROG_PRINTF("relations: \n");
        for(i=0; i<P; i++)
        {
            for(j=0; j<P; j++)
            {
                PROG_PRINTF("% 4d", relations[i].cells[j]);
            }
            PROG_PRINTF("\n");
        }

        /*PROG_PRINTF("stack frames: \n");
        for(i=0; i<P2; i++)
        {
            PROG_PRINTF("% 3d%c", pairs_stops[i], P-i%P==1 ? '\n' : ' ');
            }*/

        fflush(stderr);
    }
    while( false );
    return true;
    
violated:;
    return false;
}

int sbox_fill()
{
    int a, b;
    int subret;
    int i, j;
    short x;
    
    struct shuffled *shuffle;

    a = 0;
    b = 0;

iterate:;
    shuffle = shuffles + pairs_stacktop;
    
    // a = pairs_stacktop / P;
    // b = pairs_stacktop % P;
    a = walk[pairs_stacktop].a;
    b = walk[pairs_stacktop].b;

    // search for an existing relation first.
    x = -1;
    if( shuffle->i >= P || shuffle->i < 0 ) goto retreat;
    for(i=0; i<P; i++)
    {
        for(j=0; j<P; j++)
            if( relations[i].cells[j] == a*P+b )
            {
                x = i;
                shuffle->i = P; // assumes P+1 doesn't overflow.
                i = P; // prepare to ``break 2;''. P+1 shouldn't overflow.
                break;
            }
    }
    if( x < 0 ) x = shuffle->nibs[shuffle->i];

#if USEFORK
    if( shuffle->i >= FORK_TRSHD && shuffle->i < P && fork_index > 0 )
    {
        int waited;
        pid_t pid;

        while( chld_count >= CHLD_LIMIT )
        {
            pid = wait(&waited);
            if( pid > 0 )
            {
                if( WEXITSTATUS(waited) == EXIT_SUCCESS )
                    exit(EXIT_SUCCESS);
                chld_count--;
            }
            else if( errno == ECHILD )
            {
                chld_count = 0;
                errno = 0;
            }
        }

        pid = fork();
        if( pid < 0 )
        {
            // fork failed for some reason I can't care for,
            // so disable further forkings in this process subtree.
            fork_index = 0;
            goto nofork;
        }
        else if( pid > 0 ) chld_count++;
        else if( pid == 0 )
        {
            // setup fork recursion variables
            fork_index--;
            chld_count = 0;

            // setup "sub-process exit conditions"
            exit_trshd = pairs_stacktop;
            shuffle->i -= P*2;

            // computation body
            subret = constrain_verify(x, a, b);
            goto subroutine_returned;
        }
            
        shuffle->i++;
        if( shuffle->i < P ) x = shuffle->nibs[shuffle->i];

        goto iterate;
    }
    else
    {
    nofork:
#endif /* USEFORK */
        subret = constrain_verify(x, a, b);
        shuffle->i++;
#if USEFORK
    }
subroutine_returned:
#endif /* USEFORK */

    if( !subret )
    {
    retreat:
        sbox(a,b) = -1;

        // restore 'stack stop'.
        if( pairs_stacktop )
            pairs_stops[pairs_stacktop] = pairs_stops[pairs_stacktop-1];
        else pairs_stops[0] = 0;

        for(i=pairs_stops[pairs_stacktop]; i<P2*P2; i++)
            pairs[i].u = pairs[i].v = -1;
    
        relations_recalculate();

        // The second one occurs only as
        // "sub-process exit condition".
        // See above.
        if( shuffle->i >= P || shuffle->i < 0 )
        {
            shuffle->i = 0;
            pairs_stops[pairs_stacktop] = 0;

            if( --pairs_stacktop < exit_trshd ) return false;
            else goto retreat;
        }
        goto iterate;
    }
    else
    {
        if( ++pairs_stacktop < P2 )
        {
            // setup 'stack stop'.
            if( pairs_stacktop )
                pairs_stops[pairs_stacktop] = pairs_stops[pairs_stacktop-1];
            else pairs_stops[0] = 0;
            
            goto iterate;
        }
        else return true;
    }
}

#include "../src-crypto/endian.h"
#include "../src-crypto/shake.h"

shake256_t xof;

void shuffle1(short nibs[P], uint64_t s)
{
    int i;
    for(i=0; i<P; i++) nibs[i] = i;

    for(i=P; i>1; i--)
    {
        int p = s % i;
        int j = P - i;
        short t = nibs[p+j];
        nibs[p+j] = nibs[j];
        nibs[j] = t;
        s /= i;
    }
}

void shufflev(char *seed)
{
    uint64_t w, card, mmax;
    int i;

    card = 1;
    for(i=P; i>1; i--) card *= i;
    mmax = UINT64_MAX / card;
    mmax *= card;
    
    SHAKE256_Init(&xof);
    SHAKE_Write(&xof, seed, strlen(seed));
    SHAKE_Final(&xof);

    for(i=0; i<P2; i++)
    {
        w = mmax;
        while( w >= mmax )
        {
            SHAKE_Read(&xof, &w, sizeof(w));
            w = le64toh(w);
        }

        w %= card;
        shuffle1(shuffles[i].nibs, w);
    }
}

#if 0
void walk_init() // border-walk.
{
    int i, j, k;

    k = 0;
    for(i=0; i<P; i++)
    {
        for(j=i; j<P; j++)
        {
            walk[k].a = i;
            walk[k].b = j;
            k++;
        }
        
        for(j=i+1; j<P; j++)
        {
            walk[k].a = j;
            walk[k].b = i;
            k++;
        }
    }
}
#else
void walk_init() // diagonal walk.
{
    int i, j, k;

    k = 0;
    for(i=0; i<P; i++)
    {
        for(j=0; j<=i; j++)
        {
            walk[k].a = j;
            walk[k].b = i-j;
            k++;
        }
    }
    for(i=1; i<P; i++)
    {
        for(j=i; j<P; j++)
        {
            walk[k].a = j;
            walk[k].b = P+i-j-1;
            k++;
        }
    }
}
#endif

int main(int argc, char *argv[])
{
    int i=0;
    int shuffle = 1;

    for(i=1; i<argc; i++)
    {
        if( argv[i][0] != '-' ) break;
        if( !strcmp("--noshuffle", argv[i]) ) shuffle = 0;
    }

    if( argv[i] ) P = atoi(argv[i]);
    if( P <= 0 || P > 16 )
    {
        PROG_PRINTF("Unsupported dimension: %d\n", P);
        return 1;
    }
    else P2 = P*P;

    algo_init();

    if( shuffle )
    {
        shufflev("xifrat - public-key cryptosystem");
        // tried a few different seeds, but
        // still failing to avoid fixed points nonetheless.
    }
    walk_init();

    setvbuf(stdout, NULL, _IOLBF, 4096);
    setvbuf(stderr, NULL, _IOFBF, 4096);
    
    i = sbox_fill();
    if( exit_trshd > 0 && !i )
        exit(EXIT_FAILURE);
    
    printf("sbox_fill() returned: %s\n", i ? "true" : "false");

    printf("\n");
    printf("quasigroup sbox: \n");
    for(i=0; i<P2; i++)
    {
        printf("% 3d%c", SBox[i], P-i%P==1 ? '\n' : ' ');
    }

    printf("\n");
    printf("indicies of freedom: \n");
    for(i=0; i<P2; i++)
    {
        if( shuffles[i].i < 0 ) shuffles[i].i += P*2;
        printf("% 3d%c", shuffles[i].i, P-i%P==1 ? '\n' : ' ');
    }

    printf("\n");
    return EXIT_SUCCESS;
}
