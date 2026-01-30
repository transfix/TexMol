#include <cstring>
#include <Docking/DomainComplex.h>
#include <Docking/Segment.h>
#include <math.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GOAFileIO.h>
#include <PDBParser/GroupOfAtoms.h>
#include <UsefulMath/Quaternion.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using namespace std;
using PDBParser::GroupOfAtoms;
using PDBParser::Atom;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;
using CCVOpenGLMath::Quaternion;
using Docking::Segment;
using Docking::DomainComplex;

Segment::Segment(int startResidueIndex, int endResidueIndex, int domain1, int domain2, std::vector<int> residueList, DomainComplex* domainComplex)
{
	m_Domain1 = domain1;
	m_Domain2 = domain2;
	m_DomainComplex = domainComplex;
	if (startResidueIndex == -1 && endResidueIndex == -1)
	{
		m_StartResidueIndex = 0;
		m_EndResidueIndex = 0;
	}
	else
	{
		m_StartResidueIndex = startResidueIndex;
		m_EndResidueIndex = endResidueIndex;
		computeAtomList(residueList);
	}
}

Segment::~Segment()
{
	for (int i=0; i<m_AtomList.size(); i++)
	{
		delete m_AtomList[i];
	}
	m_AtomList.clear();
}

void Segment::computeAtomList(std::vector<int> residueList)
{
	m_AtomList.clear();
	for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
	{
		GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
		if (!residue)
		{
			continue;
		}
		for (int a=0; a<residue->m_Atoms.size(); a++)
		{
			m_AtomList.push_back(new Atom(*(residue->m_Atoms[a])));
		}
	}
}

void Segment::print(FILE* fp, std::vector<int> residueList)
{
	int chainId, startResidue, endResidue;
	if (!getEndPoints(residueList, &chainId, &startResidue, &endResidue))
	{
		fprintf(fp, "Chain 0 startRes 0 endRes 0 domain1 0 domain2 0\n");
	}
	else
	{
		fprintf(fp, "Chain %d startRes %d endRes %d domain1 %d domain2 %d\n", chainId, startResidue, endResidue, m_Domain1, m_Domain2);
	}
}

void Segment::read(FILE* fp, std::vector<int> residueList)
{
	int chainId, startResidue, endResidue;
	fscanf(fp, "Chain %d startRes %d endRes %d domain1 %d domain2 %d\n", &chainId, &startResidue, &endResidue, &m_Domain1, &m_Domain2);
	getStartEndIndices(residueList, chainId, startResidue, endResidue, &m_StartResidueIndex, &m_EndResidueIndex);
	computeAtomList(residueList);
}

bool Segment::getStartEndIndices(std::vector<int> residueList, int chainId, int startRes, int endRes, int* startIndex, int* endIndex)
{
	int res, chain;
	*startIndex = -1;
	*endIndex = -1;
	for (int i = 0; i < residueList.size()/2; i++)
	{
		res = residueList[i*2+0];
		chain = residueList[i*2+1];
		if (res == startRes && chain==chainId)
		{
			*startIndex = i;
			if (*endIndex >= 0)
			{
				return true;
			}
		}
		if (res == endRes && chain==chainId)
		{
			*endIndex = i;
			if (*startIndex >= 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool Segment::getEndPoints(std::vector<int> residueList, int* chainId, int* startResidue, int* endResidue)
{
	if (m_StartResidueIndex >= residueList.size()/2 || m_EndResidueIndex >= residueList.size()/2)
	{
		return false;
	}
	*chainId = residueList[m_StartResidueIndex*2+1]; // should also be same at end index
	*startResidue = residueList[m_StartResidueIndex*2+0];
	*endResidue = residueList[m_EndResidueIndex*2+0];
	return true;
}

bool Segment::printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, double red, double green, double blue, double alpha)
{
	if (!molecule || !fp)
	{
		return false;
	}
	int chainId, startResidue, endResidue;
	if (!getEndPoints(residueList, &chainId, &startResidue, &endResidue))
	{
		return false;
	}
	int i=startResidue;
	do
	{
		char resName[4];
		char actualChainId;
		int chainNum;
		if (!DomainComplex::getResidueData(resName, &actualChainId, &chainNum, chainId, i, molecule, 0))
		{
			return false;
		}
		//"0" "0" "A" "0" "79" "ALA" "" "" "1.0" "0.5" "0.5" "1.0"
		fprintf(fp, "\"0\" \"%d\" \"%c\" \"0\" \"%d\" \"%s\" \"\" \"\" \"%f\" \"%f\" \"%f\" \"%f\"\n",
				chainNum, actualChainId, i, resName, red, green, blue, alpha);
		i++; // SKVINAY are residues always there? some could be missing...
	}
	while (i<=endResidue);
	return true;
}

bool Segment::getAtoms(std::vector<Atom*>* atomList)
{
	atomList->clear();
	for (int i=0; i<m_AtomList.size(); i++)
	{
		Atom* atom = new Atom(*(m_AtomList[i]));
		atomList->push_back(atom);
	}
	return true;
}


bool Segment::appendAtoms(std::vector<Atom*>* atomList)
{
	for (int i=0; i<m_AtomList.size(); i++)
	{
		Atom* atom = new Atom(*(m_AtomList[i]));
		atomList->push_back(atom);
	}
	return true;
}

bool Segment::computeEndPoints(Vector* e1, Vector* e2)
{
	// get position of Calpha atom at end residues
	bool found1 = false;
	for (int i=0; i<m_AtomList.size(); i++)
	{
		Atom* atom = m_AtomList[i];
		if (strcmp(atom->getName(), " N  ") == 0)
		{
			(*e1)[0] = atom->m_Position[0];
			(*e1)[1] = atom->m_Position[1];
			(*e1)[2] = atom->m_Position[2];
			(*e1)[3] = 1;
			found1 = true;
			break;
		}
	}
	if (!found1)
	{
		return false;
	}
	for (int i=m_AtomList.size()-1; i>=0; i--)
	{
		Atom* atom = m_AtomList[i];
		if (strcmp(atom->getName(), " C  ") == 0)
		{
			(*e2)[0] = atom->m_Position[0];
			(*e2)[1] = atom->m_Position[1];
			(*e2)[2] = atom->m_Position[2];
			(*e2)[3] = 1;
			return true;
		}
	}
	return false;
}

bool Segment::computeMidPoint(Vector* m, std::vector<int> residueList)
{
	// here we assume that a segment is always within a chain!!
	int midResidueIndex = (m_StartResidueIndex + m_EndResidueIndex) / 2;
	int resSeq = residueList[midResidueIndex*2+0];
	for (int i=0; i<m_AtomList.size(); i++)
	{
		Atom* atom = m_AtomList[i];
		if (atom->getResidueNum() == resSeq && (strcmp(atom->getName(), " CA ")==0))
		{
			(*m)[0] = atom->m_Position[0];
			(*m)[1] = atom->m_Position[1];
			(*m)[2] = atom->m_Position[2];
			(*m)[3] = 1;
			return true;
		}
	}
	return false;
}

bool Segment::translate(CCVOpenGLMath::Vector disp)
{
	for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
	{
		GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
		if (!residue)
		{
			continue;
		}
		for (int a=0; a<residue->m_Atoms.size(); a++)
		{
			Atom* atom = residue->m_Atoms[a];
			atom->m_Position[0] += disp[0];
			atom->m_Position[1] += disp[1];
			atom->m_Position[2] += disp[2];
		}
	}
	{
		for (int i=0; i<m_AtomList.size(); i++)
		{
			delete m_AtomList[i];
		}
		m_AtomList.clear();
	}
	for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
	{
		GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
		if (!residue)
		{
			continue;
		}
		for (int a=0; a<residue->m_Atoms.size(); a++)
		{
			m_AtomList.push_back(new Atom(*(residue->m_Atoms[a])));
		}
	}
	return true;
}

bool Segment::translate(CCVOpenGLMath::Vector disp, int fixedDomainId)
{
	int len = m_EndResidueIndex - m_StartResidueIndex+1;
	Vector smallDisp = disp * (1.0/len);
	Vector curDisp = smallDisp;
	if (fixedDomainId == m_Domain1)
	{
		for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
		{
			GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
			if (!residue)
			{
				continue;
			}
			for (int a=0; a<residue->m_Atoms.size(); a++)
			{
				Atom* atom = residue->m_Atoms[a];
				atom->m_Position[0] += curDisp[0];
				atom->m_Position[1] += curDisp[1];
				atom->m_Position[2] += curDisp[2];
			}
			curDisp = curDisp + smallDisp;
		}
	}
	else
	{
		for (int i=m_EndResidueIndex; i>=m_StartResidueIndex; i--)
		{
			GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
			if (!residue)
			{
				continue;
			}
			for (int a=0; a<residue->m_Atoms.size(); a++)
			{
				Atom* atom = residue->m_Atoms[a];
				atom->m_Position[0] += curDisp[0];
				atom->m_Position[1] += curDisp[1];
				atom->m_Position[2] += curDisp[2];
			}
			curDisp = curDisp + smallDisp;
		}
	}
	{
		for (int i=0; i<m_AtomList.size(); i++)
		{
			delete m_AtomList[i];
		}
		m_AtomList.clear();
	}
	{
		for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
		{
			GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
			if (!residue)
			{
				continue;
			}
			for (int a=0; a<residue->m_Atoms.size(); a++)
			{
				m_AtomList.push_back(new Atom(*(residue->m_Atoms[a])));
			}
		}
	}
	return true;
}

bool Segment::rotate(CCVOpenGLMath::Matrix rotationMatrix)
{
	for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
	{
		GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
		if (!residue)
		{
			continue;
		}
		for (int a=0; a<residue->m_Atoms.size(); a++)
		{
			Atom* atom = residue->m_Atoms[a];
			Vector oldPos(atom->m_Position[0], atom->m_Position[1], atom->m_Position[2], 1);
			Vector newPos = rotationMatrix*oldPos;
			atom->m_Position[0] = newPos[0];
			atom->m_Position[1] = newPos[1];
			atom->m_Position[2] = newPos[2];
		}
	}
	{
		for (int i=0; i<m_AtomList.size(); i++)
		{
			delete m_AtomList[i];
		}
		m_AtomList.clear();
	}
	for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
	{
		GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
		if (!residue)
		{
			continue;
		}
		int a;
		for (int a=0; a<residue->m_Atoms.size(); a++)
		{
			m_AtomList.push_back(new Atom(*(residue->m_Atoms[a])));
		}
	}
	return true;
}

void Segment::getSum(float* x, float* y, float* z, int* n)
{
	for (int i=m_StartResidueIndex; i<=m_EndResidueIndex; i++)
	{
		GroupOfAtoms* residue = m_DomainComplex->getResidue(i);
		if (!residue)
		{
			continue;
		}
		for (int a=0; a<residue->m_Atoms.size(); a++)
		{
			Atom* atom = residue->m_Atoms[a];
			(*x) += atom->m_Position[0];
			(*y) += atom->m_Position[1];
			(*z) += atom->m_Position[2];
			(*n)++;
		}
	}
}

void Segment::write(FILE* fp)
{
	for (int i=0; i<m_AtomList.size(); i++)
	{
		Atom* a = m_AtomList[i];
		PDBParser::printAtomPDB(fp, a, 0);
	}
}
