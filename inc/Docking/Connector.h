#ifndef CCV_TEXMOL_DOCKING_CONNECTOR_H
#define CCV_TEXMOL_DOCKING_CONNECTOR_H

#include <stdio.h>
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
	class DomainComplex;

	class Connector : public Segment
	{
		public:
			Connector(int startResidueIndex, int endResidueIndex, int domain1, int domain2, int level, std::vector<int> residueList, DomainComplex* domainComplex);
			virtual ~Connector();
			void print(FILE* fp, std::vector<int> residueList);
			void read(FILE* fp, std::vector<int> residueList);
			bool printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, int level);
			bool rotate(CCVOpenGLMath::Matrix rotationMatrix, int fixedDomainId);
			bool rotate(CCVOpenGLMath::Matrix rotationMatrix);
			int m_Level;
	};
};

#endif
