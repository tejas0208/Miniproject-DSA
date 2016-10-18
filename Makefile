final: final.o aes.o
	cc -Wall final.o aes.o -o final
final.o: final.c aes.h data.h
	cc -c -Wall final.c
aes.o: aes.c data.h
	cc -c -Wall aes.c
clean: 
	rm *.o
