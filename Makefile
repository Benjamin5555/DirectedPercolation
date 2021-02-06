# Makefile for DirectedPercolation code

#
# C compiler and options 
#
CC=     gcc  -fopenmp  
LIB=    -lm -Iparameter.h

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
