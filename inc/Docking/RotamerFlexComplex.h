#ifndef CCV_TEXMOL_DOCKING_ROTAMER_FLEX_COMPLEX_H
#define CCV_TEXMOL_DOCKING_ROTAMER_FLEX_COMPLEX_H

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

	class RotamerFlexComplex : public DockComplex
	{
		public:
			RotamerFlexComplex();
			RotamerFlexComplex(PDBParser::GroupOfAtoms* protein, PDBParser::GroupOfAtoms* ligand);
			virtual ~RotamerFlexComplex();
			bool improveInterface(const char* unboundLigandFileName, int numberOfTransformations, double distanceCutoff,
								  const char* transformationsFilename, const char* outputFilename, bool flexDock, int numNewPredictions);

		protected:
			bool improveInterfaceGivenTransformation(FILE* fpOut, CCVOpenGLMath::Matrix transformation,
					std::vector<int> unboundLigandInterfaceAtomIndex, SimpleVolumeData* density, SimpleVolumeData* elecField,
					CCVOpenGLMath::Vector c1, CCVOpenGLMath::Vector c2, int numNewPredictions, double distanceCutoff);
			bool findInterfaceResidues(std::vector<PDBParser::Atom*> transUnbLigandAtoms, double distanceCutoff,
									   SimpleVolumeData* density, std::vector<int>* idxUnbLigIntRes);
			void transform(std::vector<PDBParser::Atom*> unboundLigandAtomList, CCVOpenGLMath::Matrix transformation, std::vector<PDBParser::Atom*>* transUnbLigandAtoms);
			double getRMSD(std::vector<PDBParser::Atom*> transUnboundLigandAtomList, std::vector<int> unboundLigandInterfaceAtomIndex,
						   double* score, SimpleVolumeData* density, SimpleVolumeData* elecField);
			void set(std::vector<PDBParser::Atom*> transUnbLigandAtoms, std::vector<PDBParser::Atom*>* copyTrUnbLigandAtoms);
			void copy(std::vector<PDBParser::Atom*> transUnbLigandAtoms, std::vector<PDBParser::Atom*>* copyTrUnbLigandAtoms);
			bool getUnbLigResiduesInfo();
			void printInfo();
			bool computeUnbLigIntResScores(std::vector<int> unbLigIntRes, std::vector<PDBParser::Atom*> transUnbLigandAtoms, double* curScore,
										   SimpleVolumeData* density, SimpleVolumeData* elecField);
			bool pickAndTransformRotamers(float* rotPositions, CCVOpenGLMath::Matrix transformation, int predictionId,
										  std::vector<int> unbLigIntRes, double* rotScore,
										  SimpleVolumeData* density, SimpleVolumeData* elecField,
										  std::vector<PDBParser::Atom*> transUnbLigandAtoms, double* curScore);
			int getGoodRotamersSorted(int* rotamerSortedIndex, double* curScore, double* rotScore, int nIntRes);
			bool update(std::vector<PDBParser::Atom*> copyTrUnbLigandAtoms, float* rotPositions, std::vector<int> unbLigIntRes, int rotIndex, int* numAtoms);

			double getRMSD(std::vector<PDBParser::Atom*> v1, std::vector<PDBParser::Atom*> v2);
			double shapeContribution(double density);
			double elecContribution(double elecValue, PDBParser::Atom* atom);
			double getAtomScore(PDBParser::Atom* atom, SimpleVolumeData* density, SimpleVolumeData* elecField);

			std::vector<PDBParser::Atom*> m_UnboundLigandAtomList;
			int* m_Residue2AtomIndex;
			int* m_Atom2ResidueIndex;
			std::vector<float*> m_Rotamers; // for each residue, we have a combined array of rotamers, given by list of xyz atom positions
			std::vector<int> m_NumRotamers; // number of rotamers for a given residue
			std::vector<int>* m_IntersectingResidues;
			int m_NumResidues;
	};

};

#endif
