/*
* Fast Summation over triangular mesh using KiFMM
* author: Shun-Chuan Albert Chen
* institute: University of Texas at Austin
*
*/

//#include <cstdlib>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include "PB2/KiFmmWrapper.h"


using namespace std;

// get computer time
extern double getTime();


KiFmmWrapper::KiFmmWrapper() {

  cout << "WARNING: avoid this constructor for KiFmmWrapper" << endl;
  srcPos = srcNor = trgPos = trgNor = NULL;
  fmm = new FMM3d("fmm3d_");
  weight = NULL;
}


KiFmmWrapper::KiFmmWrapper(int sizeSrc, int sizeTrg, DblNumMat* _srcPos,DblNumMat* _srcNor,
                           DblNumMat* _trgPos,DblNumMat* _trgNor,
                           double * weight1, int kernelType1, double * coef, int _accuracy) {

  //fmm = new FMM3d("fmm3d_");

  srcPos = _srcPos;
  srcNor = _srcNor;
  trgPos = _trgPos;
  trgNor = _trgNor;

  weight = weight1;

  kernelType = kernelType1;
  coef0 = coef[0];
  coef1 = coef[1];
  accuracy = _accuracy;


  numSrc = sizeSrc;
  numTrg = sizeTrg;


  double min[3],max[3];
  for (int i=0; i<3; i++) {
    min[i] = 100000000000.0;
    max[i] = -1000000000000.0;
  }

  for (int i=0; i<numTrg; i++) {
    for (int j=0; j<3; j++) {
      if (min[j] > (*trgPos)(j,i)) {
        min[j] = (*trgPos)(j,i);
      }
      if (max[j] < (*trgPos)(j,i)) {
        max[j] = (*trgPos)(j,i);
      }
    }
  }
  for (int i=0; i<numSrc; i++) {
    for (int j=0; j<3; j++) {
      if (min[j] > (*srcPos)(j,i)) {
        min[j] = (*srcPos)(j,i);
      }
      if (max[j] < (*srcPos)(j,i)) {
        max[j] = (*srcPos)(j,i);
      }
    }
  }

  rootlevel = max[0] - min[0];
  double len2 = max[1]-min[1];
  if(len2 > rootlevel)
    rootlevel = len2;
  double len3 = max[2]-min[2];
  if(len3 > rootlevel)
    rootlevel = len3;
  center[0] = (max[0] + min[0])/2.0;
  center[1] = (max[1] + min[1])/2.0;
  center[2] = (max[2] + min[2])/2.0;

  //fmmSetup();  // doesn't work here...


  vector<double> tmp(2);
  tmp[0] = coef0;
  tmp[1] = coef1;
  knlManual = new Kernel3d(kernelType, tmp);

}

/*
// FUTURE: eliminate this since it will never run...
KiFmmWrapper::KiFmmWrapper(int size, DblNumMat* quadPos,DblNumMat* quadNor,double * weight1, int kernelType1, double * coef) {

  //fmm = new FMM3d("fmm3d_");

  srcPos = quadPos;
  srcNor = quadNor;
  trgPos = quadPos;
  trgNor = quadNor;

  weight = weight1;

  kernelType = kernelType1;
  coef0 = coef[0];
  coef1 = coef[1];

  numSrc = size;
  numTrg = size;


  double min[3],max[3];
  for (int i=0; i<3; i++) {
    min[i] = 100000000000.0;
    max[i] = -1000000000000.0;
  }

  for (int i=0; i<size; i++) {

    for (int j=0; j<3; j++) {
      if (min[j] > (*quadPos)(j,i)) {
	min[j] = (*quadPos)(j,i);
      }
      if (max[j] < (*quadPos)(j,i)) {
	max[j] = (*quadPos)(j,i);
      }
    }
  }

  rootlevel = max[0] - min[0];
  double len2 = max[1]-min[1];
  if(len2 > rootlevel)
        rootlevel = len2;
  double len3 = max[2]-min[2];
  if(len3 > rootlevel)
        rootlevel = len3;
  center[0] = (max[0] + min[0])/2.0;
  center[1] = (max[1] + min[1])/2.0;
  center[2] = (max[2] + min[2])/2.0;

  //fmmSetup(); //doesn't work here...

}
*/

KiFmmWrapper::~KiFmmWrapper() {
  if(srcPos != NULL) {
    delete srcPos;
    srcPos = NULL;
  }
  if(srcNor != NULL) {
    delete srcNor;
    srcNor = NULL;
  }
  if(trgPos != NULL) {
    delete trgPos;
    trgPos = NULL;
  }
  if(trgNor != NULL) {
    delete trgNor;
    trgNor = NULL;
  }
  if(weight != NULL) {
    delete weight;
    weight = NULL;
  }
  if(fmm != NULL) {
    delete fmm;
    fmm = NULL;
  }
}


void KiFmmWrapper::fmmSetup() {

  vector<double> tmp(2);
  tmp[0] = coef0;
  tmp[1] = coef1;
  Kernel3d knl(kernelType, tmp);

  // setup fmm
  fmm = new FMM3d("fmm3d_");
  fmm->srcPos()=srcPos;
  fmm->srcNor()=srcNor;
  fmm->trgPos()=trgPos;
  fmm->center() = Point3(center[0],center[1],center[2]);
  //fmm->rootLevel() = rootlevel;
  fmm->rootLevel() = -((int)log2(rootlevel)+1);

  fmm->knl() = knl;

}



double KiFmmWrapper::manualEval(int numSrc1, DblNumMat & srcPos1, DblNumMat & srcNor1, double * srcDen, int trgId) {

  DblNumMat trgPos1(3, 1);
  DblNumMat inter(1,numSrc1);

  trgPos1(0,0) = (*trgPos)(0,trgId);
  trgPos1(1,0) = (*trgPos)(1,trgId);
  trgPos1(2,0) = (*trgPos)(2,trgId);


  // evaluate
  knlManual->kernel(srcPos1,srcNor1,trgPos1,inter);

  //cout << "Kernel " << kernelType << endl;
  double result = 0.0;
  for (int i=0; i<numSrc1; i++) {

    //cout << srcDen[i] << " " << inter(0,i) << endl;

    result += srcDen[i]*inter(0,i);
  }
  return result;
}


// Note: does not use the "weight" vector... quadrature weights should be premultiplied
//       into the srcDen vector.
double KiFmmWrapper::manualEval(int numSrc1, int * srcId, double* srcDen, int trgId) {


  DblNumMat srcPos1(3, numSrc1);
  DblNumMat srcNor1(3, numSrc1);
  DblNumMat trgPos1(3, 1);
  DblNumMat inter(1,numSrc1);

  // setup these matrices
  for (int i=0; i<numSrc1; i++) {
    srcPos1(0,i) = (*srcPos)(0,srcId[i]);
    srcPos1(1,i) = (*srcPos)(1,srcId[i]);
    srcPos1(2,i) = (*srcPos)(2,srcId[i]);
    srcNor1(0,i) = (*srcNor)(0,srcId[i]);
    srcNor1(1,i) = (*srcNor)(1,srcId[i]);
    srcNor1(2,i) = (*srcNor)(2,srcId[i]);
  }
  trgPos1(0,0) = (*trgPos)(0,trgId);
  trgPos1(1,0) = (*trgPos)(1,trgId);
  trgPos1(2,0) = (*trgPos)(2,trgId);

  // evaluate
  knlManual->kernel(srcPos1,srcNor1,trgPos1,inter);


  double result = 0.0;
  for (int i=0; i<numSrc1; i++) {
    result += srcDen[i]*inter(0,i);
  }
  return result;

}


void KiFmmWrapper::fmmEval(double* srcden, double* trgval) {

  //cout << "FMM Eval "<< numSrc << " " << numTrg  << endl;

  fmmSetup();


  ostringstream s1;
  s1 << accuracy << endl;

  map<string,string> options;
  options["-fmm3d_np"] = s1.str();
  //options["-fmm3d_np"] ="2";
  //options["-fmm3d_np"] ="4";
  //options["-fmm3d_np"] ="6";
  //options["-fmm3d_np"] ="8";
  options["-fmm3d_let3d_ptsMax"]= "120";
  options["-fmm3d_let3d_maxLevel"]= "20";

  iC( fmm->setup(options) );

  //cout << "start evaluation" << endl;
  //cout << "numSrc = " << numSrc << endl;
  //cout << "numTrg = " << numTrg << endl;

  DblNumVec srcDen(numSrc);
  for(int i=0; i<numSrc; i++) {
    srcDen(i) = srcden[i]*weight[i];
  }

  // old version
  //DblNumVec trgVal(numTrg);
  DblNumVec trgVal(numTrg,false,trgval);

  fmm->evaluate(srcDen, trgVal);


  // useful for error checking...
  //double relativeError;
  //fmm->check(srcDen, trgVal, 20, relativeError);
  //cout << "relative error: " << relativeError << endl;
 

  delete fmm;

}

