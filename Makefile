CC=cc
CFLAGS=-I.
DEPS = hellomake.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: custom_format.o main.o
	$(CC) -o cformat custom_format.o main.o -I.