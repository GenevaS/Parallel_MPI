/*
 * Geneva Smith
 * monte-carlo.c
 * -------------------------------------------------------------------------------------------------
 * Function: MonteCarlo
 * Inputs: double *a - a pointer to an array of doubles containing the lower bounds of the integral
 * 	   double *b - a pointer to an array of doubles containing the upper bounds of the integral
 * 	   int n - the dimension of the integral; size of a and b
 *	   long int N - the number of random points used by the MonteCarlo integration
 *	   double (*fcn)(double *x, int n) - a pointer to a function "fcn" (See: fcn.c)
 * Output: double sum - the summation of the N calls to fcn
 * -------------------------------------------------------------------------------------------------
 * This function is used to generate an array of n random points that are used to evaluate the 
 * function to be integrated. The function is called N times and the results are summed together.
*/

#include <stdlib.h>
#include <time.h>

double MonteCarlo(double *a, double *b, int n, long int N, double (*fcn)(double *x, int n))
{
	double sum = 0;
	int i, j;
	double *ptr_x;

	// Allocate space for an N size array of type double; initialized to 0
	ptr_x = (double *)calloc(n, sizeof(double));

	// Seed for random number generator
	int seed = time(NULL);
	srand(seed);

	// Generate N random sets of points
	for (i = 0; i < N; i++)
	{
		// Generate n random points and save them in ptr_x
		for (j = 0; j < n; j++) ptr_x[j] = a[j] + (b[j] - a[j])*drand48();
		
		// Add results together
		sum += fcn(ptr_x, n);
	}

	// Release the array
	free(ptr_x);

	return sum;
}
