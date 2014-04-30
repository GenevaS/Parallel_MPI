/*
 * Geneva Smith
 * JuliaVideo - julia.h
 * -------------------------------------------------------------------------------------------------
 * This header file contains function signatures for each function that is required to successfully
 * run the Julia program.
*/

void getParams(char **argv, int *flag, mpf_t *cr, mpf_t *ci, mpf_t *x, mpf_t *y, mpf_t *xrInit, mpf_t *yrInit, mpf_t *xrFin, mpf_t *yrFin, unsigned long int *height, unsigned long int *width, int *maxiterStart, int *maxiterEnd, char **image, int *fps, int *vidlen, int *startframe, int *endframe);

void GMPJulia(mpf_t xmin, mpf_t xmax, int xblock, unsigned long int xres, int startx, mpf_t ymin, mpf_t ymax, int yblock, unsigned long int yres, int starty, mpf_t cr, mpf_t ci, int flag, int maxIterations, int *iterations);

void saveBMP(char* filename, int* result, int width, int height);
