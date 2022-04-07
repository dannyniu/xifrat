/* DannyNiu/NJF, 2022-04-03. Public Domain. */

#include "xifrat-funcs.h"
#include "../src-crypto/endian.h"

#define P 8
#define P2 64

static uint16_t f_table[P2] = {
    2,  0,  4,  3,  5,  7,  1,  6,
    1,  5,  3,  4,  0,  6,  2,  7,
    7,  4,  0,  5,  3,  2,  6,  1,
    0,  2,  7,  6,  1,  4,  5,  3,
    3,  6,  1,  2,  7,  5,  4,  0,
    6,  3,  5,  0,  4,  1,  7,  2,
    4,  7,  2,  1,  6,  0,  3,  5,
    5,  1,  6,  7,  2,  3,  0,  4,
};

static uint64_t f_wide(uint64_t a, uint64_t b)
{
    const static uint64_t mbase = 0x1249249249249249;
    uint64_t m, n, u, v, ret;
    int16_t i;

    a &= INT64_MAX;
    b &= INT64_MAX;
    ret = 0;

    for(i=0; i<P2; i++)
    {
        m = (i / P) * mbase;
        n = (i % P) * mbase;

        u = ~(a ^ m);
        u &= (u >> 1) & (u >> 2) & mbase;
        u *= 7;
        
        v = ~(b ^ n);
        v &= (v >> 1) & (v >> 2) & mbase;
        v *= 7;

        ret |= (f_table[i] * mbase) & u & v;
    }

    return ret;
}

uint64_t xifrat_Blk(uint64_t a, uint64_t b)
{
    uint64_t u, v;
    int i;
    
    u = a &= INT64_MAX;
    v = b &= INT64_MAX;
    
    for(i=1; i<21; i++)
    {
        a = (a >> 3 | a << 60) & INT64_MAX;
        b = (b >> 3 | b << 60) & INT64_MAX;
        u = f_wide(u, a);
        v = f_wide(v, b);
    }

    return f_wide(f_wide(f_wide(u, v), u), v);
}

void xifrat_Vec(uint64x7_t out, const uint64x7_t a, const uint64x7_t b)
{
    uint64x7_t u, v;
    int i, j;

    for(j=0; j<VLEN; j++)
    {
        u[j] = a[j] & INT64_MAX;
        v[j] = b[j] & INT64_MAX;
    }
    
    for(i=1; i<VLEN; i++)
    {
        for(j=0; j<VLEN; j++)
        {
            u[j] = xifrat_Blk(u[j], a[(i+j)%VLEN]);
            v[j] = xifrat_Blk(v[j], b[(i+j)%VLEN]);
        }
    }

    for(j=0; j<VLEN; j++)
        out[j] = xifrat_Blk(xifrat_Blk(xifrat_Blk(u[j], v[j]), u[j]), v[j]);
}

void xifrat_Dup(uint64x14_t out, const uint64x14_t a, const uint64x14_t b)
{
    uint64x14_t u, v;
    int i, j;

    for(j=0; j<DLEN*VLEN; j++)
    {
        u[j] = a[j] & INT64_MAX;
        v[j] = b[j] & INT64_MAX;
    }
    
    for(i=1; i<DLEN; i++)
    {
        for(j=0; j<DLEN; j++)
        {
            xifrat_Vec(u+j*VLEN, u+j*VLEN, a+((i+j)%DLEN)*VLEN);
            xifrat_Vec(v+j*VLEN, v+j*VLEN, b+((i+j)%DLEN)*VLEN);
        }
    }

    for(j=0; j<DLEN; j++)
    {
        xifrat_Vec(out+j*VLEN, u+j*VLEN, v+j*VLEN);
        xifrat_Vec(u+j*VLEN, out+j*VLEN, u+j*VLEN);
        xifrat_Vec(out+j*VLEN, u+j*VLEN, v+j*VLEN);
    }
}

void xifrat_cryptogram_decode(uint64x14_t wx, const void *os)
{
    const uint64_t *buf = os;
    int i;
    
    for(i=0; i<DLEN*VLEN; i++)
        wx[i] = le64toh(buf[i]) & INT64_MAX;
}

void xifrat_cryptogram_encode(void *os, const uint64x14_t wx)
{
    uint64_t *buf = os;
    int i;

    for(i=0; i<DLEN*VLEN; i++)
        buf[i] = htole64(wx[i] & INT64_MAX);
}
