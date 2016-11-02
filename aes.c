#include <stdio.h>
#include <stdlib.h>
#include "aes.h"
#include "data.h"

void subbyte(unsigned char state[4][4]) {
	int i, j, a, b;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++) {
			a = state[i][j] >> 4;/* Get the upper nibble of the 16 bit number*/
			b = state[i][j] & 0x0f;/*Get the lower nibble of the 16 bit number*/
			state[i][j] = sbox[a][b];
		}
}
			
void invsubbyte(unsigned char state[4][4]) {
	int i, j, a, b;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++) {
			a = state[i][j] >> 4;
			b = state[i][j] & 0x0f;
			state[i][j] = inv_sbox[a][b];
		}
}

void invshiftr(unsigned char state[4][4]) {
	unsigned char orig[4][4];
	int i, j, mag;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			orig[i][j] = state[i][j]; /*Create a copy of the state matrix*/
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++) {
			mag = j - i;
			if(mag < 0)
				mag += 4;
			state[i][j] = orig[i][mag];
		}
}

void shiftr(unsigned char state[4][4]) {
	unsigned char orig[4][4];
	int i, j, mag;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			orig[i][j] = state[i][j]; /*Create a copy of the state matrix*/
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++) {
			mag = j - i;
			if(mag < 0)
				mag += 4;
			state[i][mag] = orig[i][j];
		}
}

void mixcol(unsigned char state[4][4]) {
	matrixmult(mix, state);
}

void invmixcol(unsigned char state[4][4]) {
	matrixmult(inv_mix, state);
}

unsigned char multiply(unsigned char a, unsigned char b) {
	unsigned char a_coeff[8], result;
	int i, step[8];
	for(i = 0; i < 8; i++) {
		a_coeff[i] = a & 0x01;
		a = a >> 1;
	}
	
	step[0] = b;
	for(i = 1; i < 8; i++) {
		b = b << 1;
		if(step[i - 1] >> 7)
			b = b ^ 0x1b;
		step[i] = b;
	}
	
	result = 0;
	
	for(i = 0; i < 8; i++) {
		if(a_coeff[i])
			result = result ^ step[i];
	}
	return result;
}		

void matrixmult(unsigned char arr[4][4], unsigned char state[4][4]) {
	unsigned char temp[4][4], sum;
	int i, j, k;
	
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			temp[i][j] = state[i][j];
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++) {
			sum = 0;
			for(k = 0; k < 4; k++)
				sum = sum ^ multiply(arr[i][k], temp[k][j]);
			state[i][j] = sum;
		}
}

unsigned char * rotword(unsigned char arr[4]) {
	int i;
	unsigned char * result;
	result = (unsigned char *)malloc(4);
	result[3] = arr[0];
	for(i = 1; i < 4; i++)
		result[i - 1] = arr[i];
	return result;
}

unsigned char * subword(unsigned char arr[4]) {
	int i;
	unsigned char * result;
	result = (unsigned char *)malloc(4);
	for(i = 0; i < 4; i++)
		result[i] = sbox[i >> 4][i & 0x0f];
	return result;
}


int readline(unsigned char *arr, int len) {
	int i = 0;
	unsigned char ch;
	while((ch = getchar()) != '\n' && i < len) {
		arr[i] = ch;
		i++;
	}
	return i;
}

void pad(unsigned char block[16], int x) {
	int i;
	i = x;
	x = 16 - x;
	for(; i < 16; i++)
		block[i] = x;
}

int unpad(unsigned char block[16]) {
	int x;
	x = block[15];
	return x;
}

void aes_encrypt(unsigned char state[4][4], unsigned char **word_matrix, int nr) {
	int i;
	addroundkey(state, word_matrix, 0);
	for(i = 1; i <= nr; i++) {
		subbyte(state);
		shiftr(state);
		if(i != nr)
			mixcol(state);
		addroundkey(state, word_matrix, 4 * i);
	}
}

void addroundkey(unsigned char state[4][4], unsigned char **word_matrix, int k) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			state[j][i] = state[j][i] ^ word_matrix[k + i][j];
		}
	}
}


void keyexp(unsigned char *key, unsigned char **word_matrix, int n) {
	int i, j;
	unsigned char temp[4], *operate;
	if(n == 16) {
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++)
				word_matrix[i][j] = key[i * 4 + j];
		for(i = 4; i < 44; i++) {
			if(i % 4)
				for(j = 0; j < 4; j++)
					word_matrix[i][j] = word_matrix[i - 1][j] ^ word_matrix[i - 4][j];
			else {
				operate = subword(rotword(word_matrix[i - 1]));
				for(j = 0; j < 4; j++) {
					temp[j] = operate[j] ^ rcon[i / 4 - 1];
					word_matrix[i][j] = temp[j] ^ word_matrix[i - 4][j];
				}
			}
		}
	}
	
	else if(n == 24) {
		for(i = 0; i < 6; i++)
			for(j = 0; j < 4; j++)
				word_matrix[i][j] = key[i * 4 + j];
		for(i = 6; i < 52; i++) {
			if(i % 6)
				for(j = 0; j < 4; j++)
					word_matrix[i][j] = word_matrix[i - 1][j] ^ word_matrix[i - 6][j];
			else {
				operate = subword(rotword(word_matrix[i - 1]));
				for(j = 0; j < 4; j++) {
					temp[j] = operate[j] ^ rcon[i / 4 - 1];
					word_matrix[i][j] = temp[j] ^ word_matrix[i - 6][j];
				}
			}
		}
	}
	else  {
		for(i = 0; i < 8; i++)
			for(j = 0; j < 4; j++)
				word_matrix[i][j] = key[i * 4 + j];
		for(i = 8; i < 60; i++) {
			if(i % 8)
				for(j = 0; j < 4; j++)
					word_matrix[i][j] = word_matrix[i - 1][j] ^ word_matrix[i - 8][j];
			else {
				operate = subword(rotword(word_matrix[i - 1]));
				for(j = 0; j < 4; j++) {
					temp[j] = operate[j] ^ rcon[i / 4 - 1];
					word_matrix[i][j] = temp[j] ^ word_matrix[i - 4][j];
				}
			}
			if((i % 4) == 0 && (i % 8 != 0)) {
				for(j = 0; j < 4; j++)
					word_matrix[i][j] = subword(word_matrix[i - 1])[j] ^ word_matrix[i - 8][j];
			}
		}
	}
}

void aes_decrypt(unsigned char state[4][4], unsigned char **word_matrix, int nr) {
	int i;
	addroundkey(state, word_matrix, 4 * nr);
	for(i = 1; i <= nr; i++) {
		invshiftr(state);
		invsubbyte(state);
		addroundkey(state, word_matrix, 4 * nr - 4 * i);
		if(i != nr)
			invmixcol(state);

	}
}
