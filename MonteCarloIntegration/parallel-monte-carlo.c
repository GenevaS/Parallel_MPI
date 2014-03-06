/*
 * Geneva Smith
 * parallel-monte-carlo.c
 * -------------------------------------------------------------------------------------------------
 * Function: ParallelMonteCarlo
 * Inputs: double *a - a pointer to an array of doubles containing the lower bounds of the integral
 * 	   double *b - a pointer to an array of doubles containing the upper bounds of the integral
 * 	   int n - the dimension of the integral; size of a and b
 *	   long int N - the number of random points used by the MonteCarlo integration
 *	   double (*fcn)(double *x, int n) - a pointer to a function "fcn" (See: fcn.c)
 * 	   int my_rank - the process number
 * 	   int p - the total number of processes being called
 * 	   MPI_Comm com - the MPI communicator
 * Output: double sum_integral - the collective sum of the integrals completed on all processes
 * -------------------------------------------------------------------------------------------------
 * This function divides the total points N among p processes. Each process calls MonteCarlo and the
 * results are collected by process 0 and summed together using MPI_Reduce.
*/

#include <mpi.h>

/*	External Function Declarations		*/
/* -------------------------------------------- */
// monte-carlo.c
extern double MonteCarlo(double *a, double *b, int n, long int N, double (*fcn)(double *x, int n));

/*	ParallelMonteCarlo	*/
/* ---------------------------- */
double ParallelMonteCarlo(double *a, double *b, int n, long int N, double (*fcn)(double *x, int n), int my_rank, int p, MPI_Comm com)
{
	double partial_integral;
	double sum_integral;
	long int num_points;
	long int remainder;

	// Base number of points for each process to calculate
	num_points = N/p;

	// Distribute remainder amongst processes (Assumption: remainder must be < np)
	remainder = N%p;
	if (my_rank < remainder) num_points++;

	// Send reduced number of points to monte-carlo.c
	partial_integral = MonteCarlo(a, b, n, num_points, fcn);

	// Return and sum results on process 0
	MPI_Reduce(&partial_integral, &sum_integral, 1, MPI_DOUBLE, MPI_SUM, 0, com);

	// Divide summation by N
	if (my_rank == 0) sum_integral = sum_integral / N;
	
	return sum_integral;
}
