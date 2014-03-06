/*
 * Geneva Smith
 * fcn.c
 * -------------------------------------------------------------------------------------------------
 * Function: fcn
 * Inputs: double *x - a pointer to a list of x values used in the mathematical function
 *	   int n - the size of the x array
 * Output: double r - the result of the calculation
 * -------------------------------------------------------------------------------------------------
 * This function is the mathematical formula to be integrated. For the given formula, the result is
 * exactly 1 for an integral of dimension 4 with bounds [0,1], [0,1], [0,1], and [0,1].
*/

#include <math.h>

double fcn(double *x, int n)
{
	double pi = 3.14159265358979;
	double k = pi / 2;
	double w = k;
	double r;
	int i;

	for (i = 0; i < n; i++) w *= x[i];
	
	r = k*cos(w) - 7*k*w*sin(w) - 6*k*w*w*cos(w) + k*w*w*w*sin(w);

	return r;
} 
