#ifndef GROUPOFATOMS_H
#define GROUPOFATOMS_H

#include <vector>
#include <GBEnergy/surface.h>

namespace MOLECULE
{
	class GBAtom;
	class GroupOfGBAtoms
	{
	public:
		GroupOfGBAtoms();
		GroupOfGBAtoms(GroupOfGBAtoms* m);
		void addAtom(GBAtom* currentAtom);
                void getBornRadii(GBGEOMETRY::Surface *surface);
		void getNeighbors();
		void assignBRtoChild(GroupOfGBAtoms *mol);

                //void getPolEnergy();
		//void getPolForce();
		std::vector<GBAtom*> m_Atoms;

		int numOfAtoms;
		int numOfRes;
		double total_charge;
		bool *neighbor;
                //double pol_energy;
		//PolForce *m_polForce;

	private:
		bool overlap(int i, int j);
		double dotProduct(double *u, double *v);
        };
}

#endif //GROUPOFATOMS_H
