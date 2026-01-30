#ifndef _MULTPB_H_
#define _MULTPB_H_

#include "PB2/KiFmmWrapper.h"
#include "PB2/PBLinear.h"
#include "PB2/PBKernel.h"
#include "PB2/PBEnum.h"
#include "TriElement/trielement.h"
#include "TriElement/quadrature.h"
#include "PB2/PBOptions.h"


class PBSolver;

class MultPB{
		
 protected:

  int NumKernels;
  KiFmmWrapper * fs[16];
  double kernelCoeff[16];


  DblNumMat* quadPos;  
  DblNumMat* quadNor;
  int NumQuadPoints;

  int NumSrcPoints;
  DblNumMat* srcPos;  
  DblNumMat* srcNor;
  int NumTrgPoints;
  DblNumMat* trgPos;  
  DblNumMat* trgNor;

  double kappa,epsilon;

  BIE_FORMULATION formulation;
  DISCRETIZATION_METHOD method;
  
  int count;

  TriElementNS::Quadrature * quadRule;

  PBSolver * pbsolver;

  int accuracy;


  bool useDuffyQuadrature;
  bool useNearFieldRefinement;
  
  bool withSternLayer;

 public:


  int NumIterations;

  double * weight;  
  //MultFmm( TriElement* surface , double _kappa, double _epsilon, bool linearflag );
  //MultPB( char * quadfile , double _kappa, double _epsilon, BIE_FORMULATION _formulation, DISCRETIZATION_METHOD _method);

  MultPB( int _NumSrcPoints, DblNumMat * srcPos, DblNumMat * srcNor,
	  int _NumTrgPoints, DblNumMat * trgPos, DblNumMat * trgNor,
	  double * _weight, double _kappa, double _epsilon, 
	  PBOptions * options, PBSolver * _pbsolver);



  MultPB() {}
  ~MultPB();
  
  void evaluate(double* srcValue, double* trgValue);
  void evaluateFMM(double* srcDensity, double* trgValue);

  void interpolateValuesAtQuadPoints( double* srcDensityAtVertices, double * srcDensityAtQuadPoints);

  void fixSingularQuadrature( double* srcDensityAtVertices, double * srcDensityAtQuadPoints, double * trgValue);

  void fixNearFieldQuadrature( double* srcDensityAtVertices, double * srcDensityAtQuadPoints, double * trgValue);



  void getCoeffMatrix( double* Amatrix );

  unsigned int getNumSource() { return NumSrcPoints; }
  unsigned int getNumTarget() { return NumTrgPoints; }
  
};

#endif
