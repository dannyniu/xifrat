/* 2021-04-05, DannyNiu/NJF. Public Domain */

#include "xifrat-kex.h"
#include "../src-crypto/shake.h"

void *xifrat_kex_keygen(
    xifrat_kex_dec_context_t *restrict x,
    GenFunc_t prng_gen, void *prng)
{
    shake128_t xof;
    uint64dup_t cryptogram;

    // generate acegi from seed
    prng_gen(prng, &x->seed, XIFRAT_SEEDLEN);

    SHAKE128_Init(&xof);
    SHAKE_Write(&xof, x->seed, XIFRAT_SEEDLEN);
    SHAKE_Final(&xof);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->a, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->c, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->e, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->g, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->i, cryptogram);

    // generate private key
    prng_gen(prng, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->s1, cryptogram); // b

    prng_gen(prng, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->s2, cryptogram); // h

    // compute the public key
    xifrat_Dup(x->q, x->s1, x->e);
    xifrat_Dup(x->p, x->q, x->s2);

    // pre-compute for acceleration
    xifrat_Dup(x->q, x->a, x->s1);
    xifrat_Dup(x->a, x->q, x->c); // a = (abc)
    xifrat_Dup(x->q, x->g, x->s2);
    xifrat_Dup(x->g, x->q, x->i); // g = (ghi)

    return x;
}

void *xifrat_kex_enc(
    xifrat_kex_enc_context_t *restrict x,
    void *restrict ss, size_t sslen,
    GenFunc_t prng_gen, void *prng)
{
    uint64dup_t cryptogram;
    uint64dup_t u, v, w;
    unsigned i;

    // generate ciphertext
    prng_gen(prng, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->s1, cryptogram); // d

    prng_gen(prng, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->s2, cryptogram); // f

    xifrat_Dup(w, x->s1, x->e);
    xifrat_Dup(x->q, w, x->s2);

    // compute the shared secret
    xifrat_Dup(w, x->a, x->s1);
    xifrat_Dup(u, w, x->g);

    xifrat_Dup(w, x->c, x->s2);
    xifrat_Dup(v, w, x->i);

    xifrat_Dup(w, u, x->p);
    xifrat_Dup(u, w, v);

    // output the shared secret as a cryptogram
    // (needs to be hashed by e.g. HKDF)
    xifrat_cryptogram_encode(cryptogram, u);
    for(i=0; i<sizeof(cryptogram) && i<sslen; i++)
        ((uint8_t *)ss)[i] = ((uint8_t *)cryptogram)[i];
    for(; i<sslen; i++)
        ((uint8_t *)ss)[i] = 0;

    return ss;
}

void *xifrat_kex_dec(
    xifrat_kex_dec_context_t *restrict x,
    void *restrict ss, size_t sslen)
{
    uint64dup_t cryptogram;
    unsigned i;

    // compute the shared secret
    xifrat_Dup(x->c, x->a, x->q);
    xifrat_Dup(x->i, x->c, x->g);
    
    // output the shared secret as a cryptogram
    // (needs to be hashed by e.g. HKDF)
    xifrat_cryptogram_encode(cryptogram, x->i);
    for(i=0; i<sizeof(cryptogram) && i<sslen; i++)
        ((uint8_t *)ss)[i] = ((uint8_t *)cryptogram)[i];
    for(; i<sslen; i++)
        ((uint8_t *)ss)[i] = 0;
        
    return ss;
}

void *xifrat_kex_export_pubkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_pubkey_t *restrict out, size_t outlen)
{
    unsigned i;

    if( outlen < sizeof(xifrat_kex_pubkey_t) ) return NULL;
    
    for(i=0; i<XIFRAT_SEEDLEN; i++) out->seed[i] = x->seed[i];
    xifrat_cryptogram_encode(out->p, x->p);

    return out;
}

void *xifrat_kex_decode_pubkey(
    xifrat_kex_enc_context_t *restrict x,
    xifrat_kex_pubkey_t const *restrict in, size_t inlen)
{
    shake128_t xof;
    uint64dup_t cryptogram;
    unsigned i;

    if( inlen < sizeof(xifrat_kex_pubkey_t) ) return NULL;

    // generate acegi from seed
    for(i=0; i<XIFRAT_SEEDLEN; i++) x->seed[i] = in->seed[i];
    
    SHAKE128_Init(&xof);
    SHAKE_Write(&xof, x->seed, XIFRAT_SEEDLEN);
    SHAKE_Final(&xof);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->a, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->c, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->e, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->g, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->i, cryptogram);

    // copy public cryptogram
    xifrat_cryptogram_decode(x->p, in->p);

    return x;
}

void *xifrat_kex_encode_privkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_privkey_t *restrict out, size_t outlen)
{
    unsigned i;

    if( outlen < sizeof(xifrat_kex_pubkey_t) ) return NULL;
    
    for(i=0; i<XIFRAT_SEEDLEN; i++) out->seed[i] = x->seed[i];
    xifrat_cryptogram_encode(out->s1, x->s1);
    xifrat_cryptogram_encode(out->s2, x->s2);

    return out;
}

void *xifrat_kex_decode_privkey(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_privkey_t const *restrict in, size_t inlen)
{
    shake128_t xof;
    uint64dup_t cryptogram;
    unsigned i;

    // generate acegi from seed
    for(i=0; i<XIFRAT_SEEDLEN; i++) x->seed[i] = in->seed[i];

    SHAKE128_Init(&xof);
    SHAKE_Write(&xof, x->seed, XIFRAT_SEEDLEN);
    SHAKE_Final(&xof);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->a, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->c, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->e, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->g, cryptogram);

    SHAKE_Read(&xof, cryptogram, sizeof(cryptogram));
    xifrat_cryptogram_decode(x->i, cryptogram);

    // generate private key
    xifrat_cryptogram_encode(x->s1, in->s1); // b
    xifrat_cryptogram_encode(x->s2, in->s2); // h
    
    // compute the public key
    xifrat_Dup(x->q, x->s1, x->e);
    xifrat_Dup(x->p, x->q, x->s2);

    // pre-compute for acceleration
    xifrat_Dup(x->q, x->a, x->s1);
    xifrat_Dup(x->a, x->q, x->c); // a = (abc)
    xifrat_Dup(x->q, x->g, x->s2);
    xifrat_Dup(x->g, x->q, x->i); // g = (ghi)

    return x;
}

void *xifrat_kex_encode_ciphertext(
    xifrat_kex_enc_context_t *restrict x,
    xifrat_kex_ciphertext_t *restrict out, size_t outlen)
{
    if( outlen < sizeof(xifrat_kex_ciphertext_t) ) return NULL;

    xifrat_cryptogram_encode(out->cryptogram, x->q);

    return out;
}

void *xifrat_kex_decode_ciphertext(
    xifrat_kex_dec_context_t *restrict x,
    xifrat_kex_ciphertext_t const *restrict in, size_t inlen)
{
    if( inlen < sizeof(xifrat_kex_ciphertext_t) ) return NULL;

    xifrat_cryptogram_decode(x->q, in->cryptogram);

    return x;
}
