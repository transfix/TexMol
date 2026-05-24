#include <stddef.h>
#ifndef _WIN32
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GBEnergy/CoarseGrain/coarseGrain.h"
#include <vector>

//extern "C" double dnrm2_(int *n,double *x, int* incx);
extern "C" void   dgesv_(int *n, int *nrhs, double *a, int *lda, int *ipiv, 
			 double *b, int *ldb, int *info );
//extern "C" void   dcopy_(int *n, double *x, int *incx, double *y, int *incy);
//extern "C" void   dgemm_(char *transa, char *transb,
//                        int *m, int *n, int *k, double *alpha, double *a, 
//			 int *lda, double *b, int *ldb, double *beta, double *c, int *ldc );

using namespace MOLECULE;

void CoarseGrain::LinearSystemSolver(double *AA, double *bb, int N)
{
	int n = N;
	int k = 1;
	std::vector<double> A(n*n), b(n);
	std::vector<int> ipiv(n); int info;
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = i+1; j < n; j++)
		{
			A[i*n+j] = AA[i*n+j];
			A[j*n+i] = A[i*n+j];
		}
		A[i*n+i] = AA[i*n+i];
		b[i] = bb[i];
	}
/*
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf("%f ", A[i*n+j]);
		printf(" %f\n", b[i]);
	}
*/
	dgesv_(&n, &k, A.data(), &n, ipiv.data(), b.data(), &n, &info);
	for (i = 0; i < n; i++)
		bb[i] = b[i];
}

