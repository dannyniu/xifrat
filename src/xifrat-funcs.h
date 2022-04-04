/* DannyNiu/NJF, 2022-04-03. Public Domain. */

#ifndef Xifrat_Funcs
#define Xifrat_Funcs 1

#include "common.h"

uint64_t xifrat_Blk(uint64_t a, uint64_t b);

#define VLEN 7
typedef uint64_t uint64x7_t[VLEN];

void xifrat_Enc(uint64x7_t out, uint64x7_t a, uint64x7_t b);
void xifrat_Mlt(uint64x7_t out, uint64x7_t a, uint64x7_t b);
void xifrat_Vec(uint64x7_t out, uint64x7_t a, uint64x7_t b);

void xifrat_cryptogram_decode(uint64x7_t wx, const void *os);
void xifrat_cryptogram_encode(void *os, const uint64x7_t wx);

#endif /* Xifrat_Funcs */
