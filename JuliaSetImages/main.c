/*
 * Geneva Smith 
 * main.c
 * -------------------------------------------------------------------------------------------------
 * Function: main
 * Inputs: int argc - the number of arguements passed in from the command line
 *         char *argv - the list of arguements passed in from the command line
 * -------------------------------------------------------------------------------------------------
 * This function initialize memory blocks that will be used for the duration of the program. It then
 * calls getParams to parse the command line arguements into the allocated memory before initializing
 * the MPI environment.
 * 
 * After MPI is initialize, a timer is started before the processes begin their Julia set 
 * calculations. When each process finishes, the timer is stopped and the statistics are collected on
 * process 0 for output to a stats file. Process 0 is also responsible for converting the iterations
 * calculated by Julia and converting them into .bmp files.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <string.h>

#include "julia.h"

int main(int argc, char *argv[])
{
  int width, height, maxiter, flag;
  double x[2], y[2], c[2];
  char *image, *stats;

  int comm_sz, my_rank;

  double t1, t2, delta;
  
  // Get and parse the program parameters
  getParams(argv, &flag, c, x, y, &width, &height, &maxiter, &image, &stats);

  // Allocate space for the image
  int *iterations = (int*)malloc( sizeof(int) * width * height );
  assert(iterations != NULL);
  
  // Start MPI
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  
  // Begin process timer
  t1 = MPI_Wtime();
  
  /* compute set */
  int maxCount = parallelJulia(x, width, y, height, c, flag, maxiter, iterations, my_rank, comm_sz, MPI_COMM_WORLD);

  // Stop timer and compute time elapse
  t2 = MPI_Wtime();
  delta = t2 - t1;

  if (my_rank == 0)
  {
    /* save our picture for the viewer */
    printf("\nMaster process %d creating image...\n", my_rank);
    saveBMP(image, iterations, width, height);
    printf("\nFinished image creation\n");
  }

  // Wait for all processes to finish Julia computations
  MPI_Barrier(MPI_COMM_WORLD);

  // Open stats file
  MPI_File statsFile;

  if (MPI_File_open(MPI_COMM_WORLD, stats, MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &statsFile) == MPI_SUCCESS) 
  {
    // Generate statistic string
    char message[100];
    sprintf(message, "process %d: max iterations reached = %d, time elapsed = %lf\n", my_rank, maxCount, delta);
  
    MPI_File_write_ordered(statsFile, message, strlen(message), MPI_CHAR, MPI_STATUS_IGNORE);

    MPI_File_close(&statsFile);
  }
  else printf("Problem opening file on process %d\n", my_rank);

  // Close MPI environment
  MPI_Finalize();
  
  // Free reserved memory
  free(iterations);

  return 0;
}
