#include <Utility/utility.h>
#include <libCG/CoarseGrain/coarseGrain.h>
#include <vector>

extern "C" void dgesv_(int* n, int* nrhs, double* a, int* lda, int* ipiv, double* b, int* ldb, int* info);

using namespace MOLECULE;

void CoarseGrain::LinearSystemSolver(double* AA, double* bb, int N)
{
	int n = N;
	int k = 1;
	std::vector<double> A(n*n), b(n);
	std::vector<int> ipiv(n); int info;
	int i, j;
	for(i = 0; i < n; i++)
	{
		for(j = i+1; j < n; j++)
		{
			A[i* n+j] = AA[i*n+j];
			A[j* n+i] = A[i*n+j];
		}
		A[i* n+i] = AA[i*n+i];
		b[i] = bb[i];
	}
	dgesv_(&n, &k, A.data(), &n, ipiv.data(), b.data(), &n, &info);
	for(i = 0; i < n; i++)
	{
		bb[i] = b[i];
	}
}
