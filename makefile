# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = mundo.o libconjunto.o  liblistaord.o libfila.o 
     
all: mundo.o libconjunto.o  liblef.o libfila.o libeventos.o 
	$(CC) -o mundo mundo.o libconjunto.o  liblef.o libfila.o libeventos.o $(LDFLAGS)

libconjunto.o: libconjunto.c
	$(CC) -c $(CFLAGS) libconjunto.c

liblef.o: liblef.c
	$(CC) -c $(CFLAGS) liblef.c

libfila.o: libfila.c
	$(CC) -c $(CFLAGS) libfila.c

libeventos.o: libeventos.c
	$(CC) -c $(CFLAGS) libeventos.c

mundo.o: mundo.c
	$(CC) -c $(CFLAGS) mundo.c

clean:
	rm -f $(objects) mundo


