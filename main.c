#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>
#include "aes.h"
#include "rsa.h"
#include "integer.h"

/*USAGE : ./final [-d | -e | -h] <source> <destination> <keyfile> <keysize>*/
void usage() {
	perror("USAGE : ./project [-da | -ea | -dr | -er] <source> <destination> <keyfile> <keysize(for aes only)>\n");
	printf("Try './project -h' for more information.\n");
}

void help() {
	printf("USAGE : ./project [OPTION] <source> <destination> <keyfile> <keysize(aes only)>\n");
	printf("Encrypts/Decrypts source file using the AES or the RSA algorithm\n");
	printf("For AES :\n");
	printf("	The key is read from the file specified by the user\n");
	printf("	Key size has to be specified (128 / 192 / 256)\n\n");
	printf("For RSA :\n");
	printf("	While encrypting, the key is stored in the file specified by the user\n");
	printf("	While decrypting, the key has to be read from same file\n\n");
	printf("OPTIONS :\n");
	printf(" -ea	Encrypts source file using AES\n");
	printf(" -da	Decrypts source file using AES\n\n");
	printf(" -er	Encrypts source file using RSA\n");
	printf(" -dr	Decrypts source file using RSA\n\n");
	printf("Examples :\n");
	printf("\n AES:\n");
	printf(" 	./project -ea file1.txt enc_file1 key1.dat 128\n");
	printf(" 	./project -da enc_file1 file2.txt key1.dat 128\n");
	printf("\n RSA:\n");
	printf(" 	./project -er file1.txt enc_file1 key1.dat\n");
	printf(" 	./project -dr enc_file2 file2.txt key2.dat\n");		
}

int main(int argc, char *argv[]) {
	int a_encflag = 0, a_decflag = 0, helpflag = 0, r_encflag = 0, r_decflag = 0;
	
	if(argc < 5) {
		if(argc == 2 && strcmp(argv[1], "-h") == 0)
			helpflag = 1;
		else {
			errno = EINVAL;
			usage();
			return errno;
		}
	}	
	
	if(helpflag) {
		help();
		return 0;
	}
	
	if(argc == 6) {
		if(strcmp(argv[1], "-da") == 0)
			a_decflag = 1;
		else
			if(strcmp(argv[1], "-ea") == 0)
				a_encflag = 1;
		else {
			errno = EINVAL;
			usage();
			return errno;
		}
	}
	
	else if(argc == 5) {
		if(strcmp(argv[1], "-dr") == 0)
			r_decflag = 1;
		else
			if(strcmp(argv[1], "-er") == 0)
			r_encflag = 1;
		else {
			errno = EINVAL;
			usage();
			return errno;
		}
	}

	/* argc > 6 */
	else {
		errno = EINVAL;
		usage();
		return errno;
	}

	
	/* Code  for AES algorithm */
	if(a_encflag || a_decflag) {
		int i, j, x, fd2, fd3, fd4, n;
		unsigned char *key, **word_matrix, block[16], state[4][4];
	
		fd4 = open(argv[4], O_RDONLY, S_IRUSR | S_IWUSR);
		if(fd4 == -1) {
			perror("File open failed");
			return errno;
		}
		fd2 = open(argv[2], O_RDONLY, S_IRUSR | S_IWUSR);
		if(fd2 == -1) {
			perror("File open failed");
			return errno;
		}
		fd3 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if(fd3 == -1) {
			perror("File open failed");
			return errno;
		}

		/* Check whether the key size matches the size of the file
		 * fstat shows the characteristics of a file corresponding to a file descriptor
		 * This is then used to calculate the size of the file
		 */	
		struct stat st;
		int fsize;
		fstat(fd4, &st);
		fsize = st.st_size;
		n = atoi(argv[5]);
	
		if(n != 128 && n != 192 && n != 256) {
			errno = EINVAL;
			perror("Enter valid key length (128, 192, 256)");
			printf("Try './final -h' for more information.\n");
			return errno;
		}
	
		if(fsize * 8 < n) {
			printf("Key length too small\n");
			printf("Try './final -h' for more information.\n");
			return 1;
		}
	
		key = (unsigned char*)malloc(n / 8);
		read(fd4, key, n / 8);
		int nr;
		if(n == 128)
			nr = 10;
		else if(n == 192)
			nr = 12;
		else if(n == 256)
			nr = 14;
	
		word_matrix = (unsigned char **)malloc(4 * (nr + 1) * sizeof(unsigned char *));

		for(i = 0; i < (4 * (nr + 1)); i++)
			word_matrix[i] = (unsigned char *)malloc(4);
	
		keyexp(key, word_matrix, n/8);

		if(a_encflag) {
			while(((x = read(fd2, block, 16)) == 16)) {
				for(i = 0; i < 16; i++)
					state[i % 4][i / 4] = block[i];
				aes_encrypt(state, word_matrix, nr);
				for(i = 0; i < 4; i++)
					for(j = 0; j < 4; j++)
						block[i + 4 * j] = state[i][j];
				write(fd3, block, 16);
			}
			pad(block, x);
			for(i = 0; i < 16; i++)
				state[i % 4][i / 4] = block[i];
			aes_encrypt(state, word_matrix, nr);
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					block[i + 4 * j] = state[i][j];
			write(fd3, block, 16);

		}
		if(a_decflag) {
			while((x = read(fd2, block, 16))) {
				for(i = 0; i < 16; i++)
					state[i % 4][i / 4] = block[i];
				aes_decrypt(state, word_matrix, nr);
				for(i = 0; i < 4; i++)
					for(j = 0; j < 4; j++)
						block[i + 4 * j] = state[i][j];
				write(fd3, block, 16);
			}
		
			/* To unpad read the last block of data */
			lseek(fd2, -16L, SEEK_END);
			lseek(fd3, -16L, SEEK_END);
			read(fd2, block, 16);
			for(i = 0; i < 16; i++)
				state[i % 4][i / 4] = block[i];
			aes_decrypt(state, word_matrix, nr);
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					block[i + 4 * j] = state[i][j];
			x = unpad(block);
		
			/* fstat shows the characteristics of a file corresponding to a file descriptor
			 * This is then used to calculate the size of the file
			 * Then the file is truncated corresponding to the padding bytes
			 */
		
			struct stat st;
			int size;
			fstat(fd3, &st);
			size = st.st_size;
			ftruncate(fd3, size - x);
		
		}
		close(fd2);
		close(fd3);
		close(fd4);
		for(i = 0; i < (4 * (nr + 1)); i++)
			free(word_matrix[i]);
		free(word_matrix);
		free(key);
		return 0;
	}
	
	if(r_encflag) {
		mpz_t p, q, t, e, d, product;
		Integer p1, q1, n, tot, e1;
		
		/* Opening the files to encrypt */
		FILE *fp2, *fp3, *fp4;
		fp2 = fopen(argv[2], "r");
		if(!fp2) {
			printf("File open failed\n");
			return 1;
		}
		
		fp3 = fopen(argv[3], "w");
		if(!fp3) {
			printf("File open failed\n");
			return 1;
		}

		fp4 = fopen(argv[4], "w");
		if(!fp4) {
			printf("File open failed\n");
			return 1;
		}
		
		
		/* Generate 2 512 bit prime nos */
		GeneratePrimes(p, q);
		p1 = GmpInt(p);
		q1 = GmpInt(q);
	
		/* Multiply the primes n = pq */
		n = MultiplyIntegers(p1, q1);
	
		/* Compute the totient function (p - 1)(q - 1) */
		tot = totient(p1, q1);
		IntGmp(tot, t);
	
		/* Generate the keys */
		public_key(e, t);
		private_key(d, e, t);
		e1 = GmpInt(e);	
		
		/* Encrypt */
		rsa_encrypt(fp2, fp3, e1, n);
		
		/* Store key in file */		
		IntGmp(n, product);	
		gmp_fprintf(fp4, "%Zd %Zd ", d, product);
		
		fclose(fp4);
		fclose(fp2);
		fclose(fp3);
		
		/* Free all dynamically allocated memory */
		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(t);
		mpz_clear(e);
		mpz_clear(d);
		mpz_clear(product);
		DestroyInteger(p1);
		DestroyInteger(q1);
		DestroyInteger(n);
		DestroyInteger(tot);
		DestroyInteger(e1);
		return 0;
	
	}
	
	if(r_decflag) { 
		Integer d1, n;
		char str1[500], str2[500];
		
		/* Opening the files to encrypt */
		FILE *fp2, *fp3, *fp4;
		fp2 = fopen(argv[2], "r");
		if(!fp2) {
			printf("File open failed\n");
			return 1;
		}
		
		fp3 = fopen(argv[3], "w");
		if(!fp3) {
			printf("File open failed\n");
			fclose(fp2);
			return 1;
		}

		fp4 = fopen(argv[4], "r");
		if(!fp4) {
			printf("File open failed\n");
			fclose(fp2);
			fclose(fp3);
			return 1;
		}

		/* Read private key from file */
		fscanf(fp4, "%s%s", str1, str2);
		
		d1 = CreateIntegerFromString(str1);
		n = CreateIntegerFromString(str2);
		
		/*Decrypt from fp2 to fp3 */
		rsa_decrypt(fp2, fp3, d1, n);
		
		fclose(fp2);
		fclose(fp4);
		fclose(fp3);
		
		/* Free all dynamically allocated memory */
		DestroyInteger(d1);
		DestroyInteger(n);
	
	}
	return 0;
}

