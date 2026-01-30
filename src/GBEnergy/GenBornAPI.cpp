#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
#include <vector>
//#include <time.h>
 
#include "GBEnergy/energy.h"
#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"
#include "GBEnergy/PQRParser.h"
#include "GBEnergy/geometry.h"
#include "GBEnergy/surface.h"
#include "GBEnergy/geometryParser.h"
#include "GBEnergy/polEnergy.h"
#include "GBEnergy/polForce.h"
#include "GBEnergy/CoarseGrain/coarseGrain.h"
#include "GBEnergy/visualizeforce.h"
#include "MolEnergyUtil/utils.h"
//#include "optimizer.h"

using namespace MOLECULE;
using namespace GBGEOMETRY;
using namespace FUNCTION;
using namespace std;


// this duplicates the function "energy" in energy.cpp
// without the commandline arguments...
// that function should be removed and call this one...
double GBEnergy(char * pqrFile, char * rawnFile, char * outputPrefix,
		double area, int surfType, double epsilon, double threshold) {

  FILE *fp;
  double start, total_time_1, total_time_2, total_time_3;

  MOLECULE::RADIUS_TYPE radiusType = PQR_RADIUS;

  if (surfType == 1) {
    radiusType == VDW_RADIUS;
  }
  fp = loadFile_read(pqrFile);
  GroupOfGBAtoms* molecule;
  molecule = GBParsePQR(fp, radiusType);
  cout << molecule->numOfAtoms << " atoms" << endl;
  fclose(fp);

  fp = loadFile_read(rawnFile);
  Surface* surface;
  surface = ParseGeometry(fp);
  fclose(fp);
  
  start = getTime2();
  surface->Triangle_Bezier_Patch();
  total_time_1 = getTime2() - start;
  
  start = getTime2();
  molecule->getBornRadii(surface);
  total_time_2 = getTime2() - start;
  
  // not needed...
  //fp = fopen("checkAtomBR.txt", "w");
  //for (int i = 0; i < molecule->numOfAtoms; i++)
  //  fprintf(fp, "%d %f\n", i, molecule->m_Atoms[i]->Born_radius);
  //fclose(fp);
  
  PolEnergy *m_Gpol= new PolEnergy(molecule);
  start = getTime2();
  //        molecule->getPolEnergy();
  m_Gpol->getPolEnergy(epsilon, threshold);
  total_time_3 = getTime2() - start;
  
  const double surface_tension = 19*0.001; // Simonson94 or 7.2*10^(-3) in Still90

  double nonpol_energy = surface_tension*area;
  
  double sol_energy = m_Gpol->pol_energy + nonpol_energy;

  char outFile[256];
  sprintf(outFile, "%s.energy",outputPrefix);
  
  fp = loadFile_write(outFile);
  fprintf(fp, "%f kcal/mol, %f kcal/mol, %fs, %fs, %fs, total:%fs\n", m_Gpol->pol_energy, sol_energy, total_time_1, total_time_2, total_time_3,
	  total_time_1+total_time_2+total_time_3);
  fclose(fp);
  
  return m_Gpol->pol_energy;
}

void GBForce(char * pqrFile, char * rawnFile, char * outputPrefix,
	     double area, int surfType, double epsilon, double threshold) {
  // not implemented yet

  FILE *fp;
  double start, total_time_1, total_time_2, total_time_3, force_time;
  

  MOLECULE::RADIUS_TYPE radiusType = PQR_RADIUS;

  if (surfType == 1) {
    radiusType == VDW_RADIUS;
  }
  
  fp = loadFile_read(pqrFile);
  GroupOfGBAtoms* molecule;
  molecule = GBParsePQR(fp, radiusType);
  cout << molecule->numOfAtoms << " atoms" << endl;
  fclose(fp);
  
  fp = loadFile_read(rawnFile);
  Surface* surface;
  surface = ParseGeometry(fp);
  fclose(fp);
  
  start = getTime2();
  surface->Triangle_Bezier_Patch();
  total_time_1 = getTime2() - start;
  
  start = getTime2();
  molecule->getBornRadii(surface);
  total_time_2 = getTime2() - start;
  
  PolEnergy *m_Gpol= new PolEnergy(molecule);
  start = getTime2();
  m_Gpol->getPolEnergy(epsilon, threshold); // default? eps = 0.1, theshold = 100
  total_time_3 = getTime2() - start;
  
  start = getTime2();
  PolForce *m_Fpol = new PolForce();
  m_Fpol->getPolForce(molecule, surface);
  force_time = getTime2() - start;
  
  const double surface_tension = 19*0.001; // Simonson94 or 7.2*10^(-3) in Still90

  double nonpol_energy = surface_tension*area;
  
  double sol_energy = m_Gpol->pol_energy + nonpol_energy;
  
  char outFile[256];
  sprintf(outFile, "%s.force",outputPrefix);
  
  fp = loadFile_write(outFile);

  fprintf(fp, "Force Time: %f\n", force_time);
  for (int i = 0; i < m_Fpol->force.size(); i++)
    fprintf(fp, "%f %f %f\n", m_Fpol->force[i].fx, m_Fpol->force[i].fy, m_Fpol->force[i].fz);
  fclose(fp);


  // visualize the force...

  double *F = new double[molecule->numOfAtoms];
  int *order = new int[molecule->numOfAtoms];
  double fx, fy, fz, t, r, x, y, z;
  int i, j, N;
  
  fp = loadFile_read(outFile);
  for (i = 0; i < molecule->numOfAtoms; i++) {
    fscanf(fp, "%lf %lf %lf %lf\n", &fx, &fy, &fz, &t);
    F[i] = sqrt(fx*fx + fy*fy + fz*fz);
    order[i] = i;
  }
  quicksort(0, F, order, molecule->numOfAtoms);

  bool pick;
  
  // arand: why divide by 10?  
  N = (int)molecule->numOfAtoms/10;
  printf("N = %d\n", N);


  sprintf(outFile, "%s.rawnc",outputPrefix);

  fp = loadFile_write(outFile);
  fprintf(fp, "%d %d\n", surface->numbpts, surface->numbtris);
  // points
  for (i = 0; i < surface->numbpts; i++) {
    x = surface->m_Vertices[i]->Point[0];
    y = surface->m_Vertices[i]->Point[1];
    z = surface->m_Vertices[i]->Point[2];
    pick = false;
    
    for (j = 0; j < N; j++) {
      fx = molecule->m_Atoms[order[j]]->position[0];
      fy = molecule->m_Atoms[order[j]]->position[1];
      fz = molecule->m_Atoms[order[j]]->position[2];
      r = molecule->m_Atoms[order[j]]->radius;
      double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
      if (d < (r+0.8)*(r+0.8)) {
	pick = true;
	fprintf(fp, "%f %f %f %f %f %f 1.0 0.0 0.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
	break;
      }
    }
    if (pick == true) continue;
    
    for (j = 9*N; j < molecule->numOfAtoms; j++) {
      fx = molecule->m_Atoms[order[j]]->position[0];
      fy = molecule->m_Atoms[order[j]]->position[1];
      fz = molecule->m_Atoms[order[j]]->position[2];
      r = molecule->m_Atoms[order[j]]->radius;
      double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
      if (d < (r+0.2)*(r+0.2)) {
	pick = true;
	fprintf(fp, "%f %f %f %f %f %f 0.0 0.0 1.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
	break;
      }
    }
    
    if (pick == false) {
      //	printf("something wrong\n");
      fprintf(fp, "%f %f %f %f %f %f 0.9 0.9 0.9\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
    }
  }
  
  // face
  for (i = 0; i < surface->numbtris; i++) {
    fprintf(fp, "%d %d %d\n", surface->m_Faces[i]->Index[0], surface->m_Faces[i]->Index[1], surface->m_Faces[i]->Index[2]);
  }
  fclose(fp);
  
}

