/*
 * Geneva Smith 
 * check-result.c
 * -------------------------------------------------------------------------------------------------
 * Function: checkResult
 * Inputs: double integral - an integral result returned by MonteCarlo integration
 * 	   double *a - a pointer to an array of doubles containing the lower bounds of the integral
 * 	   double *b - a pointer to an array of doubles containing the upper bounds of the integral
 * 	   int n - the dimension of the integral; size of a and b
 *	   long int N - the number of random points used by the MonteCarlo integration
 *	   double (*fcn)(double *x, int n) - a pointer to a function "fcn" (See: fcn.c)
 * Output: double - ???
 * -------------------------------------------------------------------------------------------------
 * This function is used to test the results of the distributed Monte Carlo integration program. 
 * Function body is to be replaced with the TAs' own testing function.
*/

double checkResult(double integral, double *a, double *b, int n, long int N, double (*fcn)(double *x, int n))
{
	return 0;
}
