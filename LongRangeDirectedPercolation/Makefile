## Makefile for DirectedPercolation code
#
##
## C compiler and options 
##
#CC=     gcc  -fopenmp -o3
#LIB=    -lm -iparameter.h 
#
#
##
## Object files
##
#OBJ=    LRDP.o 
#HEAD=   denominatorPreCompute.h 
#
#
##
## Compile
##
#LRDP:   $(OBJ) $(HEAD)
#	$(CC) -o $@ $(OBJ) $(LIB)
#
#.c.o:
#	$(CC) -c $<



CC=gcc
CFLAGS=-fopenmp -g -lm -I.

DEPS = denominatorPreCompute.h parameter.h
OBJ = LRDP.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

LRDP: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


#
# Clean out object files and the executable.
#
clean:
	rm *.o LRDP


