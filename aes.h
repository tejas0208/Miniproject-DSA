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

void aes_encrypt(unsigned char state[4][4], unsigned char **word_matrix, int nr);

void aes_decrypt(unsigned char state[4][4], unsigned char **word_matrix, int nr);

void pad(unsigned char block[16], int x);

int unpad(unsigned char block[16]);

#endif
