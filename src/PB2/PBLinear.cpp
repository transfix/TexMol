
#include <TexMol/cvcapp.h>
#include <cvc/volume.h>
#include <cvc/volume_file_io.h>

#include "PB2/PBLinear.h"
#include "PB2/PETScInterface.h"
#include "PB2/MultPB.h"
#include "time.h"
#include "GOAFileTypes/GOALoader.h"
#include "PDBParser/FlattenGOA.h"
#include "PDBParser/Atom.h"

#include <TriElement/aspline.h>
#include <TriElement/geometry.h>
#include <TriElement/geometryParser.h>

#include <PB2/PB2Util.h>
#include <PB2/endians.h>

#include <MolEnergyUtil/utils.h>
#include <TexMol/compat.h>


// e_c*e_c*n_a*10^10/epsilon_0
#define TOJOULESPERMOLE 17459143.4556854

// e_c*e_c*n_a*10^10/epsilon_0/1000/4.184
#define TOKCALPERMOLE 4172.83543395922


using namespace std;

//#define TEST_SIZE 1000


MultPB* mult_fmm2;


void printRawnWithQuadrature(TriElementNS::TriElement* aspline, const char* fname) {


  int numQuadPts = aspline->m_Faces[0]->Node.size();

  ofstream fout(fname);

  fout << aspline->m_Vertices.size()+aspline->m_Faces.size()*numQuadPts << " " << aspline->m_Faces.size() << endl;
  // vertices loop
  for(int i = 0; i < aspline->m_Vertices.size(); i++) {
    double P[3], N[3], C[3];
    for(int j = 0; j < 3; j++) {
      P[j] = aspline->m_Vertices[i]->Point[j];
      N[j] = aspline->m_Vertices[i]->Normal[j];
    }
    C[0] = aspline->m_Vertices[i]->Color[0];
    C[1] = aspline->m_Vertices[i]->Color[1];
    C[2] = aspline->m_Vertices[i]->Color[2];
    if(aspline->normalFlag == true && aspline->colorFlag == true) {
      //fprintf(fp, "%f %f %f %f %f %f %f %f %f\n", P[0],P[1],P[2],N[0],N[1],N[2],C[0],C[1],C[2]);
    } else if(aspline->normalFlag == true && aspline->colorFlag == false) {
      fout << P[0] << " " << P[1] << " " << P[2] << " "
           << N[0] << " " << N[1] << " " << N[2] << endl;
      //fprintf(fp, "%f %f %f %f %f %f\n", P[0],P[1],P[2],N[0],N[1],N[2]);
    } else if(aspline->normalFlag == false && aspline->colorFlag == true) {
      //fprintf(fp, "%f %f %f %f %f %f\n", P[0],P[1],P[2],C[0],C[1],C[2]);
    } else if(aspline->normalFlag == false && aspline->colorFlag == false) {
      //fprintf(fp, "%f %f %f\n", P[0],P[1],P[2]);
    }
  }
  // quad points... not part of any triangles
  for(int i = 0; i < aspline->m_Faces.size(); i++) {
    for(int j=0; j<aspline->m_Faces[i]->Node.size(); j++) {
      double P[3], N[3], C[3];
      for(int k = 0; k < 3; k++) {
        P[k] = aspline->m_Faces[i]->Node[j]->Point[k];
        N[k] = aspline->m_Faces[i]->Node[j]->Normal[k];
      }
      C[0] = 1.0;
      C[1] = 1.0;
      C[2] = 1.0;
      if(aspline->normalFlag == true && aspline->colorFlag == true) {
        //fprintf(fp, "%f %f %f %f %f %f %f %f %f\n", P[0],P[1],P[2],N[0],N[1],N[2],C[0],C[1],C[2]);
      } else if(aspline->normalFlag == true && aspline->colorFlag == false) {
        fout << P[0] << " " << P[1] << " " << P[2] << " "
             << N[0] << " " << N[1] << " " << N[2] << endl;
        //fprintf(fp, "%f %f %f %f %f %f\n", P[0],P[1],P[2],N[0],N[1],N[2]);
      } else if(aspline->normalFlag == false && aspline->colorFlag == true) {
        //fprintf(fp, "%f %f %f %f %f %f\n", P[0],P[1],P[2],C[0],C[1],C[2]);
      } else if(aspline->normalFlag == false && aspline->colorFlag == false) {
        //fprintf(fp, "%f %f %f\n", P[0],P[1],P[2]);
      }
    }
  }
  // face index loop
  for(int i = 0; i < aspline->m_Faces.size(); i++) {
    int index[3];
    for(int j = 0; j < 3; j++) {
      index[j] = aspline->m_Faces[i]->IndexInRAW[j];
    }
    //fprintf(fp, "%d %d %d\n", index[0], index[1], index[2]);
    fout << index[0] << " " << index[1] << " " << index[2] << endl;

    //cout << i << " " << index[0] << " " << index[1] << " " << index[2] << endl;
    //for(int t = 0; t < aspline->m_Faces[i]->subFacets.size(); t++)
    //  {
    //
    // }
  }
}


extern double getTime();

PBconstant* PBSolver::setEnv(PBOptions* options) {

  PrintHeading("setEnv()");
  const int numIon = 2;

  double ionQ[numIon];
  double ionConc[numIon];
  double ionRadii[numIon];

  ionQ[0] = 1.0;
  ionConc[0] = options->ionConcentration;
  ionRadii[0] = 2.0;

  ionQ[1] = -1.0;
  ionConc[1] = options->ionConcentration;
  ionRadii[1] = 2.0;
  double solventDie = options->epsilon_e;
  double soluteDie = options->epsilon_i;
  double temperature = options->temperature;
  PBconstant * pbc = new PBconstant(solventDie, soluteDie ,temperature, numIon, &ionQ[0], &ionConc[0],
                        &ionRadii[0]);

  PrintValue("kappa (Debye-Huckel screening factor)", pbc->xkappa);
  
  PrintValue("epsilon (dielectric ratio)",pbc->sdie/pbc->pdie);

  if (pbc->xkappa <10e-11) {
    cout << "Zero Ion Concentration: Using FORMULTION_ZERO_ION and DISCRETE_COLLOCATION" << endl;
    options->formulation = FORMULATION_ZERO_ION;
    options->discreteMethod = DISCRETE_COLLOCATION;
  }

  // set pb_para to point to env setup.
  pb_para = pbc;

  return pbc;
}

PetscErrorCode PBSolver::setOptions(PBOptions* options, double ep_i, double ep_e, 
				    double temp, double ionConc, int res,
				    char * pqr_f,
				    char * mesh_f, char * pot_f, char * out_f, 
				    char * crd_f, char * vol_f, char * force_f) {
  options->epsilon_i = ep_i;
  options->epsilon_e = ep_e;
  options->temperature = temp;
  //options->temperature = 300;
  options->ionConcentration = ionConc;
  options->resolution = res;
  options->solverType = LIN_SOLVE_GMRES; // GMRES
  options->formulation = FORMULATION_DBIE; // Juffer's formulation
  options->discreteMethod = DISCRETE_NYSTROM; // Nystrom method
  options->geometryType = GEOMETRY_ASPLINE; // C^1 molecular surface
  options->quadLevel = 1;
  options->quadOrderFarField = 3;
  options->quadOrderNearField = 5;
  options->quadOrderSingular = 3;
  options->accuracy = 6;

  if (pqr_f != NULL)
    sprintf(options->pqr_fname, "%s", pqr_f);

  if (mesh_f != NULL)
    sprintf(options->mesh_fname, "%s", mesh_f);

  if (pot_f != NULL)
    sprintf(options->pot_fname, "%s", pot_f);

  if (out_f != NULL)
    sprintf(options->out_fname, "%s", out_f);

  if (crd_f != NULL) 
    sprintf(options->crd_fname, "%s", crd_f);

  if (vol_f != NULL)
    sprintf(options->vol_fname, "%s", vol_f);

  if (force_f != NULL)
    sprintf(options->force_fname, "%s", force_f);

}


bool PBSolver::getOptionsFromFile(PBOptions* options, char *paramFile )
{
  char s[ 2000 ];
  char key[ 500 ], val[ 500 ];
  FILE *fp;
  
  fp = fopen( paramFile, "r" );
  if ( fp == NULL ) {
    printError( (char *) "Failed to open parameter file %s!", paramFile );
    return false;
  }
  
  setDefaultOptions(options);

  bool foundPQR=false, foundRAWN=false;

  while ( fgets( s, 1999, fp ) != NULL ) {
    if ( sscanf( s, "%s %s", key, val ) != 2 ) continue;
    
    if ( !strcasecmp( key, "pqrFile" ) ) {
      strcpy(options->pqr_fname,val);
      foundPQR = true;
    } else if ( !strcasecmp( key, "rawnFile" ) ) {
      strcpy(options->mesh_fname,val);
      foundRAWN = true;
    } else if ( !strcasecmp( key, "sternLayerRawnFile" ) ) {
      strcpy(options->stern_fname,val);
      options->useSternLayer = true;
    }
    else if ( !strcasecmp( key, "crdFile" ) ) {
      strcpy(options->crd_fname,val);
    }
    else if ( !strcasecmp( key, "sdfFile" ) ) {
      strcpy(options->sdf_fname,val);
      options->haveSDF = true;
    }
    else if ( !strcasecmp( key, "outputPrefix" ) ) strcpy(options->out_fname,val);
    else if ( !strcasecmp( key, "potentialFile" ) ) strcpy(options->pot_fname,val);
    else if ( !strcasecmp( key, "computePotential" ) ) options->computePotential=stringToBool(val);
    else if ( !strcasecmp( key, "rawncOutput" ) ) options->rawncOutput=stringToBool(val);
    else if ( !strcasecmp( key, "computeEnergy" ) ) options->computeEnergy=stringToBool(val);
    else if ( !strcasecmp( key, "computeForces" ) ) options->computeForces=stringToBool(val);
    else if ( !strcasecmp( key, "computeVolumePotential" ) ) options->computeVolumePotential=stringToBool(val);
    else if ( !strcasecmp( key, "computePotentialAtLocation" ) ) options->computePotentialAtLocation=stringToBool(val);
    else if ( !strcasecmp( key, "computePotentialAtAtoms" ) ) options->computePotentialAtAtoms=stringToBool(val);
    else if ( !strcasecmp( key, "ionConc" ) ) options->ionConcentration = atof(val);
    else if ( !strcasecmp( key, "epsilonE" ) ) options->epsilon_e = atof(val);
    else if ( !strcasecmp( key, "epsilonI" ) ) options->epsilon_i = atof(val);
    else if ( !strcasecmp( key, "temperature" ) ) options->temperature = atof(val);
    else if ( !strcasecmp( key, "quadLevel" ) ) options->quadLevel = atoi(val);
    else if ( !strcasecmp( key, "quadOrderFarField" ) ) options->quadOrderFarField = atoi(val);
    else if ( !strcasecmp( key, "quadOrderNearField" ) ) options->quadOrderNearField = atoi(val);
    else if ( !strcasecmp( key, "solvationVolumePotentialOnly" ) ) options->sovationVolumePotentialOnly = atoi(val);
    else if ( !strcasecmp( key, "quadOrderSingular" ) ) options->quadOrderSingular = atoi(val);
    else if ( !strcasecmp( key, "fmmAccuracy" ) ) options->accuracy = atoi(val);
    else if ( !strcasecmp( key, "rawivOutputResolution" ) ) options->resolution = atoi(val);
    else if ( !strcasecmp( key, "rawivOutputResolutionX" ) ) options->resolutionX = atoi(val);
    else if ( !strcasecmp( key, "rawivOutputResolutionY" ) ) options->resolutionY = atoi(val);
    else if ( !strcasecmp( key, "rawivOutputResolutionZ" ) ) options->resolutionZ = atoi(val);
    else if ( !strcasecmp( key, "rawivOutputGridSizeX" ) ) options->gridSizeX = atof(val);
    else if ( !strcasecmp( key, "rawivOutputGridSizeY" ) ) options->gridSizeY = atof(val);
    else if ( !strcasecmp( key, "rawivOutputGridSizeZ" ) ) options->gridSizeZ = atof(val);
    else if ( !strcasecmp( key, "rawivOutputCoordX" ) ) options->bboxX = atof(val);
    else if ( !strcasecmp( key, "rawivOutputCoordY" ) ) options->bboxY = atof(val);
    else if ( !strcasecmp( key, "rawivOutputCoordZ" ) ) options->bboxZ = atof(val);
    
    else if ( !strcasecmp( key, "dxForce" ) ) options->dxForce = atof(val);
    else if ( !strcasecmp( key, "linearSolver" ) ) {      
      if (val[0] == 'G' || val[0] == 'g') { // GMRES iterative
	options->solverType = LIN_SOLVE_GMRES;
      } else if (val[0] == 'L' || val[0] == 'l') { // explicit LU solver
	options->solverType = LIN_SOLVE_LU;
      }
    } else if ( !strcasecmp( key, "discretizationMethod" ) ) {
      if (val[0] == 'N' || val[0] == 'n') { //Nystrom method
	options->discreteMethod = DISCRETE_NYSTROM;
	cout << "WARNING: Nystrom method doesn't always work." << endl;
      } else if (val[0] == 'C' || val[0] == 'c') { // collocation method, p-w linear
	options->discreteMethod = DISCRETE_COLLOCATION;
      } else if (val[0] == '2') { // collocation method, p-w constant
	options->discreteMethod = DISCRETE_COLLOCATION_CONSTANT;
      }     
    } else if ( !strcasecmp( key, "bieFormulation" ) ) {
      if (val[0] == 'N' || val[0] == 'n') {
	options->formulation = FORMULATION_NBIE;
      } else if (val[0] == 'D' || val[0] == 'd') { // Juffer's formulation
	options->formulation = FORMULATION_DBIE;
      } else if (val[0] == '2' || val[0] == '2') { // Juffer's formulation again
	options->formulation = FORMULATION_DBIE2;
	cout << "WARNING: 2DBIE formulation doesn't work." << endl;
      }
    } else if ( !strcasecmp( key, "geometry" ) ) {
      if (val[0] == 'A' || val[0] == 'a') { // C^1 surface
	options->geometryType = GEOMETRY_ASPLINE;
      } else if (val[0] == 'L' || val[0] == 'l') { // C^0 surface
	options->geometryType = GEOMETRY_LINEAR;
      }
    }
  }

  fclose( fp );
    
  if (!foundPQR) { 
    printError( (char *) "Missing PQR file!" );
    return false;
  }

  if (!foundRAWN)  { 
    printError( (char *) "Missing RAWN file!" );
    return false;
  }





  printOptions(options);


  setEnv(options);

  return true;
}

PetscErrorCode PBSolver::setDefaultOptions(PBOptions* options) {
  PetscFunctionBegin;

  opt = options;

  // default values
  options->epsilon_i = 1.0;
  options->epsilon_e = 80.0;
  options->temperature = 298.15;
  //options->temperature = 300;
  options->ionConcentration = 0.15;
  options->solverType = LIN_SOLVE_GMRES; // GMRES
  options->formulation = FORMULATION_DBIE; // Juffer's formulation
  options->discreteMethod = DISCRETE_COLLOCATION; // Collocation method
  options->geometryType = GEOMETRY_ASPLINE; // C^1 molecular surface
  options->quadLevel = 1;
  options->quadOrderFarField = 3;
  options->quadOrderNearField = 5;
  options->quadOrderSingular = 3;
  options->accuracy = 6;
  options->dxForce = 0.05;

  strcpy(options->out_fname,"result");
  strcpy(options->vol_fname,"");
  strcpy(options->vol2_fname,"");
  strcpy(options->force_fname,"");


  options->computePotential = false;
  options->computeEnergy=false;
  options->computeForces=false;
  options->computeVolumePotential=false;
  options->computePotentialAtLocation=false;
  options->computePotentialAtAtoms=false;

  options->rawncOutput=true;

  options->useSternLayer=false;

  options->resolution = 16;
  options->resolutionX = -1;
  options->resolutionY = -1;
  options->resolutionZ = -1;
  
  options->bboxX = 0.0;
  options->bboxY = 0.0;
  options->bboxZ = 0.0;
  
  options->gridSizeX = -1;
  options->gridSizeY = -1;
  options->gridSizeZ = -1;
  
  options->sovationVolumePotentialOnly = false;
  
  options->haveSDF = false;
  
  PetscFunctionReturn(0);
}



// fixme: remove print statements and call printOptions
PetscErrorCode PBSolver::getOptions(PBOptions* options) {
  PrintHeading("getOptions()");

  cout << "WARNING: old commandline interface may no longer be supported..." << endl;

  PetscBool flg; // petsc 3.2
  //PetscTruth flg; // petsc 3.1
  PetscFunctionBegin;

  setDefaultOptions(options);

  char tmp[PETSC_MAX_PATH_LEN-1];

  PetscOptionsGetString(PETSC_NULL,"-rawn",options->mesh_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-quad",options->quad_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-pot",options->pot_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-pqr",options->pqr_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-out",options->out_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-crd",options->crd_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-vol",options->vol_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-vol2",options->vol2_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-vol3",options->vol3_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-vol4",options->vol4_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-force",options->force_fname,PETSC_MAX_PATH_LEN-1,&flg);
  PetscOptionsGetString(PETSC_NULL,"-solver",tmp,PETSC_MAX_PATH_LEN-1,&flg);


  string tmpPrint;
  
  tmpPrint = "GMRES";
  if (tmp[0] == 'G' || tmp[0] == 'g') { // GMRES iterative
    options->solverType = LIN_SOLVE_GMRES;
  } else if (tmp[0] == 'L' || tmp[0] == 'l') { // explicit LU solver
    options->solverType = LIN_SOLVE_LU;
    tmpPrint = "LU";
  }
  PrintValue("Linear Solver", tmpPrint);


  tmp[0] = 0;
  PetscOptionsGetString(PETSC_NULL,"-formulation",tmp,PETSC_MAX_PATH_LEN-1,&flg);

  tmpPrint = "DBIE";
  if (tmp[0] == 'N' || tmp[0] == 'n') {
    options->formulation = FORMULATION_NBIE;
    tmpPrint = "NBIE";
  } else if (tmp[0] == 'D' || tmp[0] == 'd') { // Juffer's formulation
    options->formulation = FORMULATION_DBIE;
    tmpPrint = "DBIE";
  } else if (tmp[0] == '2' || tmp[0] == '2') { // Juffer's formulation again
    options->formulation = FORMULATION_DBIE2;
    tmpPrint = "2DBIE";
    cout << "WARNING: 2DBIE formulation doesn't work." << endl;
    cout << "WARNING: 2DBIE formulation doesn't work." << endl;
    cout << "WARNING: 2DBIE formulation doesn't work." << endl;
  }
  PrintValue("Boundary Integral Formulation", tmpPrint);


  tmpPrint = "Collocation";
  tmp[0] = 0;
  PetscOptionsGetString(PETSC_NULL,"-method",tmp,PETSC_MAX_PATH_LEN-1,&flg);

  if (tmp[0] == 'N' || tmp[0] == 'n') { //Nystrom method
    options->discreteMethod = DISCRETE_NYSTROM;
    tmpPrint = "Nystrom";
    cout << "WARNING: Nystrom method doesn't always work." << endl;
  } else if (tmp[0] == 'C' || tmp[0] == 'c') { // collocation method, p-w linear
    options->discreteMethod = DISCRETE_COLLOCATION;
    tmpPrint = "Collocation, Linear Basis";
  } else if (tmp[0] == '2') { // collocation method, p-w constant
    options->discreteMethod = DISCRETE_COLLOCATION_CONSTANT;
    tmpPrint = "Collocation, Constant Basis";
  }
  PrintValue("Discretization Method", tmpPrint);

  tmpPrint = "ASpline";
  tmp[0] = 0;
  PetscOptionsGetString(PETSC_NULL,"-geometry",tmp,PETSC_MAX_PATH_LEN-1,&flg);

  if (tmp[0] == 'A' || tmp[0] == 'a') { // C^1 surface
    options->geometryType = GEOMETRY_ASPLINE;
    tmpPrint = "ASpline";
  } else if (tmp[0] == 'L' || tmp[0] == 'l') { // C^0 surface
    options->geometryType = GEOMETRY_LINEAR;
    tmpPrint = "Linear";
  }
  PrintValue("Geometry Type", tmpPrint);


  PetscOptionsGetInt(PETSC_NULL,"-quadLevel",&options->quadLevel,&flg);
  PrintValue("Singular Quadrature Size", options->quadLevel);

  PetscOptionsGetInt(PETSC_NULL,"-quadOrderFarField",&options->quadOrderFarField,&flg);
  PrintValue("Far Field Quadrature Order", options->quadOrderFarField);
  PetscOptionsGetInt(PETSC_NULL,"-quadOrderNearField",&options->quadOrderNearField,&flg);
  PrintValue("Near Field Quadrature Order", options->quadOrderNearField);
  PetscOptionsGetInt(PETSC_NULL,"-quadOrderSingular",&options->quadOrderSingular,&flg);
  PrintValue("Singular Quadrature Order", options->quadOrderSingular);


  PetscOptionsGetInt(PETSC_NULL,"-fmmAccuracy",&options->accuracy,&flg);
  PrintValue("Fast Multipole Accuracy", options->accuracy);

  PetscOptionsGetInt(PETSC_NULL,"-resolution",&options->resolution,&flg);
  PetscOptionsGetReal(PETSC_NULL,"-epsiloni",&options->epsilon_i,&flg);
  PrintValue("Interior Dielectric", options->epsilon_i);
  PetscOptionsGetReal(PETSC_NULL,"-epsilone",&options->epsilon_e,&flg);
  PrintValue("Exterior Dielectric", options->epsilon_e);
  PetscOptionsGetReal(PETSC_NULL,"-ion",&options->ionConcentration,&flg);
  PrintValue("Ion Concentration", options->ionConcentration);
  PetscOptionsGetReal(PETSC_NULL,"-temp",&options->temperature,&flg);
  PrintValue("Temperature", options->temperature);

  setEnv(options);

  PetscFunctionReturn(0);
}

PetscErrorCode PBSolver::printOptions(PBOptions* options) {
  PrintValue("Surface Potential Computation",options->computePotential);
  PrintValue("Energy Computation",options->computeEnergy);
  PrintValue("Force Computation",options->computeForces);
  PrintValue("External Potential Computation",options->computeVolumePotential);


  string tmpPrint = "DBIE";
  if (options->formulation == FORMULATION_NBIE) {
    tmpPrint = "NBIE";
  } else if (options->formulation == FORMULATION_DBIE) {
    tmpPrint = "DBIE";
  } else if (options->formulation == FORMULATION_DBIE2) {
    tmpPrint = "2DBIE";
  }
  PrintValue("Boundary Integral Formulation", tmpPrint);

  tmpPrint = "Collocation";
  if (options->discreteMethod == DISCRETE_NYSTROM) {
    tmpPrint = "Nystrom";
  } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
    tmpPrint = "Collocation, Linear Basis";
  } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
    tmpPrint = "Collocation, Constant Basis";
  }
  PrintValue("Discretization Method", tmpPrint);

  tmpPrint = "ASpline";
  if (options->geometryType == GEOMETRY_ASPLINE) {
    tmpPrint = "ASpline";
  } else if (options->geometryType == GEOMETRY_LINEAR) {
    tmpPrint = "Linear";
  }
  PrintValue("Geometry Type", tmpPrint);


  tmpPrint = "GMRES";
  if (options->solverType == LIN_SOLVE_GMRES) {
  } else if (options->solverType == LIN_SOLVE_LU) {
    tmpPrint = "LU";
  }
  PrintValue("Linear Solver", tmpPrint);

  PrintValue("PQR File",options->pqr_fname);
  PrintValue("RAWN File",options->mesh_fname);
  if (options->useSternLayer) {
    PrintValue("Stern Layer RAWN File",options->stern_fname);
  }
  PrintValue("Potential File",options->pot_fname);
  PrintValue("Output Prefix",options->out_fname);

  PrintValue("Singular Quadrature Size", options->quadLevel);
  PrintValue("Far Field Quadrature Order", options->quadOrderFarField);
  PrintValue("Near Field Quadrature Order", options->quadOrderNearField);
  PrintValue("Singular Quadrature Order", options->quadOrderSingular);
  PrintValue("Fast Multipole Accuracy", options->accuracy);
  PrintValue("Interior Dielectric", options->epsilon_i);
  PrintValue("Exterior Dielectric", options->epsilon_e);
  PrintValue("Ion Concentration", options->ionConcentration);
  PrintValue("Temperature", options->temperature);
  PrintValue("Volume Resolution", options->resolution);

  PrintValue("haveSDF", options->haveSDF);

  PetscFunctionReturn(0);
}

void PBSolver::PrintLog(PBOptions * options) {

  string fname = options->out_fname + (string)".log";
  ofstream fout(fname.c_str());

  PrintValue(fout,"Number of GMRES Iterations: ",mult_fmm2->NumIterations);
  PrintValue(fout, "Solver RunTime: ", solverTime);


}



void PBSolver::ComputeColors(PBOptions * options, int n, double * phiarray) {


  vector <double> phiVal;
  vector <double> phiArea;

  int NumTri = molSurf.NumTriangles;
  int NumVert = molSurf.NumVertices;

  phiVal.resize(NumVert);
  phiArea.resize(NumVert);

  for (int i=0; i<NumVert; i++) {
    phiVal[i] = 0.0;
    phiArea[i] = 0.0;
  }
  
  int mycount = 0;
  double mymax = -1000000000.0;
  double mymin = 1000000000.0;


  if (options->discreteMethod == DISCRETE_NYSTROM) {

    int QuadPerTri = molSurf.NumQuadPoints/NumTri;

    for (int i=0; i<NumTri; i++) {
      for (int j=0; j<QuadPerTri; j++) {

        phiVal[molSurf.rawn_t1[i]]+=phiarray[mycount]*molSurf.quadWeight[mycount];
        phiVal[molSurf.rawn_t2[i]]+=phiarray[mycount]*molSurf.quadWeight[mycount];
        phiVal[molSurf.rawn_t3[i]]+=phiarray[mycount]*molSurf.quadWeight[mycount];
        phiArea[molSurf.rawn_t1[i]] += molSurf.quadWeight[mycount];
        phiArea[molSurf.rawn_t2[i]] += molSurf.quadWeight[mycount];
        phiArea[molSurf.rawn_t3[i]] += molSurf.quadWeight[mycount];
        mycount++;
      }
    }

    for (int i=0; i<NumVert; i++) {
      if (phiArea[i] != 0.0) {
        phiVal[i] = phiVal[i]/phiArea[i];
      } else {
        phiVal[i] = 0.0;
      }
    }


  } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {

    int QuadPerTri = 1;

    for (int i=0; i<NumTri; i++) {
      for (int j=0; j<QuadPerTri; j++) {

        phiVal[molSurf.rawn_t1[i]]+=phiarray[mycount]*molSurf.centroidWeight[mycount];
        phiVal[molSurf.rawn_t2[i]]+=phiarray[mycount]*molSurf.centroidWeight[mycount];
        phiVal[molSurf.rawn_t3[i]]+=phiarray[mycount]*molSurf.centroidWeight[mycount];
        phiArea[molSurf.rawn_t1[i]] += molSurf.centroidWeight[mycount];
        phiArea[molSurf.rawn_t2[i]] += molSurf.centroidWeight[mycount];
        phiArea[molSurf.rawn_t3[i]] += molSurf.centroidWeight[mycount];
        mycount++;
      }
    }

    for (int i=0; i<NumVert; i++) {
      if (phiArea[i] != 0.0) {
        phiVal[i] = phiVal[i]/phiArea[i];
      } else {
        phiVal[i] = 0.0;
      }
    }

  } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
    for (int i=0; i<NumVert; i++) {
      phiVal[i] = phiarray[i];
    }

  }

  for (int i=0; i<NumVert; i++) {
    if (phiVal[i] > mymax) {
      mymax = phiVal[i];
    }

    if (phiarray[i] < mymin) {
      mymin = phiVal[i];
    }
  }

  if (-1.0*mymin > mymax) {
    mymax = -1.0*mymin;
  }

  mymax *= .6; // this is also kind of a hack..
  
  // arand hack:
  cout << "WARNING: color hack in place on output." << endl;
  //mymax = 0.0038;
  //mymax = 0.001;
  mymax = 0.0005;

  c_red = new double[NumVert];
  c_blue = new double[NumVert];
  c_green = new double[NumVert];


  for (int i=0; i<NumVert; i++) {
    double red = 0.0;
    double blue= 0.0;
    double green = 0.0;

    if (0 && phiArea[i] == 0) {
      cout << "WARNING: 0 area patch in PrintRawnc.  This is known to occur when passing a .raw file instead of a .rawn file." << endl;
      red = 1.0;
      blue = 1.0;
      green = 1.0;
    } else {

      // red negative, blue positive
      if (phiVal[i] < 0) {

        blue = 1.0 - pow(-phiVal[i]/mymax,0.5);
	if (blue < 0.0) blue = 0.0;
        green = blue;
        red = .5 + blue/2.0;

      } else {

        red = 1.0 - pow(phiVal[i]/mymax,0.5);
	if (red < 0.0) red = 0.0;
        green = red;
	blue = .5 + red/2.0;

      }
    }

    if (red < 0.0) {
      red = 0.0;
    }
    if (green < 0.0) {
      green = 0.0;
    }
    if (blue < 0.0) {
      blue = 0.0;
    }
    c_red[i] = red;
    c_green[i] = green;
    c_blue[i] = blue;    
  }

}

void PBSolver::PrintRawnc(const char * fname1, PBOptions * options, int n, Vec phi) {
  PetscScalar *phiarray;
  VecGetArray(phi,&phiarray);
  PrintRawnc(fname1, options, n, phi);

}


void PBSolver::PrintRawnc(const char * fname1, PBOptions * options, int n, double * phiarray) {

  ComputeColors(options, n, phiarray);

  // print the rawnc file...
  string fname;
  if (fname1 == NULL) {
    fname = options->out_fname + (string)".rawnc";
    fname1 = fname.c_str();
  }
  PrintValue("Rawnc File Name",fname1);
  //PrintValue("Rawnc Scale Factor",mymax);

  ofstream fout(fname1);
  fout << molSurf.NumVertices << " " << molSurf.NumTriangles << endl;

  for (int i=0; i<molSurf.NumVertices; i++) {
    fout << molSurf.rawn_x[i] << " " << molSurf.rawn_y[i] << " " << molSurf.rawn_z[i] << " "
         << molSurf.rawn_nx[i] << " " << molSurf.rawn_ny[i] << " " << molSurf.rawn_nz[i] << " "
         << c_red[i] << " " << c_green[i] << " " << c_blue[i] << endl;
  }

  for (int i=0; i<molSurf.NumTriangles; i++) {
    fout << molSurf.rawn_t1[i] << " " << molSurf.rawn_t2[i] << " " << molSurf.rawn_t3[i] << endl;
  }
}

/* old code
void PBSolver::PrintRawnc(const char * fname1, PBOptions * options, int n, double * phiarray) {
  // read .rawn file
  vector <double> x;
  vector <double> y;
  vector <double> z;
  vector <double> nx;
  vector <double> ny;
  vector <double> nz;
  vector <double> phiVal;
  vector <double> phiArea;

  vector <int> t1;
  vector <int> t2;
  vector <int> t3;

  int NumVert, NumTri;

  ifstream fin(options->mesh_fname);
  fin >> NumVert >> NumTri;

  for (int i=0; i<NumVert; i++) {
    double xx,yy,zz;
    fin >> xx >> yy >> zz;
    x.push_back(xx);
    y.push_back(yy);
    z.push_back(zz);
    fin >> xx >> yy >> zz;
    nx.push_back(xx);
    ny.push_back(yy);
    nz.push_back(zz);
    phiVal.push_back(0.0);
    phiArea.push_back(0.0);
  }

  for (int i=0; i<NumTri; i++) {
    int a,b,c;
    fin >> a >> b >> c;
    t1.push_back(a);
    t2.push_back(b);
    t3.push_back(c);

  }

  //cout << "Numbers " << QuadPerTri << " " << NumQuadPoints << " " << NumTri << endl;

  int mycount = 0;
  double mymax = -1000000000.0;
  double mymin = 1000000000.0;


  if (options->discreteMethod == DISCRETE_NYSTROM) {

    int QuadPerTri = NumQuadPoints/NumTri;

    for (int i=0; i<NumTri; i++) {
      for (int j=0; j<QuadPerTri; j++) {

        phiVal[t1[i]]+=phiarray[mycount]*quadWeight[mycount];
        phiVal[t2[i]]+=phiarray[mycount]*quadWeight[mycount];
        phiVal[t3[i]]+=phiarray[mycount]*quadWeight[mycount];
        phiArea[t1[i]] += quadWeight[mycount];
        phiArea[t2[i]] += quadWeight[mycount];
        phiArea[t3[i]] += quadWeight[mycount];
        mycount++;
      }
    }

    for (int i=0; i<NumVertices; i++) {
      if (phiArea[i] != 0.0) {
        phiVal[i] = phiVal[i]/phiArea[i];
      } else {
        phiVal[i] = 0.0;
      }
    }


  } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {

    int QuadPerTri = 1;

    for (int i=0; i<NumTri; i++) {
      for (int j=0; j<QuadPerTri; j++) {

        phiVal[t1[i]]+=phiarray[mycount]*centroidWeight[mycount];
        phiVal[t2[i]]+=phiarray[mycount]*centroidWeight[mycount];
        phiVal[t3[i]]+=phiarray[mycount]*centroidWeight[mycount];
        phiArea[t1[i]] += centroidWeight[mycount];
        phiArea[t2[i]] += centroidWeight[mycount];
        phiArea[t3[i]] += centroidWeight[mycount];
        mycount++;
      }
    }

    for (int i=0; i<NumVertices; i++) {
      if (phiArea[i] != 0.0) {
        phiVal[i] = phiVal[i]/phiArea[i];
      } else {
        phiVal[i] = 0.0;
      }
    }

  } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
    for (int i=0; i<NumVertices; i++) {
      phiVal[i] = phiarray[i];
    }

  }

  for (int i=0; i<NumVertices; i++) {
    if (phiVal[i] > mymax) {
      mymax = phiVal[i];
    }

    if (phiarray[i] < mymin) {
      mymin = phiVal[i];
    }
  }


  //cout << "max/min " << mymax << " " << mymin << endl;

  if (-1.0*mymin > mymax) {
    mymax = -1.0*mymin;
  }

  //cout << mymax << endl;
  mymax *= .6; // this is also kind of a hack..
  
  // arand hack:
  cout << "WARNING: color hack in place on output." << endl;
  //mymax = 0.0038;
  //mymax = 0.001;
  mymax = 0.0005;

  // print the rawnc file...

  string fname;
  if (fname1 == NULL) {
    fname = options->out_fname + (string)".rawnc";
    fname1 = fname.c_str();
  }

  PrintValue("Rawnc File Name",fname1);
  PrintValue("Rawnc Scale Factor",mymax);


  ofstream fout(fname1);
  fout << NumVert << " " << NumTri << endl;
  for (int i=0; i<NumVert; i++) {
    double red = 0.0;
    double blue= 0.0;
    double green = 0.0;

    if (0 && phiArea[i] == 0) {
      cout << "WARNING: 0 area patch in PrintRawnc.  This is known to occur when passing a .raw file instead of a .rawn file." << endl;
      red = 1.0;
      blue = 1.0;
      green = 1.0;
    } else {

      // red negative, blue positive
      if (phiVal[i] < 0) {

	//cout << phiarray[i] << " " << phiVal[i] << " " << mymax << endl;

	//red = 1.0;
        blue = 1.0 - pow(-phiVal[i]/mymax,0.5);

	if (blue < 0.0) blue = 0.0;

        green = blue;

        red = .5 + blue/2.0;

        //red = phiVal[i]/mymax;

      } else {
        //blue = -1.0*phiVal[i]/mymax;
        //blue = 1.0;
        red = 1.0 - pow(phiVal[i]/mymax,0.5);

	if (red < 0.0) red = 0.0;

        green = red;

	blue = .5 + red/2.0;
      }
    }

    if (red < 0.0) {
      red = 0.0;
    }
    if (green < 0.0) {
      green = 0.0;
    }
    if (blue < 0.0) {
      blue = 0.0;
    }

    fout << x[i] << " " << y[i] << " " << z[i] << " "
         << nx[i] << " " << ny[i] << " " << nz[i] << " "
         << red << " " << green << " " << blue << endl;
  }

  for (int i=0; i<NumTri; i++) {
    fout << t1[i] << " " << t2[i] << " " << t3[i] << endl;
  }

}
*/



void PBSolver::PrintBoundaryPotential(PBOptions * options, const char * fname, int n, Vec x) {
  PetscScalar *xarray;
  VecGetArray(x,&xarray);
  PrintBoundaryPotential(options, fname, n, xarray);
  VecRestoreArray(x,&xarray);
}

void PBSolver::PrintBoundaryPotential(PBOptions * options, const char * fname, int n, double * xarray) {

  ofstream fout(fname);
  for (int i=0; i<n; i++) {
    if (options->formulation == FORMULATION_ZERO_ION) {
      fout << xarray[i]  << endl;
    } else {
      fout << xarray[i] << " " << xarray[i+n] << endl;
    }
  }
}

/*
// old function, unused...
void PBSolver::initialGuess(PBOptions * options, Vec & x) {

  PetscScalar* xarray;
  VecCreate(PETSC_COMM_WORLD,&x);
  VecSetSizes(x,PETSC_DECIDE,molSurf.NumQuadPoints*2);
  VecSetFromOptions(x);
  VecGetArray(x,&xarray);

  for (int i=0; i<NumQuadPoints; i++) {
    xarray[i] = 0.0;
    xarray[i+NumQuadPoints] = 0.0;
  }

  VecRestoreArray(x,&xarray);


}
*/

// load pqr and surface mesh before running this...
void PBSolver::setRHS(PBOptions * options, Vec & b, Vec & x) {

  PrintHeading("Setup Right Hand Side");

  unsigned int NumAtoms = atomList.size();
  PrintValue("Number of Atoms",NumAtoms);

  DblNumMat * atomPos = new DblNumMat(3, NumAtoms);
  DblNumMat * atomNor = new DblNumMat(3, NumAtoms);
  double * weight = new double[NumAtoms];
  double * srcden = new double[NumAtoms];

  for (int i=0; i<NumAtoms; i++) {
    (*atomPos)(0,i) = atomList[i]->m_Position[0];
    (*atomPos)(1,i) = atomList[i]->m_Position[1];
    (*atomPos)(2,i) = atomList[i]->m_Position[2];
    (*atomNor)(0,i) = 0.0;
    (*atomNor)(1,i) = 0.0;
    (*atomNor)(2,i) = 0.0;

    //cout << "Atom " << i << ": " << atomList[i]->m_Position[0] << " "
    //	 << atomList[i]->m_Position[1] << " " << atomList[i]->m_Position[2] << " "
    //	 << atomList[i]->getCharge() << endl;

    srcden[i] = atomList[i]->getCharge();
    weight[i] = 1.0/pb_para->pdie;
  }

  double coef[2];
  coef[0] = pb_para->xkappa;
  coef[1] = pb_para->sdie/pb_para->pdie;




  int NumTrg = 0;
  DblNumMat * trgPos;
  DblNumMat * trgNor;


  if (options->discreteMethod == DISCRETE_NYSTROM) {// Nystrom method: unknowns at quad points

    // not: nystrom is no longer supported and probably broken

    trgPos = molSurf.quadPos;
    trgNor = molSurf.quadNor;
    NumTrg = molSurf.NumQuadPoints;

  } else if (options->discreteMethod == DISCRETE_COLLOCATION) { // collocation method
    // knowns are coefficients of p-w linear basis functions

    // read the rawn file and duplicate Nystrom case using mesh vertices
    // as the target locations

    molSurf.vertPos = new DblNumMat(3, molSurf.NumVertices);
    molSurf.vertNor = new DblNumMat(3, molSurf.NumVertices);

    for (int i=0; i<molSurf.NumVertices; i++) {
      (*molSurf.vertPos)(0,i) = molSurf.aspline->m_Vertices[i]->Point[0];
      (*molSurf.vertPos)(1,i) = molSurf.aspline->m_Vertices[i]->Point[1];
      (*molSurf.vertPos)(2,i) = molSurf.aspline->m_Vertices[i]->Point[2];
      (*molSurf.vertNor)(0,i) = molSurf.aspline->m_Vertices[i]->Normal[0];
      (*molSurf.vertNor)(1,i) = molSurf.aspline->m_Vertices[i]->Normal[1];
      (*molSurf.vertNor)(2,i) = molSurf.aspline->m_Vertices[i]->Normal[2];
    }

    trgPos = molSurf.vertPos;
    trgNor = molSurf.vertNor;
    NumTrg = molSurf.NumVertices;

    if (options->useSternLayer) {
      sternLayer.vertPos = new DblNumMat(3, sternLayer.NumVertices);
      sternLayer.vertNor = new DblNumMat(3, sternLayer.NumVertices);
      
      for (int i=0; i<sternLayer.NumVertices; i++) {
	(*sternLayer.vertPos)(0,i) = sternLayer.aspline->m_Vertices[i]->Point[0];
	(*sternLayer.vertPos)(1,i) = sternLayer.aspline->m_Vertices[i]->Point[1];
	(*sternLayer.vertPos)(2,i) = sternLayer.aspline->m_Vertices[i]->Point[2];
	(*sternLayer.vertNor)(0,i) = sternLayer.aspline->m_Vertices[i]->Normal[0];
	(*sternLayer.vertNor)(1,i) = sternLayer.aspline->m_Vertices[i]->Normal[1];
	(*sternLayer.vertNor)(2,i) = sternLayer.aspline->m_Vertices[i]->Normal[2];
      }
      
      //trgPos = sternLayer.vertPos;
      //trgNor = sternLayer.vertNor;
      //NumTrg += sternLayer.NumVertices;
    }
        
  } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) { // collocation method, p-w constant
    trgPos = molSurf.centroidPos;
    trgNor = molSurf.centroidNor;
    NumTrg = molSurf.NumTriangles;
  }

  PetscScalar* barray;
  VecCreate(PETSC_COMM_WORLD,&b);

  if (options->formulation == FORMULATION_ZERO_ION) {
    VecSetSizes(b,PETSC_DECIDE,NumTrg);
  } else {
    if (!options->useSternLayer) {
      VecSetSizes(b,PETSC_DECIDE,molSurf.NumVertices*2);
    } else {
      VecSetSizes(b,PETSC_DECIDE,molSurf.NumVertices*2+sternLayer.NumVertices*2);
    }
  }

  VecSetFromOptions(b);
  VecGetArray(b,&barray);


  KiFmmWrapper * fmm1 = new KiFmmWrapper(NumAtoms, NumTrg, atomPos,atomNor,
                                         trgPos,trgNor, weight,KNL_LAP_S_U,coef,options->accuracy);

  double * trgval = new double[NumTrg];
  for (int i=0; i<NumTrg; i++) {
    trgval[i] = 0.0;
  }

  fmm1->fmmEval(srcden,trgval);

  for (int i=0; i<NumTrg; i++) {
    barray[i] = trgval[i];
    if (options->formulation != FORMULATION_ZERO_ION) {
      barray[i+NumTrg] = 0.0;
    }
  }
  if (options->useSternLayer) {
    for (int i=0; i<sternLayer.NumVertices*2; i++) {
      barray[i+2*NumTrg] = 0.0;
    }
  }


  if (options->formulation == FORMULATION_DBIE || options->formulation == FORMULATION_DBIE2) {

    KiFmmWrapper * fmm2 = new KiFmmWrapper(NumAtoms, NumTrg, atomPos,atomNor,
                                           trgPos,trgNor, weight,KNL_LAP_DX_U,coef,options->accuracy);

    for (int i=0; i<NumTrg; i++) {
      trgval[i] = 0.0;
    }

    fmm2->fmmEval(srcden,trgval);

    for (int i=0; i<NumTrg; i++) {
      barray[i+NumTrg] += trgval[i]*(*trgNor)(0,i);
    }

    KiFmmWrapper * fmm3 = new KiFmmWrapper(NumAtoms, NumTrg, atomPos,atomNor,
                                           trgPos,trgNor, weight,KNL_LAP_DY_U,coef,options->accuracy);

    for (int i=0; i<NumTrg; i++) {
      trgval[i] = 0.0;
    }

    fmm3->fmmEval(srcden,trgval);

    for (int i=0; i<NumTrg; i++) {
      barray[i+NumTrg] += trgval[i]*(*trgNor)(1,i);
    }

    KiFmmWrapper * fmm4 = new KiFmmWrapper(NumAtoms, NumTrg, atomPos,atomNor,
                                           trgPos,trgNor, weight,KNL_LAP_DZ_U,coef,options->accuracy);

    for (int i=0; i<NumTrg; i++) {
      trgval[i] = 0.0;
    }

    fmm4->fmmEval(srcden,trgval);

    for (int i=0; i<NumTrg; i++) {
      barray[i+NumTrg] += trgval[i]*(*trgNor)(2,i);
    }
  }


  PetscScalar* xarray;
  VecCreate(PETSC_COMM_WORLD,&x);
  if (options->formulation == FORMULATION_ZERO_ION) {
    VecSetSizes(x,PETSC_DECIDE,NumTrg);
  } else {
    if (!options->useSternLayer) {
      VecSetSizes(x,PETSC_DECIDE,molSurf.NumVertices*2);
    } else {
      VecSetSizes(x,PETSC_DECIDE,molSurf.NumVertices*2+sternLayer.NumVertices*2);
    }
  }
  VecSetFromOptions(x);
  VecGetArray(x,&xarray);

  for (int i=0; i<NumTrg; i++) {
    //xarray[i] = -1.0*barray[i];
    xarray[i] = barray[i]*2.0/(1.0+coef[1]);

    if (options->formulation != FORMULATION_ZERO_ION) {
      xarray[i+NumTrg] = barray[i+NumTrg];
    }
  }
  if (options->useSternLayer) {
    for (int i=0; i<sternLayer.NumVertices*2; i++) {
      xarray[i+2*NumTrg] = 0.0; // FUTURE: precompute a better starting value...
    }
  }

  VecRestoreArray(x,&xarray);
  VecRestoreArray(b,&barray);


}

PetscErrorCode PBSolver::computeVolumePotential(PBOptions* options) {

  PrintHeading("computeVolumePotential()");

  cout << "computeVolumePotential()" << endl;

  loadPQR(options);
  cout << "PQR Loaded" << endl;
  
  computeQuad(options->mesh_fname,options,&molSurf);
  cout << "Quadrature Points Computed" << endl;

  loadPot(options);
  cout << "Potential Loaded" << endl;

  cvc::volume inputVol(cvcapp);

  if (options->haveSDF) {

    cout << "Loading Signed Distance Function from " << options->sdf_fname << endl;
    try {

      cvc::readVolumeFile(cvcapp, inputVol, options->sdf_fname);

    } catch(cvc::exception &e) {
      std:: cerr << e.what() << std::endl;
    } catch(std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

    cout << "Using SDF Dimensions to determine grid..." << endl;
    // if we have an sdf, use it to determine the grid!
    options->resolutionX = inputVol.XDim();
    options->resolutionY = inputVol.YDim();
    options->resolutionZ = inputVol.ZDim();

    options->gridSizeX = inputVol.XSpan();
    options->gridSizeY = inputVol.YSpan();
    options->gridSizeZ = inputVol.ZSpan();
    
    options->bboxX = inputVol.boundingBox().minx;
    options->bboxY = inputVol.boundingBox().miny;
    options->bboxZ = inputVol.boundingBox().minz;
  }


  // set up the grid...
  if (options->resolutionX == -1 || options->resolutionY == -1 ||
      options->resolutionY == -1) {
    // use the simple interface
    options->resolutionX = options->resolution;
    options->resolutionY = options->resolution;
    options->resolutionZ = options->resolution;

    // compute bounding box and set up

    double bbox = 0.0;
    double center[3];

    double bboxfactor = 2.0;
  
    for (int i=0; i<3; i++) {
      center[i] = (molSurf.aspline->max[i] + molSurf.aspline->min[i])/2.0;
      double len = (molSurf.aspline->max[i] - molSurf.aspline->min[i])/2.0;
      if (len > bbox) {
	bbox = len;
      }
    }
    
    options->gridSizeX = 2.0*bboxfactor*bbox/options->resolution;
    options->gridSizeY = 2.0*bboxfactor*bbox/options->resolution;
    options->gridSizeZ = 2.0*bboxfactor*bbox/options->resolution;
    
    options->bboxX = center[0] -bboxfactor*bbox;
    options->bboxY = center[1] -bboxfactor*bbox;
    options->bboxZ = center[2] -bboxfactor*bbox;

    cout << "Computed BBox: Size " << bbox 
	 << " Center (" << center[0] << "," << center[1] << "," << center[2] << ")" << endl;

  } 

  PrintValue("Bounding Box: X Dim", options->resolutionX);
  PrintValue("Bounding Box: Y Dim", options->resolutionY);
  PrintValue("Bounding Box: Z Dim", options->resolutionZ);
  PrintValue("Bounding Box: X Min", options->bboxX);
  PrintValue("Bounding Box: Y Min", options->bboxY);
  PrintValue("Bounding Box: Z Min", options->bboxZ);
  PrintValue("Bounding Box: X Span", options->gridSizeX);
  PrintValue("Bounding Box: Y Span", options->gridSizeY);
  PrintValue("Bounding Box: Z Span", options->gridSizeZ);

   

  float rawivmin[3], rawivmmax[3], rawivorigin[3], rawivspan[3];
  unsigned int rawivdim[3];

  rawivdim[0] = (unsigned int) options->resolutionX;
  rawivdim[1] = (unsigned int) options->resolutionY;
  rawivdim[2] = (unsigned int) options->resolutionZ;

  rawivmin[0] = (float) options->bboxX;
  rawivmin[1] = (float) options->bboxY;
  rawivmin[2] = (float) options->bboxZ;

  rawivspan[0] = (float) options->gridSizeX;
  rawivspan[1] = (float) options->gridSizeY;
  rawivspan[2] = (float) options->gridSizeZ;

  for(int i=0; i<3; i++)
    {
      //rawivdim[i] = (unsigned int) gridSize;
      //rawivmin[i] = (float) center[i] -bboxfactor*bbox;
      //rawivspan[i] = (float) h;
      rawivorigin[i] = rawivmin[i];
      rawivmmax[i] = rawivmin[i]+ (float)(rawivdim[i]-1)*rawivspan[i];
    }

  unsigned int rawivnumVerts = rawivdim[0]*rawivdim[1]*rawivdim[2];
  unsigned int rawivnumCells = (rawivdim[0]-1)*(rawivdim[1]-1)*(rawivdim[2]-1);

  if(!big_endian())
    {
      for( int i=0; i<3; i++) SWAP_32(&(rawivmin[i]));
      for(int i=0; i<3; i++) SWAP_32(&(rawivmmax[i]));
      SWAP_32(&(rawivnumVerts));
      SWAP_32(&(rawivnumCells));
      for(int i=0; i<3; i++) SWAP_32(&(rawivdim[i]));
      for(int i=0; i<3; i++) SWAP_32(&(rawivorigin[i]));
      for(int i=0; i<3; i++) SWAP_32(&(rawivspan[i]));
    }
 
  unsigned int NumGridCells = options->resolutionX*options->resolutionY*options->resolutionZ;

  DblNumMat * gridPos = new DblNumMat(3, NumGridCells);
  DblNumMat * gridNor = NULL;

  int counter = 0;
  for (int i=0; i<options->resolutionX; i++) {
    for (int j=0; j<options->resolutionY; j++) {
      for (int k=0; k<options->resolutionZ; k++) {
	(*gridPos)(0,counter) = options->bboxX + i*options->gridSizeX;
	(*gridPos)(1,counter) = options->bboxY + j*options->gridSizeY;
	(*gridPos)(2,counter) = options->bboxZ + k*options->gridSizeZ; 
	  counter++;
      }
    }
  }

  // compute phi_rxn at grid points...
  double coef[2];
  coef[0] = pb_para->xkappa;
  double kappa = coef[0];
  coef[1] = pb_para->sdie/pb_para->pdie;
  double epsilon = coef[1];

  double * trgval = new double[NumGridCells];
  for (int i=0; i<NumGridCells; i++) {
    trgval[i] = 0.0;
  }

  double * tmpval = new double[NumGridCells];

  for (int i=0; i<NumGridCells; i++) {
    tmpval[i] = 0.0;
  }
  KiFmmWrapper * fmm1 = new KiFmmWrapper(molSurf.NumQuadPoints, NumGridCells,
                                         molSurf.quadPos,molSurf.quadNor,gridPos,gridNor, molSurf.quadWeight,KNL_LAP_D_U,coef,options->accuracy);
  fmm1->fmmEval(phi,tmpval);

  for (int i=0; i<NumGridCells; i++) {
    trgval[i] += -1.0*tmpval[i];
    tmpval[i] = 0.0;
  }

  KiFmmWrapper * fmm2 = new KiFmmWrapper(molSurf.NumQuadPoints, NumGridCells,
                                         molSurf.quadPos,molSurf.quadNor,gridPos,gridNor, molSurf.quadWeight,KNL_BOZ_D_U,coef,options->accuracy);
  fmm2->fmmEval(phi,tmpval);

  for (int i=0; i<NumGridCells; i++) {
    trgval[i] += epsilon*tmpval[i];
    tmpval[i] = 0.0;
  }


  if (options->formulation != FORMULATION_ZERO_ION) {

    KiFmmWrapper * fmm3 = new KiFmmWrapper(molSurf.NumQuadPoints, NumGridCells,
					   molSurf.quadPos,molSurf.quadNor,gridPos,gridNor, molSurf.quadWeight,KNL_LAP_S_U,coef,options->accuracy);
    fmm3->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumGridCells; i++) {
      trgval[i] += tmpval[i];
      tmpval[i] = 0.0;
    }
    
    KiFmmWrapper * fmm4 = new KiFmmWrapper(molSurf.NumQuadPoints, NumGridCells,
					   molSurf.quadPos,molSurf.quadNor,gridPos,gridNor, molSurf.quadWeight,KNL_BOZ_S_U,coef,options->accuracy);
    fmm4->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumGridCells; i++) {
      trgval[i] += -1.0*tmpval[i];
    }
  }

  // FIXME: make this an option
  if (options->sovationVolumePotentialOnly == false) {

    // still need to add one more term... the \sum q_k G(x,x_k) term
    unsigned int NumAtoms = atomList.size();
    PrintValue("Number of Atoms",NumAtoms);
    
    DblNumMat * atomPos = new DblNumMat(3, NumAtoms);
    DblNumMat * atomNor = new DblNumMat(3, NumAtoms);
    
    double * chargeVals = new double[NumAtoms];
    
    for (int i=0; i<NumAtoms; i++) {
      (*atomPos)(0,i) = atomList[i]->m_Position[0];
      (*atomPos)(1,i) = atomList[i]->m_Position[1];
      (*atomPos)(2,i) = atomList[i]->m_Position[2];
      (*atomNor)(0,i) = 0.0;
      (*atomNor)(1,i) = 0.0;
      (*atomNor)(2,i) = 0.0;
      chargeVals[i] = atomList[i]->getCharge()/pb_para->pdie;
    }
    
    for (int i=0; i<NumGridCells; i++) { 
      tmpval[i] = 0.0;
    }
    
    KiFmmWrapper * fmm5 = new KiFmmWrapper(NumAtoms, NumGridCells,
					   atomPos,atomNor,gridPos,gridNor, molSurf.quadWeight,KNL_LAP_S_U,coef,options->accuracy);
    fmm5->fmmEval(chargeVals,tmpval);
    
    for (int i=0; i<NumGridCells; i++) {
      trgval[i] += tmpval[i];
    }
    
    
  } else {
    cout << "WARNING: Solvation potential only!" << endl;
  }


  if (options->haveSDF) {

    int count = 0;
    double bandsize = .4;

    for (int z=0; z < options->resolutionZ; z++) {
      for (int y=0; y < options->resolutionY; y++) {
	for (int x=0; x < options->resolutionX; x++) {
	  if (inputVol(x,y,z) < -bandsize) {
	    // interior, do nothing?	    
	  } else if (inputVol(x,y,z) > bandsize) {
	    // exterior
	    // fix value of exterior potential by multiplying by pb_para->pdie/pb_para->sdie
	    trgval[(x*options->resolutionY + y) * options->resolutionZ + z] *= pb_para->pdie/pb_para->sdie;
	  } else {
	    // could have integration problems
	    //trgval[(x*options->resolutionY + y) * options->resolutionZ + z];
	    trgval[(x*options->resolutionY + y) * options->resolutionZ + z] = 0;
	    count++;
	  }

	}
      }
    }
    cout << "Zeroed " << count << " voxels" << endl;


  }



    // then print a .rawiv...
    // FUTURE: use volmagick to do this (more robustly)

    string fname = options->out_fname + (string)".rawiv";
    FILE *f = fopen(fname.c_str(), "wb");

    for(int i=0; i<3; i++)
      {
	fwrite(&rawivmin[i], sizeof(float), 1, f);
      }	
    for(int i=0; i<3; i++)
      {
	fwrite(&rawivmmax[i], sizeof(float), 1, f);
      }	
    fwrite(&rawivnumVerts, sizeof(unsigned int), 1, f);
    fwrite(&rawivnumCells, sizeof(unsigned int), 1, f);
    
    for(int i=0; i<3; i++)
      {
	fwrite(&rawivdim[i], sizeof(unsigned int), 1, f);
      }	
    for(int i=0; i<3; i++)
      {
	fwrite(&rawivorigin[i], sizeof(float), 1, f);
      }	
    for(int i=0; i<3; i++)
      {
	fwrite(&rawivspan[i], sizeof(float), 1, f);
      }


    unsigned short c_short;
    //double myd;
    float myd;

    //cout << "WARNING: thresholding in place" << endl;
    

    for (int z=0; z < options->resolutionZ; z++) {
      for (int y=0; y < options->resolutionY; y++) {
	for (int x=0; x < options->resolutionX; x++) {
	  //c_short=*(trgval+ (((z*gridSize + y) * gridSize + x) * 2));
	  //SWAP_16(&(c_short));
	  //fwrite(&c_short, sizeof(unsigned short), 1, f);
	  
	  //myd=trgval[(z*gridSize + y) * gridSize + x];
	  myd=trgval[(x*options->resolutionY + y) * options->resolutionZ + z];
	  
	  //if (x < 5 && y < 5 && z < 5)
	  //cout << myd << " ";
	  
	  //mymax = 0.0005;

	  /*
	    // thresholding commented... using volSetExtents instead...
	  // FIXME: pass this value as an option?
	  float thresh = 0.002;
	  if (myd > thresh) myd = thresh;
	  if (myd < -thresh) myd = -thresh;

	  */

	  // arand: I don't understand why doubles don't work...
	  //SWAP_64(&(myd));	  
	  //fwrite(&myd, sizeof(double), 1, f);

	  SWAP_32(&(myd));
	  fwrite(&myd, sizeof(float), 1, f);

	} //cout << endl;
      } //cout << endl;
    }
    fclose(f);

  return 0;


}


PetscErrorCode PBSolver::computePotentialAtLocation(PBOptions* options) {

  PrintHeading("computePotentialAtLocation()");

  //setEnv(options);

  loadPQR(options);
  //computeQuad(options);

  computeQuad(options->mesh_fname,options,&molSurf);

  loadPot(options);


  // loadCRD
  unsigned int NumLocations = 11;

  ifstream crd(options->crd_fname);

  DblNumMat * trgPos = new DblNumMat(3, NumLocations);
  DblNumMat * trgNor = NULL;

  for (int i=0; i<11; i++) {
    double x,y,z;
    crd >> x >> y >> z;

    (*trgPos)(0,i) = x;
    (*trgPos)(1,i) = y;
    (*trgPos)(2,i) = z;
    //cout << x << " " << y << " " << z << endl;
  }


  // compute phi_rxn at target points points...
  double coef[2];
  coef[0] = pb_para->xkappa;
  double kappa = coef[0];
  coef[1] = pb_para->sdie/pb_para->pdie;
  double epsilon = coef[1];

  double * trgval = new double[NumLocations];
  for (int i=0; i<NumLocations; i++) {
    trgval[i] = 0.0;
  }

  double * tmpval = new double[NumLocations];

  for (int i=0; i<NumLocations; i++) {
    tmpval[i] = 0.0;
  }
  KiFmmWrapper * fmm1 = new KiFmmWrapper(molSurf.NumQuadPoints, NumLocations,
                                         molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,KNL_LAP_D_U,coef,options->accuracy);
  fmm1->fmmEval(phi,tmpval);

  for (int i=0; i<NumLocations; i++) {
    trgval[i] += -1.0*tmpval[i];
    tmpval[i] = 0.0;
  }

  KiFmmWrapper * fmm2 = new KiFmmWrapper(molSurf.NumQuadPoints, NumLocations,
                                         molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,KNL_BOZ_D_U,coef,options->accuracy);
  fmm2->fmmEval(phi,tmpval);

  for (int i=0; i<NumLocations; i++) {
    trgval[i] += epsilon*tmpval[i];
    tmpval[i] = 0.0;
  }


  if (options->formulation != FORMULATION_ZERO_ION) {

    KiFmmWrapper * fmm3 = new KiFmmWrapper(molSurf.NumQuadPoints, NumLocations,
					   molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,KNL_LAP_S_U,coef,options->accuracy);
    fmm3->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumLocations; i++) {
      trgval[i] += tmpval[i];
      tmpval[i] = 0.0;
    }
    
    KiFmmWrapper * fmm4 = new KiFmmWrapper(molSurf.NumQuadPoints, NumLocations,
					   molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,KNL_BOZ_S_U,coef,options->accuracy);
    fmm4->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumLocations; i++) {
      trgval[i] += -1.0*tmpval[i];
    }
  }
  
  // still need to add one more term... the \sum q_k G(x,x_k) term
  unsigned int NumAtoms = atomList.size();
  PrintValue("Number of Atoms",NumAtoms);
  
  DblNumMat * atomPos = new DblNumMat(3, NumAtoms);
  DblNumMat * atomNor = new DblNumMat(3, NumAtoms);
  
  double * chargeVals = new double[NumAtoms];

  for (int i=0; i<NumAtoms; i++) {
    (*atomPos)(0,i) = atomList[i]->m_Position[0];
    (*atomPos)(1,i) = atomList[i]->m_Position[1];
    (*atomPos)(2,i) = atomList[i]->m_Position[2];
    (*atomNor)(0,i) = 0.0;
    (*atomNor)(1,i) = 0.0;
    (*atomNor)(2,i) = 0.0;
    chargeVals[i] = 1.0*atomList[i]->getCharge()/pb_para->pdie;
    //cout << "Loc/Charge: (" << atomList[i]->m_Position[0] << ", "
    // << atomList[i]->m_Position[1] << ", "
    // << atomList[i]->m_Position[2] << "), "
    //	 << atomList[i]->getCharge() << endl;      
  }
  
   // fmm style

  for (int i=0; i<NumLocations; i++) { 
    tmpval[i] = 0.0;
  }
  
  double * dummyWeight = new double[NumAtoms];
  for (int i=0; i<NumAtoms; i++) {
    dummyWeight[i] = 1.0;
  }


  KiFmmWrapper * fmm5 = new KiFmmWrapper(NumAtoms, NumLocations,
					 atomPos,atomNor,trgPos,trgNor, dummyWeight,KNL_LAP_S_U,coef,options->accuracy);
  fmm5->fmmEval(chargeVals,tmpval);
  

  for (int i=0; i<NumLocations; i++) {
    trgval[i] += tmpval[i];
  }  

  /*
  // direct computation
  for (int i=0; i<NumAtoms; i++) {


    for (int j=0; j<11; j++) {

      if (atomList[i]->getCharge() != 0.0) {
	
	double OOFP = 1.0/(4.0*PI);
	double mydist = 0.0;
	
	mydist += ((*trgPos)(0,j) - atomList[i]->m_Position[0])*((*trgPos)(0,j) - atomList[i]->m_Position[0]);
	mydist += ((*trgPos)(1,j) - atomList[i]->m_Position[1])*((*trgPos)(1,j) - atomList[i]->m_Position[1]);
	mydist += ((*trgPos)(2,j) - atomList[i]->m_Position[2])*((*trgPos)(2,j) - atomList[i]->m_Position[2]);
	
	trgval[j] += -1.0*OOFP/sqrt(mydist)*atomList[i]->getCharge()/pb_para->pdie;

	cout << i << " " << j << " " << -1.0*OOFP/sqrt(mydist)*atomList[i]->getCharge()/pb_para->pdie << endl;
	
      }
    }

  }
  */

  string fname = options->out_fname + (string)".crd.potential";

  ofstream crdpot(fname.c_str());

  for (int i=0; i<NumLocations; i++) {
    crdpot << trgval[i] << endl;
  }  

  return 0;


}




PetscErrorCode PBSolver::computeEnergy(PBOptions* options) {

  PrintHeading("computeEnergy()");

  //setEnv(options);

  loadPQR(options);
  //computeQuad(options);

  computeQuad(options->mesh_fname,options,&molSurf);

  loadPot(options);

  double startpb = getTime2();

  unsigned int NumAtoms = atomList.size();
  PrintValue("Number of Atoms",NumAtoms);

  DblNumMat * atomPos = new DblNumMat(3, NumAtoms);
  DblNumMat * atomNor = new DblNumMat(3, NumAtoms);

  for (int i=0; i<NumAtoms; i++) {
    (*atomPos)(0,i) = atomList[i]->m_Position[0];
    (*atomPos)(1,i) = atomList[i]->m_Position[1];
    (*atomPos)(2,i) = atomList[i]->m_Position[2];
    (*atomNor)(0,i) = 0.0;
    (*atomNor)(1,i) = 0.0;
    (*atomNor)(2,i) = 0.0;
  }

  // compute phi_rxn at atomic centers...
  double coef[2];
  coef[0] = pb_para->xkappa;
  double kappa = coef[0];
  coef[1] = pb_para->sdie/pb_para->pdie;
  double epsilon = coef[1];


  double * trgval = new double[NumAtoms];
  for (int i=0; i<NumAtoms; i++) {
    trgval[i] = 0.0;
  }

  double * tmpval = new double[NumAtoms];

  for (int i=0; i<NumAtoms; i++) {
    tmpval[i] = 0.0;
  }
  KiFmmWrapper * fmm1 = new KiFmmWrapper(molSurf.NumQuadPoints, NumAtoms,
                                         molSurf.quadPos,molSurf.quadNor,atomPos,atomNor, molSurf.quadWeight,KNL_LAP_D_U,coef,options->accuracy);
  fmm1->fmmEval(phi,tmpval);

  for (int i=0; i<NumAtoms; i++) {
    trgval[i] += -1.0*tmpval[i];
    tmpval[i] = 0.0;
  }

  KiFmmWrapper * fmm2 = new KiFmmWrapper(molSurf.NumQuadPoints, NumAtoms,
                                         molSurf.quadPos,molSurf.quadNor,atomPos,atomNor, molSurf.quadWeight,KNL_BOZ_D_U,coef,options->accuracy);
  fmm2->fmmEval(phi,tmpval);

  for (int i=0; i<NumAtoms; i++) {
    trgval[i] += epsilon*tmpval[i];
    tmpval[i] = 0.0;
  }


  if (options->formulation != FORMULATION_ZERO_ION) {

    KiFmmWrapper * fmm3 = new KiFmmWrapper(molSurf.NumQuadPoints, NumAtoms,
					   molSurf.quadPos,molSurf.quadNor,atomPos,atomNor, molSurf.quadWeight,KNL_LAP_S_U,coef,options->accuracy);
    fmm3->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumAtoms; i++) {
      trgval[i] += tmpval[i];
      tmpval[i] = 0.0;
    }
    
    KiFmmWrapper * fmm4 = new KiFmmWrapper(molSurf.NumQuadPoints, NumAtoms,
					   molSurf.quadPos,molSurf.quadNor,atomPos,atomNor, molSurf.quadWeight,KNL_BOZ_S_U,coef,options->accuracy);
    fmm4->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumAtoms; i++) {
      trgval[i] += -1.0*tmpval[i];
    }
  }


  if (options->computePotentialAtAtoms) {
    // print the potential at each atom
    // useful when comparing the GB ``cross terms''
    string fname1 = options->out_fname + (string)".atompot";
    ofstream fout1(fname1.c_str());

    for (int i=0; i<NumAtoms; i++) {      
      fout1 << "Atom " << i << ": " << .5*trgval[i]*TOKCALPERMOLE << endl;
    }
  }


  totalEnergy = 0.0;
  for (int i=0; i<NumAtoms; i++) {
    trgval[i] *= .5*atomList[i]->getCharge()*TOKCALPERMOLE;
    totalEnergy += trgval[i];
    //cout << "Energy on atom " << i << ": " << trgval[i] << endl;
  }


  string fname = options->out_fname + (string)".energy";
  ofstream fout(fname.c_str());
  
  double computeTime = getTime2() - startpb;

  PrintValue(fout,"Total Energy", totalEnergy);
  PrintValue(fout,"Compute time", computeTime);


  /*
  // used for debugging to compare with a  known solution
  double exactenergy = 0.5*atomList[0]->getCharge()*atomList[0]->getCharge()/(4.0*PI*1.0)*(1.0/((1.0+pb_para->xkappa)*pb_para->sdie) - 1.0/pb_para->pdie);
  
  exactenergy *= TOKCALPERMOLE;
  
  PrintValue(fout, "Sphere Example, Exact Energy",exactenergy);
  PrintValue(fout, "Sphere Example, Error",exactenergy-totalEnergy);
  */


  /*
  for (int i=0; i<NumAtoms; i++) {

    fout << "Atom " << i << ": " << trgval[i] << endl;
  }
  */
  PetscFunctionReturn(0);
}


PetscErrorCode PBSolver::computeForces(PBOptions* options) {

  // modified computeEnergy to produce this...
  
  PrintHeading("computeForces()");

  //double dx = 0.05; // hacked for now...
  double dx = options->dxForce;

  //setEnv(options);
  
  loadPQR(options);
  //computeQuad(options);

  computeQuad(options->mesh_fname,options,&molSurf);

  loadPot(options);
  
  double startpb = getTime2();

  unsigned int NumAtoms = atomList.size();
  PrintValue("Number of Atoms",NumAtoms);

  unsigned int NumTrg = NumAtoms*6;

  DblNumMat * trgPos = new DblNumMat(3, NumTrg);
  DblNumMat * trgNor = new DblNumMat(3, NumTrg);

  //
  // List of locations at which the potential will be computed
  // so the gradient can be computed via differencing
  //
  for (int i=0; i<NumAtoms; i++) {
    (*trgPos)(0,6*i) = atomList[i]->m_Position[0]+dx;
    (*trgPos)(1,6*i) = atomList[i]->m_Position[1];
    (*trgPos)(2,6*i) = atomList[i]->m_Position[2];
    (*trgNor)(0,6*i) = 0.0;
    (*trgNor)(1,6*i) = 0.0;
    (*trgNor)(2,6*i) = 0.0;

    (*trgPos)(0,6*i+1) = atomList[i]->m_Position[0]-dx;
    (*trgPos)(1,6*i+1) = atomList[i]->m_Position[1];
    (*trgPos)(2,6*i+1) = atomList[i]->m_Position[2];
    (*trgNor)(0,6*i+1) = 0.0;
    (*trgNor)(1,6*i+1) = 0.0;
    (*trgNor)(2,6*i+1) = 0.0;

    (*trgPos)(0,6*i+2) = atomList[i]->m_Position[0];
    (*trgPos)(1,6*i+2) = atomList[i]->m_Position[1]+dx;
    (*trgPos)(2,6*i+2) = atomList[i]->m_Position[2];
    (*trgNor)(0,6*i+2) = 0.0;
    (*trgNor)(1,6*i+2) = 0.0;
    (*trgNor)(2,6*i+2) = 0.0;

    (*trgPos)(0,6*i+3) = atomList[i]->m_Position[0];
    (*trgPos)(1,6*i+3) = atomList[i]->m_Position[1]-dx;
    (*trgPos)(2,6*i+3) = atomList[i]->m_Position[2];
    (*trgNor)(0,6*i+3) = 0.0;
    (*trgNor)(1,6*i+3) = 0.0;
    (*trgNor)(2,6*i+3) = 0.0;

    (*trgPos)(0,6*i+4) = atomList[i]->m_Position[0];
    (*trgPos)(1,6*i+4) = atomList[i]->m_Position[1];
    (*trgPos)(2,6*i+4) = atomList[i]->m_Position[2]+dx;
    (*trgNor)(0,6*i+4) = 0.0;
    (*trgNor)(1,6*i+4) = 0.0;
    (*trgNor)(2,6*i+4) = 0.0;

    (*trgPos)(0,6*i+5) = atomList[i]->m_Position[0];
    (*trgPos)(1,6*i+5) = atomList[i]->m_Position[1];
    (*trgPos)(2,6*i+5) = atomList[i]->m_Position[2]-dx;
    (*trgNor)(0,6*i+5) = 0.0;
    (*trgNor)(1,6*i+5) = 0.0;
    (*trgNor)(2,6*i+5) = 0.0;

  }

  // compute phi_rxn at atomic centers...
  double coef[2];
  coef[0] = pb_para->xkappa;
  double kappa = coef[0];
  coef[1] = pb_para->sdie/pb_para->pdie;
  double epsilon = coef[1];


  double * trgval = new double[NumTrg];
  for (int i=0; i<NumTrg; i++) {
    trgval[i] = 0.0;
  }


  double * tmpval = new double[NumTrg];

  for (int i=0; i<NumTrg; i++) {
    tmpval[i] = 0.0;
  }
  KiFmmWrapper * fmm1 = new KiFmmWrapper(molSurf.NumQuadPoints, NumTrg,
                                         molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,
					 KNL_LAP_D_U,coef,options->accuracy);
  fmm1->fmmEval(phi,tmpval);

  for (int i=0; i<NumTrg; i++) {
    trgval[i] += -1.0*tmpval[i];
    tmpval[i] = 0.0;
  }

  KiFmmWrapper * fmm2 = new KiFmmWrapper(molSurf.NumQuadPoints, NumTrg,
                                         molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,
					 KNL_BOZ_D_U,coef,options->accuracy);
  fmm2->fmmEval(phi,tmpval);

  for (int i=0; i<NumTrg; i++) {
    trgval[i] += epsilon*tmpval[i];
    tmpval[i] = 0.0;
  }


  if (options->formulation != FORMULATION_ZERO_ION) {

    KiFmmWrapper * fmm3 = new KiFmmWrapper(molSurf.NumQuadPoints, NumTrg,
					   molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,
					   KNL_LAP_S_U,coef,options->accuracy);
    fmm3->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumTrg; i++) {
      trgval[i] += tmpval[i];
      tmpval[i] = 0.0;
    }
    
    KiFmmWrapper * fmm4 = new KiFmmWrapper(molSurf.NumQuadPoints, NumTrg,
					   molSurf.quadPos,molSurf.quadNor,trgPos,trgNor, molSurf.quadWeight,
					   KNL_BOZ_S_U,coef,options->accuracy);
    fmm4->fmmEval(dphi,tmpval);
    
    for (int i=0; i<NumTrg; i++) {
      trgval[i] += -1.0*tmpval[i];
    }
  }

  // compute atomic forces

  string fname = options->out_fname + (string)".force";
  ofstream fout(fname.c_str());

  for (int i=0; i<NumAtoms; i++) {
    // compute the force via a difference quotient
    double fx,fy,fz;

    // arand: reversing sign based on experimental tests:
    //        double check the formulas...

    fx = -1.0*(trgval[i*6+1] - trgval[i*6])/(2.0*dx)*atomList[i]->getCharge();
    fy = -1.0*(trgval[i*6+3] - trgval[i*6+2])/(2.0*dx)*atomList[i]->getCharge();
    fz = -1.0*(trgval[i*6+5] - trgval[i*6+4])/(2.0*dx)*atomList[i]->getCharge();
    
    fout << i << " " << fx << " " << fy << " " << fz << endl;

  }

  double computeTime = getTime2() - startpb;
  PrintValue(fout, "compute time: ", computeTime);

  PetscFunctionReturn(0);
}


PetscErrorCode PBSolver::loadPQR(PBOptions* options) {


  GOALoader* gLoader = new GOALoader();
  PDBParser::GroupOfAtoms* molecule = gLoader->loadFile( options->pqr_fname , true);
  delete gLoader;
  if( !molecule ) {
    cout << "ERROR: failed reading pqr file" << endl;
  }

  PDBParser::CollectionData* collectionData = 0;
  if( molecule->type == PDBParser::COLLECTION_TYPE ) collectionData = molecule->m_CollectionData;

  PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType =PDBParser::GroupOfAtoms::VDW_RADIUS;


  atomList.clear();

  PDBParser::FlattenGOA(molecule, atomList, collectionData, 0, 0, 0, radiusType,
			PDBParser::ATOM_TYPE, false  );

}

PetscErrorCode PBSolver::computeQuad(char * fname, PBOptions* options, PBRawnAndQuadInfo * surf) {

  // need to implement this...
  int GaussianQuadraturePoints = options->quadOrderFarField;
  int SubdivisionOrder = options->quadOrderNearField;
  int DuffyQuadratureOrder = options->quadOrderSingular;
  int NearFieldDepth = options->quadLevel - 1;

  PrintValue("Far Field Quadrature Rule",GaussianQuadraturePoints);
  PrintValue("Near Field Quadrature Rule",SubdivisionOrder*SubdivisionOrder);

  PrintValue("Duffy Quadrature Rule", DuffyQuadratureOrder*DuffyQuadratureOrder);
  PrintValue("Near Field Depth", NearFieldDepth);

  PrintValue("Surface Mesh",fname);

  TriElementNS::GeometryParser* parser = new TriElementNS::GeometryParser();

  if (options->geometryType == GEOMETRY_ASPLINE) {
    surf->aspline = parser->getASpline(fname);


    if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
      surf->asplineCentroid = parser->getASpline(fname);
      surf->asplineCenterDuffy = parser->getASpline(fname);
    } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
      surf->asplineDuffy[0] = parser->getASpline(fname,0);
      surf->asplineDuffy[1] = parser->getASpline(fname,1);
      surf->asplineDuffy[2] = parser->getASpline(fname,2);
    }
    if (NearFieldDepth > 0) {
      surf->asplineNearField = parser->getASpline(fname);
    }
  } else if (options->geometryType == GEOMETRY_LINEAR) {
    surf->aspline = parser->getLinearPatch(fname);

    if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
      surf->asplineCenterDuffy = parser->getLinearPatch(fname);
      surf->asplineCentroid = parser->getLinearPatch(fname);

    } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
      surf->asplineDuffy[0] = parser->getLinearPatch(fname,0);
      surf->asplineDuffy[1] = parser->getLinearPatch(fname,1);
      surf->asplineDuffy[2] = parser->getLinearPatch(fname,2);
    }
    if (NearFieldDepth > 0) {
      surf->asplineNearField = parser->getLinearPatch(fname);
    }
  }    

  surf->aspline->Triangle_Patch_Nodes((char *)"gaussian",GaussianQuadraturePoints);
  surf->aspline->BoundingBox();
  surf->aspline->NormalizeNormal();

  if (options->discreteMethod == DISCRETE_COLLOCATION) {
    surf->asplineDuffy[0]->Triangle_Patch_Nodes((char *)"duffy",DuffyQuadratureOrder);
    surf->asplineDuffy[1]->Triangle_Patch_Nodes((char *)"duffy",DuffyQuadratureOrder);
    surf->asplineDuffy[2]->Triangle_Patch_Nodes((char *)"duffy",DuffyQuadratureOrder);


    surf->asplineDuffy[0]->BoundingBox();
    surf->asplineDuffy[0]->NormalizeNormal();
    surf->asplineDuffy[1]->BoundingBox();
    surf->asplineDuffy[1]->NormalizeNormal();
    surf->asplineDuffy[2]->BoundingBox();
    surf->asplineDuffy[2]->NormalizeNormal();
    //printRawnWithQuadrature(surf->asplineDuffy[0],"duffy.rawn");
  }


  if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
    surf->asplineCenterDuffy->Triangle_Patch_Nodes((char *)"centerduffy",DuffyQuadratureOrder);
    surf->asplineCentroid->Triangle_Patch_Nodes((char *)"gaussian",1);
    //printRawnWithQuadrature(surf->asplineCenterDuffy,"centerduffy.rawn");

    surf->asplineCenterDuffy->BoundingBox();
    surf->asplineCenterDuffy->NormalizeNormal();
    surf->asplineCentroid->BoundingBox();
    surf->asplineCentroid->NormalizeNormal();
  }

  if (NearFieldDepth > 0) {
    surf->asplineNearField->Triangle_Patch_Nodes((char *)"subdivided",SubdivisionOrder);

    if (options->discreteMethod == DISCRETE_COLLOCATION) {
      surf->asplineNearField->SetUpNearField(NearFieldDepth);
    } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
      surf->asplineNearField->SetUpNearField2(NearFieldDepth);
    }
    surf->asplineNearField->BoundingBox();
    surf->asplineNearField->NormalizeNormal();
  }



  surf->NumQuadPoints = surf->aspline->numbtris*surf->aspline->my_quadr->numOfPts_perTri;

  surf->quadPos = new DblNumMat(3, surf->NumQuadPoints);
  surf->quadNor = new DblNumMat(3, surf->NumQuadPoints);
  surf->quadWeight = new double[surf->NumQuadPoints];

  int j=0;

  for (int i = 0; i < surf->aspline->numbtris; i++) {
    for (int k = 0; k < surf->aspline->my_quadr->numOfPts_perTri; k++) {
      (*(surf->quadPos))(0,j) = surf->aspline->m_Faces[i]->Node[k]->Point[0];
      (*(surf->quadPos))(1,j) = surf->aspline->m_Faces[i]->Node[k]->Point[1];
      (*(surf->quadPos))(2,j) = surf->aspline->m_Faces[i]->Node[k]->Point[2];
      (*(surf->quadNor))(0,j) = surf->aspline->m_Faces[i]->Node[k]->Normal[0];
      (*(surf->quadNor))(1,j) = surf->aspline->m_Faces[i]->Node[k]->Normal[1];
      (*(surf->quadNor))(2,j) = surf->aspline->m_Faces[i]->Node[k]->Normal[2];
      surf->quadWeight[j] = surf->aspline->m_Faces[i]->Node[k]->weight;
      j++;
    }

  }

  if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
    surf->NumTriangles = surf->asplineCentroid->numbtris;
    surf->centroidPos = new DblNumMat(3,surf->NumTriangles);
    surf->centroidNor = new DblNumMat(3,surf->NumTriangles);
    surf->centroidWeight = new double[surf->NumTriangles];
    
    for (int i = 0; i < surf->NumTriangles; i++) {
      (*(surf->centroidPos))(0,i) = surf->asplineCentroid->m_Faces[i]->Node[0]->Point[0];
      (*(surf->centroidPos))(1,i) = surf->asplineCentroid->m_Faces[i]->Node[0]->Point[1];
      (*(surf->centroidPos))(2,i) = surf->asplineCentroid->m_Faces[i]->Node[0]->Point[2];
      (*(surf->centroidNor))(0,i) = surf->asplineCentroid->m_Faces[i]->Node[0]->Normal[0];
      (*(surf->centroidNor))(1,i) = surf->asplineCentroid->m_Faces[i]->Node[0]->Normal[1];
      (*(surf->centroidNor))(2,i) = surf->asplineCentroid->m_Faces[i]->Node[0]->Normal[2];
      surf->centroidWeight[i] = surf->asplineCentroid->m_Faces[i]->Node[0]->weight;
    }    
  }

  return 0;
}

/*
PetscErrorCode PBSolver::computeQuad(PBOptions* options) {

  // arand: using 3/3 below is probably safer but
  //        this seems to work and runs quickly
  int GaussianQuadraturePoints = 1;
  int SubdivisionOrder = 5;
  int DuffyQuadratureOrder = 2;
  int NearFieldDepth = options->quadLevel - 1;



  GaussianQuadraturePoints = options->quadOrderFarField;
  DuffyQuadratureOrder = options->quadOrderSingular;
  SubdivisionOrder = options->quadOrderNearField;


  PrintValue("Far Field Quadrature Rule",GaussianQuadraturePoints);
  PrintValue("Near Field Quadrature Rule",SubdivisionOrder*SubdivisionOrder);

  PrintValue("Duffy Quadrature Rule", DuffyQuadratureOrder*DuffyQuadratureOrder);
  PrintValue("Near Field Depth", NearFieldDepth);

  PrintValue("Surface Mesh",options->mesh_fname);
  TriElementNS::GeometryParser* parser = new TriElementNS::GeometryParser();

  if (options->geometryType == GEOMETRY_ASPLINE) {
    molSurf.aspline = parser->getASpline(options->mesh_fname);


    if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
      asplineCentroid = parser->getASpline(options->mesh_fname);
      asplineCenterDuffy = parser->getASpline(options->mesh_fname);
    } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
      
      asplineDuffy[0] = parser->getASpline(options->mesh_fname,0);
      asplineDuffy[1] = parser->getASpline(options->mesh_fname,1);
      asplineDuffy[2] = parser->getASpline(options->mesh_fname,2);
    }

    if (NearFieldDepth > 0) {
      asplineNearField = parser->getASpline(options->mesh_fname);
    }
  } else if (options->geometryType == GEOMETRY_LINEAR) {
    aspline = parser->getLinearPatch(options->mesh_fname);

    if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
      asplineCenterDuffy = parser->getLinearPatch(options->mesh_fname);
      asplineCentroid = parser->getLinearPatch(options->mesh_fname);

    } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
      asplineDuffy[0] = parser->getLinearPatch(options->mesh_fname,0);
      asplineDuffy[1] = parser->getLinearPatch(options->mesh_fname,1);
      asplineDuffy[2] = parser->getLinearPatch(options->mesh_fname,2);
    }


    if (NearFieldDepth > 0) {
      asplineNearField = parser->getLinearPatch(options->mesh_fname);
    }
  }    

  aspline->Triangle_Patch_Nodes((char *)"gaussian",GaussianQuadraturePoints);
  aspline->BoundingBox();
  aspline->NormalizeNormal();

  if (options->discreteMethod == DISCRETE_COLLOCATION) {
    asplineDuffy[0]->Triangle_Patch_Nodes((char *)"duffy",DuffyQuadratureOrder);
    asplineDuffy[1]->Triangle_Patch_Nodes((char *)"duffy",DuffyQuadratureOrder);
    asplineDuffy[2]->Triangle_Patch_Nodes((char *)"duffy",DuffyQuadratureOrder);


    asplineDuffy[0]->BoundingBox();
    asplineDuffy[0]->NormalizeNormal();
    asplineDuffy[1]->BoundingBox();
    asplineDuffy[1]->NormalizeNormal();
    asplineDuffy[2]->BoundingBox();
    asplineDuffy[2]->NormalizeNormal();
    //printRawnWithQuadrature(asplineDuffy[0],"duffy.rawn");
  }


  if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
    asplineCenterDuffy->Triangle_Patch_Nodes((char *)"centerduffy",DuffyQuadratureOrder);
    asplineCentroid->Triangle_Patch_Nodes((char *)"gaussian",1);
    //printRawnWithQuadrature(asplineCenterDuffy,"centerduffy.rawn");

    asplineCenterDuffy->BoundingBox();
    asplineCenterDuffy->NormalizeNormal();
    asplineCentroid->BoundingBox();
    asplineCentroid->NormalizeNormal();
  }

  if (NearFieldDepth > 0) {
    asplineNearField->Triangle_Patch_Nodes((char *)"subdivided",SubdivisionOrder);

    if (options->discreteMethod == DISCRETE_COLLOCATION) {
      asplineNearField->SetUpNearField(NearFieldDepth);
    } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
      asplineNearField->SetUpNearField2(NearFieldDepth);
    }
    asplineNearField->BoundingBox();
    asplineNearField->NormalizeNormal();
  }



  NumQuadPoints = aspline->numbtris*aspline->my_quadr->numOfPts_perTri;

  quadPos = new DblNumMat(3, NumQuadPoints);
  quadNor = new DblNumMat(3, NumQuadPoints);
  quadWeight = new double[NumQuadPoints];

  int j=0;

  for (int i = 0; i < aspline->numbtris; i++) {
    for (int k = 0; k < aspline->my_quadr->numOfPts_perTri; k++) {
      (*quadPos)(0,j) = aspline->m_Faces[i]->Node[k]->Point[0];
      (*quadPos)(1,j) = aspline->m_Faces[i]->Node[k]->Point[1];
      (*quadPos)(2,j) = aspline->m_Faces[i]->Node[k]->Point[2];
      (*quadNor)(0,j) = aspline->m_Faces[i]->Node[k]->Normal[0];
      (*quadNor)(1,j) = aspline->m_Faces[i]->Node[k]->Normal[1];
      (*quadNor)(2,j) = aspline->m_Faces[i]->Node[k]->Normal[2];
      quadWeight[j] = aspline->m_Faces[i]->Node[k]->weight;
      j++;
    }

  }

  if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
    NumTriangles = asplineCentroid->numbtris;
    centroidPos = new DblNumMat(3,NumTriangles);
    centroidNor = new DblNumMat(3,NumTriangles);
    centroidWeight = new double[NumTriangles];
    
    for (int i = 0; i < NumTriangles; i++) {
      (*centroidPos)(0,i) = asplineCentroid->m_Faces[i]->Node[0]->Point[0];
      (*centroidPos)(1,i) = asplineCentroid->m_Faces[i]->Node[0]->Point[1];
      (*centroidPos)(2,i) = asplineCentroid->m_Faces[i]->Node[0]->Point[2];
      (*centroidNor)(0,i) = asplineCentroid->m_Faces[i]->Node[0]->Normal[0];
      (*centroidNor)(1,i) = asplineCentroid->m_Faces[i]->Node[0]->Normal[1];
      (*centroidNor)(2,i) = asplineCentroid->m_Faces[i]->Node[0]->Normal[2];
      centroidWeight[i] = asplineCentroid->m_Faces[i]->Node[0]->weight;
	 
    }
    
  }

  return 0;

}
*/


/* // OLD... remove soon
// future: eliminate need for the .quad file...
PetscErrorCode PBSolver::loadQuad(PBOptions* options) {

  ifstream quadin(options->quad_fname);

  quadin >> NumQuadPoints;

  quadPos = new DblNumMat(3, NumQuadPoints);
  quadNor = new DblNumMat(3, NumQuadPoints);
  quadWeight = new double[NumQuadPoints];

  // read quad points with normals...
  for (int i=0; i<NumQuadPoints; i++) {
    quadin >> (*quadPos)(0,i) >> (*quadPos)(1,i) >> (*quadPos)(2,i);
    quadin >> (*quadNor)(0,i) >> (*quadNor)(1,i) >> (*quadNor)(2,i);
    quadin >> quadWeight[i];
  }

}
*/

PetscErrorCode PBSolver::loadRawn(char * fname, PBRawnAndQuadInfo * surf) {

  ifstream fin(fname);
  fin >> surf->NumVertices >> surf->NumTriangles;

  //cout << NumVert << " " << NumTri << endl;

  PrintValue("Mesh Vertices",surf->NumVertices);
  PrintValue("Mesh Triangles",surf->NumTriangles);

  surf->rawn_x.clear();
  surf->rawn_y.clear();
  surf->rawn_z.clear();
  surf->rawn_nx.clear();
  surf->rawn_ny.clear();
  surf->rawn_nz.clear();


  for (int i=0; i<surf->NumVertices; i++) {
    double xx,yy,zz;
    fin >> xx >> yy >> zz;
    surf->rawn_x.push_back(xx);
    surf->rawn_y.push_back(yy);
    surf->rawn_z.push_back(zz);
    fin >> xx >> yy >> zz;
    surf->rawn_nx.push_back(xx);
    surf->rawn_ny.push_back(yy);
    surf->rawn_nz.push_back(zz);
  }

  surf->vert2tris.resize(surf->NumVertices);

  surf->rawn_t1.clear();
  surf->rawn_t2.clear();
  surf->rawn_t3.clear();

  for (int i=0; i<surf->NumTriangles; i++) {
    int a,b,c;
    fin >> a >> b >> c;
    surf->rawn_t1.push_back(a);
    surf->rawn_t2.push_back(b);
    surf->rawn_t3.push_back(c);

    surf->vert2tris[a].push_back(i);
    surf->vert2tris[b].push_back(i);
    surf->vert2tris[c].push_back(i);
  }
  return 0;
}

/*
PetscErrorCode PBSolver::loadRawn(PBOptions* options) {

  ifstream fin(options->mesh_fname);
  fin >> NumVertices >> NumTriangles;

  //cout << NumVert << " " << NumTri << endl;

  PrintValue("Mesh Vertices",NumVertices);
  PrintValue("Mesh Triangles",NumTriangles);

  rawn_x.clear();
  rawn_y.clear();
  rawn_z.clear();
  rawn_nx.clear();
  rawn_ny.clear();
  rawn_nz.clear();


  for (int i=0; i<NumVertices; i++) {
    double xx,yy,zz;
    fin >> xx >> yy >> zz;
    rawn_x.push_back(xx);
    rawn_y.push_back(yy);
    rawn_z.push_back(zz);
    fin >> xx >> yy >> zz;
    rawn_nx.push_back(xx);
    rawn_ny.push_back(yy);
    rawn_nz.push_back(zz);
  }

  vert2tris.resize(NumVertices);

  rawn_t1.clear();
  rawn_t2.clear();
  rawn_t3.clear();

  for (int i=0; i<NumTriangles; i++) {
    int a,b,c;
    fin >> a >> b >> c;
    rawn_t1.push_back(a);
    rawn_t2.push_back(b);
    rawn_t3.push_back(c);

    vert2tris[a].push_back(i);
    vert2tris[b].push_back(i);
    vert2tris[c].push_back(i);

  }

}
*/


PetscErrorCode PBSolver::loadPot(PBOptions* options) {

  ifstream potin(options->pot_fname);

  phi = new double[molSurf.NumQuadPoints];
  dphi = new double[molSurf.NumQuadPoints];

  if (options->discreteMethod == DISCRETE_NYSTROM) {
    // read quad points with normals...
    for (int i=0; i<molSurf.NumQuadPoints; i++) {
      potin >> phi[i] >> dphi[i];
    }

  } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {

    double * phi_tri = new double[molSurf.NumTriangles];
    double * dphi_tri = new double[molSurf.NumTriangles];

    for (int i=0; i<molSurf.NumTriangles; i++) {
      potin >> phi_tri[i] >> dphi_tri[i];
      
    }

    int count = 0;
    // interpolate values at quadrature points
    for (int i = 0; i < molSurf.aspline->numbtris; i++) {

      for (int k = 0; k < molSurf.aspline->my_quadr->numOfPts_perTri; k++) {
        phi[count] = phi_tri[i];
	dphi[count] = dphi_tri[i];
        count++;
      }
    }

  } else if (options->discreteMethod == DISCRETE_COLLOCATION) {

    //cout << "loadPot Collocation " << aspline->numbpts << endl;

    int nV = molSurf.aspline->numbpts;

    double * phi_vert = new double[nV];
    double * dphi_vert;
    if (options->formulation != FORMULATION_ZERO_ION) {
      dphi_vert = new double[nV];
    }

    // read quad points with normals...
    for (int i=0; i<nV; i++) {
      if (options->formulation == FORMULATION_ZERO_ION) {
	potin >> phi_vert[i];
      } else {
	potin >> phi_vert[i] >> dphi_vert[i];
      }
      //cout << phi_vert[i] << " " <<  dphi_vert[i] << endl;
    }

    // interpolate to the quad points...

    int count = 0;
    // interpolate values at quadrature points
    for (int i = 0; i < molSurf.aspline->numbtris; i++) {
      int t1,t2,t0;
      t0 = molSurf.aspline->m_Faces[i]->IndexInRAW[0];
      t1 = molSurf.aspline->m_Faces[i]->IndexInRAW[1];
      t2 = molSurf.aspline->m_Faces[i]->IndexInRAW[2];

      for (int k = 0; k < molSurf.aspline->my_quadr->numOfPts_perTri; k++) {
        phi[count] = 0.0;
        phi[count] += phi_vert[t0]*molSurf.aspline->my_quadr->xi1[k];
        phi[count] += phi_vert[t1]*molSurf.aspline->my_quadr->xi2[k];
        phi[count] += phi_vert[t2]*molSurf.aspline->my_quadr->xi3[k];


	if (options->formulation != FORMULATION_ZERO_ION) {
	  dphi[count] = 0.0;
	  dphi[count] += dphi_vert[t0]*molSurf.aspline->my_quadr->xi1[k];
	  dphi[count] += dphi_vert[t1]*molSurf.aspline->my_quadr->xi2[k];
	  dphi[count] += dphi_vert[t2]*molSurf.aspline->my_quadr->xi3[k];
	}

        count++;
      }
    }

  }

}

/*
Key steps in computePotential:
1.  Set up environment.
2.  Set RHS and initial guess.
    -This also reads surface mesh and sets up quadrature rules.
3.  Build FMM data structures.
4.  Solve system with FMM
5.  Log results and print potential.
 */



PetscErrorCode PBSolver::computePotential(PBOptions* options) {

  PrintHeading("computePotential()");

  PetscFunctionBegin;
  srand48( (long)time(NULL) );
  

  double kappa = pb_para->xkappa;
  double epsilon = pb_para->sdie/pb_para->pdie;


  Vec x;
  Vec b;


  if (options->formulation == FORMULATION_ZERO_ION && 
      options->useSternLayer) {
    cout << "----------------WARNING-----------------" << endl;
    cout << "----------------WARNING-----------------" << endl;
    cout << "No Stern Layer with zero ion concentration." << endl;
    cout << "----------------WARNING-----------------" << endl;
    cout << "----------------WARNING-----------------" << endl;
  }

  setRHS(options, b, x);


  PrintValue("Number of Quadrature Points", molSurf.NumQuadPoints);

  if (options->discreteMethod == DISCRETE_NYSTROM) {
    mult_fmm2 = new MultPB(molSurf.NumQuadPoints, molSurf.quadPos, molSurf.quadNor,molSurf.NumQuadPoints, molSurf.quadPos, molSurf.quadNor,
                           molSurf.quadWeight, kappa, epsilon,options,this);
  } else if (options->discreteMethod == DISCRETE_COLLOCATION) {
    mult_fmm2 = new MultPB(molSurf.NumQuadPoints, molSurf.quadPos, molSurf.quadNor, 
			   molSurf.NumVertices, molSurf.vertPos, molSurf.vertNor, 
			   molSurf.quadWeight, 
			   kappa, epsilon,options,this);
  } else if (options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {
    mult_fmm2 = new MultPB(molSurf.NumQuadPoints, molSurf.quadPos, molSurf.quadNor, molSurf.NumTriangles, molSurf.centroidPos, molSurf.centroidNor,
                           molSurf.quadWeight, kappa, epsilon,options,this);
  }

  KSPSolverPB* kspsolver = new KSPSolverPB();


  // not printing this anymore...
  //string raw_fname = options->out_fname + (string)"_input.rawnc";
  //PrintRawnc(raw_fname.c_str(), options, mult_fmm2->getNumSource(),x);


  double start = getTime();

  if (options->solverType == LIN_SOLVE_LU) {
    kspsolver->LUDirectSolver(x,b);
  } else if (options->solverType == -1) {
    cout << "WARNING: Skipping Linear Solve" << endl;
  } else {
    cout << "GMRES Iteration: ";
    kspsolver->GMRES(x,b);
  }

  double end = getTime();
  solverTime = end-start;

  PrintValue("Solver Runtime",end-start);


  PetscScalar *xarray;
  VecGetArray(x,&xarray);

  double * srcAtQuadPoints;
  

  if (options->discreteMethod == DISCRETE_COLLOCATION || 
      options->discreteMethod == DISCRETE_COLLOCATION_CONSTANT) {

    if (options->useSternLayer) {
      srcAtQuadPoints = new double[molSurf.NumQuadPoints*2 + sternLayer.NumQuadPoints*2];
    } else {
      srcAtQuadPoints = new double[molSurf.NumQuadPoints*2];
    }


    // interpolate values at quadrature points
    mult_fmm2->interpolateValuesAtQuadPoints(xarray,srcAtQuadPoints);
  } else if (options->discreteMethod == DISCRETE_NYSTROM) {
    srcAtQuadPoints = xarray;
  }


  phi = xarray;

  dphi = &xarray[molSurf.NumVertices];


  /*
  // stuff below is special to sphere example...
  // and has not been updated for the collocation method
  

  double myrad = 1.0;
  double exact = 1.0/(4.0*PI*myrad*(1.0+kappa*myrad)*pb_para->sdie);
  double exactd = -1.0/(4.0*PI*myrad*myrad*pb_para->pdie);


  double avephi, avedphi, sa;
  avephi = 0.0; avedphi = 0.0; sa = 0.0;

  double error = 0.0;
  double errord = 0.0;
  double l2norm = 0.0;
  double h1norm = 0.0;

  double weight;
  for (int i=0; i<NumQuadPoints; i++) {
    weight = quadWeight[i];
    avephi += weight*srcAtQuadPoints[i];
    avedphi += weight*srcAtQuadPoints[i+NumQuadPoints];
    sa += weight;

    error += weight*(srcAtQuadPoints[i] - exact)*(srcAtQuadPoints[i] - exact);
    l2norm += weight*(srcAtQuadPoints[i])*(srcAtQuadPoints[i]);


    if (options->formulation != FORMULATION_ZERO_ION) {
      errord += weight*(srcAtQuadPoints[i+NumQuadPoints] - exactd)*
	(srcAtQuadPoints[i+NumQuadPoints] - exactd);
      h1norm += weight*(srcAtQuadPoints[i+NumQuadPoints])*
	(srcAtQuadPoints[i+NumQuadPoints]);
    }
  }

  //cout << "Exact Val: " << exact << endl;

  
  if (options->formulation == FORMULATION_ZERO_ION) {    
    PrintValue(fout,"L2 Norm",sqrt(l2norm));
    PrintValue(fout,"L2 Error",sqrt(error));
    PrintValue(fout,"L2 Relative Error",sqrt(error)/sqrt(l2norm));    
  } else {

    PrintValue(fout,"L2 Norm",sqrt(l2norm+h1norm));
    PrintValue(fout,"L2 Error",sqrt(error+errord));
    PrintValue(fout,"L2 Relative Error",sqrt(error+errord)/sqrt(l2norm+h1norm));
  }

  //cout << "Surface area: " << sa << endl;
  //cout << "Average phi value: " << avephi/sa << endl;
  //cout << "Average dphi value: " << avedphi/sa << endl;

  */

  PetscFunctionReturn(0);
}


// warning: this is untested... we will see if this works
//          when things get connected to TexMol
Geometry * PBSolver::GetColoredSurface(PBOptions* options) {

  
  ComputeColors(options, molSurf.NumVertices, phi);
  
  Geometry* geometry = new Geometry();

  float r  = 0;
  float g  = 0;
  float b  = 0;

  geometry->AllocateTris(molSurf.NumVertices, molSurf.NumTriangles);
  geometry->AllocateTriVertColors();  

  for (int i=0; i<molSurf.NumVertices; i++) {

    geometry->m_TriVerts[i*3+0] = molSurf.rawn_x[i];
    geometry->m_TriVerts[i*3+1] = molSurf.rawn_y[i];
    geometry->m_TriVerts[i*3+2] = molSurf.rawn_z[i];
    geometry->m_TriVertNormals[i*3+0] = molSurf.rawn_nx[i];
    geometry->m_TriVertNormals[i*3+1] = molSurf.rawn_ny[i];
    geometry->m_TriVertNormals[i*3+2] = molSurf.rawn_nz[i];

    
    geometry->m_TriVertColorsTransparent[i*4+0] = c_red[i];
    geometry->m_TriVertColorsTransparent[i*4+1] = c_green[i];
    geometry->m_TriVertColorsTransparent[i*4+2] = c_blue[i];
  }

  for(int i = 0; i < molSurf.NumTriangles; i++) {
    geometry->m_Tris[i*3+0] = molSurf.rawn_t1[i];
    geometry->m_Tris[i*3+1] = molSurf.rawn_t2[i];
    geometry->m_Tris[i*3+2] = molSurf.rawn_t3[i];    
  }

  return geometry;
}


// these still need to be refactored
PetscErrorCode PBSolver::computePotentialCommandLine(PBOptions* options){

  //setEnv(options);
  loadPQR(options);
  //loadRawn(options); // old style... remove soon
  //computeQuad(options); // old style... remove soon

  loadRawn(options->mesh_fname,&molSurf);
  computeQuad(options->mesh_fname,options,&molSurf);

  if (options->useSternLayer) {
    loadRawn(options->stern_fname,&sternLayer);
    computeQuad(options->stern_fname,options,&sternLayer);
  }

  double startpb = getTime2();

  PetscErrorCode rval =  computePotential(options);

  double computeTime =  getTime2() - startpb;
  cout << "PB potential: " << computeTime << " (s)" << endl;

  string pot_fname = options->out_fname + (string)".potential";

  PrintBoundaryPotential(options, pot_fname.c_str(), mult_fmm2->getNumTarget(),phi);


  if (options->rawncOutput) 
    PrintRawnc(NULL, options, mult_fmm2->getNumSource(),phi);

  // print a log of some results...
  string fname = options->out_fname + (string)".log";
  ofstream fout(fname.c_str());

  PrintValue(fout,"Number of GMRES Iterations: ",mult_fmm2->NumIterations);
  PrintValue(fout, "Solver RunTime: ", solverTime);

  delete mult_fmm2;
  return rval;
}

// fixme: refactor these and make one master routine calling them...
PetscErrorCode PBSolver::computeEnergyCommandLine(PBOptions* options) {
  return computeEnergy(options);
}
PetscErrorCode PBSolver::computeForcesCommandLine(PBOptions* options) {
  return computeForces(options);
}
PetscErrorCode PBSolver::computeVolumePotentialCommandLine(PBOptions* options){
  return computeVolumePotential(options);
}
PetscErrorCode PBSolver::computePotentialAtLocationCommandLine(PBOptions* options){
  return computePotentialAtLocation(options);
}
