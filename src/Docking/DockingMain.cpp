#include "fftw3.h"
#include <Docking/Compare.h>
#include <Docking/Complex.h>
#include <Docking/DockingMain.h>
#include <Docking/erfs.h>
#include <Docking/fullfft.h>
#include <Docking/GaussianFFT.h>
#include <Docking/IO.h>
#include <Docking/PrintTransformations.h>
#include <Docking/Ranking.h>
#include <Docking/TopValues.h>
#include <Docking/ValuePosition3D.h>
#include <Geometry/Geometry.h>
#include <GOAFileTypes/GOALoader.h>

#if ! defined(__APPLE__)
#include <stdlib.h>
#endif

#include <math.h>
#include <pthread.h>
#include <SmoothingFunction/CardinalBSpline.h>
#include <SmoothingFunction/Gaussian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#ifndef M_PI
#define 	M_PI   3.14159265358979323846
#endif

// Include ordering error if these goes above
#include <Docking/fastfft.h>
#include <Docking/Inverse.h>

using namespace my_erfs;
using CCVOpenGLMath::Matrix;
//using PDBParser::Atom;
using PDBParser::GroupOfAtoms;

typedef struct
{
	int confID;
	int startRotation, endRotation, numberOfRotations;
	float* rotations;
	int startFineRotation, endFineRotation, numberOfFineRotations;
	float* fineRotations;
	int numberOfPositions;
	double* xkBOrig, *ykBOrig, *zkBOrig;
	double* xkB, *ykB, *zkB;
	int numCentersB;
	int gridSize, numFreq, interpFuncExtent;
	double largestEdge;
	double alpha, blobbiness, elecScale;
	double scaleA;
	fftw_complex* centerFrequenciesA, *centerElecFrequenciesA;
	fftw_complex* centerFrequenciesB, *centerElecFrequenciesB;
	fftw_complex* centerFrequenciesProduct, *centerFrequenciesElecProduct;
	fftw_complex* sparseProfile, *sparseElecProfile;
	fftw_plan freqPlan, elecFreqPlan;
	fftw_complex* fkB, *fkBElec;
	fftw_complex* freqHat;
	fftw_plan freqHatPlan;
	fftw_complex* Ian1, *ourMoreFrequencies;
	fftw_plan moreFreqPlan;
	fftw_complex* smallElectrostaticsKernel;
	SmoothingFunction* smoothingFunction;
	TopValues* localTopValues;
} PARAMS;

typedef struct
{
	double score;
	int rotationID, conformationID;
} PEAKS;

// Get the current time in seconds as a double value
extern double getTime();

bool printDataChars(const char* header, fftw_complex* data, int size, bool normalize)
{
	if (!data)
	{
		return false;
	}
	int numneg = 0;
	int numpos = 0;
	int numzero = 0;
	int minx=0, miny=0, minz=0;
	int maxx=0, maxy=0, maxz=0;
	double maxVal = data[0][0];
	double minVal = data[0][0];
	int c = 0;
	for (int i=0; i<size; i++)
	{
		for (int j=0; j<size; j++)
		{
			for (int k=0; k<size; k++)
			{
				if (data[c][0] > maxVal)
				{
					maxVal = data[c][0];
					maxx=i;
					maxy=j;
					maxz=k;
				}
				if (data[c][0] < minVal)
				{
					minVal = data[c][0];
					minx=i;
					miny=j;
					minz=k;
				}
				if (data[c][0] < -0.000001)
				{
					numneg++;
				}
				else if (data[c][0] > 0.000001)
				{
					numpos++;
				}
				else
				{
					numzero++;
				}
				c++;
			}
		}
	}
	if (normalize)
	{
		maxVal /= (pow(size, 6.0));
	}
	if (normalize)
	{
		minVal /= (pow(size, 6.0));
	}
	printf("------%s------\n", header);
	printf("\t%d %d %d neg, zero and pos in data, min,max was %lf,%lf at [%d %d %d],[%d %d %d]\n",
		   numneg, numzero, numpos, minVal, maxVal, minx, miny, minz, maxx, maxy, maxz);
	printf("---------------\n");
	return true;
}

bool printError(fftw_complex* x, fftw_complex* y, int size, const char* header)
{
	double l2Error = 0;
	double lMaxError = 0;
	if (!compareVolumes(x, y, size, &l2Error, &lMaxError))
	{
		return false;
	}
	printf("------%s------\n", header);
	printf("\tl2 and lmax error is %lf,%lf\n", l2Error, lMaxError);
	printf("---------------\n");
	return true;
}

// we need to add neg of the dot product ( since + + should repel etc )
void updateSCwithElec(fftw_complex* sparseProfile, fftw_complex* sparseElecProfile, int numFreq, double scale)
{
	for (int c=0; c<numFreq*numFreq*numFreq; c++)
	{
		if (scale < -0.01)
		{
			if (sparseProfile[c][0] > 0)
			{
				sparseProfile[c][0] = -sparseElecProfile[c][0];
			}
		}
		else
		{
			if (sparseProfile[c][0] > 0)
			{
				sparseProfile[c][0] -= sparseElecProfile[c][0]*scale;
			}
		}
	}
}

double getVal(fftw_complex* data, int size, int x, int y, int z, int off)
{
	double value = -1000000.0;
	int iidx, jidx, kidx;
	for (int i=x-off; i<=x+off; i++)
	{
		iidx = i;
		if (iidx < 0)
		{
			iidx = size-iidx;
		}
		if (iidx >=size)
		{
			iidx = iidx-size;
		}
		for (int j=y-off; j<=y+off; j++)
		{
			jidx = j;
			if (jidx < 0)
			{
				jidx = size-jidx;
			}
			if (jidx >=size)
			{
				jidx = jidx-size;
			}
			for (int k=z-off; k<=z+off; k++)
			{
				kidx = k;
				if (kidx < 0)
				{
					kidx = size-kidx;
				}
				if (kidx >=size)
				{
					kidx = kidx-size;
				}
				int idx = iidx + jidx*size + kidx*size*size;
				if (data[idx][0] > value)
				{
					value = data[idx][0];
				}
			}
		}
	}
	return value;
}

bool updatePeakGrade(int* totalAbovePeak, int* totalBelowPeak, double peakVal, fftw_complex* sparseProfile, int size)
{
	if (!sparseProfile || (size<1))
	{
		return false;
	}
	for (int c=0; c<size*size*size; c++)
	{
		if (sparseProfile[c][0] >= peakVal)
		{
			(*totalAbovePeak)++;
		}
		else
		{
			(*totalBelowPeak)++;
		}
	}
	return true;
}

bool compute3DFFT(fftw_complex* dest,fftw_complex* src, int dim1, int dim2, int dim3, int type, unsigned int flags)
{
	if (!dest || !src || dim1<1 || dim2<1 || dim3<1)
	{
		return false;
	}
	fftw_plan p;
	p = fftw_plan_dft_3d(dim1, dim2, dim3, src, dest, type, flags);
	fftw_execute(p);
	fftw_destroy_plan(p);
	return true;
}

bool fftshift3D(fftw_complex* data, int length)
{
	if (!data || (length<1))
	{
		return false;
	}
	fftw_complex* temp = (fftw_complex*)malloc(sizeof(fftw_complex) * length * length * length);
	for (int c=0; c< length * length * length; c++)
	{
		temp[c][0] = data[c][0];
		temp[c][1] = data[c][1];
	}
	for (int i=0; i<length; i++)
	{
		for (int j=0; j<length; j++)
		{
			for (int k=0; k<length; k++)
			{
				int shift_i = (i+length/2)%length;
				int shift_j = (j+length/2)%length;
				int shift_k = (k+length/2)%length;
				data[shift_i* length* length + shift_j* length + shift_k][0] = temp[i*length*length + j*length + k][0];
				data[shift_i* length* length + shift_j* length + shift_k][1] = temp[i*length*length + j*length + k][1];
			}
		}
	}
	free(temp);
	return true;
}

//   abcd -------------------------- efgh  is converted to
//   abcd 00000000000000000000000000 efgh
bool zeroPadFrequencies(fftw_complex* smallGrid, int smallSize, fftw_complex* largeGrid, int largeSize)
{
	if (!smallGrid || !largeGrid || (smallSize<1) || (largeSize<smallSize))
	{
		return false;
	}
	for (int c=0; c<largeSize*largeSize*largeSize; c++)
	{
		largeGrid[c][0] = largeGrid[c][1] = 0;
	}
	int c=0;
	for (int i=0; i<smallSize; i++)
	{
		int iIndex = i;
		if (i>= smallSize/2)
		{
			iIndex = largeSize-smallSize+i;
		}
		for (int j=0; j<smallSize; j++)
		{
			int jIndex = j;
			if (j>= smallSize/2)
			{
				jIndex = largeSize-smallSize+j;
			}
			for (int k=0; k<smallSize; k++)
			{
				int kIndex = k;
				if (k>= smallSize/2)
				{
					kIndex = largeSize-smallSize+k;
				}
				int ijkIndex = iIndex*largeSize*largeSize + jIndex*largeSize + kIndex;
				largeGrid[ijkIndex][0] = smallGrid[c][0];
				largeGrid[ijkIndex][1] = smallGrid[c][1];
				c++;
			}
		}
	}
	return true;
}

// The fft of a truncated gaussian is returned in freqHat
void fftTruncHat(fftw_complex* freqHat, int N, double blobbyness, double scale)
{
	fftw_plan p;
	int width = N/2;
	double sigma = 1.0 / (sqrt(2*-blobbyness));
	for (int i=0; i<N; i++)
	{
		freqHat[i][0] = 0;
		freqHat[i][1] = 0;
	}
	for (int i=0; i<(width)+1; i++)
	{
		freqHat[i][0] = exp(-i*i/(2.0*sigma*sigma*scale*scale));
	}
	for (int i=N-1; i>(N-1)-(width); i--)
	{
		freqHat[i][0] = exp(-(i-N)*(i-N)/(2.0*sigma*sigma*scale*scale));
	}
	p = fftw_plan_dft_1d(N, freqHat, freqHat, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
}

void fftTruncHat(fftw_complex* freqHat, fftw_plan freqHatPlan, int N, double blobbyness, double scale)
{
	int width = N/2;
	double sigma = 1.0 / (sqrt(2*-blobbyness));
	for (int i=0; i<N; i++)
	{
		freqHat[i][0] = 0;
		freqHat[i][1] = 0;
	}
	for (int i=0; i<(width)+1; i++)
	{
		freqHat[i][0] = exp(-i*i/(2.0*sigma*sigma*scale*scale));
	}
	for (int i=N-1; i>(N-1)-(width); i--)
	{
		freqHat[i][0] = exp(-(i-N)*(i-N)/(2.0*sigma*sigma*scale*scale));
	}
	fftw_execute(freqHatPlan);
}


double getDistFromCenter(int gridPoint, int size, double scale)
{
	return (size/2-gridPoint)*scale;
}

// The fft of the elec kernel is returned in 'kernel'
bool computeElecKernel(fftw_complex* kernel, int size, double scale)
{
	int c=0;
	for (int i=0; i<size; i++)
	{
		double iDist = getDistFromCenter(i, size, scale);
		for (int j=0; j<size; j++)
		{
			double jDist = getDistFromCenter(j, size, scale);
			for (int k=0; k<size; k++)
			{
				double kDist = getDistFromCenter(k, size, scale);
				double dist = sqrt(iDist*iDist + jDist*jDist + kDist*kDist);
				double radiusContribution = 0;
				double dielectricContribution = 0;
				{
					double distA = dist/scale;
					if (distA<1.0)
					{
						radiusContribution = 1;
					}
					else
					{
						radiusContribution = 1.0/distA;
					}
					if (distA<=6.0)
					{
						dielectricContribution = 4;
					}
					else if (distA < 8.0)
					{
						dielectricContribution = 38*distA-224;
					}
					else
					{
						dielectricContribution = 80;
					}
				}
				kernel[c][0] = radiusContribution + 1.0/dielectricContribution;
				kernel[c][1] = 0;
				c++;
			}
		}
	}
	if (!fftshift3D(kernel, size))
	{
		return false;
	}
	if (!compute3DFFT(kernel, kernel, size, size, size, FFTW_FORWARD, FFTW_ESTIMATE))
	{
		return false;
	}
	return true;
}

void getFullConvolution(int N, int M,  fftw_complex* fftValsA, fftw_complex* fftValsB, fftw_complex* fullConvolution, double blobbyness, double scale)
{
	fftw_complex* freqHat;
	int c = 0;
	freqHat = (fftw_complex*)malloc(sizeof(fftw_complex) * N);
	fftTruncHat(freqHat, N, blobbyness, 1);
	for (int j=0; j<N; j++)
	{
		for (int k=0; k<N; k++)
		{
			for (int i=0; i<N; i++)
			{
				double val = freqHat[i][0]*freqHat[j][0]*freqHat[k][0];
				fullConvolution[c][0] = (fftValsA[c][0] * fftValsB[c][0] - fftValsA[c][1] * fftValsB[c][1]) * val * val;
				fullConvolution[c][1] = (fftValsA[c][0] * fftValsB[c][1] + fftValsA[c][1] * fftValsB[c][0]) * val * val;
				c++;
			}
		}
	}
	free(freqHat);
}

bool multiplyFrequencyMaps(fftw_complex* frequenciesA, fftw_complex* frequenciesB, int numFreq, int gridSize, double blobbiness, fftw_complex* frequenciesProduct, double scale)
{
	if (!frequenciesA || !frequenciesB || !frequenciesProduct || (numFreq<1) || (gridSize<1) || (numFreq>gridSize))
	{
		return false;
	}
	fftw_complex* freqHat;
	freqHat = (fftw_complex*)malloc(sizeof(fftw_complex) * gridSize);
	fftTruncHat(freqHat, gridSize, blobbiness, scale);
	int i, j, k, c=0;
	for (k=0; k<numFreq; k++)
	{
		int kIndex = k;
		if (kIndex >= numFreq/2)
		{
			kIndex += (gridSize-numFreq);
		}
		for (j=0; j<numFreq; j++)
		{
			int jIndex = j;
			if (jIndex >= numFreq/2)
			{
				jIndex += (gridSize-numFreq);
			}
			for (i=0; i<numFreq; i++)
			{
				int iIndex = i;
				if (iIndex >= numFreq/2)
				{
					iIndex += (gridSize-numFreq);
				}
				double val = freqHat[iIndex][0]*freqHat[jIndex][0]*freqHat[kIndex][0];
				frequenciesProduct[c][0] = (frequenciesA[c][0] * frequenciesB[c][0] - frequenciesA[c][1] * frequenciesB[c][1]) * val * val;
				frequenciesProduct[c][1] = (frequenciesA[c][0] * frequenciesB[c][1] + frequenciesA[c][1] * frequenciesB[c][0]) * val * val;
				c++;
			}
		}
	}
	free(freqHat);
	return true;
}

bool multiplyFrequencyMaps(fftw_complex* frequenciesA, fftw_complex* frequenciesB, int numFreq, int gridSize, double blobbiness, fftw_complex* frequenciesProduct, fftw_complex* freqHat, fftw_plan freqHatPlan, double scale)
{
	if (!frequenciesA || !frequenciesB || !frequenciesProduct || (numFreq<1) || (gridSize<1) || (numFreq>gridSize))
	{
		return false;
	}
	fftTruncHat(freqHat, freqHatPlan, gridSize, blobbiness, scale);
	int i, j, k, c=0;
	for (k=0; k<numFreq; k++)
	{
		int kIndex = k;
		if (kIndex >= numFreq/2)
		{
			kIndex += (gridSize-numFreq);
		}
		for (j=0; j<numFreq; j++)
		{
			int jIndex = j;
			if (jIndex >= numFreq/2)
			{
				jIndex += (gridSize-numFreq);
			}
			for (i=0; i<numFreq; i++)
			{
				int iIndex = i;
				if (iIndex >= numFreq/2)
				{
					iIndex += (gridSize-numFreq);
				}
				double val = freqHat[iIndex][0]*freqHat[jIndex][0]*freqHat[kIndex][0];
				frequenciesProduct[c][0] = (frequenciesA[c][0] * frequenciesB[c][0] - frequenciesA[c][1] * frequenciesB[c][1]) * val * val;
				frequenciesProduct[c][1] = (frequenciesA[c][0] * frequenciesB[c][1] + frequenciesA[c][1] * frequenciesB[c][0]) * val * val;
				c++;
			}
		}
	}
	return true;
}

bool multiplyElecFrequencyMaps(fftw_complex* frequenciesA, fftw_complex* frequenciesB, int numFreq, int kernelSize, fftw_complex* frequenciesElecKernel, fftw_complex* frequenciesProduct, double scale)
{
	if (!frequenciesA || !frequenciesB || !frequenciesElecKernel || !frequenciesProduct || (numFreq<1) || (kernelSize<1) || (numFreq>kernelSize))
	{
		return false;
	}
	int i, j, k, c=0;
	for (k=0; k<numFreq; k++)
	{
		int kIndex = k;
		if (kIndex >= numFreq/2)
		{
			kIndex += (kernelSize-numFreq);
		}
		for (j=0; j<numFreq; j++)
		{
			int jIndex = j;
			if (jIndex >= numFreq/2)
			{
				jIndex += (kernelSize-numFreq);
			}
			for (i=0; i<numFreq; i++)
			{
				int iIndex = i;
				if (iIndex >= numFreq/2)
				{
					iIndex += (kernelSize-numFreq);
				}
				double val = frequenciesElecKernel[kIndex*numFreq*numFreq + jIndex*numFreq + iIndex][0];
				frequenciesProduct[c][0] = (frequenciesA[c][0] * frequenciesB[c][0] - frequenciesA[c][1] * frequenciesB[c][1]) * val;
				frequenciesProduct[c][1] = (frequenciesA[c][0] * frequenciesB[c][1] + frequenciesA[c][1] * frequenciesB[c][0]) * val;
				c++;
			}
		}
	}
	return true;
}

bool inititalizeFFTW(int size)
{
	if (size<1)
	{
		return false;
	}
	fftw_complex* data = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*size*size*size);
	{
		int i;
		for (i=0; i<size*size*size; i++)
		{
			data[i][0] = (double)rand()/((double)(RAND_MAX));
			data[i][1] = (double)rand()/((double)(RAND_MAX));
		}
	}
	if (!compute3DFFT(data,data, size, size, size, FFTW_BACKWARD, FFTW_MEASURE))
	{
		return false;
	}
	if (!compute3DFFT(data,data, size, size, size, FFTW_FORWARD, FFTW_MEASURE))
	{
		return false;
	}
	fftw_free(data);
	return true;
}


/*
  parseArguments has been modified so that it can be invoked through TexMol.
  Originally, it was written assuming a standalone Docking program.
*/

bool parseArguments(int argc, char* argv[],
					int* numThreads,
					double* alpha,
					int* interpFuncExtent,
					int* numFreq,
					int* gridSize,
					char* inputFile1,
					char* inputFile2,
					char* boundConformationFile2,
					char* rotationFile,
					int* numberOfRotations,
					int* numberOfPeaksForFineRotation,
					char* fineRotationFile,
					int* numberOfFineRotations,
					char* outputFileName,
					int* numberOfPositions,
					bool* reDocking,
					double* blobbiness,
					double* elecScale,
					int* statInterval,
					double* distanceCutoff)
{
	if (argc != 22)
	{
		printf("\n\nUsage : <exe file> -docking numThreads alpha interpFuncExtent numFreq gridSize inputFile1 inputFile2 boundConformationFile rotationFile numberOfRotations numberOfPeaksForFineRotation fineRotationFile numberOfFineRotations outputFileName numberOfOutputPositions reDocking blobbiness elecScale\n\tinterpFuncExtent should be greater than 1\nreDocking should be true or false\n\n");
		return false;
	}
	char reDockingStr[256];
	*numThreads = atoi(argv[2]);
	*alpha = atof(argv[3]);
	*interpFuncExtent = atoi(argv[4]);
	*numFreq = atoi(argv[5]);
	*gridSize = atoi(argv[6]);
	strcpy(inputFile1, argv[7]);
	strcpy(inputFile2, argv[8]);
	strcpy(boundConformationFile2, argv[9]);
	strcpy(rotationFile, argv[10]);
	*numberOfRotations = atoi(argv[11]);
	*numberOfPeaksForFineRotation = atoi(argv[12]);
	strcpy(fineRotationFile, argv[13]);
	*numberOfFineRotations = atoi(argv[14]);
	strcpy(outputFileName, argv[15]);
	*numberOfPositions = atoi(argv[16]);
	strcpy(reDockingStr, argv[17]);
	*blobbiness = atof(argv[18]);
	*elecScale = atof(argv[19]);
	*statInterval = atoi(argv[20]);
	*distanceCutoff = atof(argv[21]);
	if ((*numThreads < 1) || (*alpha)<1 || (*interpFuncExtent)<1 || (*numFreq)<1 || (*gridSize)<1 || (*blobbiness)>=0 || (*numberOfRotations<1))
	{
		return false;
	}
	if (strcmp(reDockingStr, "true") == 0)
	{
		*reDocking = true;
	}
	else if (strcmp(reDockingStr, "false") == 0)
	{
		*reDocking = false;
	}
	else
	{
		printf("Argument for 'reDocking' should be either 'true' or 'false'\n");
		return false;
	}
	return true;
}

bool parseArguments(int argc, char* argv[],
					int* numThreads,
					double* alpha,
					int* interpFuncExtent,
					int* numFreq,
					int* gridSize,
					char* inputFile1,
					char* inputFile2,
					char* boundConformationFile2,
					char* rotationFile,
					int* numberOfRotations,
					int* numberOfPeaksForFineRotation,
					char* fineRotationFile,
					int* numberOfFineRotations,
					char* outputFileName,
					int* numberOfPositions,
					bool* reDocking,
					double* blobbiness,
					double* elecScale,
					int* statInterval,
					double* distanceCutoff,
					int* numFile2)
{
	if (argc != 23)
	{
		printf("\n\nUsage : <exe file> -docking numThreads alpha interpFuncExtent numFreq gridSize inputFile1 inputFile2 boundConformationFile2 rotationFile numberOfRotations numberOfPeaksForFineRotation fineRotationFile numberOfFineRotations outputFileName numberOfOutputPositions reDocking blobbiness elecScale numFile2\n\tinterpFuncExtent should be greater than 1\nreDocking should be true or false\n\n");
		return false;
	}
	if (!parseArguments(argc - 1, argv, numThreads, alpha, interpFuncExtent, numFreq, gridSize,
						inputFile1, inputFile2, boundConformationFile2,
						rotationFile, numberOfRotations, numberOfPeaksForFineRotation, fineRotationFile, numberOfFineRotations,
						outputFileName, numberOfPositions, reDocking,
						blobbiness, elecScale, statInterval, distanceCutoff))
	{
		return false;
	}
	*numFile2 = atoi(argv[ 22 ]);
	return true;
}

bool readCenters(double** xkOrig, double** ykOrig, double** zkOrig, int* numCenters,
				 fftw_complex** fk, fftw_complex** fkElec, double real_magnitude, double imag_magnitude, const char* fileName,
				 bool docking, bool firstMol)
{
	if (!xkOrig || !ykOrig || !zkOrig || !numCenters || !fk || !fkElec || !fileName)
	{
		return false;
	}
	FILE* fp;
	if ((fp = fopen(fileName, "r")) == NULL)
	{
		return false;
	}
	fscanf(fp, "%d\n", numCenters);
	if ((*numCenters) < 1)
	{
		return false;
	}
	(*xkOrig) = new double[(*numCenters)];
	(*ykOrig) = new double[(*numCenters)];
	(*zkOrig) = new double[(*numCenters)];
	(*fk) = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (*numCenters));
	(*fkElec) = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (*numCenters));
	int i;
	for (i=0; i<(*numCenters); i++)
	{
		char atomType = 'X';
		double charge = 0;
		if (fscanf(fp, "%c %lf %lf %lf %lf\n", &atomType, &((*xkOrig)[i]), &((*ykOrig)[i]), &((*zkOrig)[i]), &charge) != 5)
		{
			fclose(fp);
			return false;
		}
		if (firstMol)
		{
			(*fkElec)[i][0] = -charge;
		}
		else
		{
			(*fkElec)[i][0] = charge;
		}
		if (atomType == 'I')
		{
			if (docking)
			{
				(*fk)[i][0] = 0;
				(*fk)[i][1] = imag_magnitude;
			}
			else
			{
				(*fk)[i][0] = real_magnitude;
				(*fk)[i][1] = 0;
			}
		}
		else if (atomType == 'E')
		{
			(*fk)[i][0] = real_magnitude;
			(*fk)[i][1] = 0;
		}
		else
		{
			fclose(fp);
			return false;
		}
	}
	fclose(fp);
	return true;
}

bool readCenters(double** xkOrig, double** ykOrig, double** zkOrig, double** xk, double** yk, double** zk, int* numCenters,
				 fftw_complex** fk, fftw_complex** fkElec, double real_magnitude, double imag_magnitude, const char* fileName,
				 bool docking, bool firstMol)
{
	if (!xk || !yk || !zk)
	{
		return false;
	}
	if (!readCenters(xkOrig, ykOrig, zkOrig, numCenters, fk, fkElec, real_magnitude, imag_magnitude, fileName, docking, firstMol))
	{
		return false;
	}
	(*xk) = new double[(*numCenters)];
	(*yk) = new double[(*numCenters)];
	(*zk) = new double[(*numCenters)];
	return true;
}

// Rotates a given point about the origin by the given angles
void rotatePointAboutOrigin(double* x, double* y, double* z, double theta1, double theta2, double theta3)
{
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	// multiply by theta1
	x1 =  cos(theta1) * (*x)  + sin(theta1) * (*y)  + 0         * (*z);
	y1 = -sin(theta1) * (*x)  + cos(theta1) * (*y)  + 0         * (*z);
	z1 = 0              * (*x)  + 0             * (*y)  + 1         * (*z);
	// multiply by theta2
	x2 =  cos(theta2) * x1 + 0              * y1 + -sin(theta2) * z1;
	y2 = 0              * x1 + 1              * y1 + 0              * z1;
	z2 =  sin(theta2) * x1 + 0              * y1 +  cos(theta2) * z1;
	// multiply by theta3
	x3 = 1              * x2 + 0              * y2 + 0              * z2;
	y3 = 0              * x2 +  cos(theta3) * y2 +  sin(theta3) * z2;
	z3 = 0              * x2 + -sin(theta3) * y2 +  cos(theta3) * z2;
	*x = x3;
	*y = y3;
	*z = z3;
}

// here we are given the rotation matrix itself.
void rotatePointAboutOrigin(double* x, double* y, double* z,
							float r1, float r2, float r3,
							float r4, float r5, float r6,
							float r7, float r8, float r9)
{
	double x1, y1, z1;
	x1 = r1*(*x) + r2*(*y) + r3*(*z);
	y1 = r4*(*x) + r5*(*y) + r6*(*z);
	z1 = r7*(*x) + r8*(*y) + r9*(*z);
	(*x) = x1;
	(*y) = y1;
	(*z) = z1;
}

// return the min max of the array
void getMinMax(double* x, double* minVal, double* maxVal, int n)
{
	(*minVal) = (*maxVal) = x[0];
	int i;
	for (i=1; i<n; i++)
	{
		if ((*minVal) > x[i])
		{
			(*minVal) = x[i];
		}
		if ((*maxVal) < x[i])
		{
			(*maxVal) = x[i];
		}
	}
}

bool center(double* x, double* y, double* z, int n, double* xTrans,  double* yTrans, double* zTrans)
{
	if (!x || !y || !z || (n<1))
	{
		return false;
	}
	double minXVal, maxXVal;
	double minYVal, maxYVal;
	double minZVal, maxZVal;
	getMinMax(x, &minXVal, &maxXVal, n);
	getMinMax(y, &minYVal, &maxYVal, n);
	getMinMax(z, &minZVal, &maxZVal, n);
	{
		*xTrans = - minXVal - (maxXVal - minXVal)/2.0;
		*yTrans = - minYVal - (maxYVal - minYVal)/2.0;
		*zTrans = - minZVal - (maxZVal - minZVal)/2.0;
		int i;
		for (i=0; i<n; i++)
		{
			x[i] = x[i] + (*xTrans);
			y[i] = y[i] + (*yTrans);
			z[i] = z[i] + (*zTrans);
		}
	}
	return true;
}

bool invert(double* x, double* y, double* z, int n)
{
	if (!x || !y || !z || (n<1))
	{
		return false;
	}
	int i;
	for (i=0; i<n; i++)
	{
		x[i] = -x[i];
		y[i] = -y[i];
		z[i] = -z[i];
	}
	return true;
}

bool getLargestSize(double* x, double* y, double* z, int n, double* maxLength)
{
	double minVal, maxVal;
	getMinMax(x, &minVal, &maxVal, n);
	double xLen = maxVal - minVal;
	getMinMax(y, &minVal, &maxVal, n);
	double yLen = maxVal - minVal;
	getMinMax(z, &minVal, &maxVal, n);
	double zLen = maxVal - minVal;
	if (xLen >= yLen && xLen >= zLen)
	{
		(*maxLength) = xLen;
	}
	else if (yLen >= xLen && yLen >= zLen)
	{
		(*maxLength) = yLen;
	}
	else
	{
		(*maxLength) = zLen;
	}
	return true;
}

bool getLargestEdge(double* x1, double* y1, double* z1, int n1, double* x2, double* y2, double* z2, int n2, double* largestEdge)
{
	if (!x1 || !y1 || !z1 || !x2 || !y2 || !z2 || (n1<1) || (n2<1) || !largestEdge)
	{
		return false;
	}
	double l1 = 0;
	if (!getLargestSize(x1, y1, z1, n1, &l1))
	{
		return false;
	}
	double l2 = 0;
	if (!getLargestSize(x2, y2, z2, n2, &l2))
	{
		return false;
	}
	if (l1 >= l2)
	{
		*largestEdge = l1;
	}
	else
	{
		*largestEdge = l2;
	}
	return true;
}

bool transformAndNormalize(double* xkOrig, double* ykOrig, double* zkOrig,
						   double* xk, double* yk, double* zk,
						   int numCenters,
						   float r1, float r2, float r3, float r4, float r5, float r6, float r7, float r8, float r9,
						   int gridSize, int numFreq, int interpFuncExtent, double largestSize, int protein,
						   double* scaleVal)
{
	if (!xkOrig || !ykOrig || !zkOrig || !xk || !yk || !zk || (numCenters<1) || (gridSize<1))
	{
		return false;
	}
	// copy
	{
		int i;
		for (i=0; i<numCenters; i++)
		{
			xk[i] = xkOrig[i];
			yk[i] = ykOrig[i];
			zk[i] = zkOrig[i];
		}
	}
	// center
	if (protein == 2)
	{
		// if we are looking at the second molecule, we flip it.
		// This is because the convolution flips one molecule.
		if (!invert(xk, yk, zk, numCenters))
		{
			return false;
		}
		// rotate the second protein as well
		int i;
		for (i=0; i<numCenters; i++)
		{
			rotatePointAboutOrigin(&(xk[i]), &(yk[i]), &(zk[i]), r1, r2, r3, r4, r5, r6, r7, r8, r9);
		}
	}
	// normalize
	{
		double scale = 1.0/largestSize;                            // [-0.5 .. 0.5)
		//(for all rotations, for both molecules )
		scale *= 0.5;                                              // [-0.25 .. 0.25)
		scale *= numFreq / ((double)(numFreq + 2*interpFuncExtent)); // add space to perform gridding
		*scaleVal = scale;
		int i;
		for (i=0; i<numCenters; i++)
		{
			xk[i] *= scale;
			yk[i] *= scale;
			zk[i] *= scale;
		}
	}
	return true;
}


void printIntermediateStats(FILE* fp, TopValues* curTopValues, float* rotations, float* fineRotations, float* translate_A, float* translate_B, float* scale_B,
			    Docking::DockComplex* baseComplex, bool reDocking, std::vector< PDBParser::Atom* > *unboundLigandAtomList, std::vector< int > *unboundLigandInterfaceAtomIndex,
							int gridSize, double functionScaleFactor, int rmsdToReport, int rmsdGood)
{
	int n = curTopValues->getCurrentNumberOfPositions();
	double gridFactor = (double) gridSize / (double) curTopValues->getGridSize();
	double v, x, y, z, rmsd;
	int r, f, c;
	double mv, mx, my, mz, mrmsd = 100000000;
	int mr, mf, mc, rank;
	int counter[ rmsdToReport ]; // keep until 20A
	int highestPos[ rmsdToReport ];
	int maxRank = n + 1;
	for (int i = 0; i < rmsdToReport; i++)
	{
		counter[ i ] = 0;
		highestPos[ i ] = maxRank;
	}
	fprintf(fp, "\nnumber of peaks = %d\n", n);
	while (n--)
	{
		curTopValues->extractMin(&v, &x, &y, &z, &r, &f, &c);
		x *= gridFactor;
		y *= gridFactor;
		z *= gridFactor;
		Matrix transformation = Matrix::translation(translate_B[ 3 * c + 0 ], translate_B[ 3 * c + 1 ], translate_B[ 3 * c + 2 ]);
		transformation = transformation.preMultiplication(Matrix(rotations[ r * 9 + 0 ], rotations[ r * 9 + 1 ], rotations[ r * 9 + 2 ], 0,
						 rotations[ r * 9 + 3 ], rotations[ r * 9 + 4 ], rotations[ r * 9 + 5 ], 0,
						 rotations[ r * 9 + 6 ], rotations[ r * 9 + 7 ], rotations[ r * 9 + 8 ], 0,
						 0,                      0,                      0, 1));
		transformation = transformation.preMultiplication(Matrix(fineRotations[ f * 9 + 0 ], fineRotations[ f * 9 + 1 ], fineRotations[ f * 9 + 2 ], 0,
						 fineRotations[ f * 9 + 3 ], fineRotations[ f * 9 + 4 ], fineRotations[ f * 9 + 5 ], 0,
						 fineRotations[ f * 9 + 6 ], fineRotations[ f * 9 + 7 ], fineRotations[ f * 9 + 8 ], 0,
						 0,                      0,                      0, 1));
		transformation = transformation.preMultiplication(Matrix::translation(-translate_B[ 3 * c + 0 ], -translate_B[ 3 * c + 1 ], -translate_B[ 3 * c + 2 ]));
		// now get the translation in real space
		// get the real pos
		double realx = (translate_A[ 0 ] - translate_B[ 3 * c + 0 ]) * scale_B[ c ] * gridSize;
		double realy = (translate_A[ 1 ] - translate_B[ 3 * c + 1 ]) * scale_B[ c ] * gridSize;
		double realz = (translate_A[ 2 ] - translate_B[ 3 * c + 2 ]) * scale_B[ c ] * gridSize;
		// normalize it if needed
		if (x > gridSize / 2)
		{
			x -= gridSize;
		}
		if (y > gridSize / 2)
		{
			y -= gridSize;
		}
		if (z > gridSize / 2)
		{
			z -= gridSize;
		}
		// now get real
		double rx = (x - realx) / (gridSize * scale_B[ c ]);
		double ry = (y - realy) / (gridSize * scale_B[ c ]);
		double rz = (z - realz) / (gridSize * scale_B[ c ]);
		// find final transformation by adding the translation
		transformation = transformation.preMultiplication(Matrix::translation(rx, ry, rz));
		if (reDocking)
		{
			rmsd = baseComplex->getRMSD(transformation);
		}
		else
		{
			rmsd = baseComplex->getRMSD(transformation, unboundLigandAtomList[ c ], unboundLigandInterfaceAtomIndex[ c ]);
		}
		fprintf(fp, "\n%f %f %f %f %d %d %d %f", v / functionScaleFactor, rx, ry, rz, r, f, c, rmsd);
		if (mrmsd > rmsd)
		{
			mrmsd = rmsd;
			mv = v;
			mx = rx;
			my = ry;
			mz = rz;
			mr = r;
			mf = f;
			mc = c;
			rank = n;
		}
		int rmsdInt = (int)(rmsd);
		if (rmsdInt < rmsdToReport)
		{
			counter[ rmsdInt ]++;
			if (highestPos[ rmsdInt ] > n)
			{
				highestPos[ rmsdInt ] = n;
			}
		}
	}
	int numberOfGoodPeaks = 0;
	int highestRank = maxRank;
	for (int i = 0; i < rmsdGood; i++)
	{
		numberOfGoodPeaks += counter[ i ];
		if (highestPos[ i ] < highestRank)
		{
			highestRank = highestPos[ i ];
		}
	}
	fprintf(fp, "\n\n");
	for (int i = 0; i < rmsdToReport; i++)
	{
		fprintf(fp, "%d --> %d %d\n", i, counter[ i ], (highestPos[ i ] == maxRank) ? -1 : highestPos[ i ]);
	}
	fprintf(fp, "\ngood peaks under %d A: count = %d, highest rank = %d, min RMSD = %f\n", rmsdGood, numberOfGoodPeaks, highestRank, mrmsd);
	fprintf(fp, "\nbest peak: rmsd = %f, rank = %d, score = %f, translation = < %f, %f, %f >, rotationIndex = %d, fineRotationIndex = %d, conformationIndex = %d\n",
			mrmsd, rank, mv / functionScaleFactor, mx, my, mz, mr, mf, mc);
}



void peakSort(PEAKS* peaks, int p, int r)
{
	PEAKS t, tq;
	while (r - p >= 30)
	{
		int q = p + (lrand48() % ((int)(r - p + 1)));
		tq = peaks[ q ];
		peaks[ q ] = peaks[ r ];
		q = p - 1;
		for (int j = p; j <= r - 1; j++)
			if ((peaks[ j ].conformationID < tq.conformationID)
					|| ((peaks[ j ].conformationID == tq.conformationID) && (peaks[ j ].rotationID < tq.rotationID))
					|| ((peaks[ j ].conformationID == tq.conformationID) && (peaks[ j ].rotationID == tq.rotationID) && (peaks[ j ].score > tq.score)))
			{
				t = peaks[ ++q ];
				peaks[ q ] = peaks[ j ];
				peaks[ j ] = t;
			}
		peaks[ r ] = peaks[ ++q ];
		peaks[ q ] = tq;
		if ((q - p < r - q) && (q - p > 1))
		{
			peakSort(peaks, p, q - 1);
			p = q + 1;
		}
		else
		{
			peakSort(peaks, q + 1, r);
			r = q - 1;
		}
	}
	int j;
	for (int i = p + 1; i <= r; i++)
	{
		tq = peaks[ i ];
		j = i - 1;
		while ((j >= p) && ((peaks[ j ].conformationID > tq.conformationID)
							|| ((peaks[ j ].conformationID == tq.conformationID) && (peaks[ j ].rotationID > tq.rotationID))
							|| ((peaks[ j ].conformationID == tq.conformationID) && (peaks[ j ].rotationID == tq.rotationID) && (peaks[ j ].score < tq.score))))
		{
			peaks[ j + 1 ] = peaks[ j ];
			j--;
		}
		if (j != i - 1)
		{
			peaks[ j + 1 ] = tq;
		}
	}
}



void peakSortByScore(PEAKS* peaks, int p, int r)
{
	PEAKS t, tq;
	while (r - p >= 30)
	{
		int q = p + (lrand48() % ((int)(r - p + 1)));
		tq = peaks[ q ];
		peaks[ q ] = peaks[ r ];
		q = p - 1;
		for (int j = p; j <= r - 1; j++)
			if ((peaks[ j ].score > tq.score)
					|| ((peaks[ j ].score == tq.score) && (peaks[ j ].conformationID < tq.conformationID))
					|| ((peaks[ j ].score == tq.score) && (peaks[ j ].conformationID == tq.conformationID) && (peaks[ j ].rotationID < tq.rotationID)))
			{
				t = peaks[ ++q ];
				peaks[ q ] = peaks[ j ];
				peaks[ j ] = t;
			}
		peaks[ r ] = peaks[ ++q ];
		peaks[ q ] = tq;
		if ((q - p < r - q) && (q - p > 1))
		{
			peakSortByScore(peaks, p, q - 1);
			p = q + 1;
		}
		else
		{
			peakSortByScore(peaks, q + 1, r);
			r = q - 1;
		}
	}
	int j;
	for (int i = p + 1; i <= r; i++)
	{
		tq = peaks[ i ];
		j = i - 1;
		while ((j >= p) && ((peaks[ j ].score < tq.score)
							|| ((peaks[ j ].score == tq.score) && (peaks[ j ].conformationID > tq.conformationID))
							|| ((peaks[ j ].score == tq.score) && (peaks[ j ].conformationID == tq.conformationID) && (peaks[ j ].rotationID > tq.rotationID))))
		{
			peaks[ j + 1 ] = peaks[ j ];
			j--;
		}
		if (j != i - 1)
		{
			peaks[ j + 1 ] = tq;
		}
	}
}



bool applyRotations(int confID, int startRotation, int endRotation, float* rotations,
					int numberOfRotations, int numberOfPositions,
					double* xkBOrig, double* ykBOrig, double* zkBOrig,
					double* xkB, double* ykB, double* zkB, int numCentersB,
					int gridSize, int numFreq, int interpFuncExtent, double largestEdge,
					double alpha, double blobbiness, double elecScale, double scaleA,
					fftw_complex* centerFrequenciesA, fftw_complex* centerElecFrequenciesA,
					fftw_complex* centerFrequenciesB, fftw_complex* centerElecFrequenciesB,
					fftw_complex* centerFrequenciesProduct, fftw_complex* centerFrequenciesElecProduct,
					fftw_complex* sparseProfile, fftw_complex* sparseElecProfile,
					fftw_plan freqPlan, fftw_plan elecFreqPlan,
					fftw_complex* fkB, fftw_complex* fkBElec,
					fftw_complex* freqHat, fftw_plan freqHatPlan,
					fftw_complex* Ian1,
					fftw_complex* ourMoreFrequencies, fftw_plan moreFreqPlan,
					fftw_complex* smallElectrostaticsKernel,
					SmoothingFunction* smoothingFunction, TopValues* localTopValues)
{
	double scaleB = scaleA;
	for (int i = startRotation; i <= endRotation; i++)
	{
		printf("\nCONFORMATION = %d, ROTATION = %d\n\n", confID, i);
		// transform B
		if (!transformAndNormalize(xkBOrig, ykBOrig, zkBOrig, xkB, ykB, zkB, numCentersB,
								   rotations[ i * 9 + 0 ],	rotations[ i * 9 + 1 ], rotations[ i * 9 + 2 ],
								   rotations[ i * 9 + 3 ], rotations[ i * 9 + 4 ], rotations[ i * 9 + 5 ],
								   rotations[ i * 9 + 6 ], rotations[ i * 9 + 7 ], rotations[ i * 9 + 8 ],
								   gridSize, numFreq, interpFuncExtent, largestEdge, 2, &scaleB))
		{
			return false;
		}
		// now, scaleB should be same as scaleA
		// get frequencies of B
		if (!getCenterFrequencies(numCentersB, alpha, interpFuncExtent, numFreq, gridSize, xkB, ykB, zkB, fkB, centerFrequenciesB, 0, smoothingFunction, Ian1, ourMoreFrequencies, moreFreqPlan))
		{
			return false;
		}
		if (!getCenterFrequencies(numCentersB, alpha, interpFuncExtent, numFreq, gridSize, xkB, ykB, zkB, fkBElec, centerElecFrequenciesB, 0, smoothingFunction, Ian1, ourMoreFrequencies, moreFreqPlan))
		{
			return false;
		}
		// get product with gaussian squared and two frequency maps
		if (!multiplyFrequencyMaps(centerFrequenciesA, centerFrequenciesB, numFreq, gridSize*4, blobbiness, centerFrequenciesProduct, freqHat, freqHatPlan, scaleA))
		{
			return false;
		}
		// using same sized grid, to be consistant with potts fast sum paper
		if (!multiplyElecFrequencyMaps(centerElecFrequenciesA, centerElecFrequenciesB, numFreq, numFreq, smallElectrostaticsKernel, centerFrequenciesElecProduct, scaleA))
		{
			return false;
		}
		// inverse the map to get the profile
		fftw_execute(freqPlan);
		fftw_execute(elecFreqPlan);
		// if scale is negative, then we only use the elec profile, else we update shape profile with elec profile
		// in both cases, take (-)ve of elec profile.
		updateSCwithElec(sparseProfile, sparseElecProfile, numFreq, elecScale);
		localTopValues->updateTopPositions(sparseProfile, i, 0, confID);
	}
	return true;
}

static void* startApplyRotationsThread(void* v)
{
	PARAMS* pr = (PARAMS*) v;
	applyRotations(pr->confID, pr->startRotation, pr->endRotation, pr->rotations,
				   pr->numberOfRotations, pr->numberOfPositions,
				   pr->xkBOrig, pr->ykBOrig, pr->zkBOrig,
				   pr->xkB, pr->ykB, pr->zkB, pr->numCentersB,
				   pr->gridSize, pr->numFreq, pr->interpFuncExtent, pr->largestEdge,
				   pr->alpha, pr->blobbiness, pr->elecScale, pr->scaleA,
				   pr->centerFrequenciesA, pr->centerElecFrequenciesA,
				   pr->centerFrequenciesB, pr->centerElecFrequenciesB,
				   pr->centerFrequenciesProduct, pr->centerFrequenciesElecProduct,
				   pr->sparseProfile, pr->sparseElecProfile,
				   pr->freqPlan, pr->elecFreqPlan,
				   pr->fkB, pr->fkBElec,
				   pr->freqHat, pr->freqHatPlan,
				   pr->Ian1,
				   pr->ourMoreFrequencies, pr->moreFreqPlan,
				   pr->smallElectrostaticsKernel,
				   pr->smoothingFunction, pr->localTopValues);
}

bool applyFineRotations(int confID, int rotationID, float* rotations,
						int startFineRotation, int endFineRotation, float* fineRotations,
						int numberOfFineRotations, int numberOfPositions,
						double* xkBOrig, double* ykBOrig, double* zkBOrig,
						double* xkB, double* ykB, double* zkB, int numCentersB,
						int gridSize, int numFreq, int interpFuncExtent, double largestEdge,
						double alpha, double blobbiness, double elecScale, double scaleA,
						fftw_complex* centerFrequenciesA, fftw_complex* centerElecFrequenciesA,
						fftw_complex* centerFrequenciesB, fftw_complex* centerElecFrequenciesB,
						fftw_complex* centerFrequenciesProduct, fftw_complex* centerFrequenciesElecProduct,
						fftw_complex* sparseProfile, fftw_complex* sparseElecProfile,
						fftw_plan freqPlan, fftw_plan elecFreqPlan,
						fftw_complex* fkB, fftw_complex* fkBElec,
						fftw_complex* freqHat, fftw_plan freqHatPlan,
						fftw_complex* Ian1,
						fftw_complex* ourMoreFrequencies, fftw_plan moreFreqPlan,
						fftw_complex* smallElectrostaticsKernel,
						SmoothingFunction* smoothingFunction, TopValues* localTopValues)
{
	double scaleB = scaleA;
	for (int i = startFineRotation; i <= endFineRotation; i++)
	{
		printf("\nCONFORMATION = %d, ROTATION = %d, FINE-ROTATION = %d\n\n", confID, rotationID, i);
		Matrix rotMat(rotations[ rotationID * 9 + 0 ], rotations[ rotationID * 9 + 1 ], rotations[ rotationID * 9 + 2 ], 0,
					  rotations[ rotationID * 9 + 3 ], rotations[ rotationID * 9 + 4 ], rotations[ rotationID * 9 + 5 ], 0,
					  rotations[ rotationID * 9 + 6 ], rotations[ rotationID * 9 + 7 ], rotations[ rotationID * 9 + 8 ], 0,
					  0,                               0,                               0, 1);
		rotMat = rotMat.preMultiplication(Matrix(fineRotations[ i * 9 + 0 ], fineRotations[ i * 9 + 1 ], fineRotations[ i * 9 + 2 ], 0,
										  fineRotations[ i * 9 + 3 ], fineRotations[ i * 9 + 4 ], fineRotations[ i * 9 + 5 ], 0,
										  fineRotations[ i * 9 + 6 ], fineRotations[ i * 9 + 7 ], fineRotations[ i * 9 + 8 ], 0,
										  0,                      0,                      0,         1));
		const float* m = rotMat.getMatrix();
		// transform B
		if (!transformAndNormalize(xkBOrig, ykBOrig, zkBOrig, xkB, ykB, zkB, numCentersB,
								   m[ 0 ],	m[ 4 ], m[ 8 ],
								   m[ 1 ], m[ 5 ], m[ 9 ],
								   m[ 2 ], m[ 6 ], m[ 10 ],
								   gridSize, numFreq, interpFuncExtent, largestEdge, 2, &scaleB))
		{
			return false;
		}
		// now, scaleB should be same as scaleA
		if (!getCenterFrequencies(numCentersB, alpha, interpFuncExtent, numFreq, gridSize, xkB, ykB, zkB, fkB, centerFrequenciesB, 0, smoothingFunction, Ian1, ourMoreFrequencies, moreFreqPlan))
		{
			return false;
		}
		if (!getCenterFrequencies(numCentersB, alpha, interpFuncExtent, numFreq, gridSize, xkB, ykB, zkB, fkBElec, centerElecFrequenciesB, 0, smoothingFunction, Ian1, ourMoreFrequencies, moreFreqPlan))
		{
			return false;
		}
		// get product with gaussian squared and two frequency maps
		if (!multiplyFrequencyMaps(centerFrequenciesA, centerFrequenciesB, numFreq, gridSize*4, blobbiness, centerFrequenciesProduct, freqHat, freqHatPlan, scaleA))
		{
			return false;
		}
		// using same sized grid, to be consistant with potts fast sum paper
		if (!multiplyElecFrequencyMaps(centerElecFrequenciesA, centerElecFrequenciesB, numFreq, numFreq, smallElectrostaticsKernel, centerFrequenciesElecProduct, scaleA))
		{
			return false;
		}
		fftw_execute(freqPlan);
		fftw_execute(elecFreqPlan);
		// if scale is negative, then we only use the elec profile, else we update shape profile with elec profile
		// in both cases, take (-)ve of elec profile.
		updateSCwithElec(sparseProfile, sparseElecProfile, numFreq, elecScale);
		localTopValues->updateTopPositions(sparseProfile, rotationID, i, confID);
	}
	return true;
}

static void* startApplyFineRotationsThread(void* v)
{
	PARAMS* pr = (PARAMS*) v;
	applyFineRotations(pr->confID, pr->startRotation, pr->rotations,
					   pr->startFineRotation, pr->endFineRotation, pr->fineRotations,
					   pr->numberOfFineRotations, pr->numberOfPositions,
					   pr->xkBOrig, pr->ykBOrig, pr->zkBOrig,
					   pr->xkB, pr->ykB, pr->zkB, pr->numCentersB,
					   pr->gridSize, pr->numFreq, pr->interpFuncExtent, pr->largestEdge,
					   pr->alpha, pr->blobbiness, pr->elecScale, pr->scaleA,
					   pr->centerFrequenciesA, pr->centerElecFrequenciesA,
					   pr->centerFrequenciesB, pr->centerElecFrequenciesB,
					   pr->centerFrequenciesProduct, pr->centerFrequenciesElecProduct,
					   pr->sparseProfile, pr->sparseElecProfile,
					   pr->freqPlan, pr->elecFreqPlan,
					   pr->fkB, pr->fkBElec,
					   pr->freqHat, pr->freqHatPlan,
					   pr->Ian1,
					   pr->ourMoreFrequencies, pr->moreFreqPlan,
					   pr->smallElectrostaticsKernel,
					   pr->smoothingFunction, pr->localTopValues);
}

int transformAndSaveGOA(int argc, char* argv[ ])
{
	char inputFile[ 256 ];
	char transFile[ 256 ];
	char rotationFile[ 256 ];
	char fineRotationFile[ 256 ];
	char outputFile[ 256 ];
	strcpy(inputFile, argv[ 2 ]);
	strcpy(transFile, argv[ 3 ]);
	strcpy(rotationFile, argv[ 4 ]);
	strcpy(fineRotationFile, argv[ 5 ]);
	strcpy(outputFile, argv[ 6 ]);
	GOALoader* gLoader = new GOALoader();
	bool deleteWater = false;
	PDBParser::GroupOfAtoms* mol;
	mol = gLoader->loadFile(inputFile, deleteWater);
	float cx, cy, cz;
	float v1, v2, tx, ty, tz;
	int r, f, c;
	FILE* fpTrans = fopen(transFile, "r");
	fscanf(fpTrans, "%f %f %f", &cx, &cy, &cz);
	fscanf(fpTrans, "%f %f %f %f %d %d %d %f", &v1, &tx, &ty, &tz, &r, &f, &c, &v2);
	printf("%f %f %f\n", cx, cy, cz);
	printf("%f %f %f\n", tx, ty, tz);
	fclose(fpTrans);
	float* rotations = new float[ 9 ];
	FILE* fpRot = fopen(rotationFile, "r");
	rotations[ 0 ] = 1;
	rotations[ 1 ] = 0;
	rotations[ 2 ] = 0;
	rotations[ 3 ] = 0;
	rotations[ 4 ] = 1;
	rotations[ 5 ] = 0;
	rotations[ 6 ] = 0;
	rotations[ 7 ] = 0;
	rotations[ 8 ] = 1;
	for (int i = 1; i <= r; i++)
		if (fscanf(fpRot, "%f %f %f %f %f %f %f %f %f\n",
				   &(rotations[ 0 ]),
				   &(rotations[ 1 ]),
				   &(rotations[ 2 ]),
				   &(rotations[ 3 ]),
				   &(rotations[ 4 ]),
				   &(rotations[ 5 ]),
				   &(rotations[ 6 ]),
				   &(rotations[ 7 ]),
				   &(rotations[ 8 ])) != 9)
		{
			return -1;
		}
	fclose(fpRot);
	printf("%f %f %f %f %f %f %f %f %f\n",
		   (rotations[ 0 ]),
		   (rotations[ 1 ]),
		   (rotations[ 2 ]),
		   (rotations[ 3 ]),
		   (rotations[ 4 ]),
		   (rotations[ 5 ]),
		   (rotations[ 6 ]),
		   (rotations[ 7 ]),
		   (rotations[ 8 ]));
	float* fineRotations = new float[ 9 ];
	fpRot = fopen(fineRotationFile, "r");
	fineRotations[ 0 ] = 1;
	fineRotations[ 1 ] = 0;
	fineRotations[ 2 ] = 0;
	fineRotations[ 3 ] = 0;
	fineRotations[ 4 ] = 1;
	fineRotations[ 5 ] = 0;
	fineRotations[ 6 ] = 0;
	fineRotations[ 7 ] = 0;
	fineRotations[ 8 ] = 1;
	for (int i = 1; i <= f; i++)
		if (fscanf(fpRot, "%f %f %f %f %f %f %f %f %f\n",
				   &(fineRotations[ 0 ]),
				   &(fineRotations[ 1 ]),
				   &(fineRotations[ 2 ]),
				   &(fineRotations[ 3 ]),
				   &(fineRotations[ 4 ]),
				   &(fineRotations[ 5 ]),
				   &(fineRotations[ 6 ]),
				   &(fineRotations[ 7 ]),
				   &(fineRotations[ 8 ])) != 9)
		{
			return -1;
		}
	printf("%f %f %f %f %f %f %f %f %f\n",
		   (fineRotations[ 0 ]),
		   (fineRotations[ 1 ]),
		   (fineRotations[ 2 ]),
		   (fineRotations[ 3 ]),
		   (fineRotations[ 4 ]),
		   (fineRotations[ 5 ]),
		   (fineRotations[ 6 ]),
		   (fineRotations[ 7 ]),
		   (fineRotations[ 8 ]));
	fclose(fpRot);
	Matrix transformation = Matrix::translation(-cx, -cy, -cz);
	transformation = transformation.preMultiplication(Matrix(rotations[ 0 ], rotations[ 1 ], rotations[ 2 ], 0,
					 rotations[ 3 ], rotations[ 4 ], rotations[ 5 ], 0,
					 rotations[ 6 ], rotations[ 7 ], rotations[ 8 ], 0,
					 0,              0,              0, 1));
	transformation = transformation.preMultiplication(Matrix(fineRotations[ 0 ], fineRotations[ 1 ], fineRotations[ 2 ], 0,
					 fineRotations[ 3 ], fineRotations[ 4 ], fineRotations[ 5 ], 0,
					 fineRotations[ 6 ], fineRotations[ 7 ], fineRotations[ 8 ], 0,
					 0,                  0,                  0, 1));
	transformation = transformation.preMultiplication(Matrix::translation(cx, cy, cz));
	transformation = transformation.preMultiplication(Matrix::translation(tx, ty, tz));
	gLoader->saveFile(outputFile, mol, 0, &transformation);
	delete gLoader;
	delete mol;
	delete [] rotations;
	delete [] fineRotations;
	return 0;
}

//      example input : ./main 2 4 16 256 sod_O.pqrAll.txt sod_Y.pqrAll.txt rotFile numRot out.txt 10 true -2.3
//      The inputs to the program are:
//         alpha - the grid sampling factor. We create a grid of size alpha*numFreq
//         interpFuncExtent - the gaussian extent
//         numFreq - Number of frequencies to extract
//         gridSize - How large a grid to embed molecule in for full resolution search
//         inputFile1, inputFile2 - The two input files of the molecules
//         rotation file, each line has 9 entries for 1 rotation matrix
//         number of rotations
//         out.txt - output file name
//         numberOfPositions - Number of top positions to retain in each search
//         performDocking - true or false, perform docking or matching
//         blobbiness - rate of decay of gaussian, negative
//         elecScale - weight for electrostatics complementarity
int dockingMain(int argc, char* argv[])
{
	if (argc == 7)
	{
		return transformAndSaveGOA(argc, argv);
	}
	double alpha;
	int interpFuncExtent;
	int numFreq;
	int gridSize;
	char outputFileName[256];
	char baseInputFile1[256];
	char inputFile1[256];
	char baseInputFile2[256];
	char inputFile2[256];
	char boundConformationFile2[256];
	int numFile2 = 1;
	char rotationFile[256];
	int numberOfRotations = 0;
	int numberOfPeaksForFineRotation = 0;
	char fineRotationFile[256];
	int numberOfFineRotations = 0;
	int statInterval = 1000000;
	int numThreads = 2;
	// results
	TopValues* localTopValues[ numThreads ];
	TopValues* globalTopValues = 0;
	for (int i = 0; i < numThreads; i++)
	{
		localTopValues[ i ] = 0;
	}
	int numberOfPositions;
	double real_magnitude = 1.0;
	double imag_magnitude = -3.0;
	double elecScale = 1.0;
	bool reDocking = true;
	bool performDocking = true;
	double blobbiness = -2.3;
	double distanceCutoff = 2;
	double mainStartTime = getTime();
	bool flexibleDocking = false;
	if (argc == 23)
	{
		if (!parseArguments(argc, argv,
							&numThreads,
							&alpha,
							&interpFuncExtent,
							&numFreq,
							&gridSize,
							baseInputFile1,
							baseInputFile2,
							boundConformationFile2,
							rotationFile,
							&numberOfRotations,
							&numberOfPeaksForFineRotation,
							fineRotationFile,
							&numberOfFineRotations,
							outputFileName,
							&numberOfPositions,
							&reDocking,
							&blobbiness,
							&elecScale,
							&statInterval,
							&distanceCutoff,
							&numFile2))
		{
			return -1;
		}
		flexibleDocking = true;
	}
	else
	{
		if (!parseArguments(argc, argv,
							&numThreads,
							&alpha,
							&interpFuncExtent,
							&numFreq,
							&gridSize,
							baseInputFile1,
							baseInputFile2,
							boundConformationFile2,
							rotationFile,
							&numberOfRotations,
							&numberOfPeaksForFineRotation,
							fineRotationFile,
							&numberOfFineRotations,
							outputFileName,
							&numberOfPositions,
							&reDocking,
							&blobbiness,
							&elecScale,
							&statInterval,
							&distanceCutoff))
		{
			return -1;
		}
	}
	if (!inititalizeFFTW(numFreq*alpha))
	{
		return -1;
	}
	fftw_complex* centerFrequenciesA = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*numFreq*numFreq*numFreq);
	fftw_complex* centerElecFrequenciesA = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*numFreq*numFreq*numFreq);
	fftw_complex* smallElectrostaticsKernel = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*numFreq*numFreq*numFreq);
	fftw_complex* centerFrequenciesB[ numThreads ];
	fftw_complex* centerFrequenciesProduct[ numThreads ];
	fftw_complex* sparseProfile[ numThreads ];
	fftw_complex* centerElecFrequenciesB[ numThreads ];
	fftw_complex* centerFrequenciesElecProduct[ numThreads ];
	fftw_complex* sparseElecProfile[ numThreads ];
	fftw_complex* freqHat[ numThreads ];
	fftw_complex* Ian1[ numThreads ];
	fftw_complex* ourMoreFrequencies[ numThreads ];
	fftw_plan freqPlan[ numThreads ];
	fftw_plan elecFreqPlan[ numThreads ];
	fftw_plan freqHatPlan[ numThreads ];
	fftw_plan moreFreqPlan[ numThreads ];
	double* xkA = 0, *ykA = 0, *zkA = 0;
	double* xkAOrig = 0, *ykAOrig = 0, *zkAOrig = 0;
	double* xkB[ numThreads ], *ykB[ numThreads ], *zkB[ numThreads ];
	double* xkBOrig = 0, *ykBOrig = 0, *zkBOrig = 0;
	fftw_complex* fkA = 0, *fkB = 0;
	fftw_complex* fkAElec = 0, *fkBElec = 0;
	int numCentersA = 0, numCentersB = 0;
	globalTopValues  = new TopValues(numberOfPositions, numFreq);
	SmoothingFunction* smoothingFunction[ numThreads ];
	for (int i = 0; i < numThreads; i++)
	{
		centerFrequenciesB[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numFreq * numFreq * numFreq);
		centerFrequenciesProduct[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numFreq * numFreq * numFreq);
		sparseProfile[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numFreq * numFreq * numFreq);
		centerElecFrequenciesB[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numFreq * numFreq * numFreq);
		centerFrequenciesElecProduct[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numFreq * numFreq * numFreq);
		sparseElecProfile[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numFreq * numFreq * numFreq);
		freqHat[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * gridSize * 4);
		double n = (int) alpha * numFreq;
		int I = (int)(n) + 2 * interpFuncExtent;
		Ian1[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * I * I * I);
		int alphaM = (int) n;
		ourMoreFrequencies[ i ] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * alphaM * alphaM * alphaM);
		freqPlan[ i ] = fftw_plan_dft_3d(numFreq, numFreq, numFreq, centerFrequenciesProduct[ i ], sparseProfile[ i ], FFTW_BACKWARD, FFTW_ESTIMATE);
		elecFreqPlan[ i ] = fftw_plan_dft_3d(numFreq, numFreq, numFreq, centerFrequenciesElecProduct[ i ], sparseElecProfile[ i ], FFTW_BACKWARD, FFTW_ESTIMATE);
		freqHatPlan[ i ] = fftw_plan_dft_1d(gridSize * 4, freqHat[ i ], freqHat[ i ], FFTW_FORWARD, FFTW_ESTIMATE);
		moreFreqPlan[ i ] = fftw_plan_dft_3d(alphaM, alphaM, alphaM, ourMoreFrequencies[ i ], ourMoreFrequencies[ i ], FFTW_FORWARD, FFTW_ESTIMATE);
		localTopValues[ i ] = new TopValues(numberOfPositions, numFreq);
		smoothingFunction[ i ] = new CardinalBSpline(alpha, interpFuncExtent, (int)(alpha*numFreq), gridSize);
	}
	float* rotations = new float[(numberOfRotations + 1) * 9 ];
	FILE* fpRot = fopen(rotationFile, "r");
	rotations[ 0 ] = 1;
	rotations[ 1 ] = 0;
	rotations[ 2 ] = 0;
	rotations[ 3 ] = 0;
	rotations[ 4 ] = 1;
	rotations[ 5 ] = 0;
	rotations[ 6 ] = 0;
	rotations[ 7 ] = 0;
	rotations[ 8 ] = 1;
	for (int i = 1; i <= numberOfRotations; i++)
		if (fscanf(fpRot, "%f %f %f %f %f %f %f %f %f\n",
				   &(rotations[ i * 9 + 0 ]),
				   &(rotations[ i * 9 + 1 ]),
				   &(rotations[ i * 9 + 2 ]),
				   &(rotations[ i * 9 + 3 ]),
				   &(rotations[ i * 9 + 4 ]),
				   &(rotations[ i * 9 + 5 ]),
				   &(rotations[ i * 9 + 6 ]),
				   &(rotations[ i * 9 + 7 ]),
				   &(rotations[ i * 9 + 8 ])) != 9)
		{
			return -1;
		}
	fclose(fpRot);
	float* fineRotations = new float[(numberOfFineRotations + 1) * 9 ];
	fpRot = fopen(fineRotationFile, "r");
	fineRotations[ 0 ] = 1;
	fineRotations[ 1 ] = 0;
	fineRotations[ 2 ] = 0;
	fineRotations[ 3 ] = 0;
	fineRotations[ 4 ] = 1;
	fineRotations[ 5 ] = 0;
	fineRotations[ 6 ] = 0;
	fineRotations[ 7 ] = 0;
	fineRotations[ 8 ] = 1;
	for (int i = 1; i <= numberOfFineRotations; i++)
		if (fscanf(fpRot, "%f %f %f %f %f %f %f %f %f\n",
				   &(fineRotations[ i * 9 + 0 ]),
				   &(fineRotations[ i * 9 + 1 ]),
				   &(fineRotations[ i * 9 + 2 ]),
				   &(fineRotations[ i * 9 + 3 ]),
				   &(fineRotations[ i * 9 + 4 ]),
				   &(fineRotations[ i * 9 + 5 ]),
				   &(fineRotations[ i * 9 + 6 ]),
				   &(fineRotations[ i * 9 + 7 ]),
				   &(fineRotations[ i * 9 + 8 ])) != 9)
		{
			return -1;
		}
	fclose(fpRot);
	Docking::DockComplex* baseComplex = new Docking::DockComplex;
	bool deleteWater = false;
	std::vector< PDBParser::Atom* > unboundLigandAtomList[ numFile2 ];
	std::vector< int > unboundLigandInterfaceAtomIndex[ numFile2 ];
	sprintf(inputFile1, "%s.pqr", baseInputFile1);
	sprintf(inputFile2, "%s.pqr", boundConformationFile2);
	if (!baseComplex->init(inputFile1, inputFile2, deleteWater))
	{
		return false;
	}
	if (!baseComplex->computeLigandInterfaceAtoms(distanceCutoff))
	{
		return false;
	}
	int numDigits = 0;
	if (flexibleDocking)
	{
		for (int k = numFile2 - 1; k > 0; k /= 10)
		{
			numDigits++;
		}
		if (numDigits == 0)
		{
			numDigits = 1;
		}
		if (!reDocking)
		{
			for (int i = 0; i < numFile2; i++)
			{
				sprintf(inputFile2, "%s_%0*d.pqr", baseInputFile2, numDigits, i);
				if (!baseComplex->getUnboundLigandAtoms(inputFile2, deleteWater, &(unboundLigandAtomList[ i ])))
				{
					return false;
				}
				int numFound = baseComplex->findCorrespondingInterfaceAtoms(&(unboundLigandInterfaceAtomIndex[ i ]), unboundLigandAtomList[ i ]);
			}
		}
	}
	else
	{
		if (!reDocking)
		{
			sprintf(inputFile2, "%s.pqr", baseInputFile2);
			if (!baseComplex->getUnboundLigandAtoms(inputFile2, deleteWater, &(unboundLigandAtomList[ 0 ])))
			{
				return false;
			}
			int numFound = baseComplex->findCorrespondingInterfaceAtoms(&(unboundLigandInterfaceAtomIndex[ 0 ]), unboundLigandAtomList[ 0 ]);
		}
	}
	sprintf(inputFile1, "%s.xyz", baseInputFile1);
	// read A
	if (!readCenters(&xkAOrig, &ykAOrig, &zkAOrig, &xkA, &ykA, &zkA, &numCentersA, &fkA, &fkAElec, real_magnitude, imag_magnitude, inputFile1, performDocking, true))
	{
		return -1;
	}
	double xTransA = 0, yTransA = 0, zTransA = 0, scaleA = 1;
	// center A
	if (!center(xkAOrig, ykAOrig, zkAOrig, numCentersA, &xTransA, &yTransA, &zTransA))
	{
		return -1;
	}
	float translate_A[ 3 ];
	translate_A[ 0 ] = xTransA;
	translate_A[ 1 ] = yTransA;
	translate_A[ 2 ] = zTransA;
	float* translate_B = new float[ numFile2 * 3 ];
	float* scale_B = new float[ numFile2 ];
	// report peak
	FILE* fpOpt = fopen(outputFileName,"w");
	int rotCount = 0, snapCount = 1;
	sprintf(inputFile2, "%s.xyz", baseInputFile2);
	for (int fn = 0; fn < numFile2; fn++)
	{
		if (flexibleDocking)
		{
			sprintf(inputFile2, "%s_%0*d.xyz", baseInputFile2, numDigits, fn);
		}
		// read B, flip B since convolution reflips? it
		if (!readCenters(&xkBOrig, &ykBOrig, &zkBOrig, &numCentersB, &fkB, &fkBElec, real_magnitude, imag_magnitude, inputFile2, performDocking, false))
		{
			return -1;
		}
		for (int i = 0; i < numThreads; i++)
		{
			xkB[ i ] = new double[ numCentersB ];
			ykB[ i ] = new double[ numCentersB ];
			zkB[ i ] = new double[ numCentersB ];
		}
		// get largest edge
		double largestEdge = 0;
		if (!getLargestEdge(xkAOrig, ykAOrig, zkAOrig, numCentersA, xkBOrig, ykBOrig, zkBOrig, numCentersB, &largestEdge))
		{
			return -1;
		}
		// transform A
		if (!transformAndNormalize(xkAOrig, ykAOrig, zkAOrig, xkA, ykA, zkA, numCentersA, 1,0,0,0,1,0,0,0,1, gridSize, numFreq, interpFuncExtent, largestEdge, 1, &scaleA))
		{
			return -1;
		}
		// get electrostatics kernel using the right scale and grid sizes
		// get elec kernel FFT defined by distance weighted dielectric.
		// this function does not return Fourier coefficients, but FFT of the function sampled on the grid. This is in
		// accordance with the fastsum paper by nfft folks.
		if (!computeElecKernel(smallElectrostaticsKernel, numFreq, scaleA))
		{
			return -1;
		}
		// get frequencies of A.
		if (!getCenterFrequencies(numCentersA, alpha, interpFuncExtent, numFreq, gridSize, xkA, ykA, zkA, fkA, centerFrequenciesA, 0, smoothingFunction[ 0 ]))
		{
			return -1;
		}
		if (!getCenterFrequencies(numCentersA, alpha, interpFuncExtent, numFreq, gridSize, xkA, ykA, zkA, fkAElec, centerElecFrequenciesA, 0, smoothingFunction[ 0 ]))
		{
			return -1;
		}
		double xTransB = 0, yTransB = 0, zTransB = 0;
		// center B
		if (!center(xkBOrig, ykBOrig, zkBOrig, numCentersB, &xTransB, &yTransB, &zTransB))
		{
			return -1;
		}
		translate_B[ fn * 3 + 0 ] = xTransB;
		translate_B[ fn * 3 + 1 ] = yTransB;
		translate_B[ fn * 3 + 2 ] = zTransB;
		scale_B[ fn ] = scaleA;
		PARAMS pr[ numThreads ];
		pthread_t p[ numThreads ];
		int startRotation, endRotation = -1;
		for (int i = 0; i < numThreads; i++)
		{
			startRotation = endRotation + 1;
			if (i == numThreads - 1)
			{
				endRotation = numberOfRotations;
			}
			else
			{
				endRotation = startRotation + (numberOfRotations / numThreads);
			}
			pr[ i ].confID = fn;
			pr[ i ].startRotation = startRotation;
			pr[ i ].endRotation = endRotation;
			pr[ i ].rotations = rotations;
			pr[ i ].numberOfRotations = numberOfRotations;
			pr[ i ].numberOfPositions = numberOfPositions;
			pr[ i ].xkBOrig = xkBOrig;
			pr[ i ].ykBOrig = ykBOrig;
			pr[ i ].zkBOrig = zkBOrig;
			pr[ i ].xkB = xkB[ i ];
			pr[ i ].ykB = ykB[ i ];
			pr[ i ].zkB = zkB[ i ];
			pr[ i ].numCentersB = numCentersB;
			pr[ i ].gridSize = gridSize;
			pr[ i ].numFreq = numFreq;
			pr[ i ].interpFuncExtent = interpFuncExtent;
			pr[ i ].largestEdge = largestEdge;
			pr[ i ].alpha = alpha;
			pr[ i ].blobbiness = blobbiness;
			pr[ i ].elecScale = elecScale;
			pr[ i ].scaleA = scaleA;
			pr[ i ].centerFrequenciesA = centerFrequenciesA;
			pr[ i ].centerElecFrequenciesA = centerElecFrequenciesA;
			pr[ i ].centerFrequenciesB = centerFrequenciesB[ i ];
			pr[ i ].centerElecFrequenciesB = centerElecFrequenciesB[ i ];
			pr[ i ].centerFrequenciesProduct = centerFrequenciesProduct[ i ];
			pr[ i ].centerFrequenciesElecProduct = centerFrequenciesElecProduct[ i ];
			pr[ i ].sparseProfile = sparseProfile[ i ];
			pr[ i ].sparseElecProfile = sparseElecProfile[ i ];
			pr[ i ].freqHat = freqHat[ i ];
			pr[ i ].freqPlan = freqPlan[ i ];
			pr[ i ].elecFreqPlan = elecFreqPlan[ i ];
			pr[ i ].freqHatPlan = freqHatPlan[ i ];
			pr[ i ].Ian1 = Ian1[ i ];
			pr[ i ].ourMoreFrequencies = ourMoreFrequencies[ i ];
			pr[ i ].moreFreqPlan = moreFreqPlan[ i ];
			pr[ i ].fkB = fkB;
			pr[ i ].fkBElec = fkBElec;
			pr[ i ].smallElectrostaticsKernel = smallElectrostaticsKernel;
			pr[ i ].smoothingFunction = smoothingFunction[ i ];
			pr[ i ].localTopValues = localTopValues[ i ];
			pthread_create(&p[ i ], NULL, startApplyRotationsThread, (void*) &pr[ i ]);
		}
		for (int i = 0; i < numThreads; i++)
		{
			pthread_join(p[ i ], NULL);
		}
		for (int i = 0; i < numThreads; i++)
		{
			int n = localTopValues[ i ]->getCurrentNumberOfPositions();
			double v, x, y, z;
			int r, f, c;
			while (n--)
			{
				localTopValues[ i ]->extractMin(&v, &x, &y, &z, &r, &f, &c);
				globalTopValues->updateTopValues(v, x, y, z, r, f, c);
			}
		}
		delete []xkBOrig;
		delete []ykBOrig;
		delete []zkBOrig;
		fftw_free(fkB);
		fftw_free(fkBElec);
		for (int i = 0; i < numThreads; i++)
		{
			delete []xkB[ i ];
			delete []ykB[ i ];
			delete []zkB[ i ];
		}
	}
	if ((numberOfFineRotations > 0) && (numberOfPeaksForFineRotation > 0))
	{
		int m, n = globalTopValues->getCurrentNumberOfPositions();
		double v, x, y, z;
		int r, f, c, k;
		m = n;
		PEAKS* curPeaks = new PEAKS[ m ];
		while (n--)
		{
			globalTopValues->extractMin(&v, &x, &y, &z, &r, &f, &c);
			k = m - n - 1;
			curPeaks[ k ].score = v;
			curPeaks[ k ].rotationID = r;
			curPeaks[ k ].conformationID = c;
		}
		peakSort(curPeaks, 0, m - 1);
		n = 0;
		for (int i = 1; i < m; i++)
			if ((curPeaks[ i ].conformationID != curPeaks[ n ].conformationID)
					|| (curPeaks[ i ].rotationID != curPeaks[ n ].rotationID))
			{
				curPeaks[ ++n ] = curPeaks[ i ];
			}
		n++;
		peakSortByScore(curPeaks, 0, n - 1);
		if (numberOfPeaksForFineRotation > n)
		{
			numberOfPeaksForFineRotation = n;
		}
		peakSort(curPeaks, 0, numberOfPeaksForFineRotation - 1);
		for (int t = 0, fn; t < numberOfPeaksForFineRotation;)
		{
			fn = curPeaks[ t ].conformationID;
			if (flexibleDocking)
			{
				sprintf(inputFile2, "%s_%0*d.xyz", baseInputFile2, numDigits, fn);
			}
			// read B, flip B since convolution reflips? it
			if (!readCenters(&xkBOrig, &ykBOrig, &zkBOrig, &numCentersB, &fkB, &fkBElec, real_magnitude, imag_magnitude, inputFile2, performDocking, false))
			{
				return -1;
			}
			for (int i = 0; i < numThreads; i++)
			{
				xkB[ i ] = new double[ numCentersB ];
				ykB[ i ] = new double[ numCentersB ];
				zkB[ i ] = new double[ numCentersB ];
			}
			// get largest edge
			double largestEdge = 0;
			if (!getLargestEdge(xkAOrig, ykAOrig, zkAOrig, numCentersA, xkBOrig, ykBOrig, zkBOrig, numCentersB, &largestEdge))
			{
				return -1;
			}
			// transform A
			if (!transformAndNormalize(xkAOrig, ykAOrig, zkAOrig, xkA, ykA, zkA, numCentersA, 1,0,0,0,1,0,0,0,1, gridSize, numFreq, interpFuncExtent, largestEdge, 1, &scaleA))
			{
				return -1;
			}
			// get electrostatics kernel using the right scale and grid sizes
			// get elec kernel FFT defined by distance weighted dielectric.
			// this function does not return Fourier coefficients, but FFT of the function sampled on the grid. This is in
			// accordance with the fastsum paper by nfft folks.
			if (!computeElecKernel(smallElectrostaticsKernel, numFreq, scaleA))
			{
				return -1;
			}
			// get frequencies of A.
			if (!getCenterFrequencies(numCentersA, alpha, interpFuncExtent, numFreq, gridSize, xkA, ykA, zkA, fkA, centerFrequenciesA, 0, smoothingFunction[ 0 ]))
			{
				return -1;
			}
			if (!getCenterFrequencies(numCentersA, alpha, interpFuncExtent, numFreq, gridSize, xkA, ykA, zkA, fkAElec, centerElecFrequenciesA, 0, smoothingFunction[ 0 ]))
			{
				return -1;
			}
			double xTransB = 0, yTransB = 0, zTransB = 0;
			// center B
			if (!center(xkBOrig, ykBOrig, zkBOrig, numCentersB, &xTransB, &yTransB, &zTransB))
			{
				return -1;
			}
			translate_B[ fn * 3 + 0 ] = xTransB;
			translate_B[ fn * 3 + 1 ] = yTransB;
			translate_B[ fn * 3 + 2 ] = zTransB;
			scale_B[ fn ] = scaleA;
			while ((t < numberOfPeaksForFineRotation) && (curPeaks[ t ].conformationID == fn))
			{
				PARAMS pr[ numThreads ];
				pthread_t p[ numThreads ];
				int startFineRotation, endFineRotation = -1;
				for (int i = 0; i < numThreads; i++)
				{
					startFineRotation = endFineRotation + 1;
					if (i == numThreads - 1)
					{
						endFineRotation = numberOfFineRotations;
					}
					else
					{
						endFineRotation = startFineRotation + (numberOfFineRotations / numThreads);
					}
					pr[ i ].confID = fn;
					pr[ i ].startRotation = curPeaks[ t ].rotationID;
					pr[ i ].endRotation = curPeaks[ t ].rotationID;
					pr[ i ].rotations = rotations;
					pr[ i ].startFineRotation = startFineRotation;
					pr[ i ].endFineRotation = endFineRotation;
					pr[ i ].fineRotations = fineRotations;
					pr[ i ].numberOfFineRotations = numberOfFineRotations;
					pr[ i ].numberOfPositions = numberOfPositions;
					pr[ i ].xkBOrig = xkBOrig;
					pr[ i ].ykBOrig = ykBOrig;
					pr[ i ].zkBOrig = zkBOrig;
					pr[ i ].xkB = xkB[ i ];
					pr[ i ].ykB = ykB[ i ];
					pr[ i ].zkB = zkB[ i ];
					pr[ i ].numCentersB = numCentersB;
					pr[ i ].gridSize = gridSize;
					pr[ i ].numFreq = numFreq;
					pr[ i ].interpFuncExtent = interpFuncExtent;
					pr[ i ].largestEdge = largestEdge;
					pr[ i ].alpha = alpha;
					pr[ i ].blobbiness = blobbiness;
					pr[ i ].elecScale = elecScale;
					pr[ i ].scaleA = scaleA;
					pr[ i ].centerFrequenciesA = centerFrequenciesA;
					pr[ i ].centerElecFrequenciesA = centerElecFrequenciesA;
					pr[ i ].centerFrequenciesB = centerFrequenciesB[ i ];
					pr[ i ].centerElecFrequenciesB = centerElecFrequenciesB[ i ];
					pr[ i ].centerFrequenciesProduct = centerFrequenciesProduct[ i ];
					pr[ i ].centerFrequenciesElecProduct = centerFrequenciesElecProduct[ i ];
					pr[ i ].sparseProfile = sparseProfile[ i ];
					pr[ i ].sparseElecProfile = sparseElecProfile[ i ];
					pr[ i ].freqHat = freqHat[ i ];
					pr[ i ].freqPlan = freqPlan[ i ];
					pr[ i ].elecFreqPlan = elecFreqPlan[ i ];
					pr[ i ].freqHatPlan = freqHatPlan[ i ];
					pr[ i ].Ian1 = Ian1[ i ];
					pr[ i ].ourMoreFrequencies = ourMoreFrequencies[ i ];
					pr[ i ].moreFreqPlan = moreFreqPlan[ i ];
					pr[ i ].fkB = fkB;
					pr[ i ].fkBElec = fkBElec;
					pr[ i ].smallElectrostaticsKernel = smallElectrostaticsKernel;
					pr[ i ].smoothingFunction = smoothingFunction[ i ];
					pr[ i ].localTopValues = localTopValues[ i ];
					pthread_create(&p[ i ], NULL, startApplyFineRotationsThread, (void*) &pr[ i ]);
				}
				for (int i = 0; i < numThreads; i++)
				{
					pthread_join(p[ i ], NULL);
				}
				t++;
			}
			delete []xkBOrig;
			delete []ykBOrig;
			delete []zkBOrig;
			fftw_free(fkB);
			fftw_free(fkBElec);
			for (int i = 0; i < numThreads; i++)
			{
				delete []xkB[ i ];
				delete []ykB[ i ];
				delete []zkB[ i ];
			}
		}
		for (int i = 0; i < numThreads; i++)
		{
			int n = localTopValues[ i ]->getCurrentNumberOfPositions();
			double v, x, y, z;
			int r, f, c;
			while (n--)
			{
				localTopValues[ i ]->extractMin(&v, &x, &y, &z, &r, &f, &c);
				globalTopValues->updateTopValues(v, x, y, z, r, f, c);
			}
		}
	}
	printf("\nComputation Time = %f\n", getTime() - mainStartTime);
	fprintf(fpOpt, "\ncomputation time = %f\n\n", getTime() - mainStartTime);
	fprintf(fpOpt, "center of the 2nd protein:\n\n");
	for (int i = 0; i < numFile2; i++)
	{
		fprintf(fpOpt, "     conformation %d: %f %f %f\n", i, -translate_B[ i * 3 + 0 ], -translate_B[ i * 3 + 1 ], -translate_B[ i * 3 + 2 ]);
	}
	printf("\n");
	printIntermediateStats(fpOpt, globalTopValues, rotations, fineRotations, translate_A, translate_B, scale_B, baseComplex,
						   reDocking, unboundLigandAtomList, unboundLigandInterfaceAtomIndex, gridSize, pow(numFreq * alpha, 6), 20, 5);
	fflush(fpOpt);
	// free memory
	fftw_free(centerFrequenciesA);
	fftw_free(centerElecFrequenciesA);
	for (int i = 0; i < numThreads; i++)
	{
		fftw_free(centerFrequenciesB[ i ]);
		fftw_free(centerFrequenciesProduct[ i ]);
		fftw_free(sparseProfile[ i ]);
		fftw_free(centerElecFrequenciesB[ i ]);
		fftw_free(centerFrequenciesElecProduct[ i ]);
		fftw_free(sparseElecProfile[ i ]);
		fftw_free(freqHat[ i ]);
		fftw_free(Ian1[ i ]);
		fftw_free(ourMoreFrequencies[ i ]);
		fftw_destroy_plan(freqPlan[ i ]);
		fftw_destroy_plan(elecFreqPlan[ i ]);
		fftw_destroy_plan(freqHatPlan[ i ]);
		fftw_destroy_plan(moreFreqPlan[ i ]);
		delete localTopValues[ i ];
		delete smoothingFunction[ i ];
	}
	delete []xkA;
	delete []ykA;
	delete []zkA;
	delete []xkAOrig;
	delete []ykAOrig;
	delete []zkAOrig;
	delete []translate_B;
	delete []scale_B;
	delete []rotations;
	delete []fineRotations;
	fftw_free(fkA);
	fftw_free(fkAElec);
	delete globalTopValues;
	printf("\nTotal Time = %f\n\n", getTime() - mainStartTime);
	fprintf(fpOpt, "\ntotal time = %f\n", getTime() - mainStartTime);
	fclose(fpOpt);
	return 0;
}
