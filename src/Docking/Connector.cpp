#include <cstring>
#include <Docking/Connector.h>
#include <Docking/DomainComplex.h>
#include <Docking/Segment.h>
#include <math.h>
#include <PDBParser/Atom.h>
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
using Docking::Connector;
using Docking::Segment;
using Docking::DomainComplex;

Connector::Connector(int startResidueIndex, int endResidueIndex, int domain1, int domain2, int level, std::vector<int> residueList, DomainComplex* domainComplex)
	: Segment(startResidueIndex, endResidueIndex, domain1, domain2, residueList, domainComplex)
{
	m_Level = level;
}

Connector::~Connector()
{
}

void Connector::print(FILE* fp, std::vector<int> residueList)
{
	if (!fp)
	{
		return;
	}
	int chainId, startResidue, endResidue;
	if (!getEndPoints(residueList, &chainId, &startResidue, &endResidue))
	{
		fprintf(fp, "Chain 0 startRes 0 endRes 0 domain1 0 domain2 0 level %d\n", m_Level);
	}
	else
	{
		fprintf(fp, "Chain %d startRes %d endRes %d domain1 %d domain2 %d level %d\n", chainId, startResidue, endResidue, m_Domain1, m_Domain2, m_Level);
	}
}

void Connector::read(FILE* fp, std::vector<int> residueList)
{
	if (!fp)
	{
		return;
	}
	int chainId, startResidue, endResidue;
	fscanf(fp, "Chain %d startRes %d endRes %d domain1 %d domain2 %d level %d\n",
		   &chainId, &startResidue, &endResidue, &m_Domain1, &m_Domain2, &m_Level);
	getStartEndIndices(residueList, chainId, startResidue, endResidue, &m_StartResidueIndex, &m_EndResidueIndex);
	computeAtomList(residueList);
}

bool Connector::printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, int level)
{
	if (!molecule || !fp || (level<0))
	{
		return false;
	}
	if (level != m_Level)
	{
		return true;    // why false?
	}
	return Segment::printColorMap(molecule, residueList, fp, 0.2, 0.2, 0.2, Docking::CONNECTOR_ALPHA);
}

// rotate appropriate end point. Find displacement vector. Displace half residues using it.
bool Connector::rotate(CCVOpenGLMath::Matrix rotationMatrix, int fixedDomainId)
{
	int numR = m_EndResidueIndex - m_StartResidueIndex+1;
	if (numR < 3)
	{
		return true;
	}
	Vector disp;
	{
		// find last CA atom
		GroupOfAtoms* endResidue = 0;
		if (fixedDomainId == m_Domain1)
		{
			endResidue = m_DomainComplex->getResidue(m_EndResidueIndex);
		}
		else
		{
			endResidue = m_DomainComplex->getResidue(m_StartResidueIndex);
		}
		bool caNotFound = true;
		for (int a=0; a<endResidue->m_Atoms.size(); a++)
		{
			Atom* atom = endResidue->m_Atoms[a];
			if (strcmp(atom->getName(), " CA ") != 0)
			{
				continue;
			}
			Vector oldPos(atom->m_Position[0], atom->m_Position[1], atom->m_Position[2], 1);
			Vector newPos = rotationMatrix*oldPos;
			disp = newPos - oldPos;
			caNotFound = false;
		}
		if (caNotFound)
		{
			return false;
		}
	}
	// from mid to end, displace using appropriate amounts
	{
		Vector smallDisp = disp * (1.0/(numR/2.0));
		Vector curDisp = smallDisp;
		int midResidueIndex = (m_EndResidueIndex+m_StartResidueIndex)/2;
		if (fixedDomainId == m_Domain1)
		{
			for (int i=midResidueIndex; i<=m_EndResidueIndex; i++)
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
			for (int i=m_EndResidueIndex; i>=midResidueIndex; i--)
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

bool Connector::rotate(CCVOpenGLMath::Matrix rotationMatrix)
{
	return Segment::rotate(rotationMatrix);
}
