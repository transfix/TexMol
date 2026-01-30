#include <math.h>
#include <stdio.h>
#include "PB2/PBconstant.h"


PBconstant::PBconstant(double dieE, double dieI, double temp, int nIon,
		double *iQ, double *iConc, double *iRadii){

	sdie = dieE;// * Vunit_eps0;

	//printf("dielectric constant of solvent : %f\n",sdie);
	pdie = dieI;// * Vunit_eps0;
	//printf("dielectric constant of molecules : %f\n",pdie);
	temperature = temp;
	//printf("temperature : %f\n",temperature);
	numIon = nIon;
	//printf("number of types of ions : %d\n", numIon);

	ionQ = new double[numIon];
	ionConc = new double[numIon];
	ionRadii = new double[numIon];

	for (int i=0; i< numIon; i++) {
		ionQ[i] = iQ[i];
                ionConc[i] = iConc[i];
                ionRadii[i] = iRadii[i];
	}
	computeParameters();
}

void PBconstant::computeParameters(){

	const double pi  = 4. * atan(1.);
	bulkIonicStrength = 0.0;
	maxIonRadius = 0.0;
        netCharge = 0.0;

        for (int i=0; i< numIon; i++) {
	        if (ionRadii[i] > maxIonRadius)
			maxIonRadius = ionRadii[i];
	        bulkIonicStrength += 0.5*ionConc[i]*ionQ[i]*ionQ[i];
	        netCharge += (ionConc[i]*ionQ[i]);
	}

	//printf("bulk ionic strength: %f\n", bulkIonicStrength);
	//printf("net charge: %f\n", netCharge);

	if (temperature == 0.0) {
       		return;
	}
	if (bulkIonicStrength == 0.) {
		xkappa  = 0.;
		deblen  = 0.;
		zkappa2 = 0.;
	} else {
	  //printf("bulk:%f pi:%f N_A:%f e_c:%f sdie:%f T:%f\n"
	  //	,bulkIonicStrength,pi,N_A,e_c,sdie,temperature);
	  	xkappa  = sqrt( bulkIonicStrength * 1.0e-16 *
	  		((8.0 * pi * N_A * e_c*e_c) /
	           (1000.0 * sdie * k_B * temperature)));
		//xkappa  = sqrt( (2.0 * bulkIonicStrength *
		//	N_A * e_c*e_c) /
	        //    (Vunit_eps0 * sdie * k_B * temperature));
	        deblen  = 1. / xkappa;
	        zkappa2 = sdie * xkappa * xkappa;
	}
	//zmagic  = ((4.0 * pi * e_c*e_c) / (k_B * temperature)) * 1.0e+8;
	zmagic = Vunit_ec/(Vunit_eps0*1.0e-10);
	//zmagic = Vunit_ec/(1.0e-10);
	//zmagic = e_c;
	//zmagic  = (4.0 * pi * e_c) * 1.0e+8;


	//printf("kappa (Debye-Huckel screening factor): %f\n", xkappa);
	//printf("inverse of kappa: %f\n", deblen);
	//printf("kappa square: %f\n", zkappa2);
	//printf("zmagic: %f\n",zmagic);
}

