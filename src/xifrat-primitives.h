/* DannyNiu/NJF, 2021-03-30. Public Domain. */

#ifndef xifrat_primitives_h
#define xifrat_primitives_h 1

#include "common.h"

#define XIFRAT_CRYPTOGRAM_ARRLEN 131
#define XIFRAT_CRYPTOGRAM_BYTELEN 50

typedef uint16_t xifrat_array_t[XIFRAT_CRYPTOGRAM_ARRLEN];
typedef uint8_t xifrat_cryptogram_t[XIFRAT_CRYPTOGRAM_BYTELEN];

// returns the result of conversion.
void *xifrat_cryptogram2array(xifrat_array_t a, const xifrat_cryptogram_t c);
void *xifrat_array2cryptogram(xifrat_cryptogram_t c, const xifrat_array_t a);

// returns r on success, and NULL on failure.
void *xifrat_function_m(xifrat_array_t r, xifrat_array_t k);

#endif /* xifrat_primitives_f_h */
