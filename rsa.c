#include <stdio.h>
#include <gmp.h>
#include "integer.h"
#include "intgmp.h"

int main() {
	mpz_t p, q, t, e, d;
	Integer p1, q1, n, tot, e1, d1, msg, enc_msg, dec_msg;
	prime2(p, q);
	p1 = GmpInt(p);
	q1 = GmpInt(q);
	n = MultiplyIntegers(p1, q1);
	PrintInteger(n);
	tot = totient(p1, q1);
	IntGmp(tot, t);
	public_key(e, t);
	private_key(d, e, t);
	e1 = GmpInt(e);
	d1 = GmpInt(d);
	//FIND d BY USING GMP LIBRARY FUNCTION
	char str[1000];
	scanf("%s", str);
	msg = CreateIntegerFromString(str);
	printf("\n Msg is\n");
	PrintInteger(msg);
	enc_msg = Mod_exp(msg, e1, n);
	printf("Encrypted \n");
	PrintInteger(enc_msg);
	dec_msg = Mod_exp(enc_msg, d1, n);
	printf("Decrypted\n");
	PrintInteger(dec_msg);
	return 0;
}
