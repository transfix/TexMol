#include "fftw3.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Include ordering dependency
#include <Docking/fullfft.h>

#ifndef M_PI
#define 	M_PI   3.14159265358979323846
#endif

extern bool compute3DFFT(fftw_complex* dest,fftw_complex* src, int dim1, int dim2, int dim3, int type, unsigned int flags);

bool shiftGrid(fftw_complex* grid, int size)
{
	if (!grid || (size<1))
	{
		return false;
	}
	fftw_complex* temp = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size* size*size);
	for (int i=0; i<size*size*size; i++)
	{
		temp[i][0] = grid[i][0];
		temp[i][1] = grid[i][1];
	}
	int c = 0;
	for (int i=0; i<size; i++)
	{
		int iIndex = (i + (size/2))%size;
		for (int j=0; j<size; j++)
		{
			int jIndex = (j + (size/2))%size;
			for (int k=0; k<size; k++)
			{
				int kIndex = (k + (size/2))%size;
				grid[iIndex* size* size + jIndex* size + kIndex][0] = temp[c][0];
				grid[iIndex* size* size + jIndex* size + kIndex][1] = temp[c][1];
				c++;
			}
		}
	}
	fftw_free(temp);
	return true;
}

bool computeDFT(fftw_complex* dest,fftw_complex* src, int size, int v)
{
	if (!dest || !src || (size<1) || (v<1))
	{
		return false;
	}
	int c = 0;
	for (int j=0; j<v; j++)
	{
		for (int k=0; k<v; k++)
		{
			for (int i=0; i<v; i++)
			{
				dest[c][0] = dest[c][1] = 0;
				int num = 0;
				for (int idx=0; idx<size; idx++)
				{
					for (int jdx=0; jdx<size; jdx++)
					{
						for (int kdx=0; kdx<size; kdx++)
						{
							int index = idx*size*size + jdx*size + kdx;
							if ((src[index][0] < -0.1) || (src[index][0] > 0.1) || (src[index][1] < -0.1) || (src[index][1] > 0.1))
							{
								double theta1 = -2*M_PI*(kdx-size/2)*(i-v/2)/((double)size);
								double theta2 = -2*M_PI*(jdx-size/2)*(k-v/2)/((double)size);
								double theta3 = -2*M_PI*(idx-size/2)*(j-v/2)/((double)size);
								double a = cos(theta1)*cos(theta2) - sin(theta1)*sin(theta2);
								double b = cos(theta1)*sin(theta2) + cos(theta2)*sin(theta1);
								double e = (a*cos(theta3) - b*sin(theta3));
								double f = (a*sin(theta3) + b*cos(theta3));
								dest[c][0] += e * src[index][0] - f * src[index][1];
								dest[c][1] += e * src[index][1] + f * src[index][0];
								num++;
							}
						}
					}
				}
				{
					dest[c][0] = dest[c][0] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
					dest[c][1] = dest[c][1] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
				}
				c++;
			}
		}
	}
	return true;
}

bool getFrequencies(int numCenters, int gridSize, double* xk, double* yk, double* zk, fftw_complex* fk, fftw_complex* frequencies)
{
	if (!xk || !yk || !zk || !fk || !frequencies || (numCenters<1))
	{
		return false;
	}
	{
		for (int i=0; i< gridSize*gridSize*gridSize; i++)
		{
			frequencies[i][0] =0.0;
			frequencies[i][1] =0.0;
		}
		for (int i=0; i < numCenters; i++)
		{
			int intxk = (int)((xk[i] + 0.5) * gridSize);
			int intyk = (int)((yk[i] + 0.5) * gridSize);
			int intzk = (int)((zk[i] + 0.5) * gridSize);
			if ((frequencies[intzk*gridSize*gridSize  + intyk*gridSize + intxk][0] < -0.1) ||
					(frequencies[intzk*gridSize*gridSize  + intyk*gridSize + intxk][1] < -0.1) ||
					(frequencies[intzk*gridSize*gridSize  + intyk*gridSize + intxk][0] >  0.1) ||
					(frequencies[intzk*gridSize*gridSize  + intyk*gridSize + intxk][1] >  0.1))
			{
			}
			else
			{
				frequencies[intzk* gridSize* gridSize  + intyk* gridSize + intxk][0] = fk[i][0];
				frequencies[intzk* gridSize* gridSize  + intyk* gridSize + intxk][1] = fk[i][1];
			}
		}
	}
	if (!compute3DFFT(frequencies, frequencies, gridSize, gridSize, gridSize, FFTW_FORWARD, FFTW_ESTIMATE))
	{
		printf("Could not perform full fft\n");
		return false;
	}
	return true;
}
