#ifndef CCV_TEXMOL_DOCKING_APBS_FORMAT_RESIDUE_H
#define CCV_TEXMOL_DOCKING_APBS_FORMAT_RESIDUE_H

#include <vector>
#include <UsefulMath/Matrix.h>

namespace PDBParser
{
	class Atom;
}

namespace Docking
{
	// We expect APBS to be consistant. if it changes atom ordering someday, this will have to be rewritten
	class ApbsFormatResidue
	{
		public:
			ApbsFormatResidue();
			virtual ~ApbsFormatResidue();
			// We need: x, y, z only, radius charge etc can be looked up.
			// given residue, compute chi1, look up table, compute all relevant rotamers
			bool constructRotamers(std::vector<PDBParser::Atom*> residue, float** rotamers, int* numRotamers);

		protected:
			bool correctOrdering(std::vector<PDBParser::Atom*> residue);
			double getAngle(std::vector<PDBParser::Atom*> residue, int angleType);
			void copy(float* atomPosCopy, std::vector<PDBParser::Atom*> residue);
			bool applyRotation(std::vector<PDBParser::Atom*> atomPosCopy, int angle, int rotamerTableIndex);
			CCVOpenGLMath::Matrix getTransformation(std::vector<PDBParser::Atom*> atomPosCopy, int angle, double delChi);
			bool getRotationVector(std::vector<PDBParser::Atom*> atomPosCopy, int angle, double* vecx, double* vecy, double* vecz);
			bool getRotationOrigin(std::vector<PDBParser::Atom*> atomPosCopy, int angle, double* origx, double* origy, double* origz);
			std::vector<int> m_AtomsCategory;
			int getNumberOfRotamers(int residueIndex, std::vector<PDBParser::Atom*> residue, int* startRotamerIndex, int* endRotamerIndex);
			int getCategory(const char* atomname);
	};
};

#endif
