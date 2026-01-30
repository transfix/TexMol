#ifndef _PB2Linear_H_
#define _PB2Linear_H_

#include "PB2/PBconstant.h"
#include "PB2/PETScInterface.h"
#include "PDBParser/Atom.h"
#include "PB2/PBEnum.h"

#include "TriElement/trielement.h"
#include "PB2/PBOptions.h"
#include "Geometry/Geometry.h"


struct PBRawnAndQuadInfo {
  // data from rawn file
  int NumVertices;
  DblNumMat * vertPos;
  DblNumMat * vertNor;
  
  // for p-w constant basis collocation
  int NumTriangles;
  DblNumMat * centroidPos;
  DblNumMat * centroidNor;
  double * centroidWeight;

  int NumQuadPoints;
  DblNumMat * quadPos;
  DblNumMat * quadNor;
  double * quadWeight;
    
  vector <double> rawn_x;
  vector <double> rawn_y;
  vector <double> rawn_z;
  vector <double> rawn_nx;
  vector <double> rawn_ny;
  vector <double> rawn_nz;	
  vector <int> rawn_t1;
  vector <int> rawn_t2;
  vector <int> rawn_t3;
  vector <vector <int> > vert2tris;

  TriElementNS::TriElement* aspline;
  TriElementNS::TriElement* asplineCentroid;
  TriElementNS::TriElement* asplineDuffy[3];
  TriElementNS::TriElement* asplineNearField;
  TriElementNS::TriElement* asplineCenterDuffy;
};


class PBSolver{
 public:
	PBconstant* pb_para;
	PBconstant* setEnv(PBOptions* options);
	PetscErrorCode setDefaultOptions(PBOptions* options);
	PetscErrorCode getOptions(PBOptions* options);
	PetscErrorCode printOptions(PBOptions* options);
	// use octree-style input file
	bool getOptionsFromFile(PBOptions* options, char *paramFile ); 
	              
	PetscErrorCode setOptions(PBOptions* options, double ep_i, double ep_e, 
				  double temp, double ionConc, int res,
				  char * pqr_f,
				  char * mesh_f, char * pot_f, char * out_f, 
				  char * crd_f, char * vol_f, char * force_f);


	// wrapper functions for command-line (vs. TexMol) interface
	// these include typical i/o calls
	PetscErrorCode computePotentialCommandLine(PBOptions* options);
	PetscErrorCode computeEnergyCommandLine(PBOptions* options);
	PetscErrorCode computeForcesCommandLine(PBOptions* options);
	PetscErrorCode computeVolumePotentialCommandLine(PBOptions* options);
	PetscErrorCode computePotentialAtLocationCommandLine(PBOptions* options);

	PetscErrorCode computePotential(PBOptions* options);
	PetscErrorCode computeEnergy(PBOptions* options);
	PetscErrorCode computeForces(PBOptions* options);
	PetscErrorCode computeVolumePotential(PBOptions* options);
	PetscErrorCode computePotentialAtLocation(PBOptions* options);

	PetscErrorCode loadPQR(PBOptions* options);
	PetscErrorCode loadQuad(PBOptions* options);
	PetscErrorCode loadRawn(PBOptions* options);
	PetscErrorCode loadRawn(char * fname, PBRawnAndQuadInfo * surf);
	PetscErrorCode loadPot(PBOptions* options);

	PetscErrorCode computeQuad(PBOptions* options);
	PetscErrorCode computeQuad(char * fname, PBOptions* options, PBRawnAndQuadInfo* surf);

	void PrintBoundaryPotential(PBOptions * options, const char * fname, int n, Vec x);
	void PrintBoundaryPotential(PBOptions * options, const char * fname, int n, double * phi);
	void PrintRawnc(const char * fname, PBOptions * options, int n, Vec x);
	void PrintRawnc(const char * fname, PBOptions * options, int n, double * phi);


	void ComputeColors(PBOptions * options, int n, double * phiarray);
	Geometry * GetColoredSurface(PBOptions * options);

	void PrintLog(PBOptions* options);

	void setRHS(PBOptions * options, Vec & b, Vec & x);
	//void initialGuess(PBOptions * options, Vec & x);

	vector <PDBParser::Atom *> atomList;
	
	// potential computed on molecular surface
	double * phi;
	double * dphi;

	double * c_red;
	double * c_green;
	double * c_blue;

	// potential computed on ion exclusion boundary
	double * phiSL;
	double * dphiSL;

	double * c_redSL;
	double * c_greenSL;
	double * c_blueSL;


	/*
	// data from .quad file
	// or now computed with computeQuad
	int NumQuadPoints;
	DblNumMat * quadPos;
	DblNumMat * quadNor;
	double * quadWeight;

	// data from rawn file
	int NumVertices;
	DblNumMat * vertPos;
	DblNumMat * vertNor;

	// for p-w constant basis collocation
	int NumTriangles;
	DblNumMat * centroidPos;
	DblNumMat * centroidNor;
	double * centroidWeight;

	vector <double> rawn_x;
	vector <double> rawn_y;
	vector <double> rawn_z;
	vector <double> rawn_nx;
	vector <double> rawn_ny;
	vector <double> rawn_nz;	
	vector <int> rawn_t1;
	vector <int> rawn_t2;
	vector <int> rawn_t3;
	vector <vector <int> > vert2tris;

	TriElementNS::TriElement* aspline;
	TriElementNS::TriElement* asplineCentroid;
	TriElementNS::TriElement* asplineDuffy[3];
	TriElementNS::TriElement* asplineNearField;
	TriElementNS::TriElement* asplineCenterDuffy;

	*/

	PBRawnAndQuadInfo molSurf;
	PBRawnAndQuadInfo sternLayer;


	double solverTime;
	double totalEnergy;

	PBOptions * opt;

};

#endif
