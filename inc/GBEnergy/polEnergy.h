#ifndef POLENERGY_H
#define POLENERGY_H

#include <vector>
#include "GBEnergy/groupOfAtoms.h"

namespace FUNCTION
{
typedef struct _group_
{
	int* member;
	int numOfMember;
} group;

class PolEnergy {
public:
	PolEnergy();
	PolEnergy(MOLECULE::GroupOfGBAtoms* molecule);
	~PolEnergy();
	void getPolEnergy(double eps, int theshold);
	double pol_energy;
private:
	void groupingAtoms(double eps, double R_min, double R_max);
	std::vector<group> groups;
	MOLECULE::GroupOfGBAtoms* molecule;
};
}
#endif // POLENERGY_H
