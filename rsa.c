/*****************************************************************************
 * Copyright (C) 2016 Tejas U. Nayak tejasunayak@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include "integer.h"
#include "rsa.h"

/* Converts the gmp library data type to my data type */
Integer GmpInt(mpz_t a) {
	Integer b;
	char *str = NULL;
	str = mpz_get_str(str, 10, a);
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

void GeneratePrimes(mpz_t a, mpz_t b) {
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
	prime = 0;
	while(prime != 1) {
		mpz_urandomb(b, q, 512);
		prime = mpz_probab_prime_p(b, 25);
	}
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
}

void private_key(mpz_t d, mpz_t e, mpz_t t) {
	mpz_init(d);
	mpz_invert(d, e, t);
}
/* Function for encrypting the file */
void rsa_encrypt(FILE *fp2, FILE *fp3, Integer e1, Integer n) {
	char ch;
	Integer msg, enc_msg; 
	mpz_t m, cipher;
	mpz_init(m);
	mpz_init(cipher);
	while(fscanf(fp2, "%c", &ch) != -1) {
		mpz_set_ui(m, ch);
		msg = GmpInt(m);
		enc_msg = Mod_exp(msg, e1, n);
		IntGmp(enc_msg, cipher);
		gmp_fprintf(fp3, "%Zd ", cipher);	
	}
	
	/* Free all dynamically allocated memory */
	mpz_clear(m);
	mpz_clear(cipher);
	DestroyInteger(msg);
	DestroyInteger(enc_msg);
}

/* Function for decrypting the file */
void rsa_decrypt(FILE *fp2, FILE *fp3, Integer d1, Integer n) {
	char ch;
	Integer enc_msg, dec_msg;
	mpz_t cipher, decipher;
	mpz_init(cipher);
	mpz_init(decipher);
	while(gmp_fscanf(fp2, "%Zd", cipher) != -1) {
		enc_msg = GmpInt(cipher);
		dec_msg = Mod_exp(enc_msg, d1, n);
		IntGmp(dec_msg, decipher);
		ch = mpz_get_ui(decipher);
		fprintf(fp3, "%c", ch);
	}
	
	/* Free all dynamically allocated memory */
	mpz_clear(decipher);
	mpz_clear(cipher);
	DestroyInteger(dec_msg);
	DestroyInteger(enc_msg);

}
