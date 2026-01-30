#ifndef CCV_TEXMOL_DOCKING_SEGMENT_H
#define CCV_TEXMOL_DOCKING_SEGMENT_H

#include <stdio.h>
#include <vector>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Vector.h>
#include <Docking/Complex.h>

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

namespace CCVOpenGLMath
{
	class Vector;
}

class SimpleVolumeData;

namespace Docking
{
	class DomainComplex;

	class Segment
	{
		public:
			Segment(int startResidueIndex, int endResidueIndex, int domain1, int domain2, std::vector<int> residueList, DomainComplex* domainComplex);   // ok to assume always in a chain ?
			virtual ~Segment();
			static bool getStartEndIndices(std::vector<int> residueList, int chainId, int startRes, int endRes, int* startIndex, int* endIndex);
			bool getEndPoints(std::vector<int> residueList, int* chainId, int* startResidue, int* endResidue);
			void print(FILE* fp, std::vector<int> residueList);
			void read(FILE* fp, std::vector<int> residueList);
			void computeAtomList(std::vector<int> residueList);
			bool getAtoms(std::vector<PDBParser::Atom*> *atomList);
			bool appendAtoms(std::vector<PDBParser::Atom*> *atomList);
			bool printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, double red, double green, double blue, double alpha);
			bool getEndPoints(CCVOpenGLMath::Vector* p1, CCVOpenGLMath::Vector* p2);
			bool getMidPoint(CCVOpenGLMath::Vector* p);
			bool translate(CCVOpenGLMath::Vector disp);
			bool translate(CCVOpenGLMath::Vector disp, int fixedDomainId);
			bool rotate(CCVOpenGLMath::Matrix rotationMatrix);
			void getSum(float* x, float* y, float* z, int* n);
			void write(FILE* fp);
			int m_Domain1;
			int m_Domain2;
			int m_StartResidueIndex;
			int m_EndResidueIndex;
			std::vector<PDBParser::Atom*> m_AtomList;
			Docking::DomainComplex* m_DomainComplex;
			bool computeEndPoints(CCVOpenGLMath::Vector* e1, CCVOpenGLMath::Vector* e2);
			bool computeMidPoint(CCVOpenGLMath::Vector* m, std::vector<int> residueList);
	};
};

#endif
