#include "api.h"
#include "rng.h"
#include <string.h>

void prng_src(void *restrict x, void *restrict data, size_t len)
{
    x = x;
    randombytes(data, len);
}

int crypto_kem_keypair(
    unsigned char *pk,
    unsigned char *sk)
{
    xifrat_kex_dec_context_t x;
    xifrat_kex_keygen(&x, prng_src, NULL);

    xifrat_kex_encode_pubkey(
        &x, (void *)pk,
        sizeof(xifrat_kex_pubkey_t));

    xifrat_kex_encode_privkey(
        &x, (void *)sk,
        sizeof(xifrat_kex_privkey_t));

    return 0;
}

int crypto_kem_enc(
    unsigned char *ct,
    unsigned char *ss,
    const unsigned char *pk)
{
    xifrat_kex_enc_context_t x;

    xifrat_kex_decode_pubkey(
        &x, (const void *)pk,
        sizeof(xifrat_kex_pubkey_t));

    xifrat_kex_enc(
        &x, (void *)ss,
        sizeof(xifrat_cryptogram_t),
        prng_src, NULL);

    xifrat_kex_encode_ciphertext(
        &x, (void *)ct,
        sizeof(xifrat_kex_ciphertext_t));

    return 0;
}

int crypto_kem_dec(
    unsigned char *ss,
    const unsigned char *ct,
    const unsigned char *sk)
{
    xifrat_kex_dec_context_t x;
    xifrat_kex_decode_privkey(
        &x, (const void *)sk,
        sizeof(xifrat_kex_privkey_t));

    xifrat_kex_decode_ciphertext(
        &x, (const void *)ct,
        sizeof(xifrat_kex_ciphertext_t));

    xifrat_kex_dec(
        &x, (void *)ss,
        sizeof(xifrat_cryptogram_t));

    return 0;
}
