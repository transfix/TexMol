#ifndef CCV_TEXMOL_DOCKING_FLEXIBLE_LOOP_H
#define CCV_TEXMOL_DOCKING_FLEXIBLE_LOOP_H

#include <vector>
#include <UsefulMath/Matrix.h>
#include <Docking/Complex.h>
#include <Docking/Segment.h>

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

class SimpleVolumeData;

namespace Docking
{
	class FlexibleLoop : public Segment
	{
		public:
			FlexibleLoop(int startResidueIndex, int endResidueIndex, int domain1, int domain2, std::vector<int> residueList, DomainComplex* domainComplex);
			virtual ~FlexibleLoop();
			bool printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, double red, double green, double blue);
	};
};

#endif
