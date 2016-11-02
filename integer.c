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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "integer.h"
#include "rsa.h"

/* Create and return an Integer from a string. The string can be assumed to be a string of digits */
Integer CreateIntegerFromString(char *str) {
	int n, i;
	Integer a;
	node *tmp;
	if(str[0] == '-') {
		a.sign = 1;
		i = 1;
	}
	else {
		a.sign = 0;
		i = 0;
	}	
	
	n = strlen(str);
	a.head = a.tail = NULL;
	a.size = 0;
	for(; i < n; i++) {
		tmp = (node *)malloc(sizeof(node));
		tmp->digit = str[i];	
		if(a.head == NULL) {
			if(str[i] == '0' && n != 1)
				continue;
			tmp->next = tmp->prev =
				a.head = a.tail = tmp;
			a.size++;
			continue;
		}
		tmp->next = a.tail->next;
		tmp->prev = a.tail;
		a.tail->next->prev = tmp;
		a.tail->next = tmp;
		a.tail = tmp;
		a.size++;
	}
	return a;
}

void DestroyInteger(Integer a) {
	node *p;
	p = a.head;
	if(p == NULL)
		return;
	while(p != a.tail) {
		a.head = a.head->next;
		free(p);
		p = a.head;
	}
	free(p);
}
/* This function adds two Integers a and b, and returns the sum as Integer */
Integer AddIntegers(Integer a, Integer b) {
	Integer ans, a1, b1;
	int max, i;
	node *p, *q, *r, *tmp;
	char carry = '0';
	a1 = Copy(a);
	b1 = Copy(b);
	if((a1.sign == 0) && b1.sign == 1) {
		b1.sign =0;
		ans = SubtractIntegers(a1, b1);
		return ans;
	
	}
	
	if((a1.sign == 1) && b1.sign == 0){
		a1.sign = 0;
		ans = SubtractIntegers(b1, a1);
		return ans;
	}
	
	if((a1.sign == 1) && b1.sign == 1)
		ans.sign = 1;
	if(a1.size > b1.size) {
		max = a1.size;
		for(i = 1; i <= a1.size - b1.size; i++) {
			tmp = (node *)malloc(sizeof(node));
			tmp->digit = '0';
			p = b1.tail;
			tmp->next = p->next;
			tmp->prev = p;
			p->next->prev = tmp;
			p->next = tmp;
			b1.head = tmp;
		}
	}
	else {
		max = b1.size;
		for(i = 1; i <= b1.size - a1.size; i++) {
			tmp = (node *)malloc(sizeof(node));
			tmp->digit = '0';
			p = a1.tail;
			tmp->next = p->next;
			tmp->prev = p;
			p->next->prev = tmp;
			p->next = tmp;
			a1.head = tmp;
		}
	}
	ans.head = ans.tail = NULL;
	ans.size = 0;
	
	for(i = 0; i < max; i++) {
		tmp = (node *)malloc(sizeof(node));
		tmp->digit = '0';	
		if(ans.head == NULL) {
			tmp->next = tmp->prev =
				ans.head = ans.tail = tmp;
			ans.size++;
			continue;
		}
		tmp->next = ans.tail->next;
		tmp->prev = ans.tail;
		ans.tail->next->prev = tmp;
		ans.tail->next = tmp;
		ans.tail = tmp;
		ans.size++;
	}
	
	
	p = a1.tail;
	q = b1.tail;
	r = ans.tail;
	do {
		r->digit = p->digit + q->digit + carry -'0' - '0';
		if(r->digit > '9') {
			r->digit = r->digit - 10;
			carry = '1';
		}
		else
			carry = '0';
		p = p->prev;
		q = q->prev;
		r = r->prev;
		
	}while(p != a1.tail);
	
	if(carry == '1') {
		tmp = (node *)malloc(sizeof(node));
		tmp->digit = '1';
		p = ans.tail;
		tmp->next = p->next;
		tmp->prev = p;
		p->next->prev = tmp;
		p->next = tmp;
		ans.head = tmp;
		ans.size++;	
	}
	return ans;
	
}

/* This function Subtracts two Integers a and b, and returns the sum as Integer */
Integer SubtractIntegers(Integer a, Integer b) {
	Integer ans, a1, b1;
	int max, i, greater;
	node *p, *q, *r, *tmp;
	char borrow = '0', x, y;
	
	a1 = Copy(a);
	b1 = Copy(b);
	if(a1.sign == 1 && b1.sign == 0) {
		b1.sign = 1;
		ans = AddIntegers(a1, b1);
		return ans;
	}
	
	if(a1.sign == 0 && b1.sign == 1) {
		b1.sign = 0;
		ans = AddIntegers(a1, b1);
		return ans;
	}
	
	if(a1.sign == 1 && b1.sign == 1) {
		b1.sign = 0;
		a1.sign = 0;
		ans = SubtractIntegers(b1, a1);
		return ans;
	}
	
	if(a1.size > b1.size) {
		greater = 1;
		max = a1.size;
		for(i = 1; i <= a1.size - b1.size; i++) {
			tmp = (node *)malloc(sizeof(node));
			tmp->digit = '0';
			p = b1.tail;
			tmp->next = p->next;
			tmp->prev = p;
			p->next->prev = tmp;
			p->next = tmp;
			b1.head = tmp;
		}
	}
	else if(a1.size < b1.size) {
		greater = 2;
		max = b1.size;
		for(i = 1; i <= b1.size - a1.size; i++) {
			tmp = (node *)malloc(sizeof(node));
			tmp->digit = '0';
			p = a1.tail;
			tmp->next = p->next;
			tmp->prev = p;
			p->next->prev = tmp;
			p->next = tmp;
			a1.head = tmp;
		}
	}
	else {
		max = a1.size;
		p = a1.head;
		q = b1.head;
		while(p->digit == q->digit && p != a1.tail) {
			p = p->next;
			q = q->next;
		}
		x = p->digit;
		y = q->digit;
		if(x >= y)
			greater = 1;
		
		else 
			greater = 2;
	}
	
	ans.head = ans.tail = NULL;
	ans.size = 0;
	
	for(i = 0; i < max; i++) {
		tmp = (node *)malloc(sizeof(node));
		tmp->digit = '0';	
		if(ans.head == NULL) {
			tmp->next = tmp->prev =
				ans.head = ans.tail = tmp;
			ans.size++;
			continue;
		}
		tmp->next = ans.tail->next;
		tmp->prev = ans.tail;
		ans.tail->next->prev = tmp;
		ans.tail->next = tmp;
		ans.tail = tmp;
		ans.size++;
	}
	
	if(greater == 1) {
		p = a1.tail;
		q = b1.tail;
		ans.sign = 0;
	}
	else {
		q = a1.tail;
		p = b1.tail;
		ans.sign = 1;
	}
		
	r = ans.tail;
	do {
		r->digit = p->digit - q->digit - borrow + '0' + '0';
		if(r->digit < '0') {
			r->digit = r-> digit + 10;
			borrow = '1';
		}
		else
			borrow = '0';
		p = p->prev;
		q = q->prev;
		r = r->prev;
		
	}while(p != a1.tail && p != b1.tail);
	
	
	return ans;
	
}

/* Print an Integer */
void PrintInteger(Integer a) {
	node *p;
	p = a.head;
	int count = 0;
	if(a.sign == 1)
		printf("-");
	do {
		if(p->digit != '0' || count != 0 || p == a.tail) {
			printf("%c", p->digit);
			count = 1;
		}
		p = p->next;
	} while(p != a.head) ;
	printf("\n");
}

Integer mult_dig(Integer a, unsigned int n) {
	Integer ans;
	int max, i, x;
	node *p, *r, *tmp;
	int carry = 0;
	max = a.size;
	ans.head = ans.tail = NULL;
	ans.size = 0;
	ans.sign = 0;
	
	for(i = 0; i < max; i++) {
		tmp = (node *)malloc(sizeof(node));
		tmp->digit = '0';	
		if(ans.head == NULL) {
			tmp->next = tmp->prev =
				ans.head = ans.tail = tmp;
			ans.size++;
			continue;
		}
		tmp->next = ans.tail->next;
		tmp->prev = ans.tail;
		ans.tail->next->prev = tmp;
		ans.tail->next = tmp;
		ans.tail = tmp;
		ans.size++;
	}
	
	
	p = a.tail;
	r = ans.tail;
	do {
		x = ((p->digit - '0') * n + carry);
		if(x > 9) {
			r->digit = (x % 10) + '0';
			carry = x / 10;
		}
		else {
			carry = 0;
			r->digit = x + '0';
		}
		p = p->prev;
		r = r->prev;
		
	}while(p != a.tail);
	
	if(carry != 0) {
		tmp = (node *)malloc(sizeof(node));
		tmp->digit = carry + '0';
		p = ans.tail;
		tmp->next = p->next;
		tmp->prev = p;
		p->next->prev = tmp;
		p->next = tmp;
		ans.head = tmp;
		ans.size++;	
	}
	return ans;
	

}

Integer MultiplyIntegers(Integer a, Integer b) {
	Integer x, ans;
	node *p, *temp;
	p = b.tail;
	int i, j;
	ans = CreateIntegerFromString("0");
	for(i = 0; i < b.size; i++) {
		x = mult_dig(a, (p->digit - '0'));
		for(j = 0; j < i; j++) {
			temp = (node *)malloc(sizeof(node));
			temp->digit = '0';
			temp->prev = x.tail;
			temp->next = x.head;
			x.head->prev = temp;
			x.tail->next = temp;
			x.tail = temp;
			x.size++;
		}
		ans = AddIntegers(ans, x);
		p = p->prev;
	}
	ans.sign = ((a.sign + b.sign) % 2);
	return ans;
}

Integer Copy(Integer a) {
	Integer ans;
	char *str;
	str = IntegertoString(a);
	ans = CreateIntegerFromString(str);
	return ans;
}

Integer append(Integer a, char c) {
	Integer ans;
	node *temp;
	temp = (node *)malloc(sizeof(node));
	temp->digit = c;
	ans = Copy(a);
	ans.tail->next = temp;
	ans.head->prev = temp;
	temp->next = ans.head;
	temp->prev = ans.tail;
	ans.tail = temp;
	ans.size++;
	return ans;
}

Integer DivideIntegers(Integer a, Integer b) {
	Integer ans, div, f, tmp;
	char *str;
	int i;
	node *y;
	ans = CreateIntegerFromString("0");
	if(SubtractIntegers(a, b).sign == 1) {
		return ans;
	}
	y = a.head;
	for(i = 0; i < b.size - 1; i++)
		y = y->next;

	str = IntegertoString(a);
	str[b.size] = '\0';
	div = CreateIntegerFromString(str);
	while(1) {
		for(i = 1; i <= 9; i++) {
			f = mult_dig(b, i);
			tmp = SubtractIntegers(div, f);
			if(tmp.sign == 1)
				break;
		}
		f = mult_dig(b, i - 1);
		div = SubtractIntegers(div, f);
		ans = append(ans, (i - 1) + '0');
		y = y->next;	
		if(y == a.head)
			break;
		div = append(div, y->digit);
		
	}
	return ans;
}

Integer Mod(Integer a, Integer b) {
	Integer q, p, ans;
	q = DivideIntegers(a, b);
	p = MultiplyIntegers(b, q);
	ans = SubtractIntegers(a, p);
	return ans;
}

Integer Mod_exp(Integer a, Integer b, Integer m) {
	Integer ans;
	mpz_t x, y, mod, rop;
	mpz_init(rop);
	IntGmp(a, x);
	IntGmp(b, y);
	IntGmp(m, mod);
	mpz_powm(rop, x, y, mod);
	ans = GmpInt(rop);
	return ans;
	
}

Integer totient(Integer p, Integer q) {
	Integer unity = CreateIntegerFromString("1");
	return MultiplyIntegers(SubtractIntegers(p, unity), SubtractIntegers(q, unity));
}

int IsOdd(Integer a) {
	if(a.tail == NULL)
		return 0;
	return ((a.tail->digit - '0') % 2);
}

int IsZero(Integer a) {
	node *p = a.head;
	if(!p) {
		return 0;
	}
	do {
		if(p->digit != '0')
			return 0;
		p = p->next;
	} while(p != a.head) ;
	return 1;
}

char * IntegertoString(Integer a) {
	char *str, *t;
	str = malloc(a.size + 2); //One extra byte for the minus sign
	t = str;
	node *p;
	p = a.head;
	int count = 0;
	if(a.sign == 1) {
		sprintf(t, "-");
		t++;
	}
	do {
		if(p->digit != '0' || count != 0 || p == a.tail) {
			sprintf(t, "%c", p->digit);
			count = 1;
			t++;
		}
		p = p->next;
	} while(p != a.head) ;
	return str;
}

