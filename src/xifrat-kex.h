/* 2021-04-05, DannyNiu/NJF. Public Domain */

#ifndef xifrat_kex_h
#define xifrat_kex_h 1

#include "xifrat-primitives.h"

typedef struct {
    xifrat_array_t C, K, P, Q;
} xifrat_kex_dec_context_t;

typedef struct {
    xifrat_array_t C, K, P, Q;
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
    xifrat_cryptogram_t C, P;
} xifrat_kex_pubkey_t;

// returns out on success and NULL on failure (e.g. outlen too short).
void *xifrat_kex_encode_pubkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_pubkey_t *restrict out, size_t outlen);

// returns x on success and NULL on failure.
void *xifrat_kex_decode_pubkey(
    xifrat_kex_enc_context_t *restrict x,
    xifrat_kex_pubkey_t const *restrict in, size_t inlen);

typedef struct {
    xifrat_cryptogram_t C, K, P;
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
    xifrat_cryptogram_t cryptogram;
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
