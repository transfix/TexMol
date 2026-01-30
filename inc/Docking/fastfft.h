#ifndef CCV_NDFT_H
#define CCV_NDFT_H

bool getCenterFrequencies(int numAtoms, double alpha, int interpFuncExtent, int numFreqCubeRoot, int gridSize, double* xk, double* yk, double* zk, fftw_complex* fk, fftw_complex* ourFrequencies, FILE* fOut, SmoothingFunction* smoothingFunction);
bool getCenterFrequencies(int numAtoms, double alpha, int interpFuncExtent, int numFreqCubeRoot, int gridSize, double* xk, double* yk, double* zk, fftw_complex* fk, fftw_complex* ourFrequencies, FILE* fOut, SmoothingFunction* smoothingFunction, fftw_complex* Ian1, fftw_complex* ourMoreFrequencies, fftw_plan moreFreqPlan);

#endif
