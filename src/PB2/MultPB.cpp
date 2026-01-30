/*
 arand: new version to solve DBIE equations
        without so many intermediate levels
 */


#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <cmath>
#include <iostream>
#include <fstream>
#include "PB2/MultPB.h"
#include "KIFMM3D/fmm3d/kernel3d.hpp"
#include "TriElement/geometry.h"

#include <PB2/PB2Util.h>

using namespace std;

extern double getTime();

MultPB::MultPB( int _NumSrcPoints, DblNumMat * _srcPos, DblNumMat * _srcNor,
                int _NumTrgPoints, DblNumMat * _trgPos, DblNumMat * _trgNor,
                double * _weight, double _kappa, double _epsilon,
                PBOptions * options, PBSolver * _pbsolver) {  

  pbsolver = _pbsolver;

  NumIterations = 0;

  count = 0;

  kappa=_kappa;
  epsilon=_epsilon;

  double coef[2];
  coef[0] = _kappa;
  coef[1] = _epsilon;

  weight = _weight;

  formulation = options->formulation;

  method = options->discreteMethod; // Nystrom or collocation

  accuracy = options->accuracy;

  NumSrcPoints = _NumSrcPoints;
  srcPos = _srcPos;
  srcNor = _srcNor;
  NumTrgPoints = _NumTrgPoints;
  trgPos = _trgPos;
  trgNor = _trgNor;


  useDuffyQuadrature = false;
  useNearFieldRefinement = false;

  if (options->quadLevel > 0) {
    useDuffyQuadrature = true;
  }
  if (options->quadLevel > 1) {
    useNearFieldRefinement = true;
  }


  //cout << "Setup MultPB: " << NumSrcPoints << " " << NumTrgPoints << endl;

  PrintHeading("Setup Fast Multipole");

  PrintValue("Number of Source Points", NumSrcPoints);
  PrintValue("Number of Target Points", NumTrgPoints);


  if (formulation == FORMULATION_ZERO_ION) {
    NumKernels = 1;
    fs[0] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_D_U,coef,accuracy);
    kernelCoeff[0] = 1.0 - epsilon;
  } else if (formulation == FORMULATION_NBIE) {


    if(!pbsolver->opt->useSternLayer) {
      NumKernels = 4;
      fs[0] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_D_U,coef,accuracy);
      fs[1] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_S_U,coef,accuracy);
      fs[2] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_D_U,coef,accuracy);
      fs[3] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_S_U,coef,accuracy);
      kernelCoeff[0] = 1.0;
      kernelCoeff[1] = -1.0;
      kernelCoeff[2] = -1.0;
      kernelCoeff[3] = 1.0/epsilon;
    }

    // added 5-25-2011
    if(pbsolver->opt->useSternLayer) {
      NumKernels = 12;
      fs[0] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_D_U,coef,accuracy);
      fs[1] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_S_U,coef,accuracy);
      fs[2] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_D_U,coef,accuracy);
      fs[3] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_S_U,coef,accuracy);

      fs[4] = new KiFmmWrapper(NumSrcPoints,pbsolver->sternLayer.NumVertices,srcPos,srcNor,
			       pbsolver->sternLayer.vertPos, pbsolver->sternLayer.vertNor, 
			       weight,KNL_LAP_D_U,coef,accuracy);
      fs[5] = new KiFmmWrapper(NumSrcPoints,pbsolver->sternLayer.NumVertices,srcPos,srcNor,
			       pbsolver->sternLayer.vertPos, pbsolver->sternLayer.vertNor, 
			       weight,KNL_LAP_S_U,coef,accuracy);



      fs[6] = new KiFmmWrapper(pbsolver->sternLayer.NumQuadPoints,NumTrgPoints,
			       pbsolver->sternLayer.quadPos, pbsolver->sternLayer.quadNor,
			       trgPos,trgNor,pbsolver->sternLayer.quadWeight,KNL_LAP_D_U,coef,accuracy);
      fs[7] = new KiFmmWrapper(pbsolver->sternLayer.NumQuadPoints,NumTrgPoints,
			       pbsolver->sternLayer.quadPos, pbsolver->sternLayer.quadNor,
			       trgPos,trgNor,pbsolver->sternLayer.quadWeight,KNL_LAP_S_U,coef,accuracy);

      fs[8] = new KiFmmWrapper(pbsolver->sternLayer.NumQuadPoints, pbsolver->sternLayer.NumVertices,
				pbsolver->sternLayer.quadPos, pbsolver->sternLayer.quadNor, 
				pbsolver->sternLayer.vertPos, pbsolver->sternLayer.vertNor, 
				pbsolver->sternLayer.quadWeight,KNL_LAP_D_U,coef,accuracy);
      fs[9] = new KiFmmWrapper(pbsolver->sternLayer.NumQuadPoints, pbsolver->sternLayer.NumVertices,
				pbsolver->sternLayer.quadPos, pbsolver->sternLayer.quadNor, 
				pbsolver->sternLayer.vertPos, pbsolver->sternLayer.vertNor, 
				pbsolver->sternLayer.quadWeight,KNL_LAP_S_U,coef,accuracy);
      fs[10] = new KiFmmWrapper(pbsolver->sternLayer.NumQuadPoints, pbsolver->sternLayer.NumVertices,
				pbsolver->sternLayer.quadPos, pbsolver->sternLayer.quadNor, 
				pbsolver->sternLayer.vertPos, pbsolver->sternLayer.vertNor, 
				pbsolver->sternLayer.quadWeight,KNL_BOZ_D_U,coef,accuracy);
      fs[11] = new KiFmmWrapper(pbsolver->sternLayer.NumQuadPoints, pbsolver->sternLayer.NumVertices,
				pbsolver->sternLayer.quadPos, pbsolver->sternLayer.quadNor, 
				pbsolver->sternLayer.vertPos, pbsolver->sternLayer.vertNor, 
				pbsolver->sternLayer.quadWeight,KNL_BOZ_S_U,coef,accuracy);

      // constants below could be buggy (5-25-2011)
      kernelCoeff[0] = 1.0;
      kernelCoeff[1] = -1.0;
      kernelCoeff[2] = -1.0;
      kernelCoeff[3] = 1.0/epsilon;
      kernelCoeff[4] = -1.0;
      kernelCoeff[5] = 1.0/epsilon;
      kernelCoeff[6] = 1.0;
      kernelCoeff[7] = -1.0;
      kernelCoeff[8] = 1.0;
      kernelCoeff[9] = -1.0;
      kernelCoeff[10] = -1.0;
      kernelCoeff[11] = 1.0;
    }



  } else if (formulation == FORMULATION_DBIE) {
    NumKernels = 16;
    fs[0] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_D_U,coef,accuracy);
    fs[1] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_D_U,coef,accuracy);
    fs[2] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_S_U,coef,accuracy);
    fs[3] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_S_U,coef,accuracy);
    fs[4] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_QX_U,coef,accuracy);
    fs[5] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_QY_U,coef,accuracy);
    fs[6] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_QZ_U,coef,accuracy);
    fs[7] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_QX_U,coef,accuracy);
    fs[8] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_QY_U,coef,accuracy);
    fs[9] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_QZ_U,coef,accuracy);
    fs[10] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_DX_U,coef,accuracy);
    fs[11] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_DY_U,coef,accuracy);
    fs[12] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_LAP_DZ_U,coef,accuracy);
    fs[13] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_DX_U,coef,accuracy);
    fs[14] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_DY_U,coef,accuracy);
    fs[15] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_BOZ_DZ_U,coef,accuracy);

    kernelCoeff[0] = 1.0;
    kernelCoeff[1] = -1.0;
    kernelCoeff[2] = -1.0;
    kernelCoeff[3] = 1.0;
    kernelCoeff[4] = 1.0;
    kernelCoeff[5] = 1.0;
    kernelCoeff[6] = 1.0;
    kernelCoeff[7] = -1.0;
    kernelCoeff[8] = -1.0;
    kernelCoeff[9] = -1.0;
    kernelCoeff[10] = -1.0;
    kernelCoeff[11] = -1.0;
    kernelCoeff[12] = -1.0;
    kernelCoeff[13] = 1.0/epsilon;
    kernelCoeff[14] = 1.0/epsilon;
    kernelCoeff[15] = 1.0/epsilon;

  } else if (formulation == FORMULATION_DBIE2) { // doesn't work!
    NumKernels = 8;
    fs[0] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_11_U,coef,accuracy);
    fs[1] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_12_U,coef,accuracy);
    fs[2] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_21X_U,coef,accuracy);
    fs[3] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_21Y_U,coef,accuracy);
    fs[4] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_21Z_U,coef,accuracy);
    fs[5] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_22X_U,coef,accuracy);
    fs[6] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_22Y_U,coef,accuracy);
    fs[7] = new KiFmmWrapper(NumSrcPoints,NumTrgPoints,srcPos,srcNor,trgPos,trgNor,weight,KNL_PBE_22Z_U,coef,accuracy);
  }

  PrintValue("Number of Kernels",NumKernels);


}

MultPB::~MultPB() {

}

void MultPB::evaluate(double* srcValue, double* trgValue) {

  NumIterations++;

  // zero target data
  for (int i=0; i<NumTrgPoints; i++) {
    trgValue[i] = 0.0;

    if (formulation != FORMULATION_ZERO_ION) {
      trgValue[i+NumTrgPoints] = 0.0;
    }
  }

  if (pbsolver->opt->useSternLayer) {
    for (int i=0; i<2*pbsolver->sternLayer.NumVertices; i++) {
      trgValue[i+2*NumTrgPoints] = 0.0;
    }
  }


  // Do FMM calculation
  if (method == DISCRETE_NYSTROM) {
    evaluateFMM(srcValue,trgValue);
  } else if (method == DISCRETE_COLLOCATION || 
	     method == DISCRETE_COLLOCATION_CONSTANT) {

    double * srcAtQuadPoints;
    if (!pbsolver->opt->useSternLayer) {
      srcAtQuadPoints =  new double[NumSrcPoints*2];
    } else {
      srcAtQuadPoints = new double[2*NumSrcPoints+2*pbsolver->sternLayer.NumQuadPoints];
    }

    // interpolate values at quadrature points
    interpolateValuesAtQuadPoints(srcValue,srcAtQuadPoints);

    // call fmm
    evaluateFMM(srcAtQuadPoints,trgValue);

    // perform accurate quadrature
    if (useDuffyQuadrature) {
      fixSingularQuadrature(srcValue, srcAtQuadPoints, trgValue);
    }

    if (useNearFieldRefinement) {
      // note: not implemented for Stern layer...
      fixNearFieldQuadrature(srcValue, srcAtQuadPoints, trgValue);
    }

    delete [] srcAtQuadPoints;

  }

  // Add diagonal part
  if (formulation == FORMULATION_ZERO_ION) {
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += srcValue[i]*.5*(1+epsilon);
    }
    
  } else if (formulation == FORMULATION_NBIE) {
    // fixme: move this up one level
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += srcValue[i]*.5;
      trgValue[i+NumTrgPoints] += srcValue[i]*.5;
    }
    // arand: added 5-25-2011
    if (pbsolver->opt->useSternLayer) {
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	trgValue[i+2*pbsolver->molSurf.NumVertices] += 
	  srcValue[i+2*pbsolver->molSurf.NumVertices]*.5;
	trgValue[i+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices] += 
	  srcValue[i+2*pbsolver->molSurf.NumVertices]*.5;
      }      

    }
  } else if (formulation == FORMULATION_DBIE || formulation == FORMULATION_DBIE2) {
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += srcValue[i]*.5*(1.0+epsilon);
      trgValue[i+NumTrgPoints] += srcValue[i+NumTrgPoints]*.5*(1.0+1.0/epsilon);
    }
  }



}

void MultPB::evaluateFMM(double* srcDensity, double* trgValue) {

  if (formulation == FORMULATION_ZERO_ION) {

    double * tmpTrg = new double[NumTrgPoints];

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }

    fs[0]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += kernelCoeff[0]*tmpTrg[i];
    }

  } else if (formulation == FORMULATION_NBIE) {

    // nBIE formulation
    double * tmpTrg = new double[NumTrgPoints];

    double * phiMol = srcDensity;
    double * dphiMol = &srcDensity[NumSrcPoints];

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[0]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += kernelCoeff[0]*tmpTrg[i];
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[1]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += kernelCoeff[1]*tmpTrg[i];
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[2]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += kernelCoeff[2]*tmpTrg[i];
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }

    fs[3]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += kernelCoeff[3]*tmpTrg[i];
    }

    // arand: added 5-25-2011, in progress
    //        5-26-2011, gives some plausible results now
    if (pbsolver->opt->useSternLayer) {
      
      double * phiStern = &srcDensity[2*NumSrcPoints];
      double * dphiStern = &srcDensity[2*NumSrcPoints+pbsolver->sternLayer.NumQuadPoints];

      for (int i=0; i<NumTrgPoints; i++) {
	tmpTrg[i] = 0.0;
      }
      fs[6]->fmmEval(phiStern,tmpTrg);
      for (int i=0; i<NumTrgPoints; i++) {
	trgValue[i+NumTrgPoints] += kernelCoeff[6]*tmpTrg[i];
      } 

      for (int i=0; i<NumTrgPoints; i++) {
	tmpTrg[i] = 0.0;
      }
      fs[7]->fmmEval(dphiStern,tmpTrg);
      for (int i=0; i<NumTrgPoints; i++) {
	trgValue[i+NumTrgPoints] += kernelCoeff[7]*tmpTrg[i];
      }

      for (int i=0; i<NumTrgPoints; i++) {
	tmpTrg[i] = 0.0;
      }
 
      double * tmpStern = new double[pbsolver->sternLayer.NumVertices];
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	tmpStern[i] = 0.0;
      }
      fs[4]->fmmEval(phiMol,tmpStern);
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	trgValue[i+NumTrgPoints*2] += kernelCoeff[4]*tmpStern[i];
      } 

      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	tmpStern[i] = 0.0;
      }
      fs[5]->fmmEval(dphiMol,tmpStern);
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	trgValue[i+NumTrgPoints*2] += kernelCoeff[5]*tmpStern[i];
      } 

      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	tmpStern[i] = 0.0;
      }
      fs[8]->fmmEval(phiStern,tmpStern);
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	trgValue[i+NumTrgPoints*2] += kernelCoeff[8]*tmpStern[i];
      } 

      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	tmpStern[i] = 0.0;
      }
      fs[9]->fmmEval(dphiStern,tmpStern);
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	trgValue[i+NumTrgPoints*2] += kernelCoeff[9]*tmpStern[i];
      }

      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	tmpStern[i] = 0.0;
      }
      fs[10]->fmmEval(phiStern,tmpStern);
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	trgValue[i+NumTrgPoints*2+pbsolver->sternLayer.NumVertices] += kernelCoeff[10]*tmpStern[i];
      }       

      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	tmpStern[i] = 0.0;
      }
      fs[11]->fmmEval(dphiStern,tmpStern);
      for (int i=0; i<pbsolver->sternLayer.NumVertices; i++) {
	trgValue[i+NumTrgPoints*2+pbsolver->sternLayer.NumVertices] += kernelCoeff[11]*tmpStern[i];
      }       

    }

    /*// debugging output
    cout << "Result" << endl;
    for (int i=0; i<NumTrgPoints; i++) {
      cout << trgValue[i] << " ";
    } cout << endl;
    cout << "X" << endl;
    for (int i=NumTrgPoints; i<2*NumTrgPoints; i++) {
      cout << trgValue[i] << " ";
    } cout << endl;
    cout << "Y" << endl;
    if (pbsolver->opt->useSternLayer) {
      for (int i=2*NumTrgPoints; i<2*NumTrgPoints+pbsolver->sternLayer.NumVertices; i++) {
	cout << trgValue[i] << " ";
      } cout << endl;
      cout << "Z" << endl;
      for (int i=2*NumTrgPoints+pbsolver->sternLayer.NumVertices; i<2*NumTrgPoints+2*pbsolver->sternLayer.NumVertices; i++) {
	cout << trgValue[i] << " ";
      } cout << endl;
    }
    */

    delete [] tmpTrg;
  } else if (formulation == FORMULATION_DBIE) { 

    // dBIE formulation
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] = 0.0;
      trgValue[i+NumTrgPoints] = 0.0;

    }

    double * tmpTrg = new double[NumTrgPoints];

    // 00 corner of the block matrix

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[0]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += 1.0*tmpTrg[i];
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[1]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += -1.0*epsilon*tmpTrg[i];
    }

    // 01 corner of the block matrix
    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[2]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += -1.0*tmpTrg[i];
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[3]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += 1.0*tmpTrg[i];
    }

    // 10 corner of the block matrix

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[4]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += 1.0*tmpTrg[i]*(*trgNor)(0,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[5]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += 1.0*tmpTrg[i]*(*trgNor)(1,i);

    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[6]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += 1.0*tmpTrg[i]*(*trgNor)(2,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[7]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += -1.0*tmpTrg[i]*(*trgNor)(0,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[8]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += -1.0*tmpTrg[i]*(*trgNor)(1,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[9]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += -1.0*tmpTrg[i]*(*trgNor)(2,i);
    }

    // 11 corner of the block matrix

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[10]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += -1.0*tmpTrg[i]*(*trgNor)(0,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[11]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += -1.0*tmpTrg[i]*(*trgNor)(1,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[12]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += -1.0*tmpTrg[i]*(*trgNor)(2,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[13]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += 1.0/epsilon*tmpTrg[i]*(*trgNor)(0,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[14]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += 1.0/epsilon*tmpTrg[i]*(*trgNor)(1,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[15]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += 1.0/epsilon*tmpTrg[i]*(*trgNor)(2,i);
    }

    delete [] tmpTrg;

  } else if (formulation == FORMULATION_DBIE2) {

    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] = 0.0;
      trgValue[i+NumTrgPoints] = 0.0;

    }

    double * tmpTrg = new double[NumTrgPoints];

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[0]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += tmpTrg[i];
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[1]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i] += tmpTrg[i];
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[2]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += tmpTrg[i]*(*trgNor)(0,i);
    }
    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[3]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += tmpTrg[i]*(*trgNor)(1,i);
    }
    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[4]->fmmEval(srcDensity,tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += tmpTrg[i]*(*trgNor)(2,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[5]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += tmpTrg[i]*(*trgNor)(0,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[6]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += tmpTrg[i]*(*trgNor)(1,i);
    }

    for (int i=0; i<NumTrgPoints; i++) {
      tmpTrg[i] = 0.0;
    }
    fs[7]->fmmEval(&srcDensity[NumSrcPoints],tmpTrg);
    for (int i=0; i<NumTrgPoints; i++) {
      trgValue[i+NumTrgPoints] += tmpTrg[i]*(*trgNor)(2,i);
    }

    delete [] tmpTrg;

  }

}


void MultPB::interpolateValuesAtQuadPoints( double* srcDensityAtVertices, double * srcDensityAtQuadPoints) {

  if (method == DISCRETE_COLLOCATION) {
    int count = 0;
    // interpolate values at quadrature points
    for (int i = 0; i < pbsolver->molSurf.aspline->numbtris; i++) {
      int t1,t2,t0;
      int tt1,tt2,tt0;
      tt0 = pbsolver->molSurf.aspline->m_Faces[i]->Index[0];
      tt1 = pbsolver->molSurf.aspline->m_Faces[i]->Index[1];
      tt2 = pbsolver->molSurf.aspline->m_Faces[i]->Index[2];
      t0 = pbsolver->molSurf.aspline->m_Faces[i]->IndexInRAW[0];
      t1 = pbsolver->molSurf.aspline->m_Faces[i]->IndexInRAW[1];
      t2 = pbsolver->molSurf.aspline->m_Faces[i]->IndexInRAW[2];
      
      for (int k = 0; k < pbsolver->molSurf.aspline->my_quadr->numOfPts_perTri; k++) {
	srcDensityAtQuadPoints[count] = 0.0;
	srcDensityAtQuadPoints[count] += srcDensityAtVertices[t0]*pbsolver->molSurf.aspline->my_quadr->xi1[k];
	srcDensityAtQuadPoints[count] += srcDensityAtVertices[t1]*pbsolver->molSurf.aspline->my_quadr->xi2[k];
	srcDensityAtQuadPoints[count] += srcDensityAtVertices[t2]*pbsolver->molSurf.aspline->my_quadr->xi3[k];
	
	
	srcDensityAtQuadPoints[count+NumSrcPoints] = 0.0;
	srcDensityAtQuadPoints[count+NumSrcPoints] += srcDensityAtVertices[t0+NumTrgPoints]*
	  pbsolver->molSurf.aspline->my_quadr->xi1[k];
	srcDensityAtQuadPoints[count+NumSrcPoints] += srcDensityAtVertices[t1+NumTrgPoints]*
	  pbsolver->molSurf.aspline->my_quadr->xi2[k];
	srcDensityAtQuadPoints[count+NumSrcPoints] += srcDensityAtVertices[t2+NumTrgPoints]*
	  pbsolver->molSurf.aspline->my_quadr->xi3[k];
	count++;
      }
    }
    // arand: added Stern layer 5-25-2011
    if (pbsolver->opt->useSternLayer) {
      count = 2*NumSrcPoints;
      for (int i = 0; i < pbsolver->sternLayer.aspline->numbtris; i++) {
	int t1,t2,t0;
	int tt1,tt2,tt0;
	tt0 = pbsolver->sternLayer.aspline->m_Faces[i]->Index[0];
	tt1 = pbsolver->sternLayer.aspline->m_Faces[i]->Index[1];
	tt2 = pbsolver->sternLayer.aspline->m_Faces[i]->Index[2];
	t0 = pbsolver->sternLayer.aspline->m_Faces[i]->IndexInRAW[0];
	t1 = pbsolver->sternLayer.aspline->m_Faces[i]->IndexInRAW[1];
	t2 = pbsolver->sternLayer.aspline->m_Faces[i]->IndexInRAW[2];
       
	
	for (int k = 0; k < pbsolver->sternLayer.aspline->my_quadr->numOfPts_perTri; k++) {
	  srcDensityAtQuadPoints[count] = 0.0;
	  srcDensityAtQuadPoints[count] += srcDensityAtVertices[t0+2*NumTrgPoints]*pbsolver->sternLayer.aspline->my_quadr->xi1[k];
	  srcDensityAtQuadPoints[count] += srcDensityAtVertices[t1+2*NumTrgPoints]*pbsolver->sternLayer.aspline->my_quadr->xi2[k];
	  srcDensityAtQuadPoints[count] += srcDensityAtVertices[t2+2*NumTrgPoints]*pbsolver->sternLayer.aspline->my_quadr->xi3[k];
	  
	  srcDensityAtQuadPoints[count+pbsolver->sternLayer.NumQuadPoints] = 0.0;
	  srcDensityAtQuadPoints[count+pbsolver->sternLayer.NumQuadPoints] += srcDensityAtVertices[t0+2*NumTrgPoints+pbsolver->sternLayer.NumVertices]*
	    pbsolver->sternLayer.aspline->my_quadr->xi1[k];
	  srcDensityAtQuadPoints[count+pbsolver->sternLayer.NumQuadPoints] += srcDensityAtVertices[t1+2*NumTrgPoints+pbsolver->sternLayer.NumVertices]*
	    pbsolver->sternLayer.aspline->my_quadr->xi2[k];
	  srcDensityAtQuadPoints[count+pbsolver->sternLayer.NumQuadPoints] += srcDensityAtVertices[t2+2*NumTrgPoints+pbsolver->sternLayer.NumVertices]*
	    pbsolver->sternLayer.aspline->my_quadr->xi3[k];
	  
	  count++;
	}
      }
     

      /* // output looks correct
      cout << "Int res" << endl;
      for (int i=0; i<NumSrcPoints; i++) {
	cout << srcDensityAtQuadPoints[i];
      }cout << endl;
      for (int i=NumSrcPoints;i<2*NumSrcPoints ; i++) {
	cout << srcDensityAtQuadPoints[i];
      }cout << endl;
      for (int i=2*NumSrcPoints;i<2*NumSrcPoints+pbsolver->sternLayer.NumQuadPoints; i++) {
	cout << srcDensityAtQuadPoints[i];
      }cout << endl;

      for (int i=2*NumSrcPoints+pbsolver->sternLayer.NumQuadPoints;i<2*NumSrcPoints+2*pbsolver->sternLayer.NumQuadPoints; i++) {
	cout << srcDensityAtQuadPoints[i];
      }cout << endl;
      */
 
    }

  } else if (method == DISCRETE_COLLOCATION_CONSTANT) {

    // note: srcDensityAtVertices is actually the coefficients of the p-w constant basis functions.

    int count = 0;
    for (int i = 0; i < pbsolver->molSurf.NumTriangles; i++) {
      for (int k = 0; k < pbsolver->molSurf.aspline->my_quadr->numOfPts_perTri; k++) {      
	srcDensityAtQuadPoints[count] = srcDensityAtVertices[i];
	srcDensityAtQuadPoints[count+NumSrcPoints] = srcDensityAtVertices[i+NumTrgPoints];
	count++;

      }
    }
  }
}
  
void MultPB::fixSingularQuadrature( double* srcDensityAtVertices, double * srcDensityAtQuadPoints, double * trgValue) {


  int nGauss = pbsolver->molSurf.aspline->my_quadr->numOfPts_perTri;
  
  
  double *tmpSrc = new double[nGauss];
  double *tmpSrcDeriv = new double[nGauss];
  int *tmpId = new int[nGauss];
    
  // loop over triangles...
  for (int i = 0; i < pbsolver->molSurf.aspline->numbtris; i++) {
    
    // get data at relevant quadrature points
    for (int q=0; q<nGauss; q++) {
      tmpSrc[q] = srcDensityAtQuadPoints[i*nGauss + q] *
	pbsolver->molSurf.aspline->m_Faces[i]->Node[q]->weight; // density times quadrature weight
      
      if (formulation != FORMULATION_ZERO_ION) {
	tmpSrcDeriv[q] = srcDensityAtQuadPoints[NumSrcPoints + i*nGauss + q] *
	  pbsolver->molSurf.aspline->m_Faces[i]->Node[q]->weight; // density times quadrature weight
	
      }
      
      tmpId[q] = i*nGauss + q;
      
    }
    

    int t1,t2,t0;
    int t[3];
    int nt;
    
    if (method == DISCRETE_COLLOCATION) {  
      t0 = pbsolver->molSurf.aspline->m_Faces[i]->IndexInRAW[0];
      t1 = pbsolver->molSurf.aspline->m_Faces[i]->IndexInRAW[1];
      t2 = pbsolver->molSurf.aspline->m_Faces[i]->IndexInRAW[2];
      
      t[0] = t0;
      t[1] = t1;
      t[2] = t2;
      
      nt = 3;
    } else if (method == DISCRETE_COLLOCATION_CONSTANT) {  
      t0 = i;
      t[0] = i;

      nt = 1;
    }
    
    // subtract off quadrature from FMM      
    for (int it=0; it<nt; it++) {
      
      
      if (formulation == FORMULATION_ZERO_ION) {
	
	trgValue[t[it]] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, t[it]);
	
      } else if (formulation == FORMULATION_NBIE) {
	
	trgValue[t[it]] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, t[it]);
	trgValue[t[it]] -= kernelCoeff[1]*fs[1]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[2]*fs[2]->manualEval(nGauss, tmpId, tmpSrc, t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[3]*fs[3]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it]);
	
      } else if (formulation == FORMULATION_DBIE) {
	
	trgValue[t[it]] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, t[it]);
	trgValue[t[it]] -= kernelCoeff[1]*fs[1]->manualEval(nGauss, tmpId, tmpSrc, t[it]);
	
	trgValue[t[it]] -= kernelCoeff[2]*fs[2]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it]);
	trgValue[t[it]] -= kernelCoeff[3]*fs[3]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it]);
	
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[4]*fs[4]->manualEval(nGauss, tmpId, tmpSrc, t[it])*(*trgNor)(0,t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[5]*fs[5]->manualEval(nGauss, tmpId, tmpSrc, t[it])*(*trgNor)(1,t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[6]*fs[6]->manualEval(nGauss, tmpId, tmpSrc, t[it])*(*trgNor)(2,t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[7]*fs[7]->manualEval(nGauss, tmpId, tmpSrc, t[it])*(*trgNor)(0,t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[8]*fs[8]->manualEval(nGauss, tmpId, tmpSrc, t[it])*(*trgNor)(1,t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[9]*fs[9]->manualEval(nGauss, tmpId, tmpSrc, t[it])*(*trgNor)(2,t[it]);
	
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[10]*fs[10]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it])*(*trgNor)(0,t[it]);	
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[11]*fs[11]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it])*(*trgNor)(1,t[it]);
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[12]*fs[12]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it])*(*trgNor)(2,t[it]);	
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[13]*fs[13]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it])*(*trgNor)(0,t[it]);	
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[14]*fs[14]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it])*(*trgNor)(1,t[it]);	
	trgValue[t[it]+NumTrgPoints] -= kernelCoeff[15]*fs[15]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it])*(*trgNor)(2,t[it]);
	
      }
      
    }
    
  }



  if (pbsolver->opt->useSternLayer) {
    
    // loop over triangles...
    for (int i = 0; i < pbsolver->sternLayer.aspline->numbtris; i++) {
      
      // get data at relevant quadrature points
      for (int q=0; q<nGauss; q++) {
	tmpSrc[q] = srcDensityAtQuadPoints[2*pbsolver->molSurf.NumQuadPoints +i*nGauss + q] *
	  pbsolver->sternLayer.aspline->m_Faces[i]->Node[q]->weight; // density times quadrature weight
	
	tmpSrcDeriv[q] = srcDensityAtQuadPoints[2*pbsolver->molSurf.NumQuadPoints + pbsolver->sternLayer.NumQuadPoints + i*nGauss + q] *
	  pbsolver->sternLayer.aspline->m_Faces[i]->Node[q]->weight; // density times quadrature weight       
	
	tmpId[q] = i*nGauss + q;
      }
    
      
      int t1,t2,t0;
      int t[3];
      int nt;
      
      if (method == DISCRETE_COLLOCATION) {  
	t0 = pbsolver->sternLayer.aspline->m_Faces[i]->IndexInRAW[0];
	t1 = pbsolver->sternLayer.aspline->m_Faces[i]->IndexInRAW[1];
	t2 = pbsolver->sternLayer.aspline->m_Faces[i]->IndexInRAW[2];
	
	t[0] = t0;
	t[1] = t1;
	t[2] = t2;
	
	nt = 3;
      }
    
      // subtract off quadrature from FMM      
      for (int it=0; it<nt; it++) {
	
	
	if (formulation == FORMULATION_NBIE) {
	  trgValue[t[it]+2*pbsolver->molSurf.NumVertices] -= kernelCoeff[8]*fs[8]->manualEval(nGauss, tmpId, tmpSrc, t[it]);
	  trgValue[t[it]+2*pbsolver->molSurf.NumVertices] -= kernelCoeff[9]*fs[9]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it]);
	  trgValue[t[it]+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices] -= kernelCoeff[10]*fs[10]->manualEval(nGauss, tmpId, tmpSrc, t[it]);
	  trgValue[t[it]+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices] -= kernelCoeff[11]*fs[11]->manualEval(nGauss, tmpId, tmpSrcDeriv, t[it]);	
	} 
	
      }
      
    }
 
  }



  if (method == DISCRETE_COLLOCATION) {  
    int nDuffy = pbsolver->molSurf.asplineDuffy[0]->my_quadr->numOfPts_perTri;
    double *tmpSrcDuffy = new double[nDuffy];
    double *tmpSrcDuffyDeriv = new double[nDuffy];
    
    for (int i = 0; i < pbsolver->molSurf.aspline->numbtris; i++) {
      int t1,t2,t0;
      int t[3];
      
      
      for (int d=0; d<3; d++) {
	
	
	DblNumMat duffyPos(3, nDuffy);
	DblNumMat duffyNor(3, nDuffy);
	
	t0 = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->IndexInRAW[0];
	t1 = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->IndexInRAW[1];
	t2 = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->IndexInRAW[2];
	
	
	for (int q=0; q<nDuffy; q++) {
	  duffyPos(0,q) = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->Point[0];
	  duffyPos(1,q) = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->Point[1];
	  duffyPos(2,q) = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->Point[2];
	  duffyNor(0,q) = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->Normal[0];
	  duffyNor(1,q) = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->Normal[1];
	  duffyNor(2,q) = pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->Normal[2];
	  
	  
	  // interpolate values at quadrature points...
	  tmpSrcDuffy[q] = srcDensityAtVertices[t0]*pbsolver->molSurf.asplineDuffy[d]->my_quadr->xi1[q] +
	    srcDensityAtVertices[t1]*pbsolver->molSurf.asplineDuffy[d]->my_quadr->xi2[q] +
	    srcDensityAtVertices[t2]*pbsolver->molSurf.asplineDuffy[d]->my_quadr->xi3[q];
	  
	  tmpSrcDuffy[q] *= pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->weight;
	  
	  
	  if (formulation != FORMULATION_ZERO_ION) {
	    tmpSrcDuffyDeriv[q] = srcDensityAtVertices[t0+NumTrgPoints]*pbsolver->molSurf.asplineDuffy[d]->my_quadr->xi1[q] +
	      srcDensityAtVertices[t1+NumTrgPoints]*pbsolver->molSurf.asplineDuffy[d]->my_quadr->xi2[q] +
	      srcDensityAtVertices[t2+NumTrgPoints]*pbsolver->molSurf.asplineDuffy[d]->my_quadr->xi3[q];
	    tmpSrcDuffyDeriv[q] *= pbsolver->molSurf.asplineDuffy[d]->m_Faces[i]->Node[q]->weight;
	  }
	}
	
	// singularity is always at t0... correct the t0 integral...
	// add back the subtracted integral...
	
	if (formulation == FORMULATION_ZERO_ION) {
	  
	  trgValue[t0] += kernelCoeff[0]*fs[0]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0);
	  
	} else if (formulation == FORMULATION_NBIE) {
	  
	  trgValue[t0] += kernelCoeff[0]*fs[0]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0);
	  trgValue[t0] += kernelCoeff[1]*fs[1]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[2]*fs[2]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[3]*fs[3]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0);
	  
	} else {
	  
	  trgValue[t0] += kernelCoeff[0]*fs[0]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0);
	  trgValue[t0] += kernelCoeff[1]*fs[1]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0);
	  
	  trgValue[t0] += kernelCoeff[2]*fs[2]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0);
	  trgValue[t0] += kernelCoeff[3]*fs[3]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0);
	  
	  trgValue[t0+NumTrgPoints] += kernelCoeff[4]*fs[4]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0)*(*trgNor)(0,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[5]*fs[5]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0)*(*trgNor)(1,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[6]*fs[6]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0)*(*trgNor)(2,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[7]*fs[7]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0)*(*trgNor)(0,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[8]*fs[8]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0)*(*trgNor)(1,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[9]*fs[9]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0)*(*trgNor)(2,t0);
	  
	  trgValue[t0+NumTrgPoints] += kernelCoeff[10]*fs[10]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0)*(*trgNor)(0,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[11]*fs[11]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0)*(*trgNor)(1,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[12]*fs[12]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0)*(*trgNor)(2,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[13]*fs[13]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0)*(*trgNor)(0,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[14]*fs[14]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0)*(*trgNor)(1,t0);
	  trgValue[t0+NumTrgPoints] += kernelCoeff[15]*fs[15]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0)*(*trgNor)(2,t0);   
	}
	
      }
      
    }
  } else if (method == DISCRETE_COLLOCATION_CONSTANT) {
    // NOT UPDATED FOR STERN LAYER
    int nDuffy = pbsolver->molSurf.asplineCenterDuffy->my_quadr->numOfPts_perTri;
    double *tmpSrcDuffy = new double[nDuffy];
    double *tmpSrcDuffyDeriv = new double[nDuffy];

    DblNumMat duffyPos(3, nDuffy);
    DblNumMat duffyNor(3, nDuffy);


    for (int i = 0; i < pbsolver->molSurf.aspline->numbtris; i++) {

      for (int q=0; q<nDuffy; q++) {
	duffyPos(0,q) = pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->Point[0];
	duffyPos(1,q) = pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->Point[1];
	duffyPos(2,q) = pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->Point[2];
	duffyNor(0,q) = pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->Normal[0];
	duffyNor(1,q) = pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->Normal[1];
	duffyNor(2,q) = pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->Normal[2];

	// interpolate values at quadrature points...
	tmpSrcDuffy[q] = srcDensityAtVertices[i];
	tmpSrcDuffy[q] *= pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->weight;

	if (formulation != FORMULATION_ZERO_ION) {
	  tmpSrcDuffyDeriv[q] = srcDensityAtVertices[i+NumTrgPoints];
	  tmpSrcDuffyDeriv[q] *= pbsolver->molSurf.asplineCenterDuffy->m_Faces[i]->Node[q]->weight;
	}

      }

      // add back the subtracted integral...
      
      if (formulation == FORMULATION_ZERO_ION) {
	
	trgValue[i] += kernelCoeff[0]*fs[0]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i);
	
      } else if (formulation == FORMULATION_NBIE) {
	
	trgValue[i] += kernelCoeff[0]*fs[0]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i);
	trgValue[i] += kernelCoeff[1]*fs[1]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i);
	trgValue[i+NumTrgPoints] += kernelCoeff[2]*fs[2]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i);
	trgValue[i+NumTrgPoints] += kernelCoeff[3]*fs[3]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i);
	
      } else {
	
	trgValue[i] += kernelCoeff[0]*fs[0]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i);
	trgValue[i] += kernelCoeff[1]*fs[1]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i);
	
	trgValue[i] += kernelCoeff[2]*fs[2]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i);
	trgValue[i] += kernelCoeff[3]*fs[3]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i);
	
	trgValue[i+NumTrgPoints] += kernelCoeff[4]*fs[4]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i)*(*trgNor)(0,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[5]*fs[5]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i)*(*trgNor)(1,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[6]*fs[6]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i)*(*trgNor)(2,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[7]*fs[7]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i)*(*trgNor)(0,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[8]*fs[8]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i)*(*trgNor)(1,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[9]*fs[9]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, i)*(*trgNor)(2,i);
	
	trgValue[i+NumTrgPoints] += kernelCoeff[10]*fs[10]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i)*(*trgNor)(0,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[11]*fs[11]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i)*(*trgNor)(1,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[12]*fs[12]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i)*(*trgNor)(2,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[13]*fs[13]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i)*(*trgNor)(0,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[14]*fs[14]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i)*(*trgNor)(1,i);
	trgValue[i+NumTrgPoints] += kernelCoeff[15]*fs[15]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, i)*(*trgNor)(2,i);   
      }

    }
  }




  if (pbsolver->opt->useSternLayer) {

    if (method == DISCRETE_COLLOCATION) {  
      int nDuffy = pbsolver->sternLayer.asplineDuffy[0]->my_quadr->numOfPts_perTri;
      double *tmpSrcDuffy = new double[nDuffy];
      double *tmpSrcDuffyDeriv = new double[nDuffy];
      
      for (int i = 0; i < pbsolver->sternLayer.aspline->numbtris; i++) {
	int t1,t2,t0;
	int t[3];
	
	for (int d=0; d<3; d++) {
	  	  
	  DblNumMat duffyPos(3, nDuffy);
	  DblNumMat duffyNor(3, nDuffy);
	  
	  t0 = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->IndexInRAW[0];
	  t1 = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->IndexInRAW[1];
	  t2 = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->IndexInRAW[2];
	  
	  
	  for (int q=0; q<nDuffy; q++) {
	    duffyPos(0,q) = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->Point[0];
	    duffyPos(1,q) = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->Point[1];
	    duffyPos(2,q) = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->Point[2];
	    duffyNor(0,q) = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->Normal[0];
	    duffyNor(1,q) = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->Normal[1];
	    duffyNor(2,q) = pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->Normal[2];
	    
	    
	    // interpolate values at quadrature points...
	    tmpSrcDuffy[q] = srcDensityAtVertices[t0+2*pbsolver->molSurf.NumVertices]*
	      pbsolver->sternLayer.asplineDuffy[d]->my_quadr->xi1[q] +
	      srcDensityAtVertices[t1+2*pbsolver->molSurf.NumVertices]*
	      pbsolver->sternLayer.asplineDuffy[d]->my_quadr->xi2[q] +
	      srcDensityAtVertices[t2+2*pbsolver->molSurf.NumVertices]*
	      pbsolver->sternLayer.asplineDuffy[d]->my_quadr->xi3[q];
	    
	    tmpSrcDuffy[q] *= pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->weight;	   	    

	    tmpSrcDuffyDeriv[q] = srcDensityAtVertices[t0+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices]*
	      pbsolver->sternLayer.asplineDuffy[d]->my_quadr->xi1[q] +
	      srcDensityAtVertices[t1+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices]*
	      pbsolver->sternLayer.asplineDuffy[d]->my_quadr->xi2[q] +
	      srcDensityAtVertices[t2+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices]*
	      pbsolver->sternLayer.asplineDuffy[d]->my_quadr->xi3[q];

	    tmpSrcDuffyDeriv[q] *= pbsolver->sternLayer.asplineDuffy[d]->m_Faces[i]->Node[q]->weight;

	  }
	  
	  // singularity is always at t0... correct the t0 integral...
	  // add back the subtracted integral...
	  
	  if (formulation == FORMULATION_NBIE) {
	    
	    trgValue[t0+2*pbsolver->molSurf.NumVertices] += kernelCoeff[8]*fs[8]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0);
	    trgValue[t0+2*pbsolver->molSurf.NumVertices] += kernelCoeff[9]*fs[9]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0);
	    trgValue[t0+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices] += 
	      kernelCoeff[10]*fs[10]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffy, t0);
	    trgValue[t0+2*pbsolver->molSurf.NumVertices+pbsolver->sternLayer.NumVertices] += 
	      kernelCoeff[11]*fs[11]->manualEval(nDuffy, duffyPos, duffyNor, tmpSrcDuffyDeriv, t0);
	    
	  } else {
	    // FUTURE: update this for DBIE
	  }
	  
	}
	
      }
    }
  }
    


  delete [] tmpSrc;
  delete [] tmpSrcDeriv;
  delete [] tmpId;



}


void MultPB::fixNearFieldQuadrature( double* srcDensityAtVertices, double * srcDensityAtQuadPoints, double * trgValue) {

  TriElementNS::TriElement * surface = pbsolver->molSurf.aspline;

  int nGauss = pbsolver->molSurf.aspline->my_quadr->numOfPts_perTri;
  int nSub = pbsolver->molSurf.asplineNearField->my_quadr->numOfPts_perTri;

  double *tmpSrc = new double[nGauss];
  double *tmpSrcDeriv = new double[nGauss];
  double *tmpSrcSub = new double[nSub];
  double *tmpSrcSubDeriv = new double[nSub];
  int *tmpId = new int[nGauss];
  
  if (method == DISCRETE_COLLOCATION) {
    
    int t0,t1,t2;
    
    // loop over each vertex
    for (int iV=0; iV<surface->vertexStar.size(); iV++) {
     
      set<int> * nearField = &(pbsolver->molSurf.asplineNearField->vertexNearField[iV]);
      
      for (set<int>::iterator sIt = nearField->begin(); 
	   sIt != nearField->end(); sIt++) {      
	
	int iT = *sIt;
	
	// get data at relevant quadrature points
	for (int q=0; q<nGauss; q++) {
	  tmpSrc[q] = srcDensityAtQuadPoints[iT*nGauss + q] *
	    pbsolver->molSurf.aspline->m_Faces[iT]->Node[q]->weight; // density times quadrature weight
	  
	  if (formulation != FORMULATION_ZERO_ION) {
	    tmpSrcDeriv[q] = srcDensityAtQuadPoints[NumSrcPoints + iT*nGauss + q] *
	      pbsolver->molSurf.aspline->m_Faces[iT]->Node[q]->weight; // density times quadrature weight
	    
	  }	
	  tmpId[q] = iT*nGauss + q;	
	}
	
	
	if (formulation == FORMULATION_ZERO_ION) {
	  
	  trgValue[iV] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, iV);
	  
	} else if (formulation == FORMULATION_NBIE) {
	  
	  trgValue[iV] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, iV);
	  trgValue[iV] -= kernelCoeff[1]*fs[1]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[2]*fs[2]->manualEval(nGauss, tmpId, tmpSrc, iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[3]*fs[3]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV);
	  
	} else if (formulation == FORMULATION_DBIE) {
	  
	  trgValue[iV] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, iV);
	  trgValue[iV] -= kernelCoeff[1]*fs[1]->manualEval(nGauss, tmpId, tmpSrc, iV);
	  
	  trgValue[iV] -= kernelCoeff[2]*fs[2]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV);
	  trgValue[iV] -= kernelCoeff[3]*fs[3]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV);
	  
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[4]*fs[4]->manualEval(nGauss, tmpId, tmpSrc, iV)*(*trgNor)(0,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[5]*fs[5]->manualEval(nGauss, tmpId, tmpSrc, iV)*(*trgNor)(1,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[6]*fs[6]->manualEval(nGauss, tmpId, tmpSrc, iV)*(*trgNor)(2,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[7]*fs[7]->manualEval(nGauss, tmpId, tmpSrc, iV)*(*trgNor)(0,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[8]*fs[8]->manualEval(nGauss, tmpId, tmpSrc, iV)*(*trgNor)(1,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[9]*fs[9]->manualEval(nGauss, tmpId, tmpSrc, iV)*(*trgNor)(2,iV);
	  
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[10]*fs[10]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV)*(*trgNor)(0,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[11]*fs[11]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV)*(*trgNor)(1,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[12]*fs[12]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV)*(*trgNor)(2,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[13]*fs[13]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV)*(*trgNor)(0,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[14]*fs[14]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV)*(*trgNor)(1,iV);
	  trgValue[iV+NumTrgPoints] -= kernelCoeff[15]*fs[15]->manualEval(nGauss, tmpId, tmpSrcDeriv, iV)*(*trgNor)(2,iV);
	  
	}
	
	// add new quadrature result...
	
	DblNumMat subPos(3, nSub);
	DblNumMat subNor(3, nSub);
	
	t0 = pbsolver->molSurf.asplineNearField->m_Faces[iT]->IndexInRAW[0];
	t1 = pbsolver->molSurf.asplineNearField->m_Faces[iT]->IndexInRAW[1];
	t2 = pbsolver->molSurf.asplineNearField->m_Faces[iT]->IndexInRAW[2];	
	
	for (int q=0; q<nSub; q++) {
	  subPos(0,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Point[0];
	  subPos(1,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Point[1];
	  subPos(2,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Point[2];
	  subNor(0,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Normal[0];
	  subNor(1,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Normal[1];
	  subNor(2,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Normal[2];
	  
	  // interpolate values at quadrature points...
	  tmpSrcSub[q] = srcDensityAtVertices[t0]*pbsolver->molSurf.asplineNearField->my_quadr->xi1[q] +
	    srcDensityAtVertices[t1]*pbsolver->molSurf.asplineNearField->my_quadr->xi2[q] +
	    srcDensityAtVertices[t2]*pbsolver->molSurf.asplineNearField->my_quadr->xi3[q];
	  
	  tmpSrcSub[q] *= pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->weight;
	  
	  
	  if (formulation != FORMULATION_ZERO_ION) {
	    tmpSrcSubDeriv[q] = srcDensityAtVertices[t0+NumTrgPoints]*pbsolver->molSurf.asplineNearField->my_quadr->xi1[q] +
	      srcDensityAtVertices[t1+NumTrgPoints]*pbsolver->molSurf.asplineNearField->my_quadr->xi2[q] +
	      srcDensityAtVertices[t2+NumTrgPoints]*pbsolver->molSurf.asplineNearField->my_quadr->xi3[q];
	    tmpSrcSubDeriv[q] *= pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->weight;
	  }
	}
	
	int loc;
	loc = iV;
	
	if (formulation == FORMULATION_ZERO_ION) {
	  
	  trgValue[loc] += kernelCoeff[0]*fs[0]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  
	} else if (formulation == FORMULATION_NBIE) {
	  
	  trgValue[loc] += kernelCoeff[0]*fs[0]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  trgValue[loc] += kernelCoeff[1]*fs[1]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[2]*fs[2]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[3]*fs[3]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  
	} else {
	  
	  trgValue[loc] += kernelCoeff[0]*fs[0]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  trgValue[loc] += kernelCoeff[1]*fs[1]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  
	  trgValue[loc] += kernelCoeff[2]*fs[2]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  trgValue[loc] += kernelCoeff[3]*fs[3]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  
	  trgValue[loc+NumTrgPoints] += kernelCoeff[4]*fs[4]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[5]*fs[5]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[6]*fs[6]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(2,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[7]*fs[7]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[8]*fs[8]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[9]*fs[9]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(2,loc);
	  
	  trgValue[loc+NumTrgPoints] += kernelCoeff[10]*fs[10]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[11]*fs[11]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[12]*fs[12]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(2,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[13]*fs[13]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[14]*fs[14]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[15]*fs[15]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(2,loc);   
	}
      }
    }
  } else if (method == DISCRETE_COLLOCATION_CONSTANT) {

    // warning: the case below is has not been thoroughly tested...

    for (int iT0=0; iT0<surface->numbtris; iT0++) {
      
      set<int> * nearField = &(pbsolver->molSurf.asplineNearField->vertexNearField[iT0]);
      
      for (set<int>::iterator sIt = nearField->begin(); 
	   sIt != nearField->end(); sIt++) {      
	
	int iT = *sIt;

	

	// get data at relevant quadrature points
	for (int q=0; q<nGauss; q++) {
	  tmpSrc[q] = srcDensityAtQuadPoints[iT*nGauss + q] *
	    pbsolver->molSurf.aspline->m_Faces[iT]->Node[q]->weight; // density times quadrature weight
	  
	  if (formulation != FORMULATION_ZERO_ION) {
	    tmpSrcDeriv[q] = srcDensityAtQuadPoints[NumSrcPoints + iT*nGauss + q] *
	      pbsolver->molSurf.aspline->m_Faces[iT]->Node[q]->weight; // density times quadrature weight
	    
	  }	
	  tmpId[q] = iT*nGauss + q;	
	}
	
	
	if (formulation == FORMULATION_ZERO_ION) {
	  
	  trgValue[iT0] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, iT0);
	  
	} else if (formulation == FORMULATION_NBIE) {
	  
	  trgValue[iT0] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, iT0);
	  trgValue[iT0] -= kernelCoeff[1]*fs[1]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[2]*fs[2]->manualEval(nGauss, tmpId, tmpSrc, iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[3]*fs[3]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0);
	  
	} else if (formulation == FORMULATION_DBIE) {
	  
	  trgValue[iT0] -= kernelCoeff[0]*fs[0]->manualEval(nGauss, tmpId, tmpSrc, iT0);
	  trgValue[iT0] -= kernelCoeff[1]*fs[1]->manualEval(nGauss, tmpId, tmpSrc, iT0);
	  
	  trgValue[iT0] -= kernelCoeff[2]*fs[2]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0);
	  trgValue[iT0] -= kernelCoeff[3]*fs[3]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0);
	  
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[4]*fs[4]->manualEval(nGauss, tmpId, tmpSrc, iT0)*(*trgNor)(0,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[5]*fs[5]->manualEval(nGauss, tmpId, tmpSrc, iT0)*(*trgNor)(1,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[6]*fs[6]->manualEval(nGauss, tmpId, tmpSrc, iT0)*(*trgNor)(2,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[7]*fs[7]->manualEval(nGauss, tmpId, tmpSrc, iT0)*(*trgNor)(0,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[8]*fs[8]->manualEval(nGauss, tmpId, tmpSrc, iT0)*(*trgNor)(1,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[9]*fs[9]->manualEval(nGauss, tmpId, tmpSrc, iT0)*(*trgNor)(2,iT0);
	  
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[10]*fs[10]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0)*(*trgNor)(0,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[11]*fs[11]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0)*(*trgNor)(1,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[12]*fs[12]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0)*(*trgNor)(2,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[13]*fs[13]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0)*(*trgNor)(0,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[14]*fs[14]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0)*(*trgNor)(1,iT0);
	  trgValue[iT0+NumTrgPoints] -= kernelCoeff[15]*fs[15]->manualEval(nGauss, tmpId, tmpSrcDeriv, iT0)*(*trgNor)(2,iT0);
	  
	}

	
	// add new quadrature result...
	
	DblNumMat subPos(3, nSub);
	DblNumMat subNor(3, nSub);
		
	for (int q=0; q<nSub; q++) {
	  subPos(0,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Point[0];
	  subPos(1,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Point[1];
	  subPos(2,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Point[2];
	  subNor(0,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Normal[0];
	  subNor(1,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Normal[1];
	  subNor(2,q) = pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->Normal[2];
	  
	  // interpolate values at quadrature points...
	  tmpSrcSub[q] = srcDensityAtVertices[iT];	  
	  tmpSrcSub[q] *= pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->weight;
	  	  
	  if (formulation != FORMULATION_ZERO_ION) {
	    tmpSrcSubDeriv[q] = srcDensityAtVertices[iT+NumTrgPoints];
	    tmpSrcSubDeriv[q] *= pbsolver->molSurf.asplineNearField->m_Faces[iT]->Node[q]->weight;
	  }
	}
	
	int loc;
	loc = iT0;
	
	if (formulation == FORMULATION_ZERO_ION) {
	  
	  trgValue[loc] += kernelCoeff[0]*fs[0]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  
	} else if (formulation == FORMULATION_NBIE) {
	  
	  trgValue[loc] += kernelCoeff[0]*fs[0]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  trgValue[loc] += kernelCoeff[1]*fs[1]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[2]*fs[2]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[3]*fs[3]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  
	} else {
	  
	  trgValue[loc] += kernelCoeff[0]*fs[0]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  trgValue[loc] += kernelCoeff[1]*fs[1]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc);
	  
	  trgValue[loc] += kernelCoeff[2]*fs[2]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  trgValue[loc] += kernelCoeff[3]*fs[3]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc);
	  
	  trgValue[loc+NumTrgPoints] += kernelCoeff[4]*fs[4]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[5]*fs[5]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[6]*fs[6]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(2,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[7]*fs[7]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[8]*fs[8]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[9]*fs[9]->manualEval(nSub, subPos, subNor, tmpSrcSub, loc)*(*trgNor)(2,loc);
	  
	  trgValue[loc+NumTrgPoints] += kernelCoeff[10]*fs[10]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[11]*fs[11]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[12]*fs[12]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(2,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[13]*fs[13]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(0,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[14]*fs[14]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(1,loc);
	  trgValue[loc+NumTrgPoints] += kernelCoeff[15]*fs[15]->manualEval(nSub, subPos, subNor, tmpSrcSubDeriv, loc)*(*trgNor)(2,loc);   
	}

      }
    }
  }

}


// FIXME: update this...
void MultPB::getCoeffMatrix( double* Amatrix ) {

  //arand update this...


  vector<vtx> vertices;
  vector<vtx> normals;

  for (int i = 0; i < NumQuadPoints; i++) {
    vtx pt( (*quadPos)(0,i),(*quadPos)(1,i),(*quadPos)(2,i));
    vtx nm( (*quadNor)(0,i),(*quadNor)(1,i),(*quadNor)(2,i));

    vertices.push_back(pt);
    normals.push_back(nm);

    //cout << i << " " << surface->m_Vertices[i]->Point[0]
    //   << " " << surface->m_Vertices[i]->Point[1]
    //	 << " " << surface->m_Vertices[i]->Point[2]
    //	 << " " << surface->m_Vertices[i]->Normal[0]
    //	 << " " << surface->m_Vertices[i]->Normal[1]
    //	 <<  " " << surface->m_Vertices[i]->Normal[2] << endl;


  }


  int matsize = NumQuadPoints*2;

  for (int i=0; i<matsize*matsize; i++) {
    Amatrix[i] = 0;
  }

  for (int i = 0; i < NumQuadPoints; i++)
    for (int j = 0; j < NumQuadPoints; j++) {

      if (i == j) {
        Amatrix[i*matsize+j] += .5*(1.0+epsilon);
        Amatrix[(i+NumQuadPoints)*matsize+j+NumQuadPoints] += .5*(1.0+1.0/epsilon);
      }

      Amatrix[i*matsize+j] += weight[j]*dG0(vertices[i],normals[i],
                                            vertices[j],normals[j],
                                            kappa, epsilon);


      Amatrix[i*matsize+j] += -1.0*epsilon*weight[j]*dGk(vertices[i],normals[i],
                              vertices[j],normals[j],
                              kappa, epsilon);



      Amatrix[i*matsize+j+NumQuadPoints] += weight[j]*Gk(vertices[i],normals[i],
                                            vertices[j],normals[j],
                                            kappa, epsilon);


      Amatrix[i*matsize+j+NumQuadPoints] += -1.0*weight[j]*G0(vertices[i],normals[i],
                                            vertices[j],normals[j],
                                            kappa, epsilon);

      Amatrix[(i+NumQuadPoints)*matsize+j] += weight[j]*ddG0(vertices[i],normals[i],
                                              vertices[j],normals[j],
                                              kappa, epsilon);

      Amatrix[(i+NumQuadPoints)*matsize+j] += -1.0*weight[j]*ddGk(vertices[i],normals[i],
                                              vertices[j],normals[j],
                                              kappa, epsilon);

      Amatrix[(i+NumQuadPoints)*matsize+j+NumQuadPoints] += 1.0/epsilon*weight[j]*dGkx(vertices[i],normals[i],
          vertices[j],normals[j],
          kappa, epsilon);

      Amatrix[(i+NumQuadPoints)*matsize+j+NumQuadPoints] += -1.0*weight[j]*dG0x(vertices[i],normals[i],
          vertices[j],normals[j],
          kappa, epsilon);

    }
}

