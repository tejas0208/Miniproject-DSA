#ifndef __AES_H
#define __AES_H
void subbyte(unsigned char state[4][4]);

void invsubbyte(unsigned char state[4][4]);

void keyexp(unsigned char key[16], unsigned char **word_matrix, int n);

unsigned char * subword(unsigned char arr[4]);

unsigned char * rotword(unsigned char arr[4]);

void invmixcol(unsigned char state[4][4]);

void matrixmult(unsigned char arr[4][4], unsigned char state[4][4]);

void mixcol(unsigned char state[4][4]);

void shiftr(unsigned char state[4][4]);

void invshiftr(unsigned char state[4][4]);

int readline(unsigned char *arr, int len);

void addroundkey(unsigned char state[4][4], unsigned char **word_matrix, int k);

void encrypt(unsigned char state[4][4], unsigned char **word_matrix, int nr);

void decrypt(unsigned char state[4][4], unsigned char **word_matrix, int nr);

void pad(unsigned char block[16], int x);

int unpad(unsigned char block[16]);

#endif
