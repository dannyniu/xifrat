/* DannyNiu/NJF, 2021-03-17. Public Domain. */
// Attempts to generate a quadratic restricted-commutative quasigroup,
// and proven that one cannot exist in finite field of order 17. 

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define mod 17
#define ANY 0
#define ALL 1

void long2vec(long s, int *v, int c)
{
    for(int i=0; i<c; i++)
    {
        v[i] = s % mod;
        s /= mod;
    }
}

int f(int a, int b, int c[])
{
    a %= mod;
    b %= mod;
    return (c[0] + c[1]*a + c[2]*b + c[3]*a*a + c[4]*a*b + c[5]*b*b) % mod;
}

int thrdcnt;
int cmax = mod*mod*mod*mod*mod*mod;

void *t(void *argp)
{
    long ind = (long)argp;
    int c[6], x[4], u, v;
    
    for(; ind<cmax; ind+=thrdcnt)
    {
        long s;
        long comm = 0, assoc = 0, fail = 0;
        s = ind;
        long2vec(s, c, 6);
        
        if( (!c[1] && !c[3] && !c[4]) || (!c[2] && !c[4] && !c[5]) )
            continue;
        if( c[1] == c[2] && !c[3] && !c[5] )
            continue;
        if( !c[3] && !c[4] && !c[5] )
            continue;
        
        for(s=0; s<mod*mod*mod*mod; s++)
        {
            long2vec(s, x, 4);
            
            if( f(x[0], x[1], c) == f(x[1], x[0], c) && x[1] != x[0] )
                comm++;
            
            if( f(f(x[0], x[1], c), x[2], c) == f(x[0], f(x[1], x[2], c), c) )
                assoc++;
            
            u = f(f(x[0], x[1], c), f(x[2], x[3], c), c);
            v = f(f(x[0], x[2], c), f(x[1], x[3], c), c);
            if( u != v ) { fail++; break; }
        }
        if( fail == 0 )
            dprintf(
                1, "%d\t%d\t%d\t%d\t%d\t%d : %d, %d\n",
                c[0], c[1], c[2], c[3], c[4], c[5], comm, assoc);
    }
    
    return NULL;
}

int main(int argc, char *argv[])
{
    thrdcnt = argc >= 2 ? atoi(argv[1]) : 1;
    pthread_t *threads = malloc(sizeof(pthread_t) * thrdcnt);
    for(int i=0; i<thrdcnt; i++)
    {
        pthread_create(threads+i, NULL, t, (void *)(intptr_t)i);
    }
    for(int i=0; i<thrdcnt; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
