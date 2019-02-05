CC=gcc
CPP=g++
CFLAGS=-O2 -std=c99 -g -Wall -pedantic
CXXFLAGS=-O2 -std=c++11 -Wall -pedantic

all: tail wordcount

tail2.o: tail2.cc
	$(CPP) $(CXXFLAGS) -c tail2.cc
tail.o: tail.c tail.h
	$(CC) $(CFLAGS) -c tail.c
tail: tail.o tail2.o
	$(CC) $(CFLAGS) -o tail tail.o
	$(CPP) $(CXXFLAGS) -o tail2 tail2.o
htablelibs: htable.h htab_bucket_count.c htab_clear.c htab_find.c htab_foreach.c htab_free.c htab_init.c htab_lookup_add.c htab_move.c htab_remove.c htab_size.c
	$(CC) $(CFLAGS) -fPIC -c htab_bucket_count.c -o htab_bucket_count-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_clear.c -o htab_clear-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_find.c -o htab_find-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_foreach.c -o htab_foreach-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_free.c -o htab_free-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_init.c -o htab_init-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c -o htab_lookup_add-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_move.c -o htab_move-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_remove.c -o htab_remove-shared.o
	$(CC) $(CFLAGS) -fPIC -c htab_size.c -o htab_size-shared.o
	$(CC) $(CFLAGS) -shared -o libhtab.so htab_bucket_count-shared.o htab_clear-shared.o htab_find-shared.o htab_foreach-shared.o htab_free-shared.o htab_init-shared.o htab_lookup_add-shared.o htab_move-shared.o htab_remove-shared.o htab_size-shared.o
	
	$(CC) $(CFLAGS) -c htab_bucket_count.c -o htab_bucket_count-static.o
	$(CC) $(CFLAGS) -c htab_clear.c -o htab_clear-static.o
	$(CC) $(CFLAGS) -c htab_find.c -o htab_find-static.o
	$(CC) $(CFLAGS) -c htab_foreach.c -o htab_foreach-static.o
	$(CC) $(CFLAGS) -c htab_free.c -o htab_free-static.o
	$(CC) $(CFLAGS) -c htab_init.c -o htab_init-static.o
	$(CC) $(CFLAGS) -c htab_lookup_add.c -o htab_lookup_add-static.o
	$(CC) $(CFLAGS) -c htab_move.c -o htab_move-static.o
	$(CC) $(CFLAGS) -c htab_remove.c -o htab_remove-static.o
	$(CC) $(CFLAGS) -c htab_size.c -o htab_size-static.o
	ar rcs libhtab.a htab_bucket_count-static.o htab_clear-static.o htab_find-static.o htab_foreach-static.o htab_free-static.o htab_init-static.o htab_lookup_add-static.o htab_move-static.o htab_remove-static.o htab_size-static.o 
	ranlib libhtab.a

wordcount.o: wordcount.c
	$(CC) $(CFLAGS) -c wordcount.c
io.o: io.c
	$(CC) $(CFLAGS) -c io.c
wordcount: htablelibs wordcount.o io.o
	$(CC) $(CFLAGS) -o wordcount-dynamic wordcount.o io.o -L. -lhtab
	$(CC) $(CFLAGS) -o wordcount wordcount.o io.o libhtab.a

clean:
	rm -f tail tail2 wordcount-dynamic wordcount htab_*-shared.o htab_*-static.o xpetra19.zip libhtab.a libhtab.so

zip: *.c *.cc *.h Makefile
	zip xpetra19.zip *.c *.cc *.h Makefile
