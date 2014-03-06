/*
 * Geneva Smith
 * julia.c
 * -------------------------------------------------------------------------------------------------
 * Function: julia
 * Inputs: const double *x - a pointer to a set of x coordinates
 *	   int xblock - the width of the block julia is computing values for 
 *         int xres - the width of the complete image
 *         int startx - x offset of the memory block that julia is working on
 *         const double *y - a pointer to a set of y coordinates
 *	   int yblock - the height of the block julia is computing values for 
 *         int yres - the height of the complete image
 *         int starty - y offset of the memory block that julia is working on
 *         const double *c - values of the imaginary number a + ib
 *         int flag - indicates if the image is Mandelbrot or Julia set
 *         int maxIterations - maximum number of hops to try to exit the unit circle
 *         int *iterations - the memory block that julia is working on
 * Outputs: int maxIterationCount - the maximum number of iterations required by any pixel in the
 *                                  memory block
 * -------------------------------------------------------------------------------------------------
 * This function generates iteration values for pixels in an image of resolution xres x yres. The
 * memory block iterations is a xblock x yblock section of the complete image. The values startx and
 * starty tell julia where the block is located in the image.
 * Julia tried maxIterations times to leave the unit circle for each pixel and records that value in
 * iterations. The memory block iterations does not need to be explicitly returned because it is 
 * passed by reference.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <complex.h>
#include <math.h>

#include "julia.h"

int julia(const double *x, int xblock, int xres, int startx, const double *y, int yblock, int yres, int starty, const double *c, 
	  int flag, int maxIterations, int *iterations)
{
  // Maximum radius of the unit circle
  const double maxRadius = 2.0;

  // Counters
  int maxIterationCount = 0;
  int i, j, my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  
  // Distance variables
  double xi, yi, xgap, ygap;
  
  // Converting coordinate to complex space
  xgap = (x[1] - x[0]) / xres;
  ygap = (y[1] - y[0]) / yres; 
  
  // Iterate over image height
  for (j = 0; j < yblock; j++)
    { 
      // Iterate over image width
      for (i = 0; i < xblock; i++)    
	{
          //printf("Running pixel %d, %d on process %d\n", i, j, my_rank);
	  /* pixel to coordinates */
	  xi = x[0] + (i + startx) * xgap;
	  yi = y[0] + (j + starty) * ygap;


	  // Calculate base values for sets
	  double complex zinit   = xi + yi*I;
	  
	  double complex C = c[0] + c[1]*I;

	  // if flag=0, z = z0, flag=1, z = C
	  double complex z0  = flag*C+(1-flag)*zinit;
	  
	  // Determine how long it takes to leave the unit circle
	  int iteration=0;
	  double complex z  = zinit;
	  while (cabs(z) < maxRadius && iteration < maxIterations)
	    {
	      z = z*z+z0;
	      iteration++;
	    }
	  
	  // Record the maximum number of iterations reached on a single pixel
	  if (iteration>maxIterationCount)
	    maxIterationCount = iteration;
	  
	  // Calculate storage location
	  int *p = iterations + j*xres+i;
	  
	  /* If radius <= 4.0, we have hit maxIterations. The point is
	     likely in the set. */
          // Store value at memory location in iterations
	  *p = iteration;
	}
    }
  
  return maxIterationCount;
}
