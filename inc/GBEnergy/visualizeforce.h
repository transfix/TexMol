#ifndef GB_VISUALIZEFORCE_H
#define GB_VISUALIZEFORCE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
//#include <time.h>

#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"
#include "GBEnergy/PQRParser.h"
#include "GBEnergy/geometry.h"
#include "GBEnergy/surface.h"
#include "GBEnergy/geometryParser.h"
//#include "polEnergy.h"
//#include "polForce.h"

using namespace MOLECULE;
using namespace GBGEOMETRY;

std::FILE *loadFile_read(char* fileName);
std::FILE *loadFile_write(char* fileName);
MOLECULE::GroupOfGBAtoms *ParsePQR(std::FILE *fp);
Surface *ParseGeometry(std::FILE *fp);
void quicksort(int t, double *F, int *order, int N);
int visualizeforce(int argc, char* argv[]);

#endif
