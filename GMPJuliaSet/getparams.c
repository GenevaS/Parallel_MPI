/*
 * Geneva Smith
 * getparams.c
 * --------------------------------------------------------------------
 * Function: getParams
 *
 * Inputs: 
 * char **argv - a list of arguements from the command line
 * int *flag - a pointer to an INT in memory to store a set flag
 * mpf_t *cr, *ci - a pointer to a pair of values representing a 
 *                  complex number cr + ci
 * mpf_t *x, *xr - a pointer to the center and radius of the x circle
 * mpf_t *y, *yr - a pointer to the center and radius of the y circle
 * unsigned long int *height - a pointer to an INT to store the width 
 *                             of the image
 * unsigned long int *width - a pointer to an INT to store the height 
 *                            of the image
 * int *maxiter - a pointer to an INT to store the maximum number of 
 *                iterations to use
 * char **image - a pointer to a file name to store the image pixels; 
 *                .bmp extension
 * --------------------------------------------------------------------
 * This function takes in an array storing arguements from a file and 
 * parses them into the program's memory. There are no values to return 
 * because all values are returned by reference.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gmp.h>

#define SIZE 100

void getParams(char **argv, int *flag, mpf_t *cr, mpf_t *ci, mpf_t *x, mpf_t *y, mpf_t *xr, mpf_t *yr, unsigned long int *height, unsigned long int *width, int *maxiter, char **image)
{
  char data[SIZE];
  char filename[SIZE];
  char tempcr[SIZE], tempci[SIZE], tempx[SIZE], tempy[SIZE], tempxr[SIZE], tempyr[SIZE];
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
    if (fgets(tempxr, SIZE, params) != NULL) if (strlen(tempxr) > max) max = strlen(tempxr);
    if (fgets(tempyr, SIZE, params) != NULL) if (strlen(tempyr) > max) max = strlen(tempyr);
    if (fgets(data, SIZE, params) != NULL) *height = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) *width = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) *maxiter = strtol(data, NULL, 0);
    if (fgets(data, SIZE, params) != NULL) 
    {
      sscanf(data,"%s", filename);
      *image = malloc(strlen(filename));
      strcpy(*image, filename);
    }

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
    mpf_set_str(*xr, tempxr, 10);
    mpf_set_str(*yr, tempyr, 10);
  }
  else perror("Error opening file\n");

  return;
}
