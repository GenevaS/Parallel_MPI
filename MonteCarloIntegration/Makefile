# ---------------------------------------------------------
# Geneva Smith (0964909)
# Makefile
# ---------------------------------------------------------
# This makefile creates an executable MPI program called
# a1 using main.c, parallel-monte-carlo.c, monte-carlo.c,
# fcn.c, and check-result.c. It requires the math library.
# ---------------------------------------------------------

CC = mpicc
CFLAGS = -Wall -O2 -g
LDFLAGS = -lm

OBJECTS1 = main.o fcn.o monte-carlo.o parallel-monte-carlo.o check-result.o

all: a1

a1: $(OBJECTS1)
	$(CC) -o a1 $(OBJECTS1) $(LDFLAGS)

clean:
	rm *.o a1
