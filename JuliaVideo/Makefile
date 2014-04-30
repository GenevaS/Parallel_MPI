# ---------------------------------------------------------
# Geneva Smith
# JuliaVideo - Makefile
# ---------------------------------------------------------
# This makefile creates an executable MPI program called
# julia using main.c, getparams.c, julia-gmp.c, and 
# savebmp.c. The file main.c also requires julia.h.
#
# It requires the math, GMP, and OpenMP libraries. OpenMP
# must be added as both a compiler flag and a link flag.
# ---------------------------------------------------------

CC = mpicc
CFLAGS= -g -Wall -O2 -fopenmp
LDFLAGS = -lgmp -lm -fopenmp

OBJS =  main.o julia-gmp.o savebmp.o getparams.o

video: $(OBJS)
	$(CC) -o video $(OBJS) $(LDFLAGS)

# ---------------------------------------------------------	
# Running program
# ---------------------------------------------------------	

run: 
	mpirun -np 8 ./video seahorse.dat

# ---------------------------------------------------------	
# clean - uncomment *.bmp to remove bitmap files as well
# ---------------------------------------------------------	
clean:
	@rm -rf $(OBJS) video *~ *.bak #*.bmp