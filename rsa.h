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

#ifndef __RSA_H
#define __RSA_H

#include "integer.h"
Integer GmpInt(mpz_t a);

void IntGmp(Integer a, mpz_t b);

void GeneratePrimes(mpz_t a, mpz_t b);

void public_key(mpz_t e, mpz_t tot);

void private_key(mpz_t d, mpz_t e, mpz_t t);

void rsa_encrypt(FILE *fp2, FILE *fp3, Integer e1, Integer n);

void rsa_decrypt(FILE *fp2, FILE *fp3, Integer d1, Integer n);

#endif
