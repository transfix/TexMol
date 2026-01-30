#include <Blurmaps/BlurMapsDataManager.h>
#include <Docking/Complex.h>
#include <Docking/Interface.h>
#include <Docking/IO.h>
#include <Geometry/Geometry.h>
#include <GOAFileTypes/GOALoader.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GOAFileIO.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/FlattenGOA.h>
#include <SignDistanceFunction/sdfLib.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Vector.h>
#include <Utility/utility.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using namespace std;
using PDBParser::GroupOfAtoms;
//using PDBParser::Atom;
using Docking::DockComplex;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;

// it takes ownership of the two molecules
DockComplex::DockComplex(PDBParser::GroupOfAtoms* protein, PDBParser::GroupOfAtoms* ligand)
{
	m_Protein = protein;
	m_Ligand = ligand;
}

DockComplex::DockComplex()
{
	m_Protein = 0;
	m_Ligand = 0;
}

// careful, we delete these ourselves
DockComplex::~DockComplex()
{
	delete m_Protein;
	m_Protein = 0;
	delete m_Ligand;
	m_Ligand = 0;
}

bool DockComplex::init(const char* inputFile1, const char* inputFile2, bool deleteWater)
{
	delete m_Protein;
	m_Protein = 0;
	delete m_Ligand;
	m_Ligand = 0;
	if (!inputFile1 || !inputFile2)
	{
		return false;
	}
	GOALoader* gLoader = new GOALoader();
	m_Protein = gLoader->loadFile(inputFile1, deleteWater);
	m_Ligand = gLoader->loadFile(inputFile2, deleteWater);
	delete gLoader;
	if (!m_Protein || !m_Ligand)
	{
		return false;
	}
	return true;
}

bool DockComplex::init(PDBParser::GroupOfAtoms* protein, PDBParser::GroupOfAtoms* ligand, bool deleteWater)
{
	delete m_Protein;
	m_Protein = 0;
	delete m_Ligand;
	m_Ligand = 0;
	m_Protein = protein;
	m_Ligand = ligand;
	if (!m_Protein || !m_Ligand)
	{
		return false;
	}
	return true;
}

double DockComplex::getInterfaceArea(double distanceCutOff)
{
	Interface* interface_complex = new Interface();
	double area = interface_complex->getArea(this, distanceCutOff);
	delete interface_complex;
	return area;
}

// make both the input files
bool DockComplex::createDockingInputFiles(const char* outputFile1, const char* outputFile2, bool printMiscFiles, double distanceCutOff)
{
	if (!m_Protein || !m_Ligand || !outputFile1 || !outputFile2)
	{
		return false;
	}
	IO* io = new IO();
	if (!io->createProteinsDockingInputFile(m_Protein, outputFile1, printMiscFiles, false))
	{
		delete io;
		return false;
	}
	if (!io->createLigandsDockingInputFile(m_Ligand, outputFile2, printMiscFiles))
	{
		delete io;
		return false;
	}
	delete io;
	return true;
}

bool DockComplex::computeLigandInterfaceAtoms(double distanceCutoff)
{
	// first compute sdf of protein
	// then go through ligand and search for all atoms within a cutoff and mark them
  PDBParser::FlattenGOA(m_Ligand, m_LigandAtomList, m_Ligand->m_CollectionData, 0, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0, false);
	if (m_LigandAtomList.size() == 0)
	{
		return false;
	}
	// compute sdf
	int size = 64;
	int dim1 = size, dim2 = size, dim3 = size;
	double isovalue = 1;
	double blobbiness = -2.3;
	SimpleVolumeData* densData = BlurMapsDataManager::getVolume(m_Protein,
								 "",
								 dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
								 false, blobbiness, PDBParser::GroupOfAtoms::ATOM, NULL, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
	// compute interface atoms
	{
		double minFuncVal = exp(blobbiness*distanceCutoff*distanceCutoff);
		for (int i=0; i<m_LigandAtomList.size(); i++)
		{
			double x = m_LigandAtomList[i]->m_Position[0];
			double y = m_LigandAtomList[i]->m_Position[1];
			double z = m_LigandAtomList[i]->m_Position[2];
			if (densData->getValueAt(0, x, y, z) >= minFuncVal)
			{
				m_LigandInterfaceAtom.push_back(i);
			}
		}
	}
	return true;
}

double DockComplex::getRMSD(CCVOpenGLMath::Matrix transformation)
{
	if (m_LigandInterfaceAtom.size() == 0)
	{
		return 0;
	}
	double rmsd = 0;
	for (int i=0; i<m_LigandInterfaceAtom.size(); i++)
	{
	  PDBParser::Atom* atom = m_LigandAtomList[m_LigandInterfaceAtom[i]];
		CCVOpenGLMath::Vector oldPos(atom->m_Position[0], atom->m_Position[1], atom->m_Position[2], 1);
		CCVOpenGLMath::Vector newPos = transformation*oldPos;
		rmsd += (oldPos[0]-newPos[0])*(oldPos[0]-newPos[0]) +
				(oldPos[1]-newPos[1])*(oldPos[1]-newPos[1]) +
				(oldPos[2]-newPos[2])*(oldPos[2]-newPos[2]);
	}
	return sqrt(rmsd / (double)(m_LigandInterfaceAtom.size()));
}

double DockComplex::getRMSD(CCVOpenGLMath::Matrix transformation, std::vector<PDBParser::Atom*> unboundLigandAtomList, std::vector<int> unboundLigandInterfaceAtomIndex)
{
	if (m_LigandInterfaceAtom.size() == 0)
	{
		return 0;
	}
	double rmsd = 0;
	int numFound = 0;
	FILE* fp = fopen("oldnew.txt","w");
	for (int i=0; i<m_LigandInterfaceAtom.size(); i++)
	{
		int index = unboundLigandInterfaceAtomIndex[i];
		if (index == -1)
		{
			continue;
		}
		numFound++;
		PDBParser::Atom* new_atom = unboundLigandAtomList[index];
		PDBParser::Atom* old_atom = m_LigandAtomList[m_LigandInterfaceAtom[i]];
		fprintf(fp, "%c %s %s %d [%f %f %f] %c %s %s %d [%f %f %f]\n",
				old_atom->getChain(),
				old_atom->getName(),
				old_atom->getResidueName(),
				old_atom->getResidueNum(),
				old_atom->m_Position[0],
				old_atom->m_Position[1],
				old_atom->m_Position[2],
				new_atom->getChain(),
				new_atom->getName(),
				new_atom->getResidueName(),
				new_atom->getResidueNum(),
				new_atom->m_Position[0],
				new_atom->m_Position[1],
				new_atom->m_Position[2]);
		CCVOpenGLMath::Vector oldPos(old_atom->m_Position[0], old_atom->m_Position[1], old_atom->m_Position[2], 1);
		CCVOpenGLMath::Vector initialPos(new_atom->m_Position[0], new_atom->m_Position[1], new_atom->m_Position[2], 1);
		CCVOpenGLMath::Vector newPos = transformation*initialPos;
		rmsd += (oldPos[0]-newPos[0])*(oldPos[0]-newPos[0]) +
				(oldPos[1]-newPos[1])*(oldPos[1]-newPos[1]) +
				(oldPos[2]-newPos[2])*(oldPos[2]-newPos[2]);
	}
	fclose(fp);
	if (numFound == 0)
	{
		return 100000;
	}
	return sqrt(rmsd / (double)(numFound));
}

bool DockComplex::getInterfaceRMSD(const char* transformationsFilename, int numberOfTransformations, double distanceCutoff, const char* outputFilename)
{
	if (!transformationsFilename || !outputFilename)
	{
		return false;
	}
	if (numberOfTransformations < 1)
	{
		return false;
	}
	if (!computeLigandInterfaceAtoms(distanceCutoff))
	{
		return false;
	}
	// read transformation, get rmsd and print out
	{
		FILE* fpIn = fopen(transformationsFilename, "r");
		if (!fpIn)
		{
			return false;
		}
		FILE* fpOut = fopen(outputFilename,  "w");
		if (!fpOut)
		{
			fclose(fpIn);
			return false;
		}
		for (int i=0; i<numberOfTransformations; i++)
		{
			float t[16];
			fscanf(fpIn, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				   &(t[0]),  &(t[1]),  &(t[2]),  &(t[3]),
				   &(t[4]),  &(t[5]),  &(t[6]),  &(t[7]),
				   &(t[8]),  &(t[9]),  &(t[10]), &(t[11]),
				   &(t[12]), &(t[13]), &(t[14]), &(t[15]));
			CCVOpenGLMath::Matrix transformation(t[0],  t[1],  t[2],  t[3],
												 t[4],  t[5],  t[6],  t[7],
												 t[8],  t[9],  t[10], t[11],
												 t[12], t[13], t[14], t[15]
												);
			fprintf(fpOut, "%lf\n", getRMSD(transformation));
		}
		fclose(fpIn);
		fclose(fpOut);
	}
	return true;
}

bool DockComplex::getInterface(double distanceCutoff, const char* outputFilename)
{
	if (!computeLigandInterfaceAtoms(distanceCutoff))
	{
		return false;
	}
	FILE* fpOut = fopen(outputFilename,  "w");
	if (!fpOut)
	{
		return false;
	}
	for (int i=0; i<m_LigandInterfaceAtom.size(); i++)
	{
		//ATOM     36  HG3 PRO     5      25.972   1.740  66.185  0.0213 1.4870
		PDBParser::Atom* atom = m_LigandAtomList[m_LigandInterfaceAtom[i]];
		fprintf(fpOut, "ATOM %6d %4s %3s %5d    %8.3f %8.3f %8.3f %7.4f %6.4f\n",
				i+1,
				atom->getName(),
				atom->getResidueName(),
				atom->getResidueNum(),
				atom->m_Position[0], atom->m_Position[1], atom->m_Position[2],
				atom->getCharge(),
				atom->getRadius());
	}
	fclose(fpOut);
	return true;
}

int DockComplex::getCorrespondingAtomIndex(PDBParser::Atom* atom, std::vector<PDBParser::Atom*> unboundLigandAtomList)
{
	for (int i=0; i<unboundLigandAtomList.size(); i++)
	{
		// check atom name, residue number and residue type
		if ((strcmp(atom->getName(), unboundLigandAtomList[i]->getName()) ==0) &&
				(strcmp(atom->getResidueName(), unboundLigandAtomList[i]->getResidueName()) ==0) &&
				(atom->getResidueNum() == unboundLigandAtomList[i]->getResidueNum()) &&
				(atom->getChain() == unboundLigandAtomList[i]->getChain()))
		{
			return i;
		}
	}
	return -1;
}

int DockComplex::findCorrespondingInterfaceAtoms(std::vector<int> *unboundLigandInterfaceAtomIndex, std::vector<PDBParser::Atom*> unboundLigandAtomList)
{
	int found = 0;
	for (int i=0; i<m_LigandInterfaceAtom.size(); i++)
	{
		PDBParser::Atom* atom = m_LigandAtomList[m_LigandInterfaceAtom[i]];
		// search for this atom and put its index into the vector
		int index = getCorrespondingAtomIndex(atom, unboundLigandAtomList);
		if (index != -1)
		{
			found++;
		}
		unboundLigandInterfaceAtomIndex->push_back(index);
	}
	return found;
}

bool DockComplex::getUnboundLigandAtoms(const char* unboundLigandFileName, bool deleteWater, std::vector<PDBParser::Atom*> *unboundLigandAtomList)
{
	GroupOfAtoms* unboundLigand = 0;
	GOALoader* gLoader = new GOALoader();
	unboundLigand = gLoader->loadFile(unboundLigandFileName, deleteWater);
	delete gLoader;
	if (!unboundLigand)
	{
		return false;
	}
	PDBParser::FlattenGOA(unboundLigand, *unboundLigandAtomList, unboundLigand->m_CollectionData, 0, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0, false);
	//	delete unboundLigand;
	if (unboundLigandAtomList->size() == 0)
	{
		return false;
	}
	return true;
}

bool DockComplex::getInterfaceRMSD(const char* transformationsFilename, int numberOfTransformations, double distanceCutoff, const char* outputFilename, const char* unboundLigandFileName)
{
	if (!transformationsFilename || !outputFilename)
	{
		return false;
	}
	if (numberOfTransformations < 1)
	{
		return false;
	}
	if (!computeLigandInterfaceAtoms(distanceCutoff))
	{
		return false;
	}
	// read in the unbound ligand
	std::vector<PDBParser::Atom*> unboundLigandAtomList;
	bool deleteWater = false;
	if (!getUnboundLigandAtoms(unboundLigandFileName, deleteWater, &unboundLigandAtomList))
	{
		return false;
	}
	// compute indices of unbound ligand atoms which correspond to bound ligand interface atoms
	std::vector<int> unboundLigandInterfaceAtomIndex; // For a given bound ligand interface atom, this points to corresponding atom in unbound ligand flat list
	int numFound = findCorrespondingInterfaceAtoms(&unboundLigandInterfaceAtomIndex, unboundLigandAtomList);
	{
		// print this out, might be useful
		FILE* fp = fopen("unboundLigandStats.txt", "a");
		fprintf(fp, "Unbound ligand: %s had %d of %lu interface atoms of bound ligand\n", unboundLigandFileName, numFound, m_LigandInterfaceAtom.size());
		fclose(fp);
	}
	// read transformation, get rmsd and print out
	{
		FILE* fpIn = fopen(transformationsFilename, "r");
		if (!fpIn)
		{
			return false;
		}
		FILE* fpOut = fopen(outputFilename,  "w");
		if (!fpOut)
		{
			fclose(fpIn);
			return false;
		}
		for (int i=0; i<numberOfTransformations; i++)
		{
			float t[16];
			fscanf(fpIn, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				   &(t[0]),  &(t[1]),  &(t[2]),  &(t[3]),
				   &(t[4]),  &(t[5]),  &(t[6]),  &(t[7]),
				   &(t[8]),  &(t[9]),  &(t[10]), &(t[11]),
				   &(t[12]), &(t[13]), &(t[14]), &(t[15]));
			CCVOpenGLMath::Matrix transformation(t[0],  t[1],  t[2],  t[3],
												 t[4],  t[5],  t[6],  t[7],
												 t[8],  t[9],  t[10], t[11],
												 t[12], t[13], t[14], t[15]
												);
			fprintf(fpOut, "%lf\n", getRMSD(transformation, unboundLigandAtomList, unboundLigandInterfaceAtomIndex));
		}
		fclose(fpIn);
		fclose(fpOut);
	}
	return true;
}

bool DockComplex::getInterfaceArea(const char* transformationsFilename, int numberOfTransformations, double distanceCutoff, const char* outputFilename)
{
	if (!transformationsFilename || !outputFilename)
	{
		return false;
	}
	if (numberOfTransformations < 1)
	{
		return false;
	}
	return true;
}

bool DockComplex::printScoreAndRMSD(FILE* fpOut, CCVOpenGLMath::Matrix transformation, std::vector<PDBParser::Atom*> unboundLigandAtomList,
								std::vector<int> unboundLigandInterfaceAtomIndex, SimpleVolumeData* density, SimpleVolumeData* elecField)
{
	if (!fpOut || !density || !elecField)
	{
		fprintf(fpOut, "-1 -1");
		return false;
	}
	if (m_LigandInterfaceAtom.size() == 0)
	{
		fprintf(fpOut, "-1 -1");
		return false;
	}
	double rmsd = 0;
	int numFound = 0;
	double score = 0;
	for (int i=0; i<m_LigandInterfaceAtom.size(); i++)
	{
		int index = unboundLigandInterfaceAtomIndex[i];
		if (index == -1)
		{
			continue;
		}
		numFound++;
		PDBParser::Atom* new_atom = unboundLigandAtomList[index];
		PDBParser::Atom* old_atom = m_LigandAtomList[m_LigandInterfaceAtom[i]];
		CCVOpenGLMath::Vector oldPos(old_atom->m_Position[0], old_atom->m_Position[1], old_atom->m_Position[2], 1);
		CCVOpenGLMath::Vector initialPos(new_atom->m_Position[0], new_atom->m_Position[1], new_atom->m_Position[2], 1);
		CCVOpenGLMath::Vector newPos = transformation*initialPos;
		rmsd += (oldPos[0]-newPos[0])*(oldPos[0]-newPos[0]) +
				(oldPos[1]-newPos[1])*(oldPos[1]-newPos[1]) +
				(oldPos[2]-newPos[2])*(oldPos[2]-newPos[2]);
		double eVal = elecField->getValueAt(0, newPos[0], newPos[1], newPos[2]);
		score -= eVal*new_atom->getCharge();
	}
	if (numFound == 0)
	{
		fprintf(fpOut, "-1 -1");
		return false;
	}
	rmsd = sqrt(rmsd / (double)(numFound));
	fprintf(fpOut, "%lf %lf ", score, rmsd);
	{
		for (int i=0; i<4; i++)
		{
			for (int j=0; j<4; j++)
			{
				fprintf(fpOut, "%f ", transformation.get(i,j));
			}
		}
	}
	return true;
}

bool DockComplex::printScoreAndRMSD(FILE* fpOut, CCVOpenGLMath::Matrix transformation, std::vector<PDBParser::Atom*> unboundLigandAtomList, std::vector<int> unboundLigandInterfaceAtomIndex, SimpleVolumeData* density, SimpleVolumeData* elecField, Vector c1, Vector c2)
{
	if (!fpOut || !density || !elecField)
	{
		return false;
	}
	double increment = 0.25; // in angstroms
	// compute 3 orthogonal vectors to move along while sampling.
	Vector v1, v2, v3;
	{
		v1 = c1-c2;
		v1.normalize();
		v2[1] = 1;
		v2[2] = 1;
		v2[3] = 0;
		v2[0] = (-v1[1] - v1[2])/v1[0];
		v2.normalize();
		v3 = v1.cross(v2);
		v3.normalize();
		v1 *= increment;
		v2 *= increment;
		v3 *= increment;
	}
	// for each 27 positions,
	//		Compute single xform matrix
	//		compute and print the score and RMSD.
	for (int i=-1; i<=1; i++)
	{
		for (int j=-1; j<=1; j++)
		{
			for (int k=-1; k<=1; k++)
			{
				Vector translation = v1*i + v2*j + v3*k;
				Matrix xform = transformation*Matrix::translation(translation);
				printScoreAndRMSD(fpOut, xform, unboundLigandAtomList, unboundLigandInterfaceAtomIndex, density, elecField);
			}
		}
	}
	fprintf(fpOut, "\n");
	return true;
}

SimpleVolumeData* DockComplex::getProteinDensity()
{
	int size = 64;
	int dim1 = size, dim2 = size, dim3 = size;
	double blobbiness = -2.3;
	return BlurMapsDataManager::getVolume(m_Protein, "", dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
										  false, blobbiness, PDBParser::GroupOfAtoms::ATOM, NULL, 0,
										  PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
}

Vector DockComplex::getCenter(GroupOfAtoms* molecule)
{
	double minX, minY, minZ, maxX, maxY, maxZ;
	molecule->getMinMax(&minX, &minY, &minZ, &maxX, &maxY, &maxZ);
	return Vector((maxX+minX)/2.0, (maxY+minY)/2.0, (maxZ+minZ)/2.0, 1);
}

Vector DockComplex::getCenter(std::vector<PDBParser::Atom*> unboundLigandAtomList)
{
	double x = 0;
	double y = 0;
	double z = 0;
	int s = unboundLigandAtomList.size();
	for (int i=0; i<s; i++)
	{
		x += unboundLigandAtomList[i]->m_Position[0];
		y += unboundLigandAtomList[i]->m_Position[1];
		z += unboundLigandAtomList[i]->m_Position[2];
	}
	return Vector(x/s, y/s, z/s, 1);
}

SimpleVolumeData* DockComplex::getProteinElecField()
{
	int size = 64;
	int dim1 = size, dim2 = size, dim3 = size;
	double blobbiness = -2.3;
	return BlurMapsDataManager::getVolume(m_Protein, "", dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTROSTATIC_POTENTIAL,
										  false, blobbiness, PDBParser::GroupOfAtoms::ATOM, NULL, 0,
										  PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
}

bool DockComplex::improveInterface(const char* unboundLigandFileName, int numberOfTransformations, double distanceCutoff,
							   const char* transformationsFilename, const char* outputFilename, bool flexDock)
{
	if (!transformationsFilename || !outputFilename)
	{
		return false;
	}
	if (numberOfTransformations < 1)
	{
		return false;
	}
	if (!computeLigandInterfaceAtoms(distanceCutoff))
	{
		return false;
	}
	// read in the unbound ligand
	std::vector<PDBParser::Atom*> unboundLigandAtomList;
	bool deleteWater = false;
	if (!getUnboundLigandAtoms(unboundLigandFileName, deleteWater, &unboundLigandAtomList))
	{
		return false;
	}
	// compute indices of unbound ligand atoms which correspond to bound ligand interface atoms
	std::vector<int> unboundLigandInterfaceAtomIndex; // For a given bound ligand interface atom, this points to corresponding atom in unbound ligand flat list
	int numFound = findCorrespondingInterfaceAtoms(&unboundLigandInterfaceAtomIndex, unboundLigandAtomList);
	{
		// print this out, might be useful
		FILE* fp = fopen("unboundLigandStats.txt", "a");
		fprintf(fp, "Unbound ligand: %s had %d of %lu interface atoms of bound ligand\n", unboundLigandFileName, numFound, m_LigandInterfaceAtom.size());
		fclose(fp);
	}
	// compute density and elec field to compute score.
	SimpleVolumeData* density = getProteinDensity();
	SimpleVolumeData* elecField = getProteinElecField();
	Vector c1 = getCenter(m_Protein);
	Vector c2 = getCenter(unboundLigandAtomList);
	// read transformation, get new set of 27 scores and rmsds and print out
	{
		FILE* fpIn = fopen(transformationsFilename, "r");
		if (!fpIn)
		{
			return false;
		}
		FILE* fpOut = fopen(outputFilename,  "w");
		if (!fpOut)
		{
			fclose(fpIn);
			return false;
		}
		for (int i=0; i<numberOfTransformations; i++)
		{
			float t[16];
			fscanf(fpIn, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				   &(t[0]),  &(t[1]),  &(t[2]),  &(t[3]),
				   &(t[4]),  &(t[5]),  &(t[6]),  &(t[7]),
				   &(t[8]),  &(t[9]),  &(t[10]), &(t[11]),
				   &(t[12]), &(t[13]), &(t[14]), &(t[15]));
			CCVOpenGLMath::Matrix transformation(t[0],  t[1],  t[2],  t[3],
												 t[4],  t[5],  t[6],  t[7],
												 t[8],  t[9],  t[10], t[11],
												 t[12], t[13], t[14], t[15]
												);
			printScoreAndRMSD(fpOut, transformation, unboundLigandAtomList, unboundLigandInterfaceAtomIndex, density, elecField, c1, c2);
			if (((i+1) % 20) == 0 || (i+1) == numberOfTransformations)
			{
				printf("%2.2f%% done \r", 100.0*(i+1)/(float)numberOfTransformations, i+1);
				fflush(stdout);
			}
		}
		printf("\n");
		fclose(fpIn);
		fclose(fpOut);
	}
	delete density;
	delete elecField;
	return true;
}
