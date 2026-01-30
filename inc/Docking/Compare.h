#ifndef CCV_COMPARE_H
#define CCV_COMPARE_H

bool compareArrays(fftw_complex* centerData, int smallSize, fftw_complex* data, int size, const char* caption, bool compareAll);
bool compareVolumes(fftw_complex* v1, fftw_complex* v2, int size, double* errorl2, double* lInfinity);

#endif
