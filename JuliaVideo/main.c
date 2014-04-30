/*
 * Geneva Smith
 * JuliaVideo - main.c
 * -------------------------------------------------------------------------------------------------
 * Function: main
 * Inputs: int argc - the number of arguements passed in from the command line
 *         char *argv - the list of arguements passed in from the command line
 * -------------------------------------------------------------------------------------------------
 * This function initialize memory blocks that will be used for the duration of the program. It then
 * calls getParams to parse the command line arguements into the allocated memory before initializing
 * the MPI environment.
 * 
 * Video frames are divided among the available processes using cyclic partitioning. Once each 
 * knows how many frames it needs to render, it starts and OpenMP environment and uses a FOR loop to
 * simulateously generate images on different threads. The systems default thread count is used. 
 * Threads generate complete images. 
 * 
 * After pixel values have been calculated, they are stored as bitmap files with the name
 * <filenameXXXXX.bmp>, where filename is a user supplied name, XXXXX is the frame number, and .bmp 
 * is the file extension. If the frame number is not five digits long, it is padded with zeroes. 
 * For example, frame 1 would have a file name like <filename00001.bmp>.
 * 
 * The program includes a timer that times how long the program takes to render the frames (in 
 * seconds).
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <gmp.h>
#include <omp.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

#include "julia.h"

int main(int argc, char** argv)
{  
  // Computation parameters
  int maxiter, maxIterStart, maxIterEnd;
  int iterStep;
  int flag;
  int comm_sz, my_rank;
  time_t t1, t2;
  double delta, maxTime;
  int i;
  int *iterations;

  // Video parameters
  unsigned long int width, height;
  int frames, curframe;
  int startframe, endframe;
  int fps, vidLen;
  float zoomx, zoomy;
  mpf_t framezoomx, framezoomy;

  // File name variables
  char *filename;
  char filenum [10];
  char *fileext = ".bmp";
  char *image = "";

  // GMP Floats
  mpf_t cr, ci;
  mpf_init(cr);
  mpf_init(ci);

  mpf_t xcent, ycent;
  mpf_init(xcent);
  mpf_init(ycent);

  mpf_t xrInit, yrInit, xrFin, yrFin;
  mpf_init(xrInit);
  mpf_init(yrInit);
  mpf_init(xrFin);
  mpf_init(yrFin);

  // Get and parse the program parameters and set GMP precision
  getParams(argv, &flag, &cr, &ci, &xcent, &ycent, &xrInit, &yrInit, &xrFin, &yrFin, &width, &height, &maxIterStart, &maxIterEnd, &filename, &fps, &vidLen, &startframe, &endframe);
  mpf_set_default_prec(mpf_get_prec(cr));

  // Calculate number of frames in the video
  frames = fps * vidLen;

  // Calculate number of iterations to add per frame
  iterStep = ceil((2*(double)maxIterEnd - 2*(double)maxIterStart) / (double)frames);

  // Calculate zoom rate based on the number of frames; since the first frame is not zoomed
  // in on, we calculate zoom with frame count (totalframes - 1)
  // x and y zoom are calculated separately to avoid stretching images
  zoomx = pow((float)mpf_get_d(xrInit) / (float)mpf_get_d(xrFin), 1.0/((float)frames - 1));
  mpf_init(framezoomx);
  mpf_set_d(framezoomx, (double)zoomx);

  zoomy = pow((float)mpf_get_d(yrInit) / (float)mpf_get_d(yrFin), 1.0/((float)frames - 1));
  mpf_init(framezoomy);
  mpf_set_d(framezoomy, (double)zoomy);

  /* Start MPI */
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  // Debugging output
  int n = 35;
  if (my_rank == 0)
  {
    gmp_printf ("x = %+.*Ff\n", n, xcent);
    gmp_printf ("xr Init = %+.*Ff\n", n, xrInit);
    gmp_printf ("xr Fin = %+.*Ff\n\n", n, xrFin);

    gmp_printf ("y = %+.*Ff\n", n, ycent);
    gmp_printf ("yr Init = %+.*Ff\n", n, yrInit);
    gmp_printf ("yr Fin = %+.*Ff\n\n", n, yrFin);

    printf("Height = %ld\t", height);
    printf("Width = %ld\n", width);
    printf("maxiterStart = %d\t", maxIterStart);
    printf("maxiterEnd = %d\t", maxIterEnd);
    printf("iterStep = %d\n\n", iterStep);

    gmp_printf ("Zoom x (GMP) = %+.*Ff\n", n, framezoomx);
    gmp_printf ("Zoom y (GMP) = %+.*Ff\n", n, framezoomy);
    printf("frames = %d\n\n", frames);
  }

  // Divide frames among processes
  startframe = startframe - 1; // Offset frame divisions
  endframe = endframe - 1;
  frames = endframe - startframe + 1; // Total frames + 1 to include end bound

  int totalframes = frames / comm_sz;
  if (my_rank < (frames % comm_sz)) totalframes++;
  printf("Process %d processess %d frames\n", my_rank, totalframes);

  // Start timer
  time(&t1);

  /* Start OpenMP */
  #pragma omp parallel shared(width, height, xcent, ycent, xrInit, yrInit, maxIterStart, iterStep, framezoomx, framezoomy) private(i, iterations, filenum, image, curframe, maxiter)
  {
    /* Variables and memory declared in this section are private to each thread */

    // Allocate space for the image
    iterations = (int*)calloc( width * height, sizeof(int) );
    assert(iterations != NULL);
 
    // Allocate space for the filenames
    image = malloc(strlen(filename) + 5 + strlen(fileext));

    // GMP variables to stored distance from center point in
    mpf_t xmin, xmax, ymin, ymax;
    mpf_init(xmin);
    mpf_init(xmax);
    mpf_init(ymin);
    mpf_init(ymax);

    /* Start OpenMP parallel FOR */
    #pragma omp for
    for ( i = 0; i < totalframes; i++ )
    {
      curframe = my_rank + (i * comm_sz) + startframe;
      maxiter = maxIterStart + (iterStep * curframe);
      printf("Process %d working on frame %d with thread %d\n", my_rank, curframe + 1, omp_get_thread_num());

      // xmin, xmax, ymin and ymax
      if (curframe == 0)
      {
        // Do not zoom for the first image
        mpf_sub(xmin, xcent, xrInit);
        mpf_add(xmax, xcent, xrInit);

        mpf_sub(ymin, ycent, yrInit);
        mpf_add(ymax, ycent, yrInit);
      }
      else
      {
        // Radius is calculated as r = rinit / (zoom^frame#)
        mpf_pow_ui(xmax, framezoomx, (unsigned long int)curframe);
        mpf_div(xmax, xrInit, xmax);
        mpf_pow_ui(ymax, framezoomy, (unsigned long int)curframe);
        mpf_div(ymax, yrInit, ymax);

        mpf_sub(xmin, xcent, xmax);
        mpf_add(xmax, xcent, xmax);
        mpf_sub(ymin, ycent, ymax);
        mpf_add(ymax, ycent, ymax);
      }

      /* Compute Julia set */
      GMPJulia(xmin, xmax, width, width, 0, ymin, ymax, height, height, 0, cr, ci, flag, maxiter, iterations);

      /* Save image as a bitmap (.bmp) */
      sprintf(filenum, "%05d", curframe + 1);

      strcpy(image, filename);
      strcat(image, filenum);
      strcat(image, fileext);  

      printf("\nProcess %d creating frame %d on thread %d...", my_rank, curframe + 1, omp_get_thread_num());
      saveBMP(image, iterations, width, height);
    }

    // Free thread-reserved memory
    free(iterations);
    free(image);
    mpf_clear(xmin);
    mpf_clear(xmax);
    mpf_clear(ymin);
    mpf_clear(ymax);
  }

  // Stop timer and calculate elapsed time
  time(&t2);
  delta = difftime(t2, t1);

  MPI_Reduce(&delta, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (my_rank == 0) printf("Computation time = %lf\n", maxTime);

  // Close MPI environment
  MPI_Finalize();

  // Free reserved memory
  //mpf_clears(cr, ci, x, y, xr, yr, xmin, xmax, ymin, ymax, (mpf_t *) 0);
  mpf_clear(cr);
  mpf_clear(ci);
  mpf_clear(xcent);
  mpf_clear(ycent);
  mpf_clear(xrInit);
  mpf_clear(yrInit);
  mpf_clear(xrFin);
  mpf_clear(yrFin);
  mpf_clear(framezoomx);
  mpf_clear(framezoomy);

   return 0;
}
