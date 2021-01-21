# Makefile for DirectedPercolation code

#
# C compiler and options for Intel
#
CC=     gcc  -fopenmp -std=c99 
LIB=    -lm

#
# Object files
#
OBJ=    DirectedPercolation.o

#
# Compile
#
DirectedPercolation:   $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB)

.c.o:
	$(CC) -c $<

#
# Clean out object files and the executable.
#
clean:
	rm *.o DirectedPercolation
