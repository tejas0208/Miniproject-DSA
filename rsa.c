#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "integer.h"
#include "intgmp.h"

int main(int argc, char *argv[]) {
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
	int fd1, fd2, fd3;
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	fd3 = open(argv[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	char ch;
	mpz_t m, cipher, decipher;
	mpz_init(m);
	mpz_init(cipher);
	mpz_init(decipher);
	while(read(fd1, &ch, 1)) {
		mpz_set_ui(m, ch);
		msg = GmpInt(m);
		enc_msg = Mod_exp(msg, e1, n);
		IntGmp(enc_msg, cipher);
		write(fd2, &cipher, sizeof(mpz_t));	
	}
	
	close(fd2);
	close(fd1);
	fd2 = open(argv[2], O_RDONLY);
	
	while(read(fd2, &cipher, sizeof(mpz_t))) {
		enc_msg = GmpInt(cipher);
		dec_msg = Mod_exp(enc_msg, d1, n);
		IntGmp(dec_msg, decipher);
		ch = mpz_get_ui(decipher);
		write(fd3, &ch, 1);
	}
	/*enc_msg = Mod_exp(msg, e1, n);
	printf("Encrypted \n");
	PrintInteger(enc_msg);
	dec_msg = Mod_exp(enc_msg, d1, n);
	printf("Decrypted\n");
	PrintInteger(dec_msg);*/
	return 0;
}
