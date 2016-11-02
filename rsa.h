#ifndef __RSA_H
#define __RSA_H

#include "integer.h"
Integer GmpInt(mpz_t a);

void IntGmp(Integer a, mpz_t b);

void GeneratePrimes(mpz_t a, mpz_t b);

void public_key(mpz_t e, mpz_t tot);

void private_key(mpz_t d, mpz_t e, mpz_t t);

void rsa_encrypt(FILE *fp2, FILE *fp3, Integer e1, Integer n);

void rsa_decrypt(FILE *fp2, FILE *fp3, Integer d1, Integer n);

#endif
