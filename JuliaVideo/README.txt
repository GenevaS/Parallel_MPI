Programs were developed using the MPICH2 implementation of MPI, OpenMP, and the GMP
library.

Compiled in the Linux terminal using the Makefile

Creates an executable file - video

This program computes a series of Mandelbrot or Julia set images that can be stitched together to
make a video. Users can choose a frame rate and video length (in seconds).

The video found at the link in <video.txt> was created using this program with the <seahorse.dat> file
and stiched together using ffmpeg:
	ffmpeg -r 60 -i seahorse%05d.jpg -vb 30M SeahorseValley.mp4 
*Note: The video was created using JPEG images. The bitmap images were created on a remote computer and
were too large to transfer to a local host. Music and title slides were added post-production.

The Mandelbrot equation is (z[k+1] = z[k]^s + z[0]), k >= 0
The Julia equation is (z[k+1] = z[k]^s + c), k >= 0
 * The variables z and c represent complex numbers a + ib

Julia is envoked with: 
	mpirun -np <#processes> ./video <parameterFile>
	
	where the <parameterFile> contains the following parameters on separate lines:
		* flag determines which set is generated. If flag = 0, the Mandelbrot set is
		  computed. If flag = 1, the Julia set is computed.
		* cr - the real components used to calculate the Julia set. If flag = 0, this 
		  parameter is ignored, but must be included in the file.
		* ci - the imaginary components used to calculate the Julia set. If flag = 0, 
		  this parameter is ignored, but must be included in the file.
		* x coordinate of the center point of a circle in the Mandelbrot domain
		* y coordinate of the center point of a circle in the Mandelbrot domain
		* xrInit - horizontal distance from the x coordinate (real) for the first frame
		* yrInit - vertical distance from the y coordinate (imaginary) for the first frame
		* xrFin - horizontal distance from the x coordinate (real) for the last frame
		* yrFin - vertical distance from the y coordinate (imaginary) for the last frame
		* height of the frames in pixels
		* width of the frames in pixels
		* maxiterStart is the maximum number of hops that the program will try on each pixel 
		  in the first frame.
		* maxiterEnd is the maximum number of hops that the program will try on each pixel 
		  in the last frame.
		* image is <filename>.bmp and <filename> is the name of the frames you are
		  generating. Each frame will be given a number in the filename.
		* fps - the desired frames per second.
		* vidLen - the desired video duration in seconds
		* startFrame - the frame that the program should begin rendering from.
		* endFrame - the frame that the program should stop rendering at.
		
The parameters <startFrame> and <endFrame> enables users to process their frames in chunks instead of 
in a single job.

main.c - Invokes other program functions and manages MPI and OpenMP environments.

julia.h - Contains all functions signatures for functions used in the program.

getparams.c - Parses command line arguements and stores them in variables that are passed-by-reference.

julia-gmp.c - Computes the Julia set iterations for pixels in the image.

savebmp.c - Converts iteration values to colours and writes them as pixels to a .bmp file.