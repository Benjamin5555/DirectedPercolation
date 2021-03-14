CC=gcc
CFLAGS= -std=c99 -fopenmp -O3 -lm -I.

DEPS= denominatorPreCompute.h parameter.h
OBJ=LRDP.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

LRDP: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)



clean:
	rm *.o LRDP
