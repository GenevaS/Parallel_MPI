/*
 * Geneva Smith
 * main.c
 * -------------------------------------------------------------------------------------------------
 * Function: main
 * Inputs: int argc - the number of arguments passed to the program from the command line
 * 	   char **argv - a pointer to a list of the command line arguments
 * Output: Exit code (return 0)
 * -------------------------------------------------------------------------------------------------
 * This function interacts with the user to get information about the integral to be computed using
 * Monte Carlo integration methods.
 * Arguments must be entered in the format integral_dimension <lower bounds> <upper bounds> points
 * Example: For an 4 dimensional integral with bounds between 0 and 1, integrate over 1000 points:
 * mpirun -np # ./<exe_name> 4 0 0 0 0 1 1 1 1 1000
*/

#include <stdio.h>
#include <mpi.h>

/*	External Function Declarations	*/
/* ------------------------------------ */
// parallel-monte-carlo.c
extern double ParallelMonteCarlo(double *a, double *b, int n, long int N, double (*fcn)(double *x, int n), int my_rank, int p, MPI_Comm com);
// check-result.c
extern double checkResult(double integral, double *a, double *b, int n, long int N, double (*fcn)(double *x, int n));
// fcn.c
extern double fcn(double *x, int n);
/* ------------------------------------ */

/*	MAIN PROGRAM	*/
int main (int argc, char **argv)
{
	int i;
	double integral;
	int comm_sz;
	int my_rank;
	
	// n = dimension of the integral
	int n;
	sscanf(argv[1], "%d", &n);

	// N = number of points to integrate over
	long int N;
	sscanf(argv[argc - 1], "%ld", &N);

	// a[] and b[] - lower and upper bounds of the integral
	double a[n];
	double b[n];

	for (i = 2; i < n + 2; i++) 
	{
		sscanf(argv[i], "%lf", &a[i - 2]);
		sscanf(argv[i + n], "%lf", &b[i - 2]);
	}
	
	// Initialize MPI environment
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Calculate integral
	integral = ParallelMonteCarlo(a, b, n, N, &fcn, my_rank, comm_sz, MPI_COMM_WORLD);

	if (my_rank == 0) 
	{
		// Call testing function and output result of integration
		checkResult(integral, a, b, n, N, &fcn);
		printf("Value of the integral is %.4e\n", integral);
	}
	
	// Close MPI environment
	MPI_Finalize();	
	
	return 0;
}
