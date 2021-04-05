/* DannyNiu/NJF, 2021-03-21. Public Domain. */
// Estimates the (non-)linearity of Daniel Nager's original s-box. 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int latinsquare[256] = {
    +7,  9, 13, 10, 15,  2,  0,  6,  3, 12,  8,  4,  1,  5, 14, 11,
    +1, 15,  6,  3,  9,  4, 11, 13, 10,  5, 14,  2,  7, 12,  8,  0,
    +3,  0, 12,  1, 11,  8,  9,  5,  7, 13,  2, 14, 10,  6,  4, 15,
    +4,  6, 15,  8, 13,  1,  5,  9, 14, 11, 10,  7,  2,  0,  3, 12,
    +0,  3,  8, 15, 10, 12,  7, 14,  9,  2, 13,  5, 11,  4,  6,  1,
    10, 11,  5,  7,  0, 14, 15, 12,  1,  6,  4,  8,  3, 13,  2,  9,
    +5, 14, 10, 13,  8, 11,  4,  3,  6,  1, 15,  0, 12,  7,  9,  2,
    15,  1,  4,  0,  7,  6, 10,  2, 11, 14,  5, 13,  9,  8, 12,  3,
    12,  8,  3,  6, 14,  0,  2, 10, 13,  7,  9, 11,  5,  1, 15,  4,
    13,  2,  7,  5,  4,  9,  8,  1, 12,  3,  0, 15,  6, 10, 11, 14,
    +6,  4,  1, 12,  2, 15, 14,  7,  5, 10, 11,  9, 13,  3,  0,  8,
    +9,  7,  2, 11,  1, 13,  3,  4,  0,  8, 12,  6, 15, 14,  5, 10,
    11, 10, 14,  9,  3,  5,  1,  8, 15,  4,  6, 12,  0,  2, 13,  7,
    14,  5, 11,  2, 12, 10,  6,  0,  4, 15,  1,  3,  8,  9,  7, 13,
    +8, 12,  0,  4,  5,  3, 13, 11,  2,  9,  7, 10, 14, 15,  1,  6,
    +2, 13,  9, 14,  6,  7, 12, 15,  8,  0,  3,  1,  4, 11, 10,  5,
};

#define P 0x11

int mulb(int a, int b) // multiplication in F_{2^4}
{
    int x = 0;
    for(int i=0; i<4; i++)
    {
        x ^= b&1 ? a : 0;
        b >>= 1;
        a <<= 1;
        a ^= a&0x10 ? P : 0;
    }
    return x;
}

int mulf(int a, int b) // modular multiplication in Z_{16}
{
    return (a * b) & 15;
}

int (*mul)(int a, int b) = mulf; // either mulb or mulf

void mksbox(int x[16], uint64_t s)
{
    int i;

    for(i=0; i<16; i++) x[i] = i;

    for(i=16; i>1; i--)
    {
        int p = s % i;
        int j = 16 - i;
        int t = x[p+j];
        x[p+j] = x[j];
        x[j] = t;
        s = (s - p) / i;
    }
}

float misses(int ab, int map[256])
{
    int a = ab >> 4, b = ab & 15;
    int x, y;
    int cand = 0;
    float min = -1;
    float num = 0, den = 0;

    if( !a || !b || a == b ) return min;
    
    for(x=0; x<256; x++) map[x] = 0;
        
    for(x=0; x<16; x++)
    {
        for(y=0; y<16; y++)
        {
            int u = mul(a,x) ^ mul(b,y);
            int v = latinsquare[x*16+y];
            map[u*16+v]++;
        }
    }
    
    for(x=0; x<16; x++)
    {
        for(y=0; y<16; y++)
        {
            num += map[x*16+y];
            if( map[x*16+y] ) den++;
        }
    }
    
    for(x=0; x<16; x++)
    {
        for(y=0; y<16; y++)
        {
            num += map[x*16+y];
            if( map[x+16*y] ) den++;
        }
    }

    dprintf(
        1, "\na: % 2d, b: % 2d\n",
        ab>>4, ab&15);
    for(int i=0; i<256; i++)
        dprintf(1, "% 3d%c", map[i], i%16==15?'\n':' ');

    min = num / den;
    return min;
}

int thrdcnt;

void *t(void *argp)
{
    long ind = (long)argp;
    int rec = -1;
    int map[256];
    float max = 0;
    float subret;

    for(; ind<256; ind+=thrdcnt)
    {
        subret = misses((int)ind, map);
        if( subret > max )
        {
            max = subret;
            rec = (int)ind;
        }
    }
    
    return NULL;
}

int main(int argc, char *argv[])
{
    thrdcnt = 1;
    t(0);
    return 0;
}
