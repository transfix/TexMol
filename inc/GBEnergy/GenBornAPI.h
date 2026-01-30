#ifndef GB_API_H
#define GB_API_H


// arand, 10-28-2010
// NFFT-based GB functions for calling in TexMol

double GBEnergy(char * pqrFile, char * rawnFile, char * outputPrefix,
		double area, int surfType, double epsilon, double threshold);

void GBForce(char * pqrFile, char * rawnFile, char * outputPrefix,
	     double area, int surfType, double epsilon, double threshold);


#endif
