/* DannyNiu/NJF, 2022-04-03. Public Domain. */

#include "xifrat-funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() // int argc, char *argv[])
{
    uint64x14_t a, b, c, d, u, v, x, y;

    int fails = 0;

    for(int i=0; i<20*20; i++)
    {
        fread(&a, 1, 112, stdin);
        fread(&b, 1, 112, stdin);
        fread(&c, 1, 112, stdin);
        fread(&d, 1, 112, stdin);

        xifrat_Enc(u, a, b);
        xifrat_Enc(v, c, d);
        xifrat_Mlt(x, u, v);

        xifrat_Mlt(u, a, c);
        xifrat_Mlt(v, b, d);
        xifrat_Enc(y, u, v);
        
        if( memcmp(x, y, 56) )
        {
            printf("Enc-Mlt failed!\n");
            fails++;
        }

        xifrat_Vec(u, a, b);
        xifrat_Vec(v, c, d);
        xifrat_Vec(x, u, v);

        xifrat_Vec(u, a, c);
        xifrat_Vec(v, b, d);
        xifrat_Vec(y, u, v);

        if( memcmp(x, y, 56) )
        {
            printf("Vec-Vec failed!\n");
            fails++;
        }

        xifrat_Dup(u, a, b);
        xifrat_Dup(v, c, d);
        xifrat_Dup(x, u, v);

        xifrat_Dup(u, a, c);
        xifrat_Dup(v, b, d);
        xifrat_Dup(y, u, v);

        if( memcmp(x, y, 112) )
        {
            printf("Dup-Dup failed!\n");
            fails++;
        }
    }

    if( !fails )
    {
        printf("Succeeded!\n");
        return EXIT_SUCCESS;
    }
    else
    {
        printf("Fails: %i\n", fails);
        return EXIT_FAILURE;
    }
}
