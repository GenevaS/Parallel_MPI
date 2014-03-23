# --------------------------------------------------------------------
# Geneva Smith
# Makefile
# --------------------------------------------------------------------
# This makefile creates an executable MPI program called
# julia using main.c, getparams.c, parallel-julia.c, 
# partition-julia.c, master-julia.c, julia.c, and savebmp.c. 
# It requires the math and GMP libraries.
# --------------------------------------------------------------------

CC = mpicc
CFLAGS= -g -Wall -O2
LDFLAGS = -lgmp -lm

OBJS =  main.o julia.o savebmp.o parallel-julia.o partition-julia.o master-julia.o getparams.o  

julia: $(OBJS)
	$(CC) -o julia $(OBJS) $(LDFLAGS)

# --------------------------------------------------------------------	
# Running program
# --------------------------------------------------------------------

run1: 
	mpirun -np 1 ./julia params.dat #; gthumb image.bmp

run2: 
	mpirun -np 2 ./julia params.dat #; gthumb image.bmp

run4: 
	mpirun -np 4 ./julia params.dat #; gthumb image.bmp

run8: 
	mpirun -np 8 ./julia params.dat #; gthumb image.bmp

run16: 
	mpirun -np 16 ./julia params.dat #; gthumb image.bmp

run32: 
	mpirun -np 32 ./julia params.dat #; gthumb image.bmp

run64: 
	mpirun -np 64 ./julia params.dat #; gthumb image.bmp

run128: 
	mpirun -np 128 ./julia params.dat #; gthumb image.bmp

run256: 
	mpirun -np 256 ./julia params.dat #; gthumb image.bmp

run512: 
	mpirun -np 512 ./julia params.dat #; gthumb image.bmp

run1024: 
	mpirun -np 1024 ./julia params.dat #; gthumb image.bmp

# --------------------------------------------------------------------
# clean
# --------------------------------------------------------------------
clean:
	@rm -rf $(OBJS) julia *~ *.bak *.bmp