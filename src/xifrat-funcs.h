/* DannyNiu/NJF, 2022-04-03. Public Domain. */

#ifndef Xifrat_Funcs
#define Xifrat_Funcs 1

#include "common.h"

uint64_t xifrat_Blk(uint64_t a, uint64_t b);

#define VLEN 7
typedef uint64_t uint64x7_t[VLEN];

#define DLEN 2
typedef uint64_t uint64x14_t[DLEN*VLEN];

void xifrat_Vec(uint64x7_t out, const uint64x7_t a, const uint64x7_t b);
void xifrat_Dup(uint64x14_t out, const uint64x14_t a, const uint64x14_t b);

void xifrat_cryptogram_decode(uint64x14_t wx, const void *os);
void xifrat_cryptogram_encode(void *os, const uint64x14_t wx);

#endif /* Xifrat_Funcs */
