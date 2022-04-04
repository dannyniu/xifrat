/* 2021-04-05, DannyNiu/NJF. Public Domain */

#ifndef xifrat_kex_h
#define xifrat_kex_h 1

#include "xifrat-funcs.h"

#define XIFRAT_SEEDLEN 8

typedef struct {
    uint8_t seed[XIFRAT_SEEDLEN];
    uint64x7_t a, c, e, g, i, s1, s2, p, q;
} xifrat_kex_dec_context_t;

typedef struct {
    uint8_t seed[XIFRAT_SEEDLEN];
    uint64x7_t a, c, e, g, i, s1, s2, p, q;
} xifrat_kex_enc_context_t;

// returns x on success and NULL on failure.
void *xifrat_kex_keygen(
    xifrat_kex_dec_context_t *restrict x,
    GenFunc_t prng_gen, void *restrict prng);

// returns ss on success and NULL on failure.
void *xifrat_kex_enc(
    xifrat_kex_enc_context_t *restrict x,
    void *restrict ss, size_t sslen,
    GenFunc_t prng_gen, void *restrict prng);

// returns x on success and NULL on failure.
void *xifrat_kex_dec(
    xifrat_kex_dec_context_t *restrict x,
    void *restrict ss, size_t sslen);

typedef struct {
    uint8_t seed[XIFRAT_SEEDLEN];
    uint64x7_t p;
} xifrat_kex_pubkey_t;

// returns out on success and NULL on failure (e.g. outlen too short).
void *xifrat_kex_export_pubkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_pubkey_t *restrict out, size_t outlen);

// returns x on success and NULL on failure.
void *xifrat_kex_decode_pubkey(
    xifrat_kex_enc_context_t *restrict x,
    xifrat_kex_pubkey_t const *restrict in, size_t inlen);

typedef struct {
    uint8_t seed[XIFRAT_SEEDLEN];
    uint64x7_t s1, s2;
} xifrat_kex_privkey_t;

// returns out on success and NULL on failure (e.g. outlen too short).
void *xifrat_kex_encode_privkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_privkey_t *restrict out, size_t outlen);

// returns x on success on NULL on failure.
void *xifrat_kex_decode_privkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_privkey_t const *restrict in, size_t inlen);

typedef struct {
    uint64x7_t cryptogram;
} xifrat_kex_ciphertext_t;

// returns out on success and NULL on failure (e.g. outlen too short).
void *xifrat_kex_encode_ciphertext(
    xifrat_kex_enc_context_t *restrict x,
    xifrat_kex_ciphertext_t *restrict out, size_t len);

// returns x on success and NULL on failure.
void *xifrat_kex_decode_ciphertext(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_ciphertext_t const *restrict in, size_t inlen);

#endif /* xifrat_kex_h */
