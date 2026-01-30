#include <Docking/GaussianFFT.h>
#include "fftw3.h"
#include <math.h>

//Include ordering dependency
#include <Docking/erfs.h>

#ifndef M_PI
#define 	M_PI   3.14159265358979323846
#endif

using namespace my_erfs;

// computed using an expansion from mathematica.
// Let Gaussian be e^{-x x w w}
// n	is the fourier coefficient index
// w	is width of Gaussian (includes rate of decay in it)
// ret	is the return value
// N	is the precision input to erf function calls.
bool GaussianFFT::getFourierCoefficient(int n, double w, double* ret_re, double* ret_im, int N)
{
	double t = exp(-n*n*M_PI*M_PI/(w*w))*sqrt(M_PI)/(2*w);
	double i_re = n*M_PI/w;
	double i_im = w/2.0;
	fftw_complex i_1, i_2;
	double o_1_re=0, o_1_im=0, o_2_re=0, o_2_im=0;
	i_1[0] = i_re;
	i_1[1] = -i_im;
	i_2[0] = i_re;
	i_2[1] = i_im;
	if (!erfi(i_1, &o_1_re, &o_1_im, N))
	{
		return false;
	}
	if (!erfi(i_2, &o_2_re, &o_2_im, N))
	{
		return false;
	}
	*ret_re = -t*o_1_im + t*o_2_im;
	*ret_im =  t*o_1_re - t*o_2_re;
	return true;
}
