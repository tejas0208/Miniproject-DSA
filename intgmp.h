#ifndef __INTGMP_H
#define __INTGMP_H

Integer GmpInt(mpz_t a);

void IntGmp(Integer a, mpz_t b);

void prime2(mpz_t a, mpz_t b);

void public_key(mpz_t e, mpz_t tot);

void private_key(mpz_t d, mpz_t e, mpz_t t);

#endif
