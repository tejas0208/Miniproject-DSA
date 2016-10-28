#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "integer.h"

/* Converts the gmp library data type to my data type */
Integer GmpInt(mpz_t a) {
	Integer b;
	char str[1000];
	gmp_sprintf(str, "%Zd", a);
	b = CreateIntegerFromString(str);
	return b;
}

/* Converts my data type to the gmp library data type */
void IntGmp(Integer a, mpz_t b) {
	mpz_init(b);
	char *str;
	str = IntegertoString(a);
	mpz_set_str(b, str, 10);
}

void prime2(mpz_t a, mpz_t b) {
	unsigned int seed;
	gmp_randstate_t p, q;
	int prime = 0;
	mpz_init(a);
	mpz_init(b);
	gmp_randinit_default(p);
	gmp_randinit_default(q);
	srand(time(NULL));
	seed = rand();
	gmp_randseed_ui(p, seed);
	seed = rand();
	gmp_randseed_ui(q, seed);
	while(prime != 1) {
		mpz_urandomb(a, p, 512);
		prime = mpz_probab_prime_p(a, 25);
	}
	gmp_printf("First prime\n%Zd\n", a);
	prime = 0;
	while(prime != 1) {
		mpz_urandomb(b, q, 512);
		prime = mpz_probab_prime_p(b, 25);
	}

	gmp_printf("Second prime\n%Zd\n", b);
}

void public_key(mpz_t e, mpz_t tot) {
	unsigned int seed;
	gmp_randstate_t p;
	mpz_init(e);
	gmp_randinit_default(p);
	srand(time(NULL));
	seed = rand();
	gmp_randseed_ui(p, seed);
	int prime = 0;
	while(prime == 0 || mpz_divisible_p(tot, e)) {
		prime = 0;
		mpz_urandomb(e, p, 16);
		prime = mpz_probab_prime_p(e, 25);
	}
	gmp_printf("Public key\n%Zd\n", e);	
}

void private_key(mpz_t d, mpz_t e, mpz_t t) {
	mpz_init(d);
	mpz_invert(d, e, t);
	gmp_printf("Private key\n%Zd\n", d);	
}
