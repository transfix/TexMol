#include <vector>
#include <string.h>
#include <stdio.h>

#include "GBEnergy/CoarseGrain/coarseGrain.h"
#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"

using namespace MOLECULE;
using namespace std;

CoarseGrain::CoarseGrain()
{

}

CoarseGrain::CoarseGrain(GroupOfGBAtoms *m)
{
	molecule = m;
}

GroupOfGBAtoms* CoarseGrain::CoarseGrainMolecule(GroupOfGBAtoms *m)
{
	int resIndex;
	int head, tail;
	int i;
	GroupOfGBAtoms *groupOfBeads = new GroupOfGBAtoms();

	molecule = m;
	head = 0;
	tail = 0;
	resIndex = molecule->m_Atoms[0]->resSeq;
	for (i = 1; i < molecule->numOfAtoms; i++)
	{
		if (molecule->m_Atoms[i]->resSeq == resIndex)
			tail = i;
		else
		{
			atomClustering(groupOfBeads, head, tail);
			resIndex = molecule->m_Atoms[i]->resSeq;
			head = i;
			tail = i;
		}
	}
	atomClustering(groupOfBeads, head, tail);
	groupOfBeads->numOfAtoms = groupOfBeads->m_Atoms.size();
	groupOfBeads->numOfRes = molecule->numOfRes;
	CoarseGrainGeometry(groupOfBeads);
//	CoarseGrainCharge(groupOfBeads); This step should be done after the geometry optimization

/*
	double Q, q;
	Q = 0.0; q = 0.0;
	for (i = 0; i < groupOfBeads->numOfAtoms; i++)
		Q += groupOfBeads->m_Atoms[i]->charge;
	for (i = 0; i < molecule->numOfAtoms; i++)
		q += molecule->m_Atoms[i]->charge;

	printf("%f %f\n", q, Q);
*/
	return groupOfBeads;
}
