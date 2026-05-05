#include "fftw3.h"

#include <stdlib.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool compareVolumes(fftw_complex* v1, fftw_complex* v2, int size, double* errorl2, double* lInfinity)
{
	if (!v1 || !v2 || (size<1) || !errorl2 || !lInfinity)
	{
		return false;
	}
	double maxVal;
	double maxDiff;
	{
		double v1r = v1[0][0], v1i = v1[0][1];
		double v2r = v2[0][0], v2i = v2[0][1];
		double v1 = sqrt(v1r*v1r + v1i*v1i);
		double v2 = sqrt(v2r*v2r + v2i*v2i);
		maxVal = v1;
		maxDiff = fabs(v1-v2);
	}
	double sum = 0, diff = 0;
	for (int i=0; i<size*size*size; i++)
	{
		double v1r = v1[i][0], v1i = v1[i][1];
		double v2r = v2[i][0], v2i = v2[i][1];
		double v1=0, v2=0;
		v1 = sqrt(v1r*v1r + v1i*v1i);
		v2 = sqrt(v2r*v2r + v2i*v2i);
		sum += v1*v1;
		diff += (v2-v1)*(v2-v1);
		if (v1 > maxVal)
		{
			maxVal = v1;
		}
		if (fabs(v1-v2) > maxDiff)
		{
			maxDiff = fabs(v1-v2);
		}
	}
	*errorl2 = sqrt(diff/sum);
	*lInfinity = maxDiff/maxVal;
	return true;
}

// the data of the smaller arrays is found in the edges of the larger array. Hence,
//   abcd -------------------------- efgh  is in
//   abcd ihfbsbfsfbowfbwfbweofbwenf efgh  of big array.
bool compareArrays(fftw_complex* centerData, int smallSize, fftw_complex* data, int size, const char* caption, bool compareAll)
{
	if (!centerData || !data || (smallSize<1) || (size<1) || (size<smallSize))
	{
		return false;
	}
	if (compareAll)
	{
		fftw_complex* temp = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size* size*size);
		{
			for (int i=0; i<size*size*size; i++)
			{
				temp[i][0] = temp[i][1] = 0;
			}
		}
		{
			int c=0;
			for (int i=0; i<smallSize; i++)
			{
				int iIndex = i;
				if (iIndex >= smallSize/2)
				{
					iIndex += (size-smallSize);
				}
				for (int j=0; j<smallSize; j++)
				{
					int jIndex = j;
					if (jIndex >= smallSize/2)
					{
						jIndex += (size-smallSize);
					}
					for (int k=0; k<smallSize; k++)
					{
						int kIndex = k;
						if (kIndex >= smallSize/2)
						{
							kIndex += (size-smallSize);
						}
						int index = iIndex*size*size + jIndex*size + kIndex;
						if (index >= size*size*size)
						{
							printf("Error!!\n");
							exit(0);
						}
						if (c >= smallSize*smallSize*smallSize)
						{
							printf("Error!!\n");
							exit(0);
						}
						temp[index][0] = centerData[c][0];
						temp[index][1] = centerData[c][1];
						c++;
					}
				}
			}
		}
		double errorl2, lInfinity;
		if (!compareVolumes(data, temp,  size, &errorl2, &lInfinity))
		{
			return false;
		}
		printf("%s l2error = %lf, lInifinity = %lf\n", caption, errorl2, lInfinity);
		fftw_free(temp);
	}
	else
	{
		fftw_complex* temp = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * smallSize* smallSize*smallSize);
		{
			for (int i=0; i<smallSize*smallSize*smallSize; i++)
			{
				temp[i][0] = temp[i][1] = 0;
			}
		}
		{
			int c = 0;
			for (int i=0; i<size; i++)
			{
				if (i>=smallSize/2 && i<size-smallSize/2)
				{
					continue;
				}
				int iIndex = i;
				if (iIndex >= smallSize/2)
				{
					iIndex -= (size-smallSize);
				}
				for (int j=0; j<size; j++)
				{
					if (j>=smallSize/2 && j<size-smallSize/2)
					{
						continue;
					}
					int jIndex = j;
					if (jIndex >= smallSize/2)
					{
						jIndex -= (size-smallSize);
					}
					for (int k=0; k<size; k++)
					{
						if (k>=smallSize/2 && k<size-smallSize/2)
						{
							continue;
						}
						int kIndex = k;
						if (kIndex >= smallSize/2)
						{
							kIndex -= (size-smallSize);
						}
						int index = iIndex*smallSize*smallSize + jIndex*smallSize + kIndex;
						if (index >= smallSize*smallSize*smallSize)
						{
							printf("Error!!\n");
							exit(0);
						}
						if (i* size* size + j* size + k >= size*size*size)
						{
							printf("Error!!\n");
							exit(0);
						}
						temp[index][0] = data[i*size*size + j*size + k][0];
						temp[index][1] = data[i*size*size + j*size + k][1];
					}
				}
			}
		}
		double errorl2, lInfinity;
		if (!compareVolumes(temp, centerData,  smallSize, &errorl2, &lInfinity))
		{
			return false;
		}
		printf("%s l2error = %lf, lInifinity = %lf\n", caption, errorl2, lInfinity);
		fftw_free(temp);
	}
	return true;
}
