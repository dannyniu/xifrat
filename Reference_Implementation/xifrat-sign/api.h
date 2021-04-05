//
//  api.h
//
//  Created by Bassham, Lawrence E (Fed) on 9/6/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//


#ifndef api_h
#define api_h

#include "../../src/xifrat-sign.h"

//  Set these three values apropriately for your algorithm
#define CRYPTO_SECRETKEYBYTES ((int)sizeof(xifrat_sign_privkey_t))
#define CRYPTO_PUBLICKEYBYTES ((int)sizeof(xifrat_sign_pubkey_t))
#define CRYPTO_BYTES ((int)sizeof(xifrat_sign_signature_t))

// Change the algorithm name
#define CRYPTO_ALGNAME "Xifrat-Sign"

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk);

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk);

#endif /* api_h */
