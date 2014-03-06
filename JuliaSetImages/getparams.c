/*
 * Geneva Smith
 * getparams.c
 * -------------------------------------------------------------------------------------------------
 * Function: getParams
 * Inputs: char **argv - a list of arguements from the command line
 *	   int *flag - a pointer to an INT in memory to store a set flag
 *         double *c - a pointer to a pair of values representing a complex number a + ib
 *         double *x - a pointer to a pair of x coordinates in the set space
 *         double *y - a pointer to a pair of y coordinates in the set space
 *         int *width - a pointer to an INT to store the width of the image
 *         int *height - a pointer to an INT to store the height of the image
 *         int *maxiter - a pointer to an INT to store the maximum number of iterations to use
 *         char **image - a pointer to a file name to store the image pixels; .bmp extension
 *         char **stats - a pointer to a file name to store the program statistics; .txt extension
 * -------------------------------------------------------------------------------------------------
 * This function takes in an array storing arguements from the command line and parses them into the
 * program's memory. There are no values to return because all values are returned by reference.
 *
 * This file was provided as part of the course.
*/

#include <stdio.h>
#include <stdlib.h>

void getParams(char **argv, int *flag, double *c, double *x, double *y, int *width, int *height,
	       int *maxiter, char **image, char **stats)
{
  *flag = strtol(argv[1], NULL, 0);

  c[0]  = strtod(argv[2], NULL);
  c[1]  = strtod(argv[3], NULL);

  x[0] = strtod(argv[4], NULL);
  x[1] = strtod(argv[5], NULL);
  y[0] = strtod(argv[6], NULL);
  y[1] = strtod(argv[7], NULL);

  *width  = strtol(argv[8], NULL, 0);
  *height = strtol(argv[9], NULL, 0);
  *maxiter= strtol(argv[10], NULL, 0);

  *image = argv[11];
  *stats = argv[12];
}
