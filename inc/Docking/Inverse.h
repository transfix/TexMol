#ifndef CCV_INVERSE_H
#define CCV_INVERSE_H

bool obtainPeaks(fftw_complex* sparseProfile, fftw_complex* centerFrequenciesProduct, int size, int numberOfPeaks, int zoomFactor, ValuePosition3D* valuePosition3D, int alpha, int m, int rotationIndex, SmoothingFunction* smoothingFunction);

#endif
