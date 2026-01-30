#ifndef _KIFMMWRAPPER_H_
#define _KIFMMWRAPPER_H_

#include "KIFMM3D/fmm3d/fmm3d.hpp"


using namespace std;


class KiFmmWrapper{
 protected:
  FMM3d* fmm;
  DblNumMat* srcPos;
  DblNumMat* srcNor;
  DblNumMat* trgPos;
  DblNumMat* trgNor;

  // weight
  double* weight;


  double rootlevel;
  map<string,string> optionsMap;
  
  unsigned int numSrc;
  unsigned int numTrg;
    
  double coef0, coef1;
  
  double center[3];

  int kernelType;

  int accuracy;
  
  Kernel3d * knlManual;

  
 public:

  KiFmmWrapper();
  KiFmmWrapper(int size, DblNumMat* quadPos,DblNumMat* quadNor,double * weight, int kernelType1, double * coef);
  KiFmmWrapper(int sizeSrc, int sizeTrg, DblNumMat* srcPos,DblNumMat* srcNor,DblNumMat* trgPos,DblNumMat* trgNor, double * weight, int kernelType1, double * coef, int accuracy);

  virtual ~KiFmmWrapper();

  void fmmSetup();
  
  double manualEval(int numSrc1, DblNumMat & srcPos1, DblNumMat & srcNor1, double * srcDen, int trgId);
  double manualEval(int numSrc1, int * srcId, double* srcDen, int trgId);
  void fmmEval(double* srcden,  double* trgval);
  
  unsigned int getNumSource(){return numSrc;}
  unsigned int getNumTarget(){return numTrg;}

};

#endif
