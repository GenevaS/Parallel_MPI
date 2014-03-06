/*
 * Geneva Smith
 * julia.h
 * -------------------------------------------------------------------------------------------------
 * This header file contains function signatures for each function that is required to successfully
 * run the Julia program.
*/

int TaskMasterJulia(const double *x, int xres, const double *y, int yres, const double *c, 
	  int flag, int maxIterations, int *iterations, int my_rank, int p, MPI_Comm comm);

int BlockPartitionJulia(const double *x, int xres, const double *y, int yres, const double *c, 
	  int flag, int maxIterations, int *iterations, int my_rank, int p, MPI_Comm comm);

int parallelJulia(const double *x, int xres, const double *y, int yres, const double *c, 
	  int flag, int maxIterations, int *iterations, int my_rank, int p, MPI_Comm comm);

int julia(const double *x, int xblock, int xres, int startx, const double *y, int yblock, int yres, int starty, const double *c, 
	  int flag, int maxIterations, int *iterations);

void getParams(char **argv,  int *flag, double *c, double *x, double *y, int *width, int *height,
	       int *maxiter, char **image, char **stats);

void saveBMP(char* filename, int* result, int width, int height);
