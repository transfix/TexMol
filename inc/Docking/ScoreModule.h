#ifndef CCV_TEXMOL_DOCKING_SCORE_MODULE_H
#define CCV_TEXMOL_DOCKING_SCORE_MODULE_H

#include <vector>
#include <UsefulMath/Matrix.h>
#include <Docking/Complex.h>

#include <vector>

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

class SimpleVolumeData;

namespace Docking
{
	static const char* potentialMapName[9] =
	{
		"VDW C potential grid",
		"VDW N potential grid",
		"VDW O potential grid",
		"VDW S potential grid",
		"VDW H potential grid",
		"HBond",
		"Electrostatic potential grid",
	};

	static const char* potentialName[9] =
	{
		"VDW_C",
		"VDW_N",
		"VDW_O",
		"VDW_S",
		"VDW_H",
		"HBond",
		"Elec",
	};

	typedef struct _ScoringTable
	{
		double r_eq;
		double epsilon;
	} ScoringTable;

	static ScoringTable vdwScoringTable[25] =
	{
		{ 4.00, 0.150 },
		{ 3.75, 0.155 },
		{ 3.60, 0.173 },
		{ 4.00, 0.173 },
		{ 3.00, 0.055 },

		{ 3.75, 0.155 },
		{ 3.50, 0.160 },
		{ 3.35, 0.179 },
		{ 3.75, 0.179 },
		{ 2.75, 0.057 },

		{ 3.60, 0.173 },
		{ 3.35, 0.179 },
		{ 3.20, 0.200 },
		{ 3.60, 0.200 },
		{ 2.60, 0.063 },

		{ 4.00, 0.173 },
		{ 3.75, 0.179 },
		{ 3.60, 0.200 },
		{ 4.00, 0.200 },
		{ 3.00, 0.063 },

		{ 3.00, 0.055 },
		{ 2.75, 0.057 },
		{ 2.60, 0.063 },
		{ 3.00, 0.063 },
		{ 2.00, 0.020 }
	};

	static ScoringTable hBondScoringTable[3] =
	{
		{ 1.90, 5.00 },
		{ 1.90, 5.00 },
		{ 2.50, 1.00 }
	};

	class ScoreModule
	{
		public:
			ScoreModule();
			virtual ~ScoreModule();

			bool computeScoringGrids(PDBParser::GroupOfAtoms* molecule, double ligandSize, double gridSpacing, const char* gridBaseFileName);
			bool computeScores();

		protected:
			bool getVolumeDimensions(unsigned int* dim, float* min, float* max, PDBParser::GroupOfAtoms* molecule, double ligandSize, double gridSpacing);
			bool getPotential(float* potentialMap, int index, unsigned int* dim, float* min, float* max, std::vector<PDBParser::Atom*> atomList);
			bool getAtomScoringParameters(PDBParser::Atom* atom, int index, int* m, int* n, double* epsilon, double* r_eq, int parentAtomType);
			double getPotential(int index, int m, int n, double epsilon, double r_eq, double distance, double cosangle, double charge);
			bool getNOSatoms(PDBParser::Atom* atom, float* prevNPos, float* prevOPos, float* prevSPos);
			double getDist(PDBParser::Atom* atom, float xpos, float ypos, float zpos);
			double getAngle(float xpos, float ypos, float zpos, float* HParentPos, float* atomPos);
			int getHAtomParentPos(std::vector<PDBParser::Atom*> residueBeginning, PDBParser::Atom* atom, float* HParentPos);
			void getNextAminoAcid(std::vector<PDBParser::Atom*> atomList, int index,  std::vector<PDBParser::Atom*> *residueBeginning);
			bool getPos(PDBParser::Atom* atom, const char* HAtomName, const char* residueName, const char* parentName, float* HParentPos, std::vector<PDBParser::Atom*> residueBeginning);
			double getSmoothedPotential(float* potentialMap, int centerX, int centerY, int centerZ, int xWin, int yWin, int zWin, unsigned int* dim);
	};

};

#endif
