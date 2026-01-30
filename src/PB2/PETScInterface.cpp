#include "PB2/PETScInterface.h"
//#include "Precondition.h"

extern MultPB* mult_fmm2;
//extern Precondition* precon;


int KSPSolverPB::count=0;

KSPSolverPB::KSPSolverPB() {
}

KSPSolverPB::~KSPSolverPB() {
}

PetscErrorCode PBPreconditioner2(PC ctx,Vec x,Vec y) {
  VecCopy(x,y);
  //precon->evaluateDiag(x,y);
  //precon->evaluate(x,y);
  return 0;
}

void KSPSolverPB::transFromFloatArrayToVec(double* input, Vec& output, int size) {
  PetscScalar* outputVal;
  VecCreate(PETSC_COMM_WORLD,&output);
  VecSetSizes(output,PETSC_DECIDE,size);
  VecSetFromOptions(output);
  VecGetArray(output,&outputVal);
  for(int i=0; i<size; i++)
    outputVal[i] = input[i];
  VecRestoreArray(output,&outputVal);
}

void KSPSolverPB::transFromVecToFloatArray(Vec& input, double* output, int size) {
  PetscScalar* inputVal;
  PetscInt inputSize;
  VecGetArray(input,&inputVal);
  VecGetSize(input, &inputSize);
  if(size != inputSize)
    printf("array size is not the same\n");
  //output = new double[size];
  for(int i=0; i<size; i++)
    output[i] = inputVal[i];
}



#undef __FUNCT__
#define __FUNCT__ "LU"
PetscErrorCode KSPSolverPB::LUDirectSolver(Vec x, Vec b) {
  PetscErrorCode ierr;
  PetscFunctionBegin;
  KSPConvergedReason reason;

  printf("lu\n");
  MPI_Comm comm = PETSC_COMM_WORLD;
  PC pc;
  Mat A;
  KSP ksp;
  //int sourceNum = mult_fmm2->getNumSource();
  //int targetNum = mult_fmm2->getNumTarget();
  PetscInt sourceNum;
  PetscInt targetNum;
  VecGetSize(x, &sourceNum);
  VecGetSize(b, &targetNum);
  int M = PETSC_DECIDE;
  int N = PETSC_DECIDE;

  MatCreateSeqAIJ(comm,sourceNum,targetNum,0,0,&A);
  double* amatrix;
  amatrix = new double[sourceNum*targetNum];
  mult_fmm2->getCoeffMatrix(amatrix);
  for(int i=0; i<sourceNum; i++) {
    for(int j=0; j<targetNum; j++) {
      int index = i * targetNum + j;
      PetscScalar v = amatrix[index];
      MatSetValues(A,1,&i,1,&j,&v,INSERT_VALUES);
    }
  }
  delete amatrix;

  MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
  PetscPrintf(comm,"start LU\n");
  KSPCreate(comm, &ksp);

  KSPSetFromOptions(ksp);
  KSPSetOperators(ksp,A,A,SAME_NONZERO_PATTERN);
  KSPSetType(ksp,KSPPREONLY);
  //KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);
  KSPGetPC(ksp,&pc);

  //Preconditioner
  PCSetType(pc,PCLU);

  //KSPSetOperators(ksp,A,PETSC_NULL,DIFFERENT_NONZERO_PATTERN);
  KSPSolve(ksp,b,x);
  KSPGetConvergedReason(ksp,&reason);
  PetscPrintf(comm,"\n ksp converged reason:%d\n",reason);
  if (reason==KSP_DIVERGED_INDEFINITE_PC) {
    printf("\nDivergence because of indefinite preconditioner;\n");
    printf("Run the executable again but with -pc_factor_shift_positive_definite option.\n");
  } else if (reason<0) {
    printf("\nOther kind of divergence: this should not happen.\n");
  } else {
    PetscInt its;
    KSPGetIterationNumber(ksp,&its);
    //printf("Convergence in %d iterations.\n",(int)its);
  }

  //KSPDestroy(ksp);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "GMRES + FMM"
PetscErrorCode KSPSolverPB::GMRES(Vec x, Vec b) {  
  PetscErrorCode ierr;
  PetscFunctionBegin;
  KSPConvergedReason reason;

  // reset the iteration count.
  count = 0;

  MPI_Comm comm = PETSC_COMM_WORLD;
  PC pc;
  Mat A;
  //Mat P;  // preconditioner
  KSP ksp;
  //PetscPrintf(comm,"mult_fmm2\n");
  //int sourceNum = mult_fmm2->getNumSource();
  //int targetNum = mult_fmm2->getNumTarget();
  PetscInt sourceNum;
  PetscInt targetNum;
  VecGetSize(x, &sourceNum);
  VecGetSize(b, &targetNum);
  //printf("%d %d\n",mult_fmm2->getNumSource(),mult_fmm2->getNumTarget());
  //PetscPrintf(comm,"mult_fmm2 %d %d\n",sourceNum,targetNum);
  int M = PETSC_DECIDE;
  int N = PETSC_DECIDE;

  //PetscPrintf(comm,"create shell\n");

  //MatSetSize(A,sourceNum,targetNum,M,Na);

  void *c;
  MatCreateShell(comm,sourceNum,targetNum, M, N, c, &A);

  MatShellSetOperation(A, MATOP_MULT, (void(*)(void))FmmMatMult );
  //PBPC(sourceNum,targetNum,A);
  // Computer preconditioner
  //MatSetFromOptions(P);
  //PBPC(sourceNum,targetNum,P);
  //MatCreateSeqAIJ(comm, sourceNum, targetNum, PETSC_DECIDE, PETSC_NULL, &P);
  MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
  //MatAssemblayBegin(P, MAT_FINAL_ASSEMBLY);
  //MatAssemblyEnd(P, MAT_FINAL_ASSEMBLY);
  //MatrixView(P);
  //PetscPrintf(comm,"start KSP\n");

  KSPCreate(comm, &ksp);
  KSPSetFromOptions(ksp);
  //KSPSetOperators(ksp,A,P, DIFFERENT_NONZERO_PATTERN);

  //KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);
  KSPSetOperators(ksp,A,A, DIFFERENT_NONZERO_PATTERN);
  KSPGetPC(ksp,&pc);

  //Preconditioner
  PCSetType(pc,PCSHELL);
  PCShellSetApply(pc,PBPreconditioner2);
  //PCView(pc, PETSC_VIEWER_STDOUT_WORLD);
  //Initial Guess
  KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);
  //KSPSetInitialGuessKnoll(ksp,PETSC_TRUE);

  //KSPSetOperators(ksp,A,PETSC_NULL,DIFFERENT_NONZERO_PATTERN);
  KSPSolve(ksp,b,x);
  KSPGetConvergedReason(ksp,&reason);
  PetscPrintf(comm,"\n ksp converged reason:%d\n",reason);
  if (reason==KSP_DIVERGED_INDEFINITE_PC) {
    printf("\nDivergence because of indefinite preconditioner;\n");
    printf("Run the executable again but with -pc_factor_shift_positive_definite option.\n");
  } else if (reason<0) {
    printf("\nOther kind of divergence: this should not happen.\n");
  } else {
    PetscInt its;
    KSPGetIterationNumber(ksp,&its);
    //printf("\nConvergence in %d iterations.\n",(int)its);
  }

  //KSPDestroy(ksp);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PBFmmMatMult"
int KSPSolverPB::FmmMatMult(Mat A, Vec x, Vec b) {

  //cout << "GMRES Iteration " << count++ << endl;
  cout << " " << ++count;

  void  *c;
  PetscErrorCode  ierr;
  PetscFunctionBegin;
  ierr = MatShellGetContext(A, (void **)&c);
  PetscScalar *xarray, *barray;
  VecGetArray(x,&xarray);
  VecGetArray(b,&barray);

  mult_fmm2->evaluate(xarray,barray);


  //int num_x;
  //VecGetSize(x,&num_x);
  //for (int i=0; i<num_x; i++) {
  //  cout << xarray[i] << " ";
  //} cout << endl;


  VecRestoreArray(x,&xarray);
  VecRestoreArray(b,&barray);
  PetscFunctionReturn(0);
}


#undef __FUNCT__
#define __FUNCT__ "PB Preconditioner"
int KSPSolverPB::PBPC(int m, int n, Mat& P) {
  MPI_Comm comm = PETSC_COMM_WORLD;
  PetscFunctionBegin;
  /*PetscInt bs = 1;
  PetscInt diag[m];
  PetscScalar* diagv[m];
  for(int i=0;i<m;i++)
  {
  	PetscMalloc(bs*sizeof(PetscScalar),&diagv[i]);
  	diag[i] = 0;
  	diagv[i][0] = 1.0;
  }
  //MatCreateMPIBDiag(comm,PETSC_DECIDE,m,n,m,1, diag, diagv, &P);
  MatCreateSeqBDiag(comm,m,n,m,bs, diag, diagv, &P);*/

  PetscInt nnz[m];
  for(int i=0; i<m; i++)
    nnz[i] = 1;

  MatCreateSeqAIJ(comm,m,n,1,nnz,&P);
  MatSetFromOptions(P);
  //MatCreateMPIAIJ(comm,PETSC_DECIDE,PETSC_DECIDE, m, n, 1, nnz, 0, PETSC_NULL, &P);
  PetscScalar v = 1.0;
  for(int i=0; i<m; i++)
    MatSetValue(P,i,i,v,INSERT_VALUES);
  //MatSetValues(P,i,&i,i,&i,&v,INSERT_VALUES);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PB View"
int KSPSolverPB::MatrixView(Mat A) {
  PetscViewer viewer;
  PetscErrorCode ierr;
  ierr = PetscViewerASCIIOpen(PETSC_COMM_WORLD, "result.m", &viewer);
  //ierr = PetscViewerPushFormat(viewer,	PETSC_VIEWER_ASCII_DEFAULT);
  ierr = PetscViewerPushFormat(viewer,	PETSC_VIEWER_ASCII_INFO);
  ierr = PetscObjectSetName((PetscObject)A,"Amat");
  ierr = MatView(A,viewer);
  return 0;
}
