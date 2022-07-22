/* DannyNiu/NJF, 2022-04-03. Public Domain. */

#include "xifrat-funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() // int argc, char *argv[])
{
    uint64dup_t a, b, c, d, u, v, x, y;

    int total = 50;
    int fails = 0;

    setvbuf(stdout, NULL, _IONBF, 512);

    for(int i=0; i<total; i++)
    {
        fread(&a, 1, sizeof(uint64dup_t), stdin);
        fread(&b, 1, sizeof(uint64dup_t), stdin);
        fread(&c, 1, sizeof(uint64dup_t), stdin);
        fread(&d, 1, sizeof(uint64dup_t), stdin);

        xifrat_Vec(u, a, b);
        xifrat_Vec(v, c, d);
        xifrat_Vec(x, u, v);

        xifrat_Vec(u, a, c);
        xifrat_Vec(v, b, d);
        xifrat_Vec(y, u, v);

        if( memcmp(x, y, sizeof(uint64vec_t)) )
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

        if( memcmp(x, y, sizeof(uint64dup_t)) )
        {
            printf("Dup-Dup failed!\n");
            fails++;
        }

        printf("\t%d/%d\r", i, total);
    }
    printf("\n");

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
