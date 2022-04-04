//
//  api.h
//
//  Created by Bassham, Lawrence E (Fed) on 9/6/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//


#ifndef api_h
#define api_h

#include "../../src/xifrat-kex.h"

//  Set these three values apropriately for your algorithm
#define CRYPTO_SECRETKEYBYTES ((int)sizeof(xifrat_kex_privkey_t))
#define CRYPTO_PUBLICKEYBYTES ((int)sizeof(xifrat_kex_pubkey_t))
#define CRYPTO_BYTES ((int)sizeof(uint64x7_t))
#define CRYPTO_CIPHERTEXTBYTES ((int)sizeof(xifrat_kex_ciphertext_t))

// Change the algorithm name
#define CRYPTO_ALGNAME "Xifrat-Kex"

int crypto_kem_keypair(
    unsigned char *pk,
    unsigned char *sk);

int crypto_kem_enc(
    unsigned char *ct,
    unsigned char *ss,
    const unsigned char *pk);

int crypto_kem_dec(
    unsigned char *ss,
    const unsigned char *ct,
    const unsigned char *sk);

#endif /* api_h */
