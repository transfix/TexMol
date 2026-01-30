#ifndef POLFORCE_H
#define POLFORCE_H

#include "groupOfAtoms.h"
#include "surface.h"
#include <vector>

namespace FUNCTION
{
typedef struct force_func
{
	double fx, fy, fz;
}FORCEFUNC;

class PolForce {
public:
	PolForce();
	void getPolForce(MOLECULE::GroupOfGBAtoms *molecule, GBGEOMETRY::Surface *surface);
	std::vector<FORCEFUNC> force;

private:
	// void derivGpolOverR(double *Gpol_Ri, MOLECULE::GroupOfGBAtoms *molecule);
	void derivRi_xk(MOLECULE::GroupOfGBAtoms *molecule, int k, double *Rk_xk_2ndInt, const double w, const double pi, double *Ri_xk);
	void derivRi_xk_2ndInt(MOLECULE::GroupOfGBAtoms *molecule, GBGEOMETRY::Surface *surfac, double *Rk_xk_2ndInt);
	void derivRi_xk_1stInt(MOLECULE::GroupOfGBAtoms *molecule, int i, int k, const double w, const double pi, double *Ri_xk_1stInt);
	void evaluateIntegrand(MOLECULE::GroupOfGBAtoms *molecule, int i, int k, double r, double theta, double phi, const double w, double *f);
	double Density(MOLECULE::GroupOfGBAtoms *molecule, int p, int *k_neighbor, double *rhom, int k_neighbor_size, int N, int depth);
	double evaluateAtomicDensity(MOLECULE::GroupOfGBAtoms *molecule, double *x, int j, const double w);
	double derivGikOverDistance(MOLECULE::GroupOfGBAtoms *molecule, int i, int k);
	void deriv_rik_xk(MOLECULE::GroupOfGBAtoms *molecule, int i, int k, double *rik_xk);
	double derivGijOverRi(MOLECULE::GroupOfGBAtoms *molecule, int i,  int j);
	double derivGiiOverRi(MOLECULE::GroupOfGBAtoms *molecule, int i);
	double dotProduct(double *x1, double *x2);
};
}
#endif // POLFORCE_H
