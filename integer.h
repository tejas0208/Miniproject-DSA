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

#ifndef __INTEGER_H
#define __INTEGER_H

typedef struct node {
	char digit;
	struct node *next;
	struct node *prev;
}node;

typedef struct Integer {
	node *head;
	node *tail;
	int sign;
	int size;
}Integer;


/* Create and return an Integer from a string. The string can be assumed to be a string of digits */
Integer CreateIntegerFromString(char *str);

/* This function adds two Integers a and b, and returns the sum as Integer */
Integer AddIntegers(Integer a, Integer b);

/* This function Subtracts two Integers a and b, and returns the diff as Integer */
Integer SubtractIntegers(Integer a, Integer b);

/* Print an Integer */
void PrintInteger(Integer a);

/* This function multiplies two Integers a and b, and returns the product as Integer */
Integer MultiplyIntegers(Integer a, Integer b);

/* This function divides two Integers a and b, and returns the quotient as Integer */
Integer DivideIntegers(Integer a, Integer b);

/* This function multiplies Integer a digit n, and returns the product as Integer */
Integer mult_dig(Integer a, unsigned int n);

/* This function creates a string from an integer*/
char * IntegertoString(Integer a);

/* This function returns a copy of integer a*/
Integer Copy(Integer a);

/* This function divides two Integers a and b, and returns the remainder as Integer */
Integer Mod(Integer a, Integer b);

int IsZero(Integer a);

int IsOdd(Integer a);

/* Computes x ^ Y mod m */
Integer Mod_exp(Integer x, Integer y, Integer m);

Integer totient(Integer p, Integer q);

void DestroyInteger(Integer a);
#endif
