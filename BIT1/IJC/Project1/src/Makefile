CC=gcc
CFLAGS=-O2 -std=c99 -g -Wall -pedantic
all: primes steg-decode

primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) -o primes primes.o eratosthenes.o error.o -lm
	$(CC) $(CFLAGS) -DUSE_INLINE -o primes-i primes.o eratosthenes.o error.o -lm
primes.o: primes.c
	$(CC) $(CFLAGS) -c primes.c
eratosthenes.o: eratosthenes.c
	$(CC) $(CFLAGS) -c eratosthenes.c
error.o: error.c
	$(CC) $(CFLAGS) -c error.c
ppm.o: ppm.c
	$(CC) $(CFLAGS) -c ppm.c
steg-decode: steg-decode.o eratosthenes.o ppm.o error.o
	$(CC) $(CFLAGS) -o steg-decode steg-decode.o eratosthenes.o ppm.o error.o -lm
clean : 
	rm -f steg-decode primes primes-i xpetra19.zip
run: primes primes-i
	time ./primes
	time ./primes-i

zip: *.c *.h Makefile
	zip xpetra19.zip *.c *.h Makefile
