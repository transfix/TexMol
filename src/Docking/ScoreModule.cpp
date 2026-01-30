#include <Blurmaps/BlurMapsDataManager.h>
#include <cstring>
#include <Docking/ScoreModule.h>
#include <math.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/FlattenGOA.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <stdlib.h>
#include <UsefulMath/Vector.h>
#include <VolumeFileTypes/VolumeLoader.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using namespace std;
using Docking::ScoreModule;

ScoreModule::ScoreModule()
{
}

ScoreModule::~ScoreModule()
{
}

bool ScoreModule::getVolumeDimensions(unsigned int* dim, float* min, float* max, PDBParser::GroupOfAtoms* molecule, double ligandSize, double gridSpacing)
{
	if (!dim || !min || !max || !molecule)
	{
		return false;
	}
	if (ligandSize < 0 || gridSpacing <= 0)
	{
		return 0;
	}
	double dmin[3], dmax[3];
	molecule->getMinMax(&(dmin[0]), &(dmin[1]), &(dmin[2]), &(dmax[0]), &(dmax[1]), &(dmax[2]));
	if (dmin[0] >= dmax[0])
	{
		return false;    // sanity check
	}
	min[0] = dmin[0] - ligandSize;
	min[1] = dmin[1] - ligandSize;
	min[2] = dmin[2] - ligandSize;
	max[0] = dmax[0] + ligandSize;
	max[1] = dmax[1] + ligandSize;
	max[2] = dmax[2] + ligandSize;
	dim[0] = (max[0] - min[0]) / gridSpacing + 1;
	dim[1] = (max[1] - min[1]) / gridSpacing + 1;
	dim[2] = (max[2] - min[2]) / gridSpacing + 1;
	return true;
}

// this returning false is not a bad thing
bool ScoreModule::getAtomScoringParameters(PDBParser::Atom* atom, int index, int* m, int* n, double* epsilon, double* r_eq, int parentAtomType)
{
	if (index == 6)
	{
		return true;    // nothing to compute for elec map
	}
	if (!atom || !m || !n || !epsilon || !r_eq)
	{
		return false;
	}
	int numVDW = 5;
	if (index >= 0 && index <= 4)
	{
		int atomIndex = 0; // default is C
		const char* name = atom->getName();
		if (name[0] == ' ' && name[1] == 'N')
		{
			atomIndex = 1;
		}
		if (name[0] == ' ' && name[1] == 'O')
		{
			atomIndex = 2;
		}
		if (name[0] == ' ' && name[1] == 'S')
		{
			atomIndex = 3;
		}
		if (name[0] == ' ' && name[1] == 'H')
		{
			atomIndex = 4;
		}
		if (name[0] == 'H')
		{
			atomIndex = 4;    // can be 2 forms of H
		}
		//if( atomIndex != index ) return false;
		*n = 12;
		*m = 6;
		*epsilon = vdwScoringTable[index*numVDW+atomIndex].epsilon;
		*r_eq    = vdwScoringTable[index*numVDW+atomIndex].r_eq;
		return true;
	}
	if (index == 5)
	{
		if (parentAtomType == -1)
		{
			return false;    //shouldnt happen
		}
		if (parentAtomType > 2)
		{
			return false;    // we only have N=0, O=1 and S=2
		}
		*n = 12;
		*m = 10;
		*epsilon = hBondScoringTable[parentAtomType].epsilon; // apparently doesnt matter which one the acceptor is
		*r_eq    = hBondScoringTable[parentAtomType].r_eq;
		return true;
	}
	return true;
}

double ScoreModule::getPotential(int index, int m, int n, double epsilon, double r_eq, double distance, double cosangle, double charge)
{
	if (index == 6)
	{
		if (distance < 0.001)
		{
			distance = 0.001;    // dont want wierdness
		}
		double A=-8.5525;
		double B=78.4-A;
		double lambda=0.003627;
		double k=7.7839;
		//double ce=1.60219e-19;
		double q1 = 1;// are all these units correct??
		double q2 = charge;
		double diel = (A+B/(1+k*exp(-lambda*B*distance)));
		double ret = q1*q2 / (4*3.14159*diel*distance);
		if (ret < 0)
		{
			printf("Cant happen when all charges are positive\n");
		}
		return ret;
	}
	if (distance < 0.001)
	{
		distance = 0.001;    // dont want wierdness
	}
	double t1 = m/(n-m)*epsilon*pow(r_eq, n);
	t1 /= pow(distance, n);
	double t2 = n/(n-m)*epsilon*pow(r_eq, m);
	t2 /= pow(distance, m);
	if (index == 5)
	{
		return cosangle*(t1-t2);
	}
	return t1-t2;
}

bool ScoreModule::getNOSatoms(PDBParser::Atom* atom, float* prevNPos, float* prevOPos, float* prevSPos)
{
	const char* name = atom->getName();
	if (name[0] == ' ' && name[1] == 'N')
	{
		prevNPos[0] = atom->m_Position[0];
		prevNPos[1] = atom->m_Position[1];
		prevNPos[2] = atom->m_Position[2];
		return true;
	}
	if (name[0] == ' ' && name[1] == 'O')
	{
		prevOPos[0] = atom->m_Position[0];
		prevOPos[1] = atom->m_Position[1];
		prevOPos[2] = atom->m_Position[2];
		return true;
	}
	if (name[0] == ' ' && name[1] == 'S')
	{
		prevSPos[0] = atom->m_Position[0];
		prevSPos[1] = atom->m_Position[1];
		prevSPos[2] = atom->m_Position[2];
		return true;
	}
	return false;
}

double ScoreModule::getDist(PDBParser::Atom* atom, float xpos, float ypos, float zpos)
{
	float x = atom->m_Position[0];
	float y = atom->m_Position[1];
	float z = atom->m_Position[2];
	return sqrt((x-xpos)*(x-xpos) + (y-ypos)*(y-ypos) + (z-zpos)*(z-zpos));
}

double ScoreModule::getAngle(float xpos, float ypos, float zpos, float* HParentPos, float* atomPos)
{
	// use dot product
	CCVOpenGLMath::Vector v1(atomPos[0]-HParentPos[0],
							 atomPos[1]-HParentPos[1],
							 atomPos[2]-HParentPos[2],
							 0);
	CCVOpenGLMath::Vector v2(xpos - atomPos[0],
							 ypos - atomPos[1],
							 zpos - atomPos[2],
							 0);
	v1.normalize();
	v2.normalize();
	double cosangle = v1.dot(v2);
	if (cosangle < 0)
	{
		cosangle = 0;
	}
	return cosangle;
}

bool ScoreModule::getPos(PDBParser::Atom* atom, const char* HAtomName, const char* residueName, const char* parentName, float* HParentPos, std::vector<PDBParser::Atom*> residueBeginning)
{
	const char* atomName = atom->getName();
	// check if correct H atom
	if (strcmp(atomName, HAtomName) != 0)
	{
		return false;
	}
	if (strcmp(residueName, atom->getResidueName()) != 0)
	{
		return false;
	}
	// search for parent and return its position
	for (int i=0; i<residueBeginning.size(); i++)
	{
		PDBParser::Atom* parentAtom = residueBeginning[i];
		if (strcmp(parentAtom->getName(), parentName) == 0)
		{
			HParentPos[0] = parentAtom->m_Position[0];
			HParentPos[1] = parentAtom->m_Position[1];
			HParentPos[2] = parentAtom->m_Position[2];
			return true;
		}
	}
	return false;
}

int ScoreModule::getHAtomParentPos(std::vector<PDBParser::Atom*> residueBeginning, PDBParser::Atom* atom, float* HParentPos)
{
	// check if it is relevant H atom, and then get its parent position
	const char* name = atom->getName();
	if (!((name[0] == ' ' && name[1] == 'H') || (name[0] == 'H')))
	{
		return -1;
	}
	// if just ' H  ' , parent is ' N  '
	if (getPos(atom, " H  ", atom->getResidueName(), " N  ", HParentPos, residueBeginning))
	{
		return 0;
	}
	// else, elaborate lookup
	if (getPos(atom, " HG ", "SER", " OG ", HParentPos, residueBeginning))
	{
		return 1;
	}
	if (getPos(atom, " HG1", "THR", " OG1", HParentPos, residueBeginning))
	{
		return 1;
	}
	if (getPos(atom, " HG ", "CYS", " SG ", HParentPos, residueBeginning))
	{
		return 2;
	}
	if (getPos(atom, "HD21", "ASN", " ND2", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, "HD22", "ASN", " ND2", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, "HE21", "GLN", " NE2", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, "HE22", "GLN", " NE2", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, " HE ", "ARG", " NE ", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, "HH11", "ARG", " NH1", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, "HH12", "ARG", " NH1", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, "HH21", "ARG", " NH2", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, "HH22", "ARG", " NH2", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, " HZ1", "LYS", " NZ ", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, " HZ2", "LYS", " NZ ", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, " HZ3", "LYS", " NZ ", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, " HD1", "HIS", " ND1", HParentPos, residueBeginning))
	{
		return 0;
	}
	if (getPos(atom, " HH ", "TYR", " OH ", HParentPos, residueBeginning))
	{
		return 1;
	}
	if (getPos(atom, " HE1", "TRP", " NE1", HParentPos, residueBeginning))
	{
		return 0;
	}
	return -1;
}

void ScoreModule::getNextAminoAcid(std::vector<PDBParser::Atom*> atomList, int index,  std::vector<PDBParser::Atom*> *residueBeginning)
{
	for (int i=0; i<residueBeginning->size(); i++)
	{
		delete residueBeginning->at(i);
	}
	residueBeginning->clear();
	int resSeq = atomList[index]->getResidueNum();
	for (int i=index; i<atomList.size(); i++)
	{
		if (atomList[i]->getResidueNum() != resSeq)
		{
			break;
		}
		residueBeginning->push_back(new PDBParser::Atom(*(atomList[i])));
	}
}

double ScoreModule::getSmoothedPotential(float* potentialMap, int centerX, int centerY, int centerZ, int xWin, int yWin, int zWin, unsigned int* dim)
{
	int xmin, ymin, zmin, xmax, ymax, zmax;
	xmin = centerX-xWin;
	if (xmin<0)
	{
		xmin = 0;
	}
	ymin = centerY-yWin;
	if (ymin<0)
	{
		ymin = 0;
	}
	zmin = centerZ-zWin;
	if (zmin<0)
	{
		zmin = 0;
	}
	xmax = centerX+xWin;
	if (xmax>=dim[0])
	{
		xmax = dim[0]-1;
	}
	ymax = centerY+yWin;
	if (ymax>=dim[1])
	{
		ymax = dim[1]-1;
	}
	zmax = centerZ+zWin;
	if (zmax>=dim[2])
	{
		zmax = dim[2]-1;
	}
	int c = centerX + centerY*dim[0] + centerZ*dim[0]*dim[1];
	double minVal = potentialMap[c];
	for (int i=xmin; i<=xmax; i++)
	{
		for (int j=ymin; j<=ymax; j++)
		{
			for (int k=zmin; k<=zmax; k++)
			{
				int c = i + j*dim[0] + k*dim[0]*dim[1];
				double val = potentialMap[c];
				if (val < minVal)
				{
					minVal = val;
				}
			}
		}
	}
	return minVal;
}

bool ScoreModule::getPotential(float* potentialMap, int index, unsigned int* dim, float* min, float* max, std::vector<PDBParser::Atom*> atomList)
{
	{
		int i;
		for (int i=0; i<dim[0]*dim[1]*dim[2]; i++)
		{
			potentialMap[i] = 0;
		}
	}
	float span[3];
	span[0] = (max[0] - min[0]) / (dim[0]-1);
	span[1] = (max[1] - min[1]) / (dim[1]-1);
	span[2] = (max[2] - min[2]) / (dim[2]-1);
	float HParentPos[3];
	std::vector<PDBParser::Atom*> residueBeginning;
	for (int a=0; a<atomList.size(); a++)
	{
		{
			const char* name = atomList[a]->getName();
			// clear current and fill up full residue
			if (name[0] == ' ' && name[1] == 'N' && name[2] == ' ' && name[3] == ' ')
			{
				getNextAminoAcid(atomList, a, &residueBeginning);
			}
		}
		int parentAtomType = -1;
		if (index == 5)
		{
			parentAtomType = getHAtomParentPos(residueBeginning, atomList[a], HParentPos);
			if (parentAtomType == -1)
			{
				continue;
			}
		}
		// see autodock for their meaning
		int m, n;
		double epsilon, r_eq;
		if (!getAtomScoringParameters(atomList[a], index, &m, &n, &epsilon, &r_eq, parentAtomType))
		{
			continue;
		}
		for (int k=0; k<dim[2]; k++)
		{
			for (int j=0; j<dim[1]; j++)
			{
				for (int i=0; i<dim[0]; i++)
				{
					int c = i + j*dim[0] + k*dim[0]*dim[1];
					float xpos = min[0] + i*span[0];
					float ypos = min[1] + j*span[1];
					float zpos = min[2] + k*span[2];
					float distance = getDist(atomList[a], xpos, ypos, zpos);
					double cosangle = 0;
					if (index == 5)
					{
						cosangle = getAngle(xpos, ypos, zpos, HParentPos, atomList[a]->m_Position);
					}
					potentialMap[c] += getPotential(index, m, n, epsilon, r_eq, distance, cosangle, atomList[a]->getCharge());
				}
			}
		}
	}
	{
		double smoothingWin = 0.5;
		float* temp = new float[dim[0]*dim[1]*dim[2]];
		// smooth map
		for (int k=0; k<dim[2]; k++)
		{
			for (int j=0; j<dim[1]; j++)
			{
				for (int i=0; i<dim[0]; i++)
				{
					int c = i + j*dim[0] + k*dim[0]*dim[1];
					int xWin = ceil(smoothingWin/span[0]);
					int yWin = ceil(smoothingWin/span[1]);
					int zWin = ceil(smoothingWin/span[2]);
					temp[c] = getSmoothedPotential(potentialMap, i, j, k, xWin, yWin, zWin, dim);
				}
			}
		}
		for (int i=0; i<dim[0]*dim[1]*dim[2]; i++)
		{
			potentialMap[i] = temp[i];
		}
		delete []temp;
	}
	{
		for (int i=0; i<residueBeginning.size(); i++)
		{
			delete residueBeginning[i];
		}
		residueBeginning.clear();
	}
	return true;
}

bool ScoreModule::computeScoringGrids(PDBParser::GroupOfAtoms* molecule, double ligandSize, double gridSpacing, const char* gridBaseFileName)
{
	if (!molecule || ligandSize < 0 || gridSpacing <= 0 || !gridBaseFileName)
	{
		return false;
	}
	// quite simple algo:
	//		get size, position and spacings of grids
	//		allocate all at same time ? or one after the other ?
	//		go through all atoms and compute all, one by one?. Also smooth ...
	//		print them all
	unsigned int dim[3];
	float min[3];
	float max[3];
	if (!getVolumeDimensions(dim, min, max, molecule, ligandSize, gridSpacing))
	{
		return false;
	}
	std::vector<PDBParser::Atom*> atomList;
	PDBParser::FlattenGOA(molecule, atomList, 0, 0, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0, false);
	// create 7 volumes: 5 for vdw, 1 for hbond and 1 for elec. Or do this in serial order.
	for (int i=0; i<7; i++)
	{
		float* potentialMap = new float[dim[0]*dim[1]*dim[2]];
		if (!getPotential(potentialMap, i, dim, min, max, atomList))
		{
			delete []potentialMap;
			return false;
		}
		// save data as rawiv file
		{
			SimpleVolumeData* sData = new SimpleVolumeData(dim);
			sData->setDimensions(dim);
			sData->setNumberOfVariables(1);
			sData->setData(0, potentialMap);
			sData->setType(0, SimpleVolumeData::FLOAT);
			sData->setName(0, potentialMapName[i]);
			sData->setMinExtent(min);
			sData->setMaxExtent(max);
			char outputFileName[1024];
			sprintf(outputFileName, "%s_%s.rawiv", gridBaseFileName, potentialName[i]);
			VolumeLoader* volumeLoader = new VolumeLoader();
			bool ret = volumeLoader->saveFile(outputFileName, sData);
			delete volumeLoader;
			delete sData; // should delete potentialMap also;
			if (!ret)
			{
				return false;
			}
		}
	}
	return true;
}

bool ScoreModule::computeScores()
{
	return true;
}
