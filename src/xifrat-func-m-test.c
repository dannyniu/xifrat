/* 2021-03-31, DannyNiu/NJF. Public Domain. */

#include "xifrat-primitives.h"

#include <stdio.h>
#include <string.h>

xifrat_array_t
A, B, C, D, X, Y;
xifrat_cryptogram_t
a, b, c, d;

int fails = 0;

void func_m_test()
{
    int i;
    
    xifrat_cryptogram2array(A, a);
    xifrat_cryptogram2array(B, b);
    xifrat_cryptogram2array(C, c);
    xifrat_cryptogram2array(D, d);

    memcpy(X, D, sizeof(xifrat_array_t));
    memcpy(Y, C, sizeof(xifrat_array_t));
    xifrat_function_m(Y, X);
    memcpy(X, A, sizeof(xifrat_array_t));
    xifrat_function_m(X, B);
    xifrat_function_m(X, Y);

    xifrat_array2cryptogram(a, X);

    memcpy(X, D, sizeof(xifrat_array_t));
    memcpy(Y, B, sizeof(xifrat_array_t));
    xifrat_function_m(Y, X);
    memcpy(X, A, sizeof(xifrat_array_t));
    xifrat_function_m(X, C);
    xifrat_function_m(X, Y);

    xifrat_array2cryptogram(b, X);
    if( memcmp(a, b, sizeof(xifrat_cryptogram_t)) )
    {
        fails++;
        printf("Fail +1 !\n");
    }
        
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++)
        printf("%o", X[i]);
    printf("\n");
}

int main()
{
    int i = 0;

    for(i=0; i<256; i++)
    {
        memset(a, i, sizeof(xifrat_cryptogram_t));
        memset(b, i, sizeof(xifrat_cryptogram_t));
        memset(c, i, sizeof(xifrat_cryptogram_t));
        memset(d, i, sizeof(xifrat_cryptogram_t));
        func_m_test();
    }

    printf("\n");
    
    for(i=0; i<256; i++)
    {
        fread(a, 1, sizeof(xifrat_cryptogram_t), stdin);
        fread(b, 1, sizeof(xifrat_cryptogram_t), stdin);
        fread(c, 1, sizeof(xifrat_cryptogram_t), stdin);
        fread(d, 1, sizeof(xifrat_cryptogram_t), stdin);
        func_m_test();
    }

    printf("%d fails\n", fails);
    return 0;
}
