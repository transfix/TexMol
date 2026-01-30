#include "fftw3.h"
#include <math.h>

//Include ordering dependency prevents this from moving higher
#include <Docking/erfs.h>

#ifndef M_PI
#define 	M_PI   3.14159265358979323846
#endif

using namespace my_erfs;

// The approximation given in:
// Handbook of Mathematical Functions by  Milton Abramowitz and Irene A Stegun pg 299
double my_erfs::erf(double x)
{
	bool flip = false;
	if (x<0)
	{
		flip = true;
		x *= -1;
	}
	double p = 0.3275911;
	double a1 = 0.254829592;
	double a2 = -0.284496736;
	double a3 = 1.421413741;
	double a4 = -1.453152027;
	double a5 = 1.061405429;
	double t = 1.0/(1+p*x);
	double res = 1-(a1*t + a2*t*t + a3*t*t*t + a4*t*t*t*t + a5*t*t*t*t*t) * exp(-x*x);
	if (flip)
	{
		return -1*res;
	}
	return res;
}

// From
// Computation  of the Complex Error Function by J. A. C. Weideman
// SIAM J Numer Anal 31, 5, 1497-1518, 94
bool faddeeva(fftw_complex z, double* w_re, double* w_im, int N)
{
	//line 1
	int M = 2*N;
	int M2 = 2*M;
	int* k = new int[M2-1];
	for (int i=0; i<M2-1; i++)
	{
		k[i] = i-M+1;
	}
	//line 2
	double L = sqrt((double)N/sqrt(2.0));
	//line 3
	double* t = new double[M2-1];
		double* theta = new double[M2-1];
		for (int i=0; i<M2-1; i++)
		{
			theta[i] = k[i]*M_PI/(double)M;
			t[i] = L*tan(theta[i]/2.0);
		}
		delete []k;
		k=0;
		delete []theta;
		theta = 0;
	//line 4
	fftw_complex* f = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*M2);
	{
		f[0][0] = f[1][0] = 0;
		for (int i=0; i<M2-1; i++)
		{
			f[i+1][0] = exp(-t[i]*t[i]) * (L*L + t[i]*t[i]);
			f[i+1][1] = 0;
		}
		delete []t;
		t=0;
	}
	//line 5
	double* a = new double[M2];
	{
		fftw_complex* temp = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*M2);
		for (int i=0; i<M2; i++)
		{
			temp[i][0] = f[i][0];
		}
		for (int i=0; i<M2; i++)
		{
			int index = i+M2/2;
			if (index >= M2)
			{
				index -= M2;
			}
			f[i][0] = temp[index][0];
		}
		fftw_free(temp);
		temp = 0;
		fftw_plan p = fftw_plan_dft_1d(M2, f, f, FFTW_FORWARD, FFTW_ESTIMATE);
		fftw_execute(p);
		fftw_destroy_plan(p);
		for (int i=0; i<M2; i++)
		{
			a[i] = f[i][0] / ((double)M2);
		}
		fftw_free(f);
		f = 0;
	}
	// line 6
	{
		double* temp = new double[N];
		for (int i=0; i<N; i++)
		{
			temp[i] = a[i+1];
		}
		delete []a;
		a = new double[N];
		for (int i=0; i<N; i++)
		{
			a[i] = temp[N-i-1];
		}
		delete []temp;
	}
	// line 7
	fftw_complex Z;
	{
		double a = z[0];
		double b = z[1];
		double denom = a*a + (b+L)*(b+L);
		Z[0] = (-a*a - b*b + L*L) / denom;
		Z[1] = (2*a*L) / denom;
	}
	fftw_complex p;
	{
		// p = polyval(a,Z);
		fftw_complex b;
		b[0] = a[0];
		b[1] = 0;
		for (int i=1; i<N; i++)
		{
			// b_new = a[i] + b_old*Z;
			fftw_complex temp;
			temp[0] = b[0]*Z[0] - b[1]*Z[1];
			temp[1] = b[1]*Z[0] + b[0]*Z[1];
			b[0] = temp[0] + a[i];
			b[1] = temp[1];
		}
		//p = b
		p[0] = b[0];
		p[1] = b[1];
	}
	delete []a;
	a=0;
	// line 8
	{
		double a = p[0];
		double b = p[1];
		double c = z[0];
		double d = z[1];
		double denom = c*c + (d+L)*(d+L);
		*(w_re) = (-2*a*c*c - 4*b*c*d + 2*a*d*d - 4*b*c*L + 4*a*d*L + 2*a*L*L) / (denom*denom) + (d/sqrt(M_PI) + L/sqrt(M_PI)) / denom;
		*(w_im) = (-2*b*c*c + 4*a*c*d + 2*b*d*d + 4*a*c*L + 4*b*d*L + 2*b*L*L) / (denom*denom) + (c/sqrt(M_PI)) / denom;
	}
	return true;
}

bool my_erfs::erf(fftw_complex w, double* ret_re, double* ret_im, int N)
{
	//Let input be w. Set z=i*w. Compute Faddeeva(z). erf(w)=1-Faddeeva(z,N)/exp(-z*z)
	fftw_complex z;
	z[0] = -w[1];
	z[1] = w[0];
	fftw_complex f;
	f[0] = f[1] = 0;
	// call faddeeva with the right imaginary component.
	if (z[1] > 0)
	{
		faddeeva(z, &(f[0]), &(f[1]), N);
	}
	else
	{
		fftw_complex neg_z;
		neg_z[0] = -1*z[0];
		neg_z[1] = -1*z[1];
		fftw_complex minusZZ;
		minusZZ[0] = -z[0]*z[0] + z[1]*z[1];
		minusZZ[1] = -2.0*z[0]*z[1];
		fftw_complex twoExpMinusZZ;
		twoExpMinusZZ[0] = 2.0*exp(minusZZ[0])*cos(minusZZ[1]);
		twoExpMinusZZ[1] = 2.0*exp(minusZZ[0])*sin(minusZZ[1]);
		faddeeva(neg_z, &(f[0]), &(f[1]), N);
		f[0] = twoExpMinusZZ[0] - f[0];
		f[1] = twoExpMinusZZ[1] - f[1];
	}
	{
		double a = f[0];
		double b = f[1];
		double c = z[0];
		double d = z[1];
		double cos2cd = cos(2*c*d);
		double sin2cd = sin(2*c*d);
		double e = exp(c*c-d*d);
		*(ret_re) = 1 - a*e*cos2cd + b*e*sin2cd;
		*(ret_im) = -b*e*cos2cd -a*e*sin2cd;
	}
	return true;
}

// erfi(w) (used in mathematica) is erf(iw)/i
bool my_erfs::erfi(fftw_complex w, double* ret_re, double* ret_im, int N)
{
	if (!ret_re || !ret_im)
	{
		return false;
	}
	fftw_complex iw;
	iw[0] = -w[1];
	iw[1] = w[0];
	if (!erf(iw, ret_re, ret_im, N))
	{
		return false;
	}
	double a = *ret_re, b = *ret_im;
	*ret_re = b;
	*ret_im = -a;
	return true;
}
