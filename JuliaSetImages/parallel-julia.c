/*
 * Geneva Smith 
 * parallel-julia.c
 * -------------------------------------------------------------------------------------------------
 * Function: parallelJulia
 * Inputs: const double *x - a pointer to a set of x coordinates
 *         int xres - the width of the complete image
 *         const double *y - a pointer to a set of y coordinates
 *	   int yblock - the height of the block julia is computing values for 
 *         int yres - the height of the complete image
 *         int starty - y offset of the memory block that julia is working on
 *         const double *c - values of the imaginary number a + ib
 *         int flag - indicates if the image is Mandelbrot or Julia set
 *         int maxIterations - maximum number of hops to try to exit the unit circle
 *         int *iterations - the memory block that julia is working on
 *         int my_rank - the id of the current process
 *         int p - the total number of processes running
 *         MPI_Comm comm - the MPI communicator of the program
 * Outputs: int maxCount - the maximum number of iterations required by any pixel in the
 *                         process
 * -------------------------------------------------------------------------------------------------
 * This function determines which Julia computation algorithm to use based on the number of 
 * processes:
 *  - # Processes = 1: Serial program; call julia function directly
 *  - # Processes = 2: Not enough processes to require a task master; send to BlockPartitionJulia
 *  - # Processes > 2: Enough processes to require a task master; send to TaskMasterJulia
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>

#include "julia.h"

int parallelJulia(const double *x, int xres, const double *y, int yres, const double *c, 
	  int flag, int maxIterations, int *iterations, int my_rank, int p, MPI_Comm comm)
{
  int maxCount;

  if (p == 1)
  {
    if(my_rank == 0) printf("Single process - serial version\n\n");
    printf("Process %d...aren't you happy I'm here?\n", my_rank);

    maxCount = julia(x, xres, xres, 0, y, yres, yres, 0, c, flag, maxIterations, iterations);
    printf("Max count with one process: %d\n", maxCount);
  }
  else if (p == 2)
  {
    if(my_rank == 0) printf("Not enough processes - divide image at middle height and use scatterv/gatherv\n\n");
    maxCount = BlockPartitionJulia(x, xres, y, yres, c, flag, maxIterations, iterations, my_rank, p, comm);
  }
  else
  {
    if(my_rank == 0) printf("Sufficient processes - run process 0 as task master\n\n");
    maxCount = TaskMasterJulia(x, xres, y, yres, c, flag, maxIterations, iterations, my_rank, p, comm);
  }

  return maxCount;
}
