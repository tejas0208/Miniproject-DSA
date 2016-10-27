#include <gmp.h>
#include <stdio.h>
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

/*int main() {
	char str[100000];
	Integer a, c;
	mpz_t b;
	scanf("%s", str);
	a = CreateIntegerFromString(str);
	PrintInteger(a);
	IntGmp(a, b);
	gmp_printf("%Zd\n", b);
	c = GmpInt(b);
	PrintInteger(c);
	return 0;
}*/
