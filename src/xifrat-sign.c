/* 2021-03-31, DannyNiu/NJF. Public Domain */

#include "xifrat-sign.h"
#include "../src-crypto/shake.h"

void *xifrat_sign_keygen(
    xifrat_sign_privkey_context_t *restrict x,
    GenFunc_t prng_gen, void *restrict prng)
{
    xifrat_cryptogram_t cryptogram;
    int i;

    prng_gen(prng, &cryptogram, sizeof(cryptogram));
    xifrat_cryptogram2array(x->C, cryptogram);
    
    prng_gen(prng, &cryptogram, sizeof(cryptogram));
    xifrat_cryptogram2array(x->K, cryptogram);
    
    prng_gen(prng, &cryptogram, sizeof(cryptogram));
    xifrat_cryptogram2array(x->Q, cryptogram);

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) x->P1[i] = x->C[i];
    xifrat_function_m(x->P1, x->K);

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) x->P2[i] = x->K[i];
    xifrat_function_m(x->P2, x->Q);

    return x;
}

void *xifrat_sign_sign(
    xifrat_sign_privkey_context_t *restrict x,
    void const *restrict msg, size_t msglen)
{
    xifrat_cryptogram_t cryptogram;
    shake256_t hash;
    int i;

    for(i=0; i<XIFRAT_CRYPTOGRAM_BYTELEN; i++) cryptogram[i] = 0;
    SHAKE256_Init(&hash);
    SHAKE_Write(&hash, msg, msglen);
    SHAKE_Final(&hash);
    SHAKE_Read(&hash, &cryptogram, 48); // 384 bits.
    xifrat_cryptogram2array(x->signature, cryptogram);
    
    xifrat_function_m(x->signature, x->Q);

    return x;
}

void const *xifrat_sign_verify(
    xifrat_sign_pubkey_context_t *restrict x,
    void const *restrict msg, size_t msglen)
{
    xifrat_array_t t1, t2; // signature verification transcripts.
    xifrat_cryptogram_t cryptogram;
    shake256_t hash;
    uint32_t v;
    int i;

    for(i=0; i<XIFRAT_CRYPTOGRAM_BYTELEN; i++) cryptogram[i] = 0;
    SHAKE256_Init(&hash);
    SHAKE_Write(&hash, msg, msglen);
    SHAKE_Final(&hash);
    SHAKE_Read(&hash, &cryptogram, 48); // 384 bits.
    xifrat_cryptogram2array(t2, cryptogram);
    
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) t1[i] = x->C[i];
    xifrat_function_m(t1, t2);
    xifrat_function_m(t1, x->P2);

    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) t2[i] = x->P1[i];
    xifrat_function_m(t2, x->signature);

    v = 0;
    for(i=0; i<XIFRAT_CRYPTOGRAM_ARRLEN; i++) v |= t1[i] ^ t2[i];

    v = (v - 1) >> 31;
    if( v ) return msg;
    else return NULL;
}

void *xifrat_sign_encode_pubkey(
    xifrat_sign_privkey_context_t *restrict x,
    xifrat_sign_pubkey_t *restrict out, size_t outlen)
{
    if( outlen < sizeof(xifrat_sign_pubkey_t) ) return NULL;

    xifrat_array2cryptogram(out->C, x->C);
    xifrat_array2cryptogram(out->P1, x->P1);
    xifrat_array2cryptogram(out->P2, x->P2);

    return out;
}

void *xifrat_sign_decode_pubkey(
    xifrat_sign_pubkey_context_t *restrict x,
    xifrat_sign_pubkey_t const *restrict in, size_t inlen)
{
    if( inlen < sizeof(xifrat_sign_pubkey_t) ) return NULL;

    xifrat_cryptogram2array(x->C, in->C);
    xifrat_cryptogram2array(x->P1, in->P1);
    xifrat_cryptogram2array(x->P2, in->P2);

    return x;
}

void *xifrat_sign_encode_privkey(
    xifrat_sign_privkey_context_t *restrict x,
    xifrat_sign_privkey_t *restrict out, size_t outlen)
{
    if( outlen < sizeof(xifrat_sign_privkey_t) ) return NULL;

    xifrat_array2cryptogram(out->C, x->C);
    xifrat_array2cryptogram(out->K, x->K);
    xifrat_array2cryptogram(out->Q, x->Q);
    xifrat_array2cryptogram(out->P1, x->P1);
    xifrat_array2cryptogram(out->P2, x->P2);

    return out;
}

void *xifrat_sign_decode_privkey(
    xifrat_sign_privkey_context_t *restrict x,
    xifrat_sign_privkey_t const *restrict in, size_t inlen)
{
    if( inlen < sizeof(xifrat_sign_privkey_t) ) return NULL;

    xifrat_cryptogram2array(x->C, in->C);
    xifrat_cryptogram2array(x->K, in->K);
    xifrat_cryptogram2array(x->Q, in->Q);
    xifrat_cryptogram2array(x->P1, in->P1);
    xifrat_cryptogram2array(x->P2, in->P2);

    return x;
}

void *xifrat_sign_encode_signature(
    xifrat_sign_privkey_context_t *restrict x,
    xifrat_sign_signature_t *restrict out, size_t outlen)
{
    if( outlen < sizeof(xifrat_sign_signature_t) ) return NULL;

    xifrat_array2cryptogram(out->signature, x->signature);

    return out;
}

void *xifrat_sign_decode_signature(
    xifrat_sign_pubkey_context_t *restrict x,
    xifrat_sign_signature_t const *restrict in, size_t inlen)
{
    if( inlen < sizeof(xifrat_sign_signature_t) ) return NULL;

    xifrat_cryptogram2array(x->signature, in->signature);

    return x;
}
