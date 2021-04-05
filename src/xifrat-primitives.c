/* DannyNiu/NJF, 2021-03-30. Public Domain. */

#include "xifrat-primitives.h"

static uint16_t xifrat_function_f(uint16_t a, uint16_t b)
{
    static const uint16_t sbox8[] = {
        5,  3,  1,  6,  7,  2,  0,  4,
        3,  5,  0,  2,  4,  6,  1,  7,
        6,  2,  4,  5,  0,  3,  7,  1,
        4,  7,  6,  1,  3,  0,  2,  5,
        0,  1,  3,  7,  6,  4,  5,  2,
        7,  4,  2,  0,  5,  1,  6,  3,
        2,  6,  7,  3,  1,  5,  4,  0,
        1,  0,  5,  4,  2,  7,  3,  6,
    };
    
    uint16_t ret = 0;
    uint16_t p = a * 8 + b;
    uint16_t i;
    uint32_t c;
    
    for(i=0; i<64; i++)
    {
        c = (i ^ p) - 1;
        c >>= 16;
        ret |= c & sbox8[i];
    }

    return ret;
}

#if VERIFY_FUNCTION_F
static int verify_xifrat_function_f()
{
    int h, i, j, k;
    uint16_t x, y;
    for(h=0; h<8; h++)
        for(i=0; i<8; i++)
            for(j=0; j<8; j++)
                for(k=0; k<8; k++)
                {
                    x = xifrat_function_f(
                        xifrat_function_f(h, i),
                        xifrat_function_f(j, k));
                    
                    y = xifrat_function_f(
                        xifrat_function_f(h, j),
                        xifrat_function_f(i, k));

                    if( x != y ) return false;
                }

    return true;
}

#include <stdio.h>
int main()
{
    if( !verify_xifrat_function_f() )
        printf("Inconsistent!\n");
    else printf("Ok!\n");
    return 0;
}
#endif

void *xifrat_cryptogram2array(xifrat_array_t a, const xifrat_cryptogram_t c)
{
    int i, shift;
    uint32_t buf;

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) a[i] = 0;
    
    buf = shift = 0;
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++)
    {
        while( shift < 3 )
        {
            buf |= (uint32_t)*c++ << shift;
            shift += 8;
        }

        a[i] = (uint16_t)(buf & 0x7);
        buf >>= 3;
        shift -= 3;
    }

    return a;
}

void *xifrat_array2cryptogram(xifrat_cryptogram_t c, const xifrat_array_t a)
{
    int i, shift;
    uint32_t buf;

    for(i=0; i<XIFRAT_CRYPTOGRAM_BYTELEN; i++) c[i] = 0;
    
    buf = shift = 0;
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++)
    {
        buf |= (0x7 & (uint32_t)a[i]) << shift;
        shift += 3;

        while( shift >= 8 )
        {
            *(c++) = (uint8_t)buf;
            buf >>= 8;
            shift -= 8;
        }
    }

    if( shift ) *(c++) = (uint8_t)buf;

    return c;
}

void *xifrat_function_m(xifrat_array_t r, xifrat_array_t k)
{
    uint16_t u[XIFRAT_CRYPTOGRAM_ARRLEN]; // shuffled index - main
    uint16_t v[XIFRAT_CRYPTOGRAM_ARRLEN]; // shuffled index - temp
    uint16_t e, d;
    uint32_t x, y;
    int i, j;

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++)
        r[i] = xifrat_function_f(r[i], k[i]);

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++)
    {
        // u[i] := v[i] := 
        // ((i * 3 + 5) ** 17) * 7 + 11 mod XIFRAT_CRYPTOGRAM_ARRLEN.
        // indicies randomized to block divide-and-conquer attacks.
        x = (i * 3 + 5) % XIFRAT_CRYPTOGRAM_ARRLEN;
        y = (x * x) % XIFRAT_CRYPTOGRAM_ARRLEN;
        y = (y * y) % XIFRAT_CRYPTOGRAM_ARRLEN;
        y = (y * y) % XIFRAT_CRYPTOGRAM_ARRLEN;
        y = (y * y) % XIFRAT_CRYPTOGRAM_ARRLEN;
        x = (x * y) % XIFRAT_CRYPTOGRAM_ARRLEN;
        x = (x * 7 + 11) % XIFRAT_CRYPTOGRAM_ARRLEN;
        u[i] = v[i] = x;
    }

    for(j=0; j<XIFRAT_CRYPTOGRAM_ARRLEN; j++)
    {
        for(i=1; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++)
        {
            e = u[i-1];
            d = u[i];
            r[d] = xifrat_function_f(r[d], r[e]);
        }

        e = u[i-1];
        for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) v[i] = u[u[i]];
        for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) u[i] = v[i];
        d = u[0];
        r[d] = xifrat_function_f(r[d], r[e]);
    }
    
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++)
        r[i] = xifrat_function_f(r[i], k[i]);

    return r;
}
