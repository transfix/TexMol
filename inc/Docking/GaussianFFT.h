#ifndef CCV_DOCKING_GAUSSIANFFT_H
#define CCV_DOCKING_GAUSSIANFFT_H

namespace GaussianFFT
{
	bool getFourierCoefficient(int n, double w, double* ret_re, double* ret_im, int N);
};

#endif
