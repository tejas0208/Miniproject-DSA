project: main.o aes.o rsa.o integer.o
	cc -Wall main.o rsa.o integer.o aes.o -o project -lgmp
	rm *.o
main.o: main.c aes.h rsa.h integer.h
	cc -c -Wall main.c
rsa.o: rsa.c integer.h rsa.h
	cc -c -Wall rsa.c
integer.o: integer.c integer.h rsa.h
	cc -c -Wall integer.c
aes.o: aes.c data.h
	cc -c -Wall aes.c
clean: 
	rm project
