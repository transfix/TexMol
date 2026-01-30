#ifndef CVC_DOCKING_ERFS_H
#define CVC_DOCKING_ERFS_H

namespace my_erfs
{
	double erf(double x);
	bool erf(fftw_complex w, double* ret_re, double* ret_im, int N);
	bool erfi(fftw_complex w, double* ret_re, double* ret_im, int N);
};

#endif
