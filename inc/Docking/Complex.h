#ifndef CCV_TEXMOL_DOCKING_COMPLEX_H
#define CCV_TEXMOL_DOCKING_COMPLEX_H

#include <vector>
#include <cstdio>
#include <UsefulMath/Matrix.h>

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

class SimpleVolumeData;

namespace Docking
{
	class DockComplex
	{
		public:
			DockComplex(PDBParser::GroupOfAtoms* protein, PDBParser::GroupOfAtoms* ligand);
			DockComplex();
			virtual ~DockComplex();
			bool init(const char* inputFile1, const char* inputFile2, bool deleteWater);
			bool init(PDBParser::GroupOfAtoms* protein, PDBParser::GroupOfAtoms* ligand, bool deleteWater);
			double getInterfaceArea(double distanceCutOff);
			bool createDockingInputFiles(const char* outputFile1, const char* outputFile2, bool printMiscFiles, double distanceCutOff=2);
			// call compute interface ligand atoms first
			// then get rmsd with a transformation for the ligand will compute how far away this transformation is from the ideal.
			bool computeLigandInterfaceAtoms(double distanceCutoff);
			double getRMSD(CCVOpenGLMath::Matrix transformation);
			double getRMSD(CCVOpenGLMath::Matrix transformation, std::vector<PDBParser::Atom*> unboundLigandAtomList, std::vector<int> unboundLigandInterfaceAtomIndex);
			// or call once with lots of transformation in a file
			bool getInterfaceRMSD(const char* transformations, int numberOfTransformations, double distanceCutoff, const char* outputFilename);
			bool getInterfaceRMSD(const char* transformations, int numberOfTransformations, double distanceCutoff, const char* outputFilename, const char* unboundLigandFileName);
			bool getInterfaceArea(const char* transformations, int numberOfTransformations, double distanceCutoff, const char* outputFilename);
			bool getInterface(double distanceCutoff, const char* outputFilename);
			bool improveInterface(const char* unboundLigandFileName, int numberOfTransformations, double distanceCutoff,const char* transformationsFilename, const char* outputFilename, bool flexDock);
			int findCorrespondingInterfaceAtoms(std::vector<int> *unboundLigandInterfaceAtomIndex, std::vector<PDBParser::Atom*> unboundLigandAtomList);
			int getCorrespondingAtomIndex(PDBParser::Atom* atom, std::vector<PDBParser::Atom*> unboundLigandAtomList);
			bool getUnboundLigandAtoms(const char* unboundLigandFileName, bool deleteWater, std::vector<PDBParser::Atom*>* unboundLigandAtomList);

			PDBParser::GroupOfAtoms* getProtein()
			{
				return m_Protein;
			}
			PDBParser::GroupOfAtoms* getLigand()
			{
				return m_Ligand;
			}

		protected:
			bool printScoreAndRMSD(std::FILE* fpOut, CCVOpenGLMath::Matrix transformation, std::vector<PDBParser::Atom*> unboundLigandAtomList,
								   std::vector<int> unboundLigandInterfaceAtomIndex, SimpleVolumeData* density, SimpleVolumeData* elecField,
								   CCVOpenGLMath::Vector c1, CCVOpenGLMath::Vector c2);
			bool printScoreAndRMSD(std::FILE* fpOut, CCVOpenGLMath::Matrix transformation, std::vector<PDBParser::Atom*> unboundLigandAtomList,
								   std::vector<int> unboundLigandInterfaceAtomIndex, SimpleVolumeData* density, SimpleVolumeData* elecField);
			SimpleVolumeData* getProteinDensity();
			SimpleVolumeData* getProteinElecField();
			CCVOpenGLMath::Vector getCenter(PDBParser::GroupOfAtoms* molecule);
			CCVOpenGLMath::Vector getCenter(std::vector<PDBParser::Atom*> unboundLigandAtomList);
			// variables to compute ligand rmsd position for diff transformations
			std::vector<PDBParser::Atom*> m_LigandAtomList;
			std::vector<int> m_LigandInterfaceAtom;
			PDBParser::GroupOfAtoms* m_Protein;
			PDBParser::GroupOfAtoms* m_Ligand;
	};
};

#endif
