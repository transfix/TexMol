#ifndef GB_ATOM_H
#define GB_ATOM_H

#include "GBEnergy/surface.h"

namespace MOLECULE{
	typedef struct densityFunc
	{
	    double para_c;
	    double coeffOfr[4];
	}density;

        enum RADIUS_TYPE {VDW_RADIUS, PQR_RADIUS};

	class GBAtom
	{
	public:
		GBAtom();
		GBAtom(GBAtom*);
		void setAtomID(char *atom_name);
		void setVDWradius();
		void setMass();
		void setAtomName(char *atom_name);
		void setResName(char *res_name);
		void setChainID(char chain_id);
		void setIndexInPQR(int i);
		void setResIndex(int res_seq);
		void setPosition(double *p);
		void setCharge(double q);
		void setDefinedRadius(double r);
		
		void getData(int i, char* atom_name, char* res_name, char chainID, int res_seq, double* p, double q, double r, RADIUS_TYPE radiusType);
                void getData(double *p, double q, double r, char *res_name);
                void getBornRadius(GBGEOMETRY::Surface *surface);
		
		int indexInPQR; // start from 0
		char atomID;   // name

                char atomName[10];
                char resName[5];
		char chainID;
                int resSeq;

		double position[3];
		double radius;
		double charge;
                double Born_radius;

		double mass;
		int *child, numOfChildren;

		density *rho;	
        private:
		void getDensityInfo();
                double DotProduct(double*, double*);
	};
}

#endif //ATOM_H

