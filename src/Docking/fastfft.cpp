#include <fftw3.h>

#if ! defined (__APPLE__)
#include <stdlib.h>
#endif

#include <math.h>
#include <SmoothingFunction/SmoothingFunction.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define 	M_PI   3.14159265358979323846
#endif

extern double getTime();
extern bool fftshift3D(fftw_complex* data, int length);
extern bool compute3DFFT(fftw_complex* dest,fftw_complex* src, int dim1, int dim2, int dim3, int type, unsigned int flags);

// Embed a sum of gaussians / bsplines into a grid of size 2*m + alpha*M.
// We use a table lookup for fast gridding.
void gridding(fftw_complex* fk, double* xk, double* yk, double* zk, double n, int numAtoms, int m, fftw_complex* Ian1, SmoothingFunction* smoothingFunction)
{
	int index, yindex, zindex;
	double inc = n/2.0+m;
	double len = n+2.0*m;
	double lenlen = len*len;
	for (int s=0; s<numAtoms; s++)
	{
		int xL = floor(xk[s]*n)-m;
		int xU = ceil(xk[s]*n)+m;
		int yL = floor(yk[s]*n)-m;
		int yU = ceil(yk[s]*n)+m;
		int zL = floor(zk[s]*n)-m;
		int zU = ceil(zk[s]*n)+m;
		{
			zindex = (int)((zL+inc)*(lenlen));
			double zBellValIndex = zL-n*zk[s];
			for (int zt=zL; zt<=zU; zt++)
			{
				double zbellval = smoothingFunction->getPhi(zBellValIndex);
				yindex = (int)((yL+inc)*(len));
				double yBellValIndex = yL-n*yk[s];
				for (int yt=yL; yt<=yU; yt++)
				{
					double yzbellval = zbellval* smoothingFunction->getPhi(yBellValIndex);
					index = zindex + yindex  + (int)(xL+inc);
					double xBellValIndex = xL-n*xk[s];
					for (int xt=xL; xt<=xU; xt++)
					{
						double bellVal = yzbellval* smoothingFunction->getPhi(xBellValIndex);
						Ian1[index][0] += fk[s][0] * bellVal;
						Ian1[index][1] += fk[s][1] * bellVal;
						index++;
						xBellValIndex++;
					}
					yindex += len;
					yBellValIndex++;
				}
				zindex += lenlen;
				zBellValIndex++;
			}
		}
	}
}


// The input frequencies 'ourFrequencies' is compared with an actual DFT.
// This function could take too long for many frequencies.
void compareWithDFT(fftw_complex* ourFrequencies, int v, int numAtoms, double* xk, double* yk, double* zk, fftw_complex* fk, FILE* fOut)
{
	fftw_complex* dftFrequencies;
	double maxError = 0.0;
	double energyError = 0.0;
	double maxDiffVal = 0.0;
	double maxVal = 0.0;
	double sumActualSq = 0.0;
	double sumDiffSq = 0.0;
	double t = 0;
	double absVal = 0;
	double absValim = 0;
	dftFrequencies = (fftw_complex*)malloc(sizeof(fftw_complex)*v*v*v);
	double tstart = getTime();
	int c = 0;
	for (int j=0; j<v; j++)
	{
		for (int k=0; k<v; k++)
		{
			for (int i=0; i<v; i++)
			{
				dftFrequencies[c][0] = dftFrequencies[c][1] = 0;
				for (int idx=0; idx<numAtoms; idx++)
				{
					double theta1 = -2*M_PI*(xk[idx])*(i-v/2);
					double theta2 = -2*M_PI*(yk[idx])*(k-v/2);
					double theta3 = -2*M_PI*(zk[idx])*(j-v/2);
					double a = cos(theta1)*cos(theta2) - sin(theta1)*sin(theta2);
					double b = cos(theta1)*sin(theta2) + cos(theta2)*sin(theta1);
					double e = (a*cos(theta3) - b*sin(theta3));
					double f = (a*sin(theta3) + b*cos(theta3));
					dftFrequencies[c][0] += e * fk[idx][0] - f * fk[idx][1];
					dftFrequencies[c][1] += e * fk[idx][1] + f * fk[idx][0];
				}
				{
					dftFrequencies[c][0] = dftFrequencies[c][0] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
					dftFrequencies[c][1] = dftFrequencies[c][1] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
				}
				c++;
			}
		}
	}
	for (c=0; c<v*v*v; c++)
	{
		double diffVal;
		double val;
		diffVal= sqrt((dftFrequencies[c][0]-ourFrequencies[c][0])*(dftFrequencies[c][0]-ourFrequencies[c][0]) +
					  (dftFrequencies[c][1]-ourFrequencies[c][1])*(dftFrequencies[c][1]-ourFrequencies[c][1]));
		if (diffVal > maxDiffVal)
		{
			maxDiffVal = diffVal;
		}
		val = sqrt(dftFrequencies[c][0]*dftFrequencies[c][0] + dftFrequencies[c][1]*dftFrequencies[c][1]);
		if (val > maxVal)
		{
			maxVal = val;
		}
		sumActualSq += (val*val);
		sumDiffSq += (diffVal*diffVal);
		if (dftFrequencies[c][0] >= 0)
		{
			absVal += dftFrequencies[c][0];
		}
		else
		{
			absVal -= dftFrequencies[c][0];
		}
		if (dftFrequencies[c][1] >= 0)
		{
			absValim += dftFrequencies[c][1];
		}
		else
		{
			absValim -= dftFrequencies[c][1];
		}
	}
	maxError = maxDiffVal / maxVal;
	energyError = (sqrt(sumDiffSq)) / (sqrt(sumActualSq));
	{
		FILE* fp = fopen("DFT.txt","w");
		for (int i=0; i<v*v*v; i++)
		{
			fprintf(fp, "%lf %lf\n", dftFrequencies[i][0], dftFrequencies[i][1]);
		}
		fclose(fp);
	}
	free(dftFrequencies);
}

// Our NDFT algorithm is used to obtain the low frequencies from the input molecule.
// The frequencies are returned in the array ourFrequencies.
bool getCenterFrequencies(int numAtoms, double alpha, int interpFuncExtent, int numFreq, int gridSize, double* xk, double* yk, double* zk, fftw_complex* fk, fftw_complex* ourFrequencies, FILE* fOut, SmoothingFunction* smoothingFunction)
{
	if (!xk || !yk || !zk || !fk || !ourFrequencies || !smoothingFunction)
	{
		return false;
	}
	fftw_complex* Ian1 = 0;
	double n;
	int I;
	int v;
	double freqTime = getTime();
	// convinient as we keep dividing by it!
	n = (int)alpha*numFreq;
	v = numFreq;
	I = (int)(n)+2*interpFuncExtent;
	Ian1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*I*I*I);
	for (int i=0; i<I*I*I; i++)
	{
		Ian1[i][0] = Ian1[i][1] = 0;
	}
	{
		double griddingTime = getTime();
		gridding(fk, xk, yk, zk, n, numAtoms, interpFuncExtent, Ian1, smoothingFunction);
	}
	{
		int gSize = (int)(n+2*interpFuncExtent);
		fftw_complex* ourMoreFrequencies;
		int m = interpFuncExtent;
		int alphaM = (int)n;
		ourMoreFrequencies = (fftw_complex*)malloc(sizeof(fftw_complex)*(alphaM)*(alphaM)*(alphaM));
		for (int c=0; c<alphaM*alphaM*alphaM; c++)
		{
			ourMoreFrequencies[c][0] = ourMoreFrequencies[c][1] = 0;
		}
		for (int c=0; c<v*v*v; c++)
		{
			ourFrequencies[c][0] = ourFrequencies[c][1] = 0;
		}
		// do the shifts to the middle cube + 8 corner cubes + 12 side bars + 6 faces. Add all into the same area as FFT is commutative in addition
		int c = 0;
		int wrapValue = alphaM + 2*m;
		for (int i=0; i<2*m+alphaM; i++)
		{
			for (int j=0; j<2*m+alphaM; j++)
			{
				for (int k=0; k<2*m+alphaM; k++)
				{
					// to get correct indices, do the following:
					// 1. get the index
					// 2. index = index - m
					// 3. if( index < 0 ) index += alphaM+2m
					// 4. index = index mod alphaM
					// this should be symmetrical in all indices
					int newI = i-m, newJ = j-m, newK = k-m;
					if (newI < 0)
					{
						newI += wrapValue;
					}
					if (newJ < 0)
					{
						newJ += wrapValue;
					}
					if (newK < 0)
					{
						newK += wrapValue;
					}
					newI = newI % alphaM;
					newJ = newJ % alphaM;
					newK = newK % alphaM;
					ourMoreFrequencies[newI* alphaM* alphaM + newJ* alphaM + newK][0] += Ian1[i*gSize*gSize + j*gSize + k][0];
					ourMoreFrequencies[newI* alphaM* alphaM + newJ* alphaM + newK][1] += Ian1[i*gSize*gSize + j*gSize + k][1];
				}
			}
		}
		{
			double fftTime = getTime();
			if (!compute3DFFT(ourMoreFrequencies,ourMoreFrequencies, alphaM, alphaM, alphaM, FFTW_FORWARD, FFTW_ESTIMATE))
			{
				return false;
			}
		}
		c = 0;
		for (int j=0; j<alphaM; j++)
		{
			for (int k=0; k<alphaM; k++)
			{
				for (int i=0; i<alphaM; i++)
				{
					if (j%2 == 1)
					{
						ourMoreFrequencies[c][0] *= -1;
						ourMoreFrequencies[c][1] *= -1;
					}
					if (k%2 == 1)
					{
						ourMoreFrequencies[c][0] *= -1;
						ourMoreFrequencies[c][1] *= -1;
					}
					if (i%2 == 1)
					{
						ourMoreFrequencies[c][0] *= -1;
						ourMoreFrequencies[c][1] *= -1;
					}
					c++;
				}
			}
		}
		{
			c = 0;
			for (int j=0; j<alphaM; j++)
			{
				for (int k=0; k<alphaM; k++)
				{
					for (int i=0; i<alphaM; i++)
					{
						if ((j<v/2 || j>=alphaM-v/2) && (k<v/2 || k>=alphaM-v/2) && (i<v/2 || i>=alphaM-v/2))
						{
							int i1, j1, k1;
							if (j<v/2)
							{
								j1 = j+v/2;
							}
							else
							{
								j1 = j-alphaM+v/2;
							}
							if (k<v/2)
							{
								k1 = k+v/2;
							}
							else
							{
								k1 = k-alphaM+v/2;
							}
							if (i<v/2)
							{
								i1 = i+v/2;
							}
							else
							{
								i1 = i-alphaM+v/2;
							}
							ourFrequencies[j1* v* v + k1* v +i1][0] = ourMoreFrequencies[c][0];
							ourFrequencies[j1* v* v + k1* v +i1][1] = ourMoreFrequencies[c][1];
						}
						c++;
					}
				}
			}
			{
				c = 0;
				for (int i=0; i<v; i++)
				{
					double iPsiBar = smoothingFunction->getPhiHat(abs(i-v/2));
					for (int j=0; j<v; j++)
					{
						double ijPsiBar =  iPsiBar * smoothingFunction->getPhiHat(abs(j-v/2));
						for (int k=0; k<v; k++)
						{
							double ijkPsiBar =  ijPsiBar * smoothingFunction->getPhiHat(abs(k-v/2));
							ourFrequencies[c][0] /= ijkPsiBar;
							ourFrequencies[c][1] /= ijkPsiBar;
							c++;
						}
					}
				}
			}
		}
		{
			c = 0;
			for (int j=0; j<v; j++)
			{
				for (int k=0; k<v; k++)
				{
					for (int i=0; i<v; i++)
					{
						if ((i-v/2)+(j-v/2)+(k-v/2) != 0)
						{
							ourFrequencies[c][0] = ourFrequencies[c][0] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
							ourFrequencies[c][1] = ourFrequencies[c][1] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
						}
						c++;
					}
				}
			}
		}
		free(ourMoreFrequencies);
	}
	fftw_free(Ian1);
	if (!fftshift3D(ourFrequencies, v))
	{
		return false;
	}
	return true;
}

// Our NDFT algorithm is used to obtain the low frequencies from the input molecule.
// The frequencies are returned in the array ourFrequencies.
bool getCenterFrequencies(int numAtoms, double alpha, int interpFuncExtent, int numFreq, int gridSize, double* xk, double* yk, double* zk, fftw_complex* fk, fftw_complex* ourFrequencies, FILE* fOut, SmoothingFunction* smoothingFunction, fftw_complex* Ian1, fftw_complex* ourMoreFrequencies, fftw_plan moreFreqPlan)
{
	if (!xk || !yk || !zk || !fk || !ourFrequencies || !smoothingFunction || !Ian1 || !ourMoreFrequencies)
	{
		return false;
	}
	int t = 0;
	double n;
	int I;
	int v;
	double freqTime = getTime();
	n = (int)alpha*numFreq;
	v = numFreq;
	I = (int)(n)+2*interpFuncExtent;
	for (int i=0; i<I*I*I; i++)
	{
		Ian1[i][0] = Ian1[i][1] = 0;
	}
	{
		double griddingTime = getTime();
		gridding(fk, xk, yk, zk, n, numAtoms, interpFuncExtent, Ian1, smoothingFunction);
	}
	{
		int gSize = (int)(n+2*interpFuncExtent);
		int m = interpFuncExtent;
		int alphaM = (int)n;
		for (int c=0; c<alphaM*alphaM*alphaM; c++)
		{
			ourMoreFrequencies[c][0] = ourMoreFrequencies[c][1] = 0;
		}
		for (int c=0; c<v*v*v; c++)
		{
			ourFrequencies[c][0] = ourFrequencies[c][1] = 0;
		}
		// do the shifts to the middle cube + 8 corner cubes + 12 side bars + 6 faces. Add all into the same area as FFT is commutative in addition
		int wrapValue = alphaM + 2*m;
		for (int i=0; i<2*m+alphaM; i++)
		{
			for (int j=0; j<2*m+alphaM; j++)
			{
				for (int k=0; k<2*m+alphaM; k++)
				{
					// to get correct indices, do the following:
					// 1. get the index
					// 2. index = index - m
					// 3. if( index < 0 ) index += alphaM+2m
					// 4. index = index mod alphaM
					// this should be symmetrical in all indices
					int newI = i-m, newJ = j-m, newK = k-m;
					if (newI < 0)
					{
						newI += wrapValue;
					}
					if (newJ < 0)
					{
						newJ += wrapValue;
					}
					if (newK < 0)
					{
						newK += wrapValue;
					}
					newI = newI % alphaM;
					newJ = newJ % alphaM;
					newK = newK % alphaM;
					ourMoreFrequencies[newI* alphaM* alphaM + newJ* alphaM + newK][0] += Ian1[i*gSize*gSize + j*gSize + k][0];
					ourMoreFrequencies[newI* alphaM* alphaM + newJ* alphaM + newK][1] += Ian1[i*gSize*gSize + j*gSize + k][1];
				}
			}
		}
			{
			double fftTime = getTime();
			fftw_execute(moreFreqPlan);
		}
			int c = 0;
		for (int j=0; j<alphaM; j++)
		{
			for (int k=0; k<alphaM; k++)
			{
				for (int i=0; i<alphaM; i++)
				{
					if (j%2 == 1)
					{
						ourMoreFrequencies[c][0] *= -1;
						ourMoreFrequencies[c][1] *= -1;
					}
					if (k%2 == 1)
					{
						ourMoreFrequencies[c][0] *= -1;
						ourMoreFrequencies[c][1] *= -1;
					}
					if (i%2 == 1)
					{
						ourMoreFrequencies[c][0] *= -1;
						ourMoreFrequencies[c][1] *= -1;
					}
					c++;
				}
			}
		}
			{
			c = 0;
			for (int j=0; j<alphaM; j++)
			{
				for (int k=0; k<alphaM; k++)
				{
					for (int i=0; i<alphaM; i++)
					{
						if ((j<v/2 || j>=alphaM-v/2) && (k<v/2 || k>=alphaM-v/2) && (i<v/2 || i>=alphaM-v/2))
						{
							int i1, j1, k1;
							if (j<v/2)
							{
								j1 = j+v/2;
							}
							else
							{
								j1 = j-alphaM+v/2;
							}
							if (k<v/2)
							{
								k1 = k+v/2;
							}
							else
							{
								k1 = k-alphaM+v/2;
							}
							if (i<v/2)
							{
								i1 = i+v/2;
							}
							else
							{
								i1 = i-alphaM+v/2;
							}
							ourFrequencies[j1* v* v + k1* v +i1][0] = ourMoreFrequencies[c][0];
							ourFrequencies[j1* v* v + k1* v +i1][1] = ourMoreFrequencies[c][1];
						}
						c++;
					}
				}
			}
					{
				c = 0;
				for (int i=0; i<v; i++)
				{
					double iPsiBar = smoothingFunction->getPhiHat(abs(i-v/2));
					for (int j=0; j<v; j++)
					{
						double ijPsiBar =  iPsiBar * smoothingFunction->getPhiHat(abs(j-v/2));
						for (int k=0; k<v; k++)
						{
							double ijkPsiBar =  ijPsiBar * smoothingFunction->getPhiHat(abs(k-v/2));
							ourFrequencies[c][0] /= ijkPsiBar;
							ourFrequencies[c][1] /= ijkPsiBar;
							c++;
						}
					}
				}
			}
		}
		{
			c = 0;
			for (int j=0; j<v; j++)
			{
				for (int k=0; k<v; k++)
				{
					for (int i=0; i<v; i++)
					{
						if ((i-v/2)+(j-v/2)+(k-v/2) != 0)
						{
							ourFrequencies[c][0] = ourFrequencies[c][0] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
							ourFrequencies[c][1] = ourFrequencies[c][1] * pow(-1,(double)((i-v/2)+(j-v/2)+(k-v/2)));
						}
						c++;
					}
				}
			}
		}
	}
	if (!fftshift3D(ourFrequencies, v))
	{
		return false;
	}
	return true;
}
