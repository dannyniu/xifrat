/* 2021-04-05, DannyNiu/NJF. Public Domain */

#include "xifrat-kex.h"
#include "../src-crypto/shake.h"

void *xifrat_kex_keygen(
    xifrat_kex_dec_context_t *restrict x,
    GenFunc_t prng_gen, void *prng)
{
    xifrat_cryptogram_t cryptogram;
    int i;

    // generate base cryptogram
    prng_gen(prng, &cryptogram, sizeof(cryptogram));
    xifrat_cryptogram2array(x->C, cryptogram);

    // generate private key
    prng_gen(prng, &cryptogram, sizeof(cryptogram));
    xifrat_cryptogram2array(x->K, cryptogram);

    // compute the public key
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) x->P[i] = x->C[i];
    xifrat_function_m(x->P, x->K);

    return x;
}

void *xifrat_kex_enc(
    xifrat_kex_enc_context_t *restrict x,
    void *restrict ss, size_t sslen,
    GenFunc_t prng_gen, void *prng)
{
    xifrat_array_t S, T;
    xifrat_cryptogram_t cryptogram;
    int i;

    if( sslen < sizeof(xifrat_cryptogram_t) ) return NULL;

    // generate ciphertext    
    prng_gen(prng, &cryptogram, sizeof(cryptogram));
    xifrat_cryptogram2array(x->K, cryptogram);

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) x->Q[i] = x->C[i];
    xifrat_function_m(x->Q, x->K);

    // compute the shared secret
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) T[i] = x->K[i];
    xifrat_function_m(T, x->C);

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) S[i] = x->P[i];
    xifrat_function_m(S, T);

    // output the shared secret as a cryptogram
    // (needs to be hashed by e.g. HKDF)
    xifrat_array2cryptogram(cryptogram, S);
    for(i=0; i<XIFRAT_CRYPTOGRAM_BYTELEN && i<sslen; i++)
        ((uint8_t *)ss)[i] = cryptogram[i];
    for(; i<sslen; i++)
        ((uint8_t *)ss)[i] = 0;

    return ss;
}

void *xifrat_kex_dec(
    xifrat_kex_dec_context_t *restrict x,
    void *restrict ss, size_t sslen)
{
    xifrat_array_t S, T;
    xifrat_cryptogram_t cryptogram;
    int i;

    if( sslen < sizeof(xifrat_cryptogram_t) ) return NULL;
    
    // compute the shared secret
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) T[i] = x->K[i];
    xifrat_function_m(T, x->C);

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) S[i] = x->Q[i];
    xifrat_function_m(S, T);

    // output the shared secret as a cryptogram
    // (needs to be hashed by e.g. HKDF)
    xifrat_array2cryptogram(cryptogram, S);
    for(i=0; i<XIFRAT_CRYPTOGRAM_BYTELEN && i<sslen; i++)
        ((uint8_t *)ss)[i] = cryptogram[i];
    for(; i<sslen; i++)
        ((uint8_t *)ss)[i] = 0;
        
    return ss;
}

void *xifrat_kex_encode_pubkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_pubkey_t *restrict out, size_t outlen)
{
    if( outlen < sizeof(xifrat_kex_pubkey_t) ) return NULL;

    xifrat_array2cryptogram(out->C, x->C);
    xifrat_array2cryptogram(out->P, x->P);

    return out;
}

void *xifrat_kex_decode_pubkey(
    xifrat_kex_enc_context_t *restrict x,
    xifrat_kex_pubkey_t const *restrict in, size_t inlen)
{
    if( inlen < sizeof(xifrat_kex_pubkey_t) ) return NULL;

    xifrat_cryptogram2array(x->C, in->C);
    xifrat_cryptogram2array(x->P, in->P);

    return x;
}

void *xifrat_kex_encode_privkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_privkey_t *restrict out, size_t outlen)
{
    if( outlen < sizeof(xifrat_kex_privkey_t) ) return NULL;

    xifrat_array2cryptogram(out->C, x->C);
    xifrat_array2cryptogram(out->K, x->K);
    xifrat_array2cryptogram(out->P, x->P);

    return out;
}

void *xifrat_kex_decode_privkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_privkey_t const *restrict in, size_t inlen)
{
    if( inlen < sizeof(xifrat_kex_privkey_t) ) return NULL;

    xifrat_cryptogram2array(x->C, in->C);
    xifrat_cryptogram2array(x->K, in->K);
    xifrat_cryptogram2array(x->P, in->P);

    return x;
}

void *xifrat_kex_encode_ciphertext(
    xifrat_kex_enc_context_t *restrict x,
    xifrat_kex_ciphertext_t *restrict out, size_t outlen)
{
    if( outlen < sizeof(xifrat_kex_ciphertext_t) ) return NULL;

    xifrat_array2cryptogram(out->cryptogram, x->Q);

    return out;
}

void *xifrat_kex_decode_ciphertext(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_ciphertext_t const *restrict in, size_t inlen)
{
    if( inlen < sizeof(xifrat_kex_ciphertext_t) ) return NULL;

    xifrat_cryptogram2array(x->Q, in->cryptogram);

    return x;
}
