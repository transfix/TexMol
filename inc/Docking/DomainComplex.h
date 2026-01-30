#ifndef CCV_TEXMOL_DOCKING_DOMAIN_COMPLEX_H
#define CCV_TEXMOL_DOCKING_DOMAIN_COMPLEX_H

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
	const double CONNECTOR_ALPHA = 0.2;
	const double FLEXIBLE_LOOP_ALPHA = 0.2;
	//const double DOMAIN_ALPHA = 1.0;
	const double DOMAIN_ALPHA = 0.2;
	const double COLOR_EPS = 0.1;
	class Domain;
	class Connector;
	class FlexibleLoop;
	class Flexibility;
	class DomainComplex
	{
		public:
			DomainComplex(PDBParser::GroupOfAtoms* molecule);
			virtual ~DomainComplex();
			void clearData();
			// First create DomainComplex
			// Next add additional levels of domains if required
			// Last, call classifyFlexibleSegments
			bool readDomainFinderOpt(const char* domainListFileName, int level);
			bool classifyFlexibleSegments();
			bool computeFlexibility();
			bool printColorMap(const char* fname, int level);
			bool printFCC(const char* fccFileName);
			bool readFCC(const char* filename);
			bool getConformations(const char* FCCFileName, int level, const char* newLevelConformationBaseFileName, int recommendedNumber);
			void computeResidueList(PDBParser::GroupOfAtoms* molecule, int chainId, int subgroupindex);
			void addDomainIds(int* domainId, int curLevel, int reqLevel, Domain* domain);
			void getDomainIds(int* domainId, int level);
			void addNewDomain(Domain* domain, int reqLevel, int curLevel);
			bool getNextUnclassifiedResidue(int* curResidueIndex, int* domainId, int* begDomainId);
			bool getLastUnclassifiedResidue(int* lastResidueIndex, int* domainId, int* endDomainId);
			bool addLoopToDomain(FlexibleLoop* flexibleLoop, int domainId, int reqLevel);
			Flexibility* computeDomainInteraction(Domain* d1, Domain* d2);
			Domain* getDomain(int id, int level);
			PDBParser::GroupOfAtoms* getResidue(int residueListIndex);
			PDBParser::GroupOfAtoms* getResidue(int chainId, int resNum, PDBParser::GroupOfAtoms* molecule, int curChain);
			static bool getResidueData(char* resName, char* actualChainId, int* chainNum, int chainId, int resNum, PDBParser::GroupOfAtoms* molecule, int curChain);
			static void getNearbyColor(double baseSegmentRed, double baseSegmentGreen, double baseSegmentBlue, int randVal, double* red, double* green, double* blue);
			std::vector<Connector*> m_Connectors; // level 1 followed by level 2....
			std::vector<int> m_ResidueList; // <residue number, chain id>
			std::vector<Flexibility*> m_Flexibility;
			std::vector<Domain*> m_Domains;

		protected:
			bool getConformations(int level, const char* newLevelConformationBaseFileName, int curFlexibility, PDBParser::GroupOfAtoms* molecule, int* counter, int recommendedNumber);
			bool allocateNumberOfDiscreteFlexibilitiesPerInterface(int recommendedNumber);
			PDBParser::GroupOfAtoms* m_Molecule;
			int m_MaxLevelId;
	};
};

#endif
