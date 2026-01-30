#include <Docking/ApbsFormatResidue.h>
#include <Docking/DunbrackLibrary.h>
#include <math.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GOAMolGeometry.h>
#include <PDBParser/ResidueInformation.h>
#include <stdio.h>
#include <string.h>
#include <UsefulMath/Quaternion.h>

using PDBParser::Atom;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;
using CCVOpenGLMath::Quaternion;
using namespace Docking;

bool isZero(double val)
{
	double eps = 0.000000001;
	if (fabs(val) < eps)
	{
		return true;
	}
	return false;
}

ApbsFormatResidue::ApbsFormatResidue()
{
}

ApbsFormatResidue::~ApbsFormatResidue()
{
}

bool strcmpLWS(const char* S1, const char* wsS2)
{
	char S2[1024];
	S2[0] = '\0';
	{
		int l2 = strlen(wsS2);
		int c = 0;
		for (int i=0; i<l2; i++)
		{
			if (wsS2[i] != ' ')
			{
				S2[c] = wsS2[i];
				c++;
				S2[c] = '\0';
			}
			else
			{
				// let us assume wierd in between spaces dont occur
			}
		}
	}
	if (strcmp(S1, S2) == 0)
	{
		return true;
	}
	return false;
}

int getResidueIndex(const char* residueName)
{
	if (strcmp(residueName, "ARG") == 0)
	{
		return 0;
	}
	if (strcmp(residueName, "ASN") == 0)
	{
		return 1;
	}
	if (strcmp(residueName, "ASP") == 0)
	{
		return 2;
	}
	if (strcmp(residueName, "CYS") == 0)
	{
		return 3;
	}
	if (strcmp(residueName, "GLN") == 0)
	{
		return 4;
	}
	if (strcmp(residueName, "GLU") == 0)
	{
		return 5;
	}
	if (strcmp(residueName, "HIS") == 0)
	{
		return 6;
	}
	if (strcmp(residueName, "ILE") == 0)
	{
		return 7;
	}
	if (strcmp(residueName, "LEU") == 0)
	{
		return 8;
	}
	if (strcmp(residueName, "LYS") == 0)
	{
		return 9;
	}
	if (strcmp(residueName, "MET") == 0)
	{
		return 10;
	}
	if (strcmp(residueName, "PHE") == 0)
	{
		return 11;
	}
	if (strcmp(residueName, "PRO") == 0)
	{
		return 12;
	}
	if (strcmp(residueName, "SER") == 0)
	{
		return 13;
	}
	if (strcmp(residueName, "THR") == 0)
	{
		return 14;
	}
	if (strcmp(residueName, "TRP") == 0)
	{
		return 15;
	}
	if (strcmp(residueName, "TYR") == 0)
	{
		return 16;
	}
	if (strcmp(residueName, "VAL") == 0)
	{
		return 17;
	}
	return -1;
}

int ApbsFormatResidue::getNumberOfRotamers(int residueIndex, std::vector<Atom*> residue, int* startRotamerIndex, int* endRotamerIndex)
{
	if (residueIndex < 0 || residueIndex >= Docking::NUMBER_OF_RESIDUE_TYPES_IN_LIB)
	{
		return 0;
	}
	double chi1 = getAngle(residue, PDBParser::AA_CHI1);
	// convert chi1 to degrees:
	chi1 = chi1* 180.0 / 3.14159265;
	// convert from 0..360 to -180..180
	if (chi1 > 180)
	{
		chi1 = chi1 - 360;
	}
	// except for PRO, 3 ranges for all, [0 .. 120], [120 ..180] or [-180 .. -120], [0 .. -120]
	// for PRO, either [0 .. 90], [0 .. -90], assume 0..120, 0..-120
	int category = 0;
	if (chi1 >= 0 && chi1 <= 120)
	{
		category = 0;
	}
	if ((chi1 >= 120 && chi1 <= 180) || (chi1 >= -180 && chi1 <= -120))
	{
		category = 1;
	}
	if (chi1 <= 0 && chi1 >= -120)
	{
		category = 2;
	}
	// Changed idea: take all rotamers, and rotate all, including chi1
	*startRotamerIndex = rotamerCategoryIndices[residueIndex][0*2+0];
	*endRotamerIndex = rotamerCategoryIndices[residueIndex][2*2+1];
	// now check to make sure there is some chi2. Otherwise there is no point as
	// we are only rotating chi2, 3, 4, 5
	return (*endRotamerIndex) - (*startRotamerIndex) + 1;
}

// can be H, HA, HB, HG, HG1, HG2, HD*, HE*, NH1*, NH2*, HZ, HH
int ApbsFormatResidue::getCategory(const char* atomname)
{
	if (strcmpLWS("HB", atomname))
	{
		return PDBParser::AA_CHI1;
	}
	// also rotate all further atoms with first chi angle
	if (strcmpLWS("HZ", atomname))
	{
		return PDBParser::AA_CHI1;
	}
	if (strcmpLWS("HH", atomname))
	{
		return PDBParser::AA_CHI1;
	}
	if (strlen(atomname) >= 2 && atomname[0] =='H' && atomname[1] == 'G')
	{
		return PDBParser::AA_CHI2;
	}
	if (strlen(atomname) >= 2 && atomname[0] =='H' && atomname[1] == 'D')
	{
		return PDBParser::AA_CHI3;
	}
	if (strlen(atomname) >= 2 && atomname[0] =='H' && atomname[1] == 'E')
	{
		return PDBParser::AA_CHI4;
	}
	// these are from the backbone
	if (strcmpLWS("H",  atomname))
	{
		return -1;
	}
	if (strcmpLWS("HA", atomname))
	{
		return -1;
	}
	// rotate anything else with all
	return PDBParser::AA_CHI1;
}

// this function will also assign H and other extra atoms to correct groups
bool ApbsFormatResidue::correctOrdering(std::vector<Atom*> residue)
{
	int m = (int) residue.size();
	//if( m > 14 ) return false;
	int resIndex = -1;
	{
		for (int i=0; i<PDBParser::TOTAL_AA_RES_TYPE; i++)
		{
			if (strcmp(PDBParser::AminoAcidResidueContentsOrdering[i][0], residue[0]->getResidueName()) == 0)
			{
				resIndex = i;
				break;
			}
		}
		if (resIndex == -1)
		{
			return false;
		}
	}
	int numRelevant = PDBParser::relevantNumberOfAtoms[resIndex];
	if (m < numRelevant)
	{
		return false;
	}
	for (int j=0; j < numRelevant; j++)
	{
		PDBParser::Atom* currAtom = residue[j];
		if (!strcmpLWS(currAtom->getName(), PDBParser::AminoAcidResidueContentsOrdering[resIndex][j+1]))
		{
			return false;
		}
		if (j<=3)
		{
			m_AtomsCategory.push_back(-1);
		}
		if (j==4)
		{
			m_AtomsCategory.push_back(PDBParser::AA_CHI1);
		}
		if (j==5)
		{
			m_AtomsCategory.push_back(PDBParser::AA_CHI2);
		}
		if (j==6)
		{
			m_AtomsCategory.push_back(PDBParser::AA_CHI3);
		}
		if (j==7)
		{
			m_AtomsCategory.push_back(PDBParser::AA_CHI4);
		}
		if (j>=8)
		{
			m_AtomsCategory.push_back(PDBParser::AA_CHI4);
		}
	}
	for (int j=numRelevant; j<m; j++)
	{
		// assign extra atoms to correct CHI angle group. Keep track so u can place them in correct position for new rotamers
		// can be H, HA, HB, HG, HG1, HG2, HD*, HE*, NH1*, NH2*, HZ, HH
		int category = getCategory(residue[j]->getName());
		m_AtomsCategory.push_back(category);
	}
	return true;
}

// return in radians.
double ApbsFormatResidue::getAngle(std::vector<Atom*> residue, int angleType)
{
	// chi1 : 0, 1, 4, 5
	// chi2 : 1, 4, 5, 6
	// chi3 : 4, 5, 6, 7
	// chi4 : 5, 6, 7, 8
	// chi5 : 6, 7, 8, 9
	Atom* nx, *nr, *fr, *fx;
	switch (angleType)
	{
	case PDBParser::AA_CHI1:
		if (residue.size() < 6)
		{
			return 0;
		}
		nx = residue[0];
		nr = residue[1];
		fr = residue[4];
		fx = residue[5];
		return PDBParser::findTorsion(nx, nr, fr, fx);
	case PDBParser::AA_CHI2:
		if (residue.size() < 7)
		{
			return 0;
		}
		nx = residue[1];
		nr = residue[4];
		fr = residue[5];
		fx = residue[6];
		return PDBParser::findTorsion(nx, nr, fr, fx);
	case PDBParser::AA_CHI3:
		if (residue.size() < 8)
		{
			return 0;
		}
		nx = residue[4];
		nr = residue[5];
		fr = residue[6];
		fx = residue[7];
		return PDBParser::findTorsion(nx, nr, fr, fx);
	case PDBParser::AA_CHI4:
		if (residue.size() < 9)
		{
			return 0;
		}
		nx = residue[5];
		nr = residue[6];
		fr = residue[7];
		fx = residue[8];
		return PDBParser::findTorsion(nx, nr, fr, fx);
	case PDBParser::AA_CHI5:
		if (residue.size() < 10)
		{
			return 0;
		}
		nx = residue[6];
		nr = residue[7];
		fr = residue[8];
		fx = residue[9];
		return PDBParser::findTorsion(nx, nr, fr, fx);
	default:
		return 0;
	}
}

void ApbsFormatResidue::copy(float* atomPosCopy, std::vector<Atom*> residue)
{
	for (int i=0; i<residue.size(); i++)
	{
		atomPosCopy[i*3+0] = residue[i]->m_Position[0];
		atomPosCopy[i*3+1] = residue[i]->m_Position[1];
		atomPosCopy[i*3+2] = residue[i]->m_Position[2];
	}
}

bool ApbsFormatResidue::getRotationOrigin(std::vector<Atom*> atomPosCopy, int angle, double* origx, double* origy, double* origz)
{
	// chi1 : 0, 1, 4, 5
	// chi2 : 1, 4, 5, 6
	// chi3 : 4, 5, 6, 7
	// chi4 : 5, 6, 7, 8
	// chi5 : 6, 7, 8, 9
	Atom* pAtom = 0;
	switch (angle)
	{
	case PDBParser::AA_CHI1:
		pAtom = atomPosCopy[4];
		break;
	case PDBParser::AA_CHI2:
		pAtom = atomPosCopy[5];
		break;
	case PDBParser::AA_CHI3:
		pAtom = atomPosCopy[6];
		break;
	case PDBParser::AA_CHI4:
		pAtom = atomPosCopy[7];
		break;
	case PDBParser::AA_CHI5:
		pAtom = atomPosCopy[8];
		break;
	default:
		return false;
	}
	*origx = pAtom->m_Position[0];
	*origy = pAtom->m_Position[1];
	*origz = pAtom->m_Position[2];
	return true;
}

bool ApbsFormatResidue::getRotationVector(std::vector<Atom*> atomPosCopy, int angle, double* vecx, double* vecy, double* vecz)
{
	// chi1 : 0, 1, 4, 5
	// chi2 : 1, 4, 5, 6
	// chi3 : 4, 5, 6, 7
	// chi4 : 5, 6, 7, 8
	// chi5 : 6, 7, 8, 9
	Atom* pAtom = 0;
	Atom* tAtom = 0;
	switch (angle)
	{
	case PDBParser::AA_CHI1:
		pAtom = atomPosCopy[4]; // CB
		tAtom = atomPosCopy[1]; // CA
		break;
	case PDBParser::AA_CHI2:
		pAtom = atomPosCopy[5]; // CG, CG1
		tAtom = atomPosCopy[4]; // CB
		break;
	case PDBParser::AA_CHI3:
		pAtom = atomPosCopy[6]; //
		tAtom = atomPosCopy[5];
		break;
	case PDBParser::AA_CHI4:
		pAtom = atomPosCopy[7];
		tAtom = atomPosCopy[6];
		break;
	case PDBParser::AA_CHI5:
		pAtom = atomPosCopy[8];
		tAtom = atomPosCopy[7];
		break;
	default:
		return false;
	}
	*vecx = pAtom->m_Position[0] - tAtom->m_Position[0];
	*vecy = pAtom->m_Position[1] - tAtom->m_Position[1];
	*vecz = pAtom->m_Position[2] - tAtom->m_Position[2];
	return true;
}

Matrix ApbsFormatResidue::getTransformation(std::vector<Atom*> atomPosCopy, int angle, double delChi)
{
	Matrix identity;
	double vecx = 1, vecy = 0, vecz = 0;
	if (!getRotationVector(atomPosCopy, angle, &vecx, &vecy, &vecz))
	{
		return identity;
	}
	double origx=0, origy=0, origz=0;
	if (!getRotationOrigin(atomPosCopy, angle, &origx, &origy, &origz))
	{
		return identity;
	}
	Matrix ntrans = Matrix::translation(-origx, -origy, -origz);
	Matrix rot = Quaternion::rotation(delChi, vecx, vecy, vecz).buildMatrix();
	Matrix ptrans = Matrix::translation(origx, origy, origz);
	Matrix mat = ptrans * rot * ntrans;
	return mat;
}

bool ApbsFormatResidue::applyRotation(std::vector<Atom*> atomPosCopy, int angle, int rotamerTableIndex)
{
	double desiredChi=0; // if 0 in table, assume it is invalid
	switch (angle)
	{
	case PDBParser::AA_CHI1:
		desiredChi = rotamerTable[rotamerTableIndex].chi1;
		if (isZero(desiredChi))
		{
			return true;    // nothing to do
		}
		break;
	case PDBParser::AA_CHI2:
		desiredChi = rotamerTable[rotamerTableIndex].chi2;
		if (isZero(desiredChi))
		{
			return true;    // nothing to do
		}
		break;
	case PDBParser::AA_CHI3:
		desiredChi = rotamerTable[rotamerTableIndex].chi3;
		if (isZero(desiredChi))
		{
			return true;    // nothing to do
		}
		break;
	case PDBParser::AA_CHI4:
		desiredChi = rotamerTable[rotamerTableIndex].chi4;
		if (isZero(desiredChi))
		{
			return true;    // nothing to do
		}
		break;
	default:
		return false;
	}
	// convert from -180..180  to radians
	desiredChi = ((3.14159265 * desiredChi) / 180.0);
	double curChi = getAngle(atomPosCopy, angle);   // gets in radians
	double delChi = desiredChi - curChi;
	if (delChi > 2*3.1415927)
	{
		printf("Too high\n");
	}
	if (delChi <-2*3.1415927)
	{
		printf("Too low\n");
	}
	Matrix matrix = getTransformation(atomPosCopy, angle, delChi);
	// apply those rotations to valid atoms only
	for (int i=0; i<atomPosCopy.size(); i++)
	{
		if (m_AtomsCategory[i] < angle)
		{
			continue;
		}
		Vector oldPos(atomPosCopy[i]->m_Position[0], atomPosCopy[i]->m_Position[1], atomPosCopy[i]->m_Position[2], 1);
		Vector newPos = matrix*oldPos;
		atomPosCopy[i]->m_Position[0] = newPos[0];
		atomPosCopy[i]->m_Position[1] = newPos[1];
		atomPosCopy[i]->m_Position[2] = newPos[2];
	}
	return true;
}

bool ApbsFormatResidue::constructRotamers(std::vector<Atom*> residue, float** rotamers, int* numRotamers)
{
	if (!rotamers)
	{
		printf("No input rotamer\n");
		return false;
	}
	*numRotamers = 0;
	// check if all atoms are present and assign any H and other atoms to correct groups
	if (!correctOrdering(residue))
	{
		printf("Not correct ordering\n");
		return false;
	}
	int residueIndex = getResidueIndex(residue[0]->getResidueName());
	if (residueIndex < 0)
	{
		printf("Unknown residue %s\n", residue[0]->getResidueName());
		return false;
	}
	// depending on chi1, get the range of valid rotamers
	int startRotamerIndex = 0;
	int endRotamerIndex = 0;
	*numRotamers = getNumberOfRotamers(residueIndex, residue, &startRotamerIndex, &endRotamerIndex);
	if ((*numRotamers) <= 0)
	{
		printf("No rotamers or no chi2 angles in rotamers for this residue\n");
		return false;
	}
	(*rotamers) = new float[(*numRotamers)*residue.size()*3];
	{
		std::vector<Atom*> atomPosCopy;
		// make new copy
		{
			for (int j=0; j<residue.size(); j++)
			{
				atomPosCopy.push_back(new Atom(*(residue[j])));
			}
		}
		int curRotamerTableIndex = startRotamerIndex;
		for (int i=0; i<(*numRotamers); i++)
		{
			// reset positions
			{
				for (int j=0; j<residue.size(); j++)
				{
					atomPosCopy[j]->m_Position[0] = residue[j]->m_Position[0];
					atomPosCopy[j]->m_Position[1] = residue[j]->m_Position[1];
					atomPosCopy[j]->m_Position[2] = residue[j]->m_Position[2];
				}
			}
			applyRotation(atomPosCopy, PDBParser::AA_CHI1, curRotamerTableIndex);
			applyRotation(atomPosCopy, PDBParser::AA_CHI2, curRotamerTableIndex);
			applyRotation(atomPosCopy, PDBParser::AA_CHI3, curRotamerTableIndex);
			applyRotation(atomPosCopy, PDBParser::AA_CHI4, curRotamerTableIndex);
			// enter xyz values for new rotamers
			{
				for (int j=0; j<residue.size(); j++)
				{
					(*rotamers)[(i*residue.size() + j)*3 + 0] = atomPosCopy[j]->m_Position[0];
					(*rotamers)[(i*residue.size() + j)*3 + 1] = atomPosCopy[j]->m_Position[1];
					(*rotamers)[(i*residue.size() + j)*3 + 2] = atomPosCopy[j]->m_Position[2];
				}
			}
			curRotamerTableIndex++;
		}
		{
			for (int j=0; j<residue.size(); j++)
			{
				delete atomPosCopy[j];
			}
			atomPosCopy.clear();
		}
	}
	return true;
}
