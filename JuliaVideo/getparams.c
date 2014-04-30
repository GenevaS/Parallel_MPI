/*
 * Geneva Smith
 * JuliaVideo - getparams.c
 * -------------------------------------------------------------------------------------------------
 * Function: getParams
 * Inputs: char **argv - a list of arguements from the command line
 *	   int *flag - a pointer to an INT in memory to store a set flag
 *         mpf_t *cr, *ci - a pointer to a pair of values representing a complex number cr + ci
 *         mpf_t *x, *y - a pointer to the center of a circle in set space
 *         mpf_t *xrInit, *yrInit - a pointer to the initial radius which subsequent images zoom
 *                                  into (the first image radius)
 *         mpf_t *xrFin, *yrFin - a pointer to the final radius which subsequent images zoom to
 *                                (the final image radius)
 *         unsigned long int *height - a pointer to an INT to store the width of the image
 *         unsigned long int *width - a pointer to an INT to store the height of the image
 *         int *maxiterStart - a pointer to an INT to store the maximum number of iterations to use
 *                             for the first image
 *         int *maxiterEnd - a pointer to an INT to store the maximum number of iterations to use
 *                             for the last image
 *         char **image - a pointer to a file name to store the image pixels; no extension
 *         int *fps - a pointer to an INT storing the frame rate of the video (frames per second)
 *         int *vidlen - a pointer to an INT storing the length of the final video in seconds
 *         int *startframe - a pointer to an INT storing the frame number that the program will 
 *                           begin rendering from 
 *         int *endframe - a pointer to an INT storing the frame number that the program will stop 
 *                         rendering at
 * -------------------------------------------------------------------------------------------------
 * This function takes in a series of pointers and assigns values to them from a file passed in by 
 * **argv. GMP values are temporarily stored as strings in order to determine what precision is
 * required to ensure that precision is maintained throughout the calculations.
 * 
 * The variables startframe and endframe allow for frame generation in blocks. In order to render
 * the entire video with a single run, set startframe = 1 and endframe = (fps*vidlen).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gmp.h>

#define SIZE 100

void getParams(char **argv, int *flag, mpf_t *cr, mpf_t *ci, mpf_t *x, mpf_t *y, mpf_t *xrInit, mpf_t *yrInit, mpf_t *xrFin, mpf_t *yrFin, unsigned long int *height, unsigned long int *width, int *maxiterStart, int *maxiterEnd, char **image, int *fps, int *vidlen, int *startframe, int *endframe)
{
  char data[SIZE];
  char filename[SIZE];
  char tempcr[SIZE], tempci[SIZE], tempx[SIZE], tempy[SIZE], tempxrInit[SIZE], tempyrInit[SIZE], tempxrFin[SIZE], tempyrFin[SIZE];
  int max = 0;
  int inputPrecision, precision;

  FILE *params;
  params = fopen(argv[1], "r");

  if (params != NULL)
  {
    if (fgets(data, SIZE, params) != NULL) *flag = strtol(data, NULL, 0);

    if (fgets(tempcr, SIZE, params) != NULL) if (strlen(tempcr) > max) max = strlen(tempcr);
    if (fgets(tempci, SIZE, params) != NULL) if (strlen(tempci) > max) max = strlen(tempci);

    if (fgets(tempx, SIZE, params) != NULL) if (strlen(tempx) > max) max = strlen(tempx);
    if (fgets(tempy, SIZE, params) != NULL) if (strlen(tempy) > max) max = strlen(tempy);

    if (fgets(tempxrInit, SIZE, params) != NULL) if (strlen(tempxrInit) > max) max = strlen(tempxrInit);
    if (fgets(tempyrInit, SIZE, params) != NULL) if (strlen(tempyrInit) > max) max = strlen(tempyrInit);

    if (fgets(tempxrFin, SIZE, params) != NULL) if (strlen(tempxrFin) > max) max = strlen(tempxrFin);
    if (fgets(tempyrFin, SIZE, params) != NULL) if (strlen(tempyrFin) > max) max = strlen(tempyrFin);

    if (fgets(data, SIZE, params) != NULL) *height = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) *width = strtol(data, NULL, 0);

    if (fgets(data, SIZE, params) != NULL) *maxiterStart = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) *maxiterEnd = strtol(data, NULL, 0);

    if (fgets(data, SIZE, params) != NULL) 
    {
      sscanf(data,"%s", filename);
      *image = malloc(strlen(filename));
      strcpy(*image, filename);
    }

    if (fgets(data, SIZE, params) != NULL) *fps = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) *vidlen = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) *startframe = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) *endframe = strtol(data, NULL, 0);

    fclose(params);

    // Determine precision to use
    inputPrecision = ceil(max * (log(10) / log(2))) + 11;
    precision = mpf_get_default_prec();
    if (inputPrecision > precision) precision = inputPrecision;
    mpf_set_default_prec(precision);
      
    // Assign values to GMP Floats
    mpf_set_str(*cr, tempcr, 10);
    mpf_set_str(*ci, tempci, 10);
    mpf_set_str(*x, tempx, 10);
    mpf_set_str(*y, tempy, 10);
    mpf_set_str(*xrInit, tempxrInit, 10);
    mpf_set_str(*yrInit, tempyrInit, 10);
    mpf_set_str(*xrFin, tempxrFin, 10);
    mpf_set_str(*yrFin, tempyrFin, 10);
  }
  else perror("Error opening file\n");

  return;
}
