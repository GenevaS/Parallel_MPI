Programs were developed using the MPICH2 implementation of MPI and the GMP
library.

Compiled in the Linux terminal using the Makefile

Creates an executable file - julia

This program computes the Mandelbrot or Julia set of a complex number and
generates the associated image as a .bmp file. Statistics about each
are collected and written to a .txt file.

The file <image.bmp> was created using this program.

The Mandelbrot equation is (z[k+1] = z[k]^s + z[0]), k >= 0
The Julia equation is (z[k+1] = z[k]^s + c), k >= 0
 * The variables z and c represent complex numbers a + ib

Julia is envoked with: 
	mpirun -np <#processes> ./julia <parameterFile>
	
	where the <parameterFile> contains the following parameters on separate lines:
		* flag determines which set is generated. If flag = 0, the Mandelbrot set is
		  computed. If flag = 1, the Julia set is computed.
		* cr - the real components used to calculate the Julia set. If flag = 0, this 
		  parameter is ignored, but must be included in the file.
		* ci - the imaginary components used to calculate the Julia set. If flag = 0, 
		  this parameter is ignored, but must be included in the file.
		* x coordinate of the center point of a circle in the Madelbrot domain
		* y coordinate of the center point of a circle in the Madelbrot domain
		* xr - horizontal distance from the x coordinate (real)
		* yr - vertical distance from the y coordinate (imaginary)
		* height of the image in pixels
		* width of the image in pixels
		* maxit is the maximum number of hops that the program will try on each pixel 
		  in the image.
		* image is <filename>.bmp and <filename> is the name of the image you are
		  generating.

main.c - Invokes other program functions, manages MPI environment, and gathers program statistics.

julia.h - Contains all functions signatures for functions used in the program.

getparams.c - Parses command line arguements and stores them in variables that are passed-by-reference.

julia.c - Computes the Julia set iterations for pixels in the image.

parallel-julia.c - Determines which load balancing algorithm the program should used based on the number
                   of processes available.

master-julia.c - Designates process 0 as a master process that delegates work to slave processes. The
                 master process also collects work completed by slave processes and inserts it into the
				 image memory block.

partition-julia.c - Partitions the image into smaller blocks that are assigned to each process.

savebmp.c - Converts iteration values to colours and writes them as pixels to a .bmp file.