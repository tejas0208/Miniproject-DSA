#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "aes.h"

/*USAGE : ./final [-d | -e | -h] <source> <destination> <keyfile> <keysize>*/

int main(int argc, char *argv[]) {
	int i, j, x, fd2, fd3, fd4, encflag = 0, decflag = 0, helpflag = 0, n;
	unsigned char *key, **word_matrix, block[16], state[4][4];
	
	if(argc != 6) {
		if(argc > 1 && strcmp(argv[1], "-h") == 0)
			helpflag = 1;
		else {
			errno = EINVAL;
			perror("USAGE : ./final [-d | -e | -h] <source> <destination> <keyfile> <keysize>\n");
			printf("Try './final -h' for more information.\n");
			return errno;
		}
	}
	
	if(strcmp(argv[1], "-h") == 0)
		helpflag = 1;
	if(helpflag) {
		printf("USAGE : ./final [OPTION] <source> <destination> <keyfile> <keysize>\n");
		printf("Encrypts source file using the AES algorithm\n");
		printf("The key of specified size is read from the file specified by the user\n");
		printf(" -e	Encrypts source file\n");
		printf(" -d	Decrypts source file\n\n");
		printf("Examples :\n");
		printf(" ./final -e file1.txt enc_file1 key1.dat 128\n");
		printf(" ./final -d enc_file2 file2.txt key2.dat 256\n");		
		return 0;
	}
	
		
	if(strcmp(argv[1], "-d") == 0)
		decflag = 1;
	else
		if(strcmp(argv[1], "-e") == 0)
		encflag = 1;
	else {
		errno = EINVAL;
		perror("USAGE : ./final [-d | -e | -h] <source> <destination> <keyfile> <keysize>");
		printf("Try './final -h' for more information.\n");
		return errno;
	}
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
	fd3 = open(argv[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
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

	if(encflag) {
		while(((x = read(fd2, block, 16)) == 16)) {
			for(i = 0; i < 16; i++)
				state[i % 4][i / 4] = block[i];
			encrypt(state, word_matrix, nr);
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					block[i + 4 * j] = state[i][j];
			write(fd3, block, 16);
		}
		pad(block, x);
		for(i = 0; i < 16; i++)
			state[i % 4][i / 4] = block[i];
		encrypt(state, word_matrix, nr);
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++)
				block[i + 4 * j] = state[i][j];
		write(fd3, block, 16);

	}
	if(decflag) {
		while((x = read(fd2, block, 16))) {
			for(i = 0; i < 16; i++)
				state[i % 4][i / 4] = block[i];
			decrypt(state, word_matrix, nr);
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					block[i + 4 * j] = state[i][j];
			write(fd3, block, 16);
		}
		lseek(fd2, -16L, SEEK_END);
		lseek(fd3, -16L, SEEK_END);
		read(fd2, block, 16);
		for(i = 0; i < 16; i++)
			state[i % 4][i / 4] = block[i];
		decrypt(state, word_matrix, nr);
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

