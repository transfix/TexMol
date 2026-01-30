#ifndef CCV_TEXMOL_DOCKING_DOMAIN_H
#define CCV_TEXMOL_DOCKING_DOMAIN_H

#include <stdio.h>
#include <vector>
#include <UsefulMath/Matrix.h>
#include <Docking/Complex.h>

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

class SimpleVolumeData;

namespace Docking
{
	class Segment;
	class FlexibleLoop;
	class DomainComplex;

	class Domain
	{
		public:
			Domain(DomainComplex* domainComplex);
			virtual ~Domain();
			int getNumSegments() const;
			bool parseProperties(const char* line);
			bool addNewSegment(const char* line, std::vector<int> residueList);   // what if input segment is out of range, when we are lower level in tree ?
			void print(FILE* fp, std::vector<int> residueList, int level);
			void read(FILE* fp, std::vector<int> residueList, int level);
			std::vector<Domain*> m_Domains; // children
			bool addNewDomain(Domain* domain, int reqLevel, int curLevel);
			bool isSubDomain(Domain* domain);
			bool addLoopToDomain(FlexibleLoop* flexibleLoop, int domainId, int reqLevel, int curLevel);
			bool printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, int levelReq, int curLevel);
			bool getAtoms(std::vector<PDBParser::Atom*> *atomList);
			bool translate(CCVOpenGLMath::Vector disp);
			bool rotate(CCVOpenGLMath::Matrix rotationMatrix);
			CCVOpenGLMath::Vector getCenter();
			void write(FILE* fp);
			std::vector<Segment*> m_Segments; // these are the segments that make up this domain only
			std::vector<FlexibleLoop*> m_FlexibleLoops;
			int m_Id;
			int m_NumberOfResidues;
			double m_SimilarityIndex;
			DomainComplex* m_DomainComplex;
	};
};

#endif
