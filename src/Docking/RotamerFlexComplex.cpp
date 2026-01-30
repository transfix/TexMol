#include <Docking/ApbsFormatResidue.h>
#include <Docking/Complex.h>
#include <Docking/RotamerFlexComplex.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GroupOfAtoms.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <Utility/utility.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using PDBParser::GroupOfAtoms;
using PDBParser::Atom;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;
using Docking::RotamerFlexComplex;

RotamerFlexComplex::RotamerFlexComplex() : DockComplex()
{
	m_Residue2AtomIndex = 0;
	m_Atom2ResidueIndex = 0;
	m_IntersectingResidues = 0;
	m_NumResidues = 0;
}

RotamerFlexComplex::RotamerFlexComplex(PDBParser::GroupOfAtoms* protein, PDBParser::GroupOfAtoms* ligand) : DockComplex(protein, ligand)
{
	m_Residue2AtomIndex = 0;
	m_Atom2ResidueIndex = 0;
	m_IntersectingResidues = 0;
}

RotamerFlexComplex::~RotamerFlexComplex()
{
	if (m_Residue2AtomIndex)
	{
		delete []m_Residue2AtomIndex;
		m_Residue2AtomIndex = 0;
	}
	if (m_Atom2ResidueIndex)
	{
		delete []m_Atom2ResidueIndex;
		m_Atom2ResidueIndex = 0;
	}
	if (m_IntersectingResidues)
	{
		delete []m_IntersectingResidues;
		m_IntersectingResidues = 0;
	}
	{
		for (int i=0; i<m_Rotamers.size(); i++)
		{
			delete []m_Rotamers[i];
		}
		m_Rotamers.clear();
	}
}

bool RotamerFlexComplex::findInterfaceResidues(std::vector<Atom*> transUnbLigandAtoms, double distanceCutoff,
		SimpleVolumeData* density, std::vector<int>* idxUnbLigIntRes)
{
	if (!density)
	{
		return false;
	}
	if (transUnbLigandAtoms.size() == 0)
	{
		return false;
	}
	// go through ligand and search for all atoms within a cutoff and mark them
	double blobbiness = -2.3;
	double minFuncVal = exp(blobbiness*distanceCutoff*distanceCutoff);
	{
		for (int i=0; i<m_NumResidues; i++)
		{
			int start = m_Residue2AtomIndex[i];
			int end;
			if (i==m_NumResidues-1)
			{
				end = m_UnboundLigandAtomList.size()-1;
			}
			else
			{
				end = m_Residue2AtomIndex[i+1]-1;
			}
			for (int j=start; j<=end; j++)
			{
				Atom* atom = transUnbLigandAtoms[j];
				double x = atom->m_Position[0];
				double y = atom->m_Position[1];
				double z = atom->m_Position[2];
				if (density->getValueAt(0, x, y, z) >= minFuncVal)
				{
					idxUnbLigIntRes->push_back(i);
					break;
				}
			}
		}
	}
	printf("Found %lu interface residues\n", idxUnbLigIntRes->size());
	if (idxUnbLigIntRes->size() == 0)
	{
		return false;
	}
	return true;
}

void RotamerFlexComplex::transform(std::vector<Atom*> unboundLigandAtomList, Matrix transformation, std::vector<Atom*>* transUnbLigandAtoms)
{
	for (int i=0; i<unboundLigandAtomList.size(); i++)
	{
		Atom* atom = new Atom(*(unboundLigandAtomList[i]));
		Vector center(atom->m_Position[0], atom->m_Position[1], atom->m_Position[2], 1);
		Vector trCenter = transformation*center;
		atom->m_Position[0] = trCenter[0];
		atom->m_Position[1] = trCenter[1];
		atom->m_Position[2] = trCenter[2];
		transUnbLigandAtoms->push_back(atom);
	}
}

double RotamerFlexComplex::shapeContribution(double density)
{
	if (density < 0.001)
	{
		return -0.01;    // too far away
	}
	if (density > 3.00)
	{
		return -0.1;
	}
	return 0.2;
}

double RotamerFlexComplex::elecContribution(double elecValue, Atom* atom)
{
	return -1* atom->getCharge() * elecValue;
}

double RotamerFlexComplex::getAtomScore(Atom* atom, SimpleVolumeData* density, SimpleVolumeData* elecField)
{
	double x = atom->m_Position[0];
	double y = atom->m_Position[1];
	double z = atom->m_Position[2];
	return 	shapeContribution(density->getValueAt(0, x, y, z)) +
			10*elecContribution(elecField->getValueAt(0, x, y, z), atom);
}

double RotamerFlexComplex::getRMSD(std::vector<Atom*> transUnboundLigandAtomList, std::vector<int> unboundLigandInterfaceAtomIndex,
								   double* score, SimpleVolumeData* density, SimpleVolumeData* elecField)
{
	if (m_LigandInterfaceAtom.size() == 0)
	{
		return 0;
	}
	double rmsd = 0;
	*score = 0;
	int numFound = 0;
	//FILE* fp = fopen("oldnew.txt","w");
	for (int i=0; i<m_LigandInterfaceAtom.size(); i++)
	{
		int index = unboundLigandInterfaceAtomIndex[i];
		if (index == -1)
		{
			continue;
		}
		numFound++;
		Atom* new_atom = transUnboundLigandAtomList[index];
		Atom* old_atom = m_LigandAtomList[m_LigandInterfaceAtom[i]];
		CCVOpenGLMath::Vector oldPos(old_atom->m_Position[0], old_atom->m_Position[1], old_atom->m_Position[2], 1);
		CCVOpenGLMath::Vector newPos(new_atom->m_Position[0], new_atom->m_Position[1], new_atom->m_Position[2], 1);
		rmsd += (oldPos[0]-newPos[0])*(oldPos[0]-newPos[0]) +
				(oldPos[1]-newPos[1])*(oldPos[1]-newPos[1]) +
				(oldPos[2]-newPos[2])*(oldPos[2]-newPos[2]);
		*score += getAtomScore(new_atom, density, elecField);
	}
	if (numFound == 0)
	{
		return 100000;
	}
	return sqrt(rmsd / (double)(numFound));
}

void RotamerFlexComplex::copy(std::vector<Atom*> transUnbLigandAtoms, std::vector<Atom*>* copyTrUnbLigandAtoms)
{
	for (int i=0; i<transUnbLigandAtoms.size(); i++)
	{
		copyTrUnbLigandAtoms->push_back(new Atom(*(transUnbLigandAtoms[i])));
	}
}

void RotamerFlexComplex::set(std::vector<Atom*> transUnbLigandAtoms, std::vector<Atom*>* copyTrUnbLigandAtoms)
{
	for (int i=0; i<transUnbLigandAtoms.size(); i++)
	{
		(copyTrUnbLigandAtoms->at(i))->m_Position[0] = transUnbLigandAtoms[i]->m_Position[0];
		(copyTrUnbLigandAtoms->at(i))->m_Position[1] = transUnbLigandAtoms[i]->m_Position[1];
		(copyTrUnbLigandAtoms->at(i))->m_Position[2] = transUnbLigandAtoms[i]->m_Position[2];
	}
}

double RotamerFlexComplex::getRMSD(std::vector<Atom*> v1, std::vector<Atom*> v2)
{
	double rmsd = 0;
	for (int j=0; j<v1.size(); j++)
	{
		double x1 = v1[j]->m_Position[0];
		double y1 = v1[j]->m_Position[1];
		double z1 = v1[j]->m_Position[2];
		double x2 = v2[j]->m_Position[0];
		double y2 = v2[j]->m_Position[1];
		double z2 = v2[j]->m_Position[2];
		rmsd += (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
		rmsd = sqrt(rmsd/((double)v1.size()));
	}
	return rmsd;
}

bool RotamerFlexComplex::computeUnbLigIntResScores(std::vector<int> unbLigIntRes, std::vector<Atom*> transUnbLigandAtoms, double* curScore,
		SimpleVolumeData* density, SimpleVolumeData* elecField)
{
	if (!density || !elecField || !curScore)
	{
		return false;
	}
	for (int r=0; r<unbLigIntRes.size(); r++)
	{
		int res = unbLigIntRes[r];
		int start = m_Residue2AtomIndex[res];
		int end;
		if (res==m_NumResidues-1)
		{
			end = m_UnboundLigandAtomList.size()-1;
		}
		else
		{
			end = m_Residue2AtomIndex[res+1]-1;
		}
		// go through atoms, compute three scores and set them
		double shapeComplScore = 0;
		double elecComplScore = 0;
		double shapeSelfIntScore = 0;
		for (int a=start; a<=end; a++)
		{
			Atom* atom = transUnbLigandAtoms[a];
			double x = atom->m_Position[0];
			double y = atom->m_Position[1];
			double z = atom->m_Position[2];
			shapeComplScore += shapeContribution(density->getValueAt(0, x, y, z));
			elecComplScore += elecContribution(elecField->getValueAt(0, x, y, z), atom);
			// go through intersecting residues and add up any clashes
		}
		curScore[r*3+0] = shapeComplScore;
		curScore[r*3+1] = elecComplScore;
		curScore[r*3+2] = shapeSelfIntScore;
		printf("[%lf %lf %lf] ", shapeComplScore, elecComplScore, shapeSelfIntScore);
	}
	return true;
}

bool RotamerFlexComplex::pickAndTransformRotamers(float* rotPositions, Matrix transformation, int predictionId,
		std::vector<int> unbLigIntRes, double* rotScore,
		SimpleVolumeData* density, SimpleVolumeData* elecField,
		std::vector<Atom*> transUnbLigandAtoms, double*  curScore) // last 2 param just for printing
{
	for (int i=0; i<unbLigIntRes.size(); i++)
	{
		int r = unbLigIntRes[i];
		if (!m_Rotamers[r])
		{
			continue;
		}
		// pick a rotamer, enter its transformed values and compute its score
		int rotamerIndex = 0;
		{
			if (m_NumRotamers[r] == 2)
			{
				rotamerIndex = predictionId%2;
			}
			else if (m_NumRotamers[r] == 3)
			{
				rotamerIndex = predictionId%3;
			}
			else
			{
				int category = predictionId%3;
				int rotamersInCategory = m_NumRotamers[r]/3;
				int randRotamerInCategory = rotamersInCategory*rand()/(RAND_MAX);
				rotamerIndex = category*rotamersInCategory + randRotamerInCategory;
			}
			if (rotamerIndex >= m_NumRotamers[r])
			{
				rotamerIndex = m_NumRotamers[r]-1;
			}
		}
		{
			int start = m_Residue2AtomIndex[r];
			int end;
			if (r==m_NumResidues-1)
			{
				end = m_UnboundLigandAtomList.size()-1;
			}
			else
			{
				end = m_Residue2AtomIndex[r+1]-1;
			}
			int len = end-start+1;
			{
				int bigArrayPos = start;
				rotScore[i*3+0] = rotScore[i*3+1] = rotScore[i*3+2] = 0;
				for (int k=rotamerIndex*len; k<(rotamerIndex+1)*len; k++)
				{
					Vector oldPos = Vector(m_Rotamers[r][k*3+0], m_Rotamers[r][k*3+1], m_Rotamers[r][k*3+2], 1);
					Vector newPos = transformation*oldPos;
					rotPositions[bigArrayPos*3+0] = newPos[0];
					rotPositions[bigArrayPos*3+1] = newPos[1];
					rotPositions[bigArrayPos*3+2] = newPos[2];
					rotScore[i*3+0] += shapeContribution(density->getValueAt(0, newPos[0], newPos[1], newPos[2]));
					rotScore[i*3+1] += -1*elecField->getValueAt(0, newPos[0], newPos[1], newPos[2])*transUnbLigandAtoms[bigArrayPos]->getCharge();
					bigArrayPos++;
				}
			}
		}
	}
	return true;
}

int RotamerFlexComplex::getGoodRotamersSorted(int* rotamerSortedIndex, double* curScore, double* rotScore, int nIntRes)
{
	double* combinedScore = new double[nIntRes];
	for (int i=0; i<nIntRes; i++)
	{
		combinedScore[i] =	(curScore[i*3+0] - rotScore[i*3+0]) +
							(curScore[i*3+1] - rotScore[i*3+1]) +
							(curScore[i*3+2] - rotScore[i*3+2]);
	}
	for (int i=0; i<nIntRes-1; i++)
	{
		for (int j=i+1; j<nIntRes; j++)
		{
			if (combinedScore[j] > combinedScore[i])
			{
				double tscore = combinedScore[j];
				combinedScore[j] = combinedScore[i];
				combinedScore[i] = tscore;
				int tindex = rotamerSortedIndex[i];
				rotamerSortedIndex[i] = rotamerSortedIndex[j];
				rotamerSortedIndex[j] = tindex;
			}
		}
	}
	int numGood = 0;
	for (int i=0; i<nIntRes; i++)
	{
		if (combinedScore[i] > 0)
		{
			numGood++;
		}
	}
	delete []combinedScore;
	return numGood;
}

bool RotamerFlexComplex::update(std::vector<Atom*> copyTrUnbLigandAtoms, float* rotPositions, std::vector<int> unbLigIntRes, int rotIndex, int* numAtoms)
{
	if (!rotPositions)
	{
		return false;
	}
	if (rotIndex >= unbLigIntRes.size())
	{
		return false;
	}
	int r = unbLigIntRes[rotIndex];
	if (!m_Rotamers[r])
	{
		return false;
	}
	int start = m_Residue2AtomIndex[r];
	int end;
	if (r==m_NumResidues-1)
	{
		end = m_UnboundLigandAtomList.size()-1;
	}
	else
	{
		end = m_Residue2AtomIndex[r+1]-1;
	}
	double rmsd = 0;
	for (int a=start; a<=end; a++)
	{
		double x1 = copyTrUnbLigandAtoms[a]->m_Position[0];
		double y1 = copyTrUnbLigandAtoms[a]->m_Position[1];
		double z1 = copyTrUnbLigandAtoms[a]->m_Position[2];
		double x2 = rotPositions[a*3+0];
		double y2 = rotPositions[a*3+1];
		double z2 = rotPositions[a*3+2];
		rmsd += (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
		rmsd = sqrt(rmsd/((double)(end-start+1.0)));
		copyTrUnbLigandAtoms[a]->m_Position[0] = rotPositions[a*3+0];
		copyTrUnbLigandAtoms[a]->m_Position[1] = rotPositions[a*3+1];
		copyTrUnbLigandAtoms[a]->m_Position[2] = rotPositions[a*3+2];
	}
	printf("%lf\n", rmsd);
	*numAtoms += end-start+1;
	return true;
}

bool RotamerFlexComplex::improveInterfaceGivenTransformation(FILE* fpOut, CCVOpenGLMath::Matrix transformation,
		std::vector<int> unboundLigandInterfaceAtomIndex, SimpleVolumeData* density, SimpleVolumeData* elecField,
		Vector c1, Vector c2, int numNewPredictions, double distanceCutoff)
{
	if (!fpOut || !density || !elecField)
	{
		return false;
	}
	int numLigandAtoms = m_UnboundLigandAtomList.size();
	// compute transformed copy of ligand
	std::vector<Atom*> transUnbLigandAtoms;
	transform(m_UnboundLigandAtomList, transformation, &transUnbLigandAtoms);
	// For each residue, keep
	// 2. Interface or not
	// 3. Valid or not
	// 2. Score, Cur score
	// 3. Rotamers and their scores, cur rotamer scores
	// find interface residues
	std::vector<int> unbLigIntRes;
	if (!findInterfaceResidues(transUnbLigandAtoms, distanceCutoff, density, &unbLigIntRes))
	{
		return false;
	}
	int nIntRes = unbLigIntRes.size();
	double* curScore = new double[nIntRes*3]; // shape with other protein, elec with other protein and self intersection
	double* rotScore = new double[nIntRes*3]; // for given prediction, we use only one rotamer. We try to sample as good as possible
	float* rotPositions = new float[transUnbLigandAtoms.size()*3]; // array is unneccessarily big
	int* rotamerSortedIndex = new int[nIntRes];
	{
		for (int i=0; i<nIntRes; i++)
		{
			rotamerSortedIndex[i] = i;
		}
	}
	// compute residue scores
	computeUnbLigIntResScores(unbLigIntRes, transUnbLigandAtoms, curScore, density, elecField);
	printf("\n");
	// make new copy of ligand
	std::vector<Atom*> copyTrUnbLigandAtoms;
	copy(transUnbLigandAtoms, &copyTrUnbLigandAtoms);
	int maxTrials = numNewPredictions*3;
	int succeeded = 0;
	int trial = 0;
	while (succeeded < numNewPredictions && trial < maxTrials)
	{
		printf("Trial %d\n", trial);
		set(transUnbLigandAtoms, &copyTrUnbLigandAtoms);
		// pick a rotamer for each residue based on rand and probability
		// try to include all 3 groups for chi1
		pickAndTransformRotamers(rotPositions, transformation, trial, unbLigIntRes, rotScore, density, elecField, transUnbLigandAtoms, curScore);
		int numPositiveEntries = getGoodRotamersSorted(rotamerSortedIndex, curScore, rotScore, nIntRes);
		if (numPositiveEntries > 0)
		{
			int numAtomsUpdated = 0;
			for (int r=0; r<numPositiveEntries; r++)
			{
				// pick highest score, apply, update intersecting rotamers
				// if( no +ves left)
				//		break;
				// sort in order, apply all positives.
				update(copyTrUnbLigandAtoms, rotPositions, unbLigIntRes, rotamerSortedIndex[r], &numAtomsUpdated);
			}
			// compute and print RMSD and score
			double score=0, rmsd=0;
			rmsd = getRMSD(copyTrUnbLigandAtoms, unboundLigandInterfaceAtomIndex, &score, density, elecField);
			double oldScore=0;
			double rmsd2 = getRMSD(transUnbLigandAtoms, unboundLigandInterfaceAtomIndex, &oldScore, density, elecField);
			fprintf(fpOut, "%lf %lf %lf %lf %d\n", score, rmsd, oldScore, rmsd2, numAtomsUpdated);
			succeeded++;
		}
		trial++;
	}
	{
		for (int i=0; i<copyTrUnbLigandAtoms.size(); i++)
		{
			delete copyTrUnbLigandAtoms[i];
		}
		copyTrUnbLigandAtoms.clear();
	}
	delete []rotamerSortedIndex;
	delete []rotPositions;
	delete []curScore;
	delete []rotScore;
	unbLigIntRes.clear();
	{
		for (int i=0; i<transUnbLigandAtoms.size(); i++)
		{
			delete transUnbLigandAtoms[i];
		}
		transUnbLigandAtoms.clear();
	}
	return true;
}

void RotamerFlexComplex::printInfo()
{
	FILE* fp = fopen("resStats.txt", "w");
	fprintf(fp, "Num atoms = %lu num res = %d\n", m_UnboundLigandAtomList.size(), m_NumResidues);
	fprintf(fp, "Atom 2 res indices:\n");
	fprintf(fp, "%d.", m_Atom2ResidueIndex[0]);
	int curA2R = m_Atom2ResidueIndex[0];
	for (int i=1; i<m_UnboundLigandAtomList.size(); i++)
	{
		if (m_Atom2ResidueIndex[i] != curA2R)
		{
			curA2R = m_Atom2ResidueIndex[i];
			fprintf(fp, "\n");
		}
		fprintf(fp, "%d.", curA2R);
	}
	fprintf(fp, "\n");
	fprintf(fp, "Res 2 Atom indices:\n");
	for (int i=0; i<m_NumResidues; i++)
	{
		fprintf(fp, " %d ", m_Residue2AtomIndex[i]);
	}
	fprintf(fp, "\n");
	fprintf(fp, "Intersecting residues:\n");
	for (int i=0; i<m_NumResidues; i++)
	{
		for (int j=0; j<m_IntersectingResidues[i].size(); j++)
		{
			fprintf(fp, " %d ", m_IntersectingResidues[i][j]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "Rotamers:\n");
	for (int i=0; i<m_NumResidues; i++)
	{
		int start = m_Residue2AtomIndex[i];
		int end;
		if (i==m_NumResidues-1)
		{
			end = m_UnboundLigandAtomList.size()-1;
		}
		else
		{
			end = m_Residue2AtomIndex[i+1]-1;
		}
		int len = end-start+1;
		fprintf(fp, "Old pos:\n");
		{
			for (int j=start; j<=end; j++)
			{
				fprintf(fp, " [%f %f %f]", m_UnboundLigandAtomList[j]->m_Position[0], m_UnboundLigandAtomList[j]->m_Position[1], m_UnboundLigandAtomList[j]->m_Position[2]);
			}
			fprintf(fp, "\n");
		}
		if (m_Rotamers[i])
		{
			fprintf(fp, "Has %d rotamers:\n", m_NumRotamers[i]);
			for (int j=0; j<m_NumRotamers[i]; j++)
			{
				double rmsd = 0;
				int c = start;
				for (int k=j*len; k<(j+1)*len; k++)
				{
					double x1 = m_Rotamers[i][k*3+0];
					double y1 = m_Rotamers[i][k*3+1];
					double z1 = m_Rotamers[i][k*3+2];
					double x2 = m_UnboundLigandAtomList[c]->m_Position[0];
					double y2 = m_UnboundLigandAtomList[c]->m_Position[1];
					double z2 = m_UnboundLigandAtomList[c]->m_Position[2];
					c++;
					rmsd += (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
					rmsd = sqrt(rmsd/((double)(end-start+1)));
					fprintf(fp, " [%f %f %f]", m_Rotamers[i][k*3+0], m_Rotamers[i][k*3+1], m_Rotamers[i][k*3+2]);
				}
				fprintf(fp, "\n");
				fprintf(fp, "RMSD: %lf\n", rmsd);
			}
			fprintf(fp, "\n");
		}
		else
		{
			fprintf(fp, "No luck\n");
		}
		fprintf(fp, "\n\n");
	}
	fclose(fp);
}

// find how many residues, allocate residue2Atom array, fill it up. Fill up Atom2residue array
// check which residues intersect.
bool RotamerFlexComplex::getUnbLigResiduesInfo()
{
	int numAtoms = m_UnboundLigandAtomList.size();
	if (numAtoms < 1)
	{
		return false;
	}
	m_Atom2ResidueIndex = new int[numAtoms];
	int curResIndex = 0;
	{
		int chain = m_UnboundLigandAtomList[0]->getChain();
		int residueNum = m_UnboundLigandAtomList[0]->getResidueNum();
		m_Atom2ResidueIndex[0] = curResIndex;
		for (int i=1; i<numAtoms; i++)
		{
			Atom* atom = m_UnboundLigandAtomList[i]; // assume it exists
			int newChain = (int)(atom->getChain());
			int newResidueNum = (int)(atom->getResidueNum());
			if (newChain != chain || newResidueNum != residueNum)
			{
				curResIndex++;
				chain = newChain;
				residueNum = newResidueNum;
			}
			m_Atom2ResidueIndex[i] = curResIndex;
		}
		if (curResIndex == 0)
		{
			delete []m_Atom2ResidueIndex;
			m_Atom2ResidueIndex = 0;
			return false;
		}
	}
	m_NumResidues = curResIndex+1;
	m_Residue2AtomIndex = new int[m_NumResidues];
	{
		int curAtomResIndex = 0;
		int res = 0;
		m_Residue2AtomIndex[res] = curAtomResIndex;
		for (int i=1; i<numAtoms; i++)
		{
			if (m_Atom2ResidueIndex[i] != curAtomResIndex)
			{
				curAtomResIndex = m_Atom2ResidueIndex[i];
				res++;
				if (res >= m_NumResidues)
				{
					printf("Problem!\n");
				}
				m_Residue2AtomIndex[res] = i;
			}
		}
	}
	m_IntersectingResidues = new std::vector<int>[m_NumResidues];
	{
		float* centerRadii = new float[m_NumResidues*4];
		for (int i=0; i<m_NumResidues; i++)
		{
			float min[3], max[3];
			int start = m_Residue2AtomIndex[i];
			int end;
			if (i==m_NumResidues-1)
			{
				end = numAtoms-1;
			}
			else
			{
				end = m_Residue2AtomIndex[i+1]-1;
			}
			float r = m_UnboundLigandAtomList[start]->getRadius();
			min[0] = m_UnboundLigandAtomList[start]->m_Position[0]-r;
			min[1] = m_UnboundLigandAtomList[start]->m_Position[1]-r;
			min[2] = m_UnboundLigandAtomList[start]->m_Position[2]-r;
			max[0] = m_UnboundLigandAtomList[start]->m_Position[0]+r;
			max[1] = m_UnboundLigandAtomList[start]->m_Position[1]+r;
			max[2] = m_UnboundLigandAtomList[start]->m_Position[2]+r;
			for (int a=start+1; a<=end; a++)
			{
				float x = m_UnboundLigandAtomList[a]->m_Position[0];
				float y = m_UnboundLigandAtomList[a]->m_Position[1];
				float z = m_UnboundLigandAtomList[a]->m_Position[2];
				float r = m_UnboundLigandAtomList[a]->getRadius();
				if (min[0] > x-r)
				{
					min[0] = x-r;
				}
				if (min[1] > y-r)
				{
					min[1] = y-r;
				}
				if (min[2] > z-r)
				{
					min[2] = z-r;
				}
				if (max[0] < x+r)
				{
					max[0] = x+r;
				}
				if (max[1] < y+r)
				{
					max[1] = y+r;
				}
				if (max[2] < z+r)
				{
					max[2] = z+r;
				}
			}
			// find center radius
			centerRadii[i*4+0] = (min[0]+max[0]) / 2.0;
			centerRadii[i*4+1] = (min[1]+max[1]) / 2.0;
			centerRadii[i*4+2] = (min[2]+max[2]) / 2.0;
			centerRadii[i*4+3] = sqrt((max[0]-min[0])*(max[0]-min[0]) +
									  (max[1]-min[1])*(max[1]-min[1]) +
									  (max[2]-min[2])*(max[2]-min[2])) / 2.0;
		}
		for (int i=0; i<m_NumResidues-1; i++)
		{
			float ix = centerRadii[i*4+0];
			float iy = centerRadii[i*4+1];
			float iz = centerRadii[i*4+2];
			float ir = centerRadii[i*4+3];
			for (int j=i+1; j<m_NumResidues; j++)
			{
				float jx = centerRadii[j*4+0];
				float jy = centerRadii[j*4+1];
				float jz = centerRadii[j*4+2];
				float jr = centerRadii[j*4+3];
				// see if i and jth residue intersect and if so, add to both
				if ((ix-jx)*(ix-jx) + (iy-jy)*(iy-jy) + (iz-jz)*(iz-jz) < (ir+jr)*(ir+jr))
				{
					m_IntersectingResidues[i].push_back(j);
					m_IntersectingResidues[j].push_back(i);
				}
			}
		}
		delete []centerRadii;
	}
	// compute all rotamers
	{
		for (int r=0; r<m_NumResidues; r++)
		{
			// make copy of residue
			std::vector<Atom*> residue;
			int start = m_Residue2AtomIndex[r];
			int end;
			if (r==m_NumResidues-1)
			{
				end = numAtoms-1;
			}
			else
			{
				end = m_Residue2AtomIndex[r+1]-1;
			}
			for (int i=start; i<=end; i++)
			{
				residue.push_back(new Atom(*(m_UnboundLigandAtomList[i])));
			}
			float* rotamerPositions=0;
			ApbsFormatResidue* apbsFormatResidue = new ApbsFormatResidue();
			int numRotamers = 0;
			apbsFormatResidue->constructRotamers(residue, &rotamerPositions, &numRotamers);
			delete apbsFormatResidue;
			m_Rotamers.push_back(rotamerPositions);
			m_NumRotamers.push_back(numRotamers);
		}
	}
	return true;
}

bool RotamerFlexComplex::improveInterface(const char* unboundLigandFileName, int numberOfTransformations, double distanceCutoff,
		const char* transformationsFilename, const char* outputFilename, bool flexDock, int numNewPredictions)
{
	if (!flexDock)
		return DockComplex::improveInterface(unboundLigandFileName, numberOfTransformations, distanceCutoff,transformationsFilename,
										 outputFilename, flexDock);
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
	bool deleteWater = false;
	if (!getUnboundLigandAtoms(unboundLigandFileName, deleteWater, &m_UnboundLigandAtomList))
	{
		return false;
	}
	// compute residue indices, intersecting residues, and create all rotamers
	if (!getUnbLigResiduesInfo())
	{
		return false;
	}
	// compute indices of unbound ligand atoms which correspond to bound ligand interface atoms
	std::vector<int> unboundLigandInterfaceAtomIndex; // For a given bound ligand interface atom, this points to corresponding atom in unbound ligand flat list
	int numFound = findCorrespondingInterfaceAtoms(&unboundLigandInterfaceAtomIndex, m_UnboundLigandAtomList);
	// compute density and elec field to compute score.
	SimpleVolumeData* density = getProteinDensity();
	SimpleVolumeData* elecField = getProteinElecField();
	//SimpleVolumeData* elecField = getProteinDensity();
	Vector c1 = getCenter(m_Protein);
	Vector c2 = getCenter(m_UnboundLigandAtomList);
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
			printf("New transformation %d\n---------------------\n", i);
			float t[16];
			fscanf(fpIn, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				   &(t[0]),  &(t[1]),  &(t[2]),  &(t[3]),
				   &(t[4]),  &(t[5]),  &(t[6]),  &(t[7]),
				   &(t[8]),  &(t[9]),  &(t[10]), &(t[11]),
				   &(t[12]), &(t[13]), &(t[14]), &(t[15]));
			CCVOpenGLMath::Matrix transformation(t[0],  t[1],  t[2],  t[3], t[4],  t[5],  t[6],  t[7], t[8],  t[9],  t[10], t[11], t[12], t[13], t[14], t[15]);
			// get best fit
			improveInterfaceGivenTransformation(fpOut, transformation, unboundLigandInterfaceAtomIndex, density, elecField, c1, c2, numNewPredictions, distanceCutoff);
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
