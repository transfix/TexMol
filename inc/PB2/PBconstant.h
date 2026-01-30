#ifndef _PBCONSTANT
#define _PBCONSTANT

#include "PB2/PBunit.h"

#define N_A  6.022045000e+23
#define e_c  4.803242384e-10
#define k_B  1.380662000e-16

class PBconstant{

public:

  //static const double N_A = 6.022045000e+23; //
  //static const double e_c = 4.803242384e-10; //unit of electron
  //static const double k_B = 1.380662000e-16; //Debye-Huckel screening length at bulk solvent

	/* arbitrary parameters */
	double xkappa;//inverse of Debye-Huckel screening length
	double deblen;
	double zkappa2;
	double zmagic;

	double bulkIonicStrength;
	double maxIonRadius;
	double netCharge;


	/* Input */
	double sdie; //dieletric constant of solvent
        double pdie; //dieletirc contatnt of molecules
        double temperature; // temperature
        int numIon;

	double *ionQ; //mobile ion species charge (in e_c)
	double *ionConc; //mobile ion species concentration (in M)
	double *ionRadii; //mobile ion species radius (in Angstrom)

	/* Compute parameters: (from APBS)
	 *
	 * kappa^2 = (8 pi N_A e_c^2) I_s / (1000 eps_w k_B T)
	 * kappa   = 0.325567 * I_s^{1/2}   angstroms^{-1}
	 * deblen  = 1 / kappa
	 *         = 3.071564378 * I_s^{1/2}   angstroms
	 * \bar{kappa}^2 = eps_w * kappa^2
	 * zmagic  = (4 * pi * e_c^2) / (k_B T)   (we scale the diagonal later)
	 *         = 7046.528838
	 */

	PBconstant(){}

	PBconstant(double sdie, double pdie, double temperature, int numIon,
			double *ionQ, double *ionConc, double *ionRadii);

	void computeParameters();

};

#endif
