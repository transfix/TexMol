#ifndef _PETSC_INT_H_
#define _PETSC_INT_H_

extern "C" {
#include <petscmat.h>
#include <petscksp.h>
}
#include "PB2/MultPB.h"


class KSPSolverPB
{
 public:
  KSPSolverPB();
  ~KSPSolverPB();
  
  PetscErrorCode GMRES(Vec x, Vec b);
  
  PetscErrorCode LUDirectSolver(Vec x, Vec b);
  static void transFromFloatArrayToVec(double* input, Vec& output, int size);
  static void transFromVecToFloatArray(Vec& input, double* output, int size);
 protected:
  
  static int FmmMatMult(Mat, Vec, Vec);
  static int PBPC(int m, int n, Mat& P);
  static int MatrixView(Mat);
  
  static int count;
  
};


#endif
