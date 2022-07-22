/* DannyNiu/NJF, 2022-04-01. Public Domain. */

#include "../src-crypto/endian.h"
#include "../src-crypto/shake.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

shake128_t prng_ctx;
#define prng(buf,len) SHAKE_Read(&prng_ctx, buf, len)

#define P_MAX 64
#define P2_MAX (P_MAX * P_MAX)

// row-major.
typedef int16_t sbox_t[P2_MAX];

typedef struct {
    // Set to the count of 0s in vec.
    int16_t left;

    // The set of occupied and available cell values.
    int16_t vec[P_MAX];
} cellopts_t;

void cellopts_init(cellopts_t *co, int16_t p)
{
    co->left = p;
    for(int16_t i=0; i<P_MAX; i++)
        co->vec[i] = 0;
}

int16_t cellopts_samp(cellopts_t *co)
{
    if( co->left <= 0 ) return -1;
    uint32_t w;
    int16_t v;

    do
    {
        prng(&w, sizeof w);
        w = le32toh(w);
    }
    while( (UINT32_MAX / co->left) * co->left <= w );

    v = w % co->left--;
    for(int16_t i=0; i<=v; i++)
        v += co->vec[i];
    
    co->vec[v] = 1;
    return v;
}

void cellopts_set(cellopts_t *co, int16_t v)
{
    if( co->vec[v] ) return;
    co->vec[v] = 1;
    co->left--;
}

typedef struct {
    int16_t     p, p2;
    int16_t     sind; // current position in sbox.
    cellopts_t  co, rows[P_MAX];
    sbox_t      sbox;
} state_t;

#if true

int group_test(state_t *state)
{
    int16_t a, b, c, u, v;
    int16_t *t1, *t2, *t3, *t4;
    int16_t p = state->p;

    for(a=0; a<p; a++)
    {   
        for(b=a; b<p; b++)
        {
            for(c=b; c<p; c++)
            {
                // test for associativity.

                u = state->sbox[a*p+b];
                v = state->sbox[b*p+c];
                if( u<0 || v<0 ) continue;

                if( (u == a && c != v) ||
                    (u == v && c != a) ||
                    (c == a && u != v) ||
                    (c == v && u != a) )
                    return false;

                t1 = state->sbox + (u*p+c);
                t2 = state->sbox + (c*p+u);
                t3 = state->sbox + (a*p+v);
                t4 = state->sbox + (v*p+a);

                if( *t1<0 && *t2<0 && *t3<0 && *t4<0 ) continue;
                else if(
                    (*t1>=0 && *t2>=0 && *t1!=*t2) ||
                    (*t1>=0 && *t3>=0 && *t1!=*t3) ||
                    (*t1>=0 && *t4>=0 && *t1!=*t4) ||
                    (*t2>=0 && *t3>=0 && *t2!=*t3) ||
                    (*t2>=0 && *t4>=0 && *t2!=*t4) ||
                    (*t3>=0 && *t4>=0 && *t3!=*t4) )
                {
                    return false;
                }
                
                else if( *t1>=0 ) *t2 = *t3 = *t4 = *t1;
                else if( *t2>=0 ) *t1 = *t3 = *t4 = *t2;
                else if( *t3>=0 ) *t1 = *t2 = *t4 = *t3;
                else if( *t4>=0 ) *t1 = *t2 = *t3 = *t4;
            }
        }
    }

    return true;
}

#else

int group_test(const state_t *state)
{
    int16_t a, b, c, d, u, v, x;
    int16_t p = state->p;

    for(a=0; a<p; a++)
    {   
        for(b=0; b<p; b++)
        {
            for(c=0; c<p; c++)
            {
                // test for associativity.

                u = state->sbox[a*p+b];
                v = state->sbox[b*p+c];
                if( u<0 || v<0 ) continue;
                    
                d = state->sbox[u*p+c];
                if( d<0 ) continue;
                    
                x = state->sbox[a*p+v];
                if( x<0 ) continue;

                if( d != x ) return false;
            }
        }
    }

    return true;
}

#endif

static state_t statestack[P2_MAX + P_MAX];
static state_t *sp = statestack;
static int isp = 0;

void state_try1step(state_t *state, int16_t v)
{
    int16_t p = state->p;
    // int16_t p2 = state->p2;
    int16_t i, j;

    i = state->sind;

    // set current sbox.
    state->sbox[i] = v;
    cellopts_set(state->rows+i/p, v);

    j = i / p;
    i %= p;

    state->sbox[i*p+j] = v;

    // old code related to abelian quasigroup.
    /* for(j=i; j-=p, j>=0 && j<p2; )
       {
       if( state->rows[i/p].vec[state->sbox[j]] )
       continue;
        
       // trace the current column and set current row partially.
       for(int16_t m=i%p, ni=i-m, nj=j-m; m<p; m++)
       {
       if( state->sbox[m+nj] != v )
       continue;

       state->sbox[m+ni] = state->sbox[j];
       cellopts_set(state->rows+i/p, state->sbox[j]);
       break;
       }
       } */
}

void sp_push()
{
    memcpy(sp+1, sp, sizeof(state_t));
    sp++;
    isp++;
}

void sp_pop()
{
    sp--;
    isp--;
}

static uint64_t scnt;

int state_iter(int16_t p)
{
    int16_t i, j;
    
    int16_t p2 = p * p;
    int16_t v;

    sp->p = p;
    sp->p2 = p2;
    sp->sind = 0;

    cellopts_init(&sp->co, p);
    for(i=0; i<p; i++) cellopts_init(sp->rows+i, p);
    for(i=0; i<p2; i++) sp->sbox[i] = -1;
    
    if(0)for(i=0; i<p; i++)
    {
        // 2022-07-21: why?
        sp->sbox[i] = sp->sbox[i*p] = i;
        sp->sbox[i*p+i] = 0;
    }

loop_iter:
    scnt++;
    v = cellopts_samp(&sp->co);
    if( v >= 0 && v < p )
    {
        sp_push();
        state_try1step(sp, v);
        
        if( group_test(sp) )
        {
            cellopts_init(&sp->co, p);
            
            while( sp->sbox[sp->sind] != -1 )
            {
                sp->sind++;
                
                if( sp->sind >= sp->p2 )
                    return true;
            }
            
            i = sp->sind / p;
            j = sp->sind % p;
            for(int16_t t=0; t<p; t++)
            {
                v = sp->sbox[i*p+t];
                if( v >= 0 && v < p ) cellopts_set(&sp->co, v);
                
                v = sp->sbox[t*p+j];
                if( v >= 0 && v < p ) cellopts_set(&sp->co, v);
            }
            
            goto loop_iter;
        }
        else
        {
            sp_pop();
            goto loop_iter;
        }
    }
    else
    {
        if( sp == statestack )
            return false;
        
        sp_pop();
        goto loop_iter;
    }

    printf("Fell Through.\n");
    return false;
}

typedef struct {
    int16_t     p;
    int16_t     aind;
    cellopts_t  co;
    int16_t     map[P_MAX];
} automorph_t;

int automorph_test(const automorph_t *am)
{
    int16_t a, i, j, u, v;
    int16_t p = am->p;
    
    for(i=0; i<p; i++)
    {
        for(j=i; j<p; j++)
        {
            a = sp->sbox[i*p+j];
            a = am->map[a];
            if( a<0 ) continue;

            u = am->map[i];
            v = am->map[j];
            if( u<0 || v<0 ) continue;

            if( sp->sbox[u*p+v] != a ) return false;
        }
    }

    return true;
}

static automorph_t am1, am2, astack[P_MAX];
static automorph_t *ap = astack;
static int iap = 0;

void automorph_try1step(automorph_t *automorph, int16_t v)
{
    // int16_t p = automorph->p;
    int16_t i;

    i = automorph->aind;

    automorph->map[i] = v;
    cellopts_set(&automorph->co, v);
}

void ap_push()
{
    memcpy(ap+1, ap, sizeof(automorph_t));
    ap++;
    iap++;
}

void ap_pop()
{
    ap--;
    iap--;
}

int automorph_iter(int16_t p)
{
    int16_t i;
    int16_t v;

    ap->p = p;
    ap->aind = 0;
    
    cellopts_init(&ap->co, p);
    for(i=0; i<p; i++) ap->map[i] = -1;

loop_iter:
    scnt++;
    v = cellopts_samp(&ap->co);
    if( v >= 0 && v < p )
    {
        ap_push();
        automorph_try1step(ap, v);

        if( automorph_test(ap) )
        {
            cellopts_init(&ap->co, p);
            ap->aind++;
            
            if( ap->aind >= p )
                return true;

            for(i=0; i<p; i++)
            {
                v = ap->map[i];
                if( v >= 0 && v < p ) cellopts_set(&ap->co, v);
            }
            
            goto loop_iter;
        }
        else
        {
            ap_pop();
            
            goto loop_iter;
        }
    }
    else
    {
        if( ap == astack )
            return false;

        ap_pop();
        goto loop_iter;
    }

    printf("Fell Through.\n");
    return false;
}

int16_t QGValue(int16_t a, int16_t b, int16_t c)
{
    int16_t s;
    int16_t p = sp->p;
    
    a = am1.map[a];
    b = am2.map[b];

    if( a<0 || b<0 ) return -1;

    if( (s = sp->sbox[a*p+b]) < 0 ) return -1;
    return   sp->sbox[s*p+c];
}

int QGTest(int16_t *kout)
{
    int16_t p = sp->p;
    int16_t a, b, c, d, u, v, x;
    int16_t k;

    printf("\n");
    for(k=0; k<p; k++)
    {
        for(a=0; a<p; a++)
        {
            if( QGValue(a, a, k) == a )
            {
                printf("a: %d, k: %d.\n", a, k);
                break;
            }
        }
        if( a >= p )
            break;
    }
    if( k >= p )
    {
        // printf("fixed-point!\n");
        // return false;
    }
    
    for(a=0; a<p; a++)
    {
        for(b=0; b<p; b++)
            for(c=0; c<p; c++)
                for(d=0; d<p; d++)
                {
                    u = QGValue(a, b, k);
                    v = QGValue(c, d, k);
                    x = QGValue(u, v, k);

                    u = QGValue(a, c, k);
                    v = QGValue(b, d, k);
                    if( QGValue(u, v, k) != x )
                    {
                        printf("inconsistent!\n");
                        return false;
                    }
                }
    }

    if( kout ) *kout = k;
    return true;
}

#define GP_REGEN_MAX 640
#define AM_REGEN_MAX 480

int main(int argc, char *argv[])
{
    const static char seed[] = "xifrat - public-key cryptosystem";

    int16_t i;
    int16_t p = 8;
    int16_t k;
    long regen_max, regen_max_group;
    int subret = true;

    setvbuf(stdout, NULL, _IONBF, 512);
    SHAKE128_Init(&prng_ctx);
    SHAKE_Write(&prng_ctx, seed, strlen(seed));
    SHAKE_Final(&prng_ctx);

    if( argc > 1 ) p = atoi(argv[1]);

    // group table.

    regen_max_group = GP_REGEN_MAX;
regen_group:
    sp = statestack;
    isp = 0;
    subret = state_iter(p);

    // automorphism map 1.
    regen_max = AM_REGEN_MAX;
regen_automorph1:

    ap = astack;
    iap = 0;

    subret = subret && automorph_iter(p);

    // reject identity map.
    for(i=0; i<p; i++)
        if( ap->map[i] != i )
            break;
    
    if( i >= p )
    {
        if( !regen_max-- ) goto allwrong;
        else goto regen_automorph1;
    }
    
    memcpy(&am1, ap, sizeof(automorph_t));

    // automorphism map 2.
regen_automorph2:

    ap = astack;
    iap = 0;

    subret = subret && automorph_iter(p);

    // reject identity map.
    for(i=0; i<p; i++)
        if( ap->map[i] != i )
            break;
    
    if( i >= p )
    {
        if( !regen_max-- ) goto allwrong;
        else goto regen_automorph2;
    }

    // reject am2 === am1.
    for(i=0; i<p; i++)
        if( ap->map[i] != am1.map[i] )
            break;
    
    if( i >= p )
    {
        if( !regen_max-- ) goto allwrong;
        else goto regen_automorph2;
    }
    
    memcpy(&am2, ap, sizeof(automorph_t));

    if( !QGTest(&k) )
    { allwrong:
        if( !regen_max_group-- ) subret = false;
        else goto regen_group;
        //goto regen_automorph1;
    }

    // status output.
        
    printf("\nGroup Table:\n");
    for(int16_t i=0; i<p; i++)
    {
        for(int16_t j=0; j<p; j++)
        {
            printf("% 3d ", sp->sbox[i*p+j]);
        }
        printf("\n");
    }

    printf("\nAutomorphism Table 1:\n");
    for(int16_t i=0; i<p; i++)
        printf("% 3d ", am1.map[i]);
    printf("\n");

    printf("\nAutomorphism Table 2:\n");
    for(int16_t i=0; i<p; i++)
        printf("% 3d ", am2.map[i]);
    printf("\n");

    printf("\nAbelian Quasigroup Table:\n");
    for(int16_t i=0; i<p; i++)
    {
        for(int16_t j=0; j<p; j++)
        {
            printf("% 3d ", QGValue(i, j, k));
        }
        printf("\n");
    }

    if( subret )
    {
        printf("\nSuccess! scnt:%llu\n", (unsigned long long)scnt);
        return EXIT_SUCCESS;
    }
    else
    {
        printf("\nFailed! scnt:%llu\n", (unsigned long long)scnt);
        return EXIT_FAILURE;
    }
}
