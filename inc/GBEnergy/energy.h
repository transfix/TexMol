#ifndef GB_ENERGY_H
#define GB_ENERGY_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <math.h>
#include <vector>
//#include <time.h>

#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"
#include "GBEnergy/PQRParser.h"
#include "GBEnergy/geometry.h"
#include "GBEnergy/surface.h"
#include "GBEnergy/geometryParser.h"
#include "GBEnergy/polEnergy.h"
#include "GBEnergy/polForce.h"
#include "CoarseGrain/coarseGrain.h"
//#include "optimizer.h"

using namespace MOLECULE;
using namespace GBGEOMETRY;
using namespace FUNCTION;
using namespace std;

void printUsage();
FILE *loadFile_read(char* fileName);
FILE *loadFile_write(char* fileName);
GroupOfGBAtoms *GBParsePQR(FILE *fp, MOLECULE::RADIUS_TYPE radiusType);
GroupOfGBAtoms *GBParseOptimalPQR(FILE *fp, MOLECULE::RADIUS_TYPE radiusType);
Surface *ParseGeometry(FILE *fp);
void loadBornRadius(GroupOfGBAtoms *molecule, FILE *fp);
void ParseCGInfo(FILE *fp, GroupOfGBAtoms *groupOfBeads);
void savePQR(GroupOfGBAtoms *goa, FILE *fp);

double getTime();
void aspline(int argc, char* argv[]);
double evaluate(double *x, double *p, double r, double c);
void surfaceError(int argc, char* argv[]);
void energy(int argc, char* argv[]);
void force(int argc, char* argv[]);
void dividePQR(int argc, char* argv[]);
void mergePQR(int argc, char* argv[]);
void coarseGrain(int argc, char* argv[]);
void coarseGrainCluster(int argc, char* argv[]);
void coarseGrainCharge(int argc, char* argv[]);
void coarseGrainChargeGB(int argc, char* argv[]);
void coarseGrainEnergy(int argc, char* argv[]);
void coarseGrainForce(int argc, char* argv[]);
void getBornRadius(int argc, char* argv[]);

#endif
