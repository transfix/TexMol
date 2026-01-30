#include <Utility/utility.h>
#include <Docking/DomainComplex.h>
#include <Docking/Domain.h>
#include <Docking/FlexibleLoop.h>
#include <Docking/Segment.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using PDBParser::GroupOfAtoms;
using PDBParser::Atom;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;
using Docking::Domain;
using Docking::FlexibleLoop;
using Docking::DomainComplex;

Domain::Domain(DomainComplex* domainComplex)
{
	m_DomainComplex = domainComplex;
}

Domain::~Domain()
{
	for (int i=0; i<m_Domains.size(); i++)
	{
		delete m_Domains[i];
	}
	m_Domains.clear();
	for (int i=0; i<m_Segments.size(); i++)
	{
		delete m_Segments[i];
	}
	m_Segments.clear();
	int i;
	for (int i=0; i<m_FlexibleLoops.size(); i++)
	{
		delete m_FlexibleLoops[i];
	}
	m_FlexibleLoops.clear();
}

int Domain::getNumSegments() const
{
	return m_Segments.size();
}

bool Domain::parseProperties(const char* line)
{
	if (!line)
	{
		return false;
	}
	//Example: Domain 1 (yellow), 88 residues, similarity 105.13433198374884
	char color[1024];
	if (sscanf(line, "Domain %d %s %d residues, similarity %lf", &m_Id, color, &m_NumberOfResidues, &m_SimilarityIndex) != 4)
	{
		return false;
	}
	return true;
}

bool Domain::addNewSegment(const char* line, std::vector<int> residueList)
{
	if (!line)
	{
		return false;
	}
	// Example 1: Chain 1, Phe14 - Tyr33
	// Example 2: Chain 1, Gly43
	char c1, c2, c3, c4, c5, c6;
	int chainId, startRes, endRes;
	if (sscanf(line, "Chain %d, %c%c%c%d - %c%c%c%d", &chainId, &c1, &c2, &c3, &startRes, &c4, &c5, &c6, &endRes) == 9)
	{
		int startIndex, endIndex;
		if (!Segment::getStartEndIndices(residueList, chainId, startRes, endRes, &startIndex, &endIndex))
		{
			return false;
		}
		Segment* segment = new Segment(startIndex, endIndex, m_Id, m_Id, residueList, m_DomainComplex);
		m_Segments.push_back(segment);
		return true;
	}
	if (sscanf(line, "Chain %d, %c%c%c%d", &chainId, &c1, &c2, &c3, &startRes) == 5)
	{
		int startIndex, endIndex;
		if (!Segment::getStartEndIndices(residueList, chainId, startRes, startRes, &startIndex, &endIndex))
		{
			return false;
		}
		Segment* segment = new Segment(startIndex, endIndex, m_Id, m_Id, residueList, m_DomainComplex);
		m_Segments.push_back(segment);
		return true;
	}
	return false;
}

void Domain::read(FILE* fp, std::vector<int> residueList, int level)
{
	if (!fp)
	{
		return;
	}
	int tlevel;
	int numSegments, numFlexLoops, numChildren;
	fscanf(fp, "Level %d Id %d residues %d similarityIndex %lf segments %d flexloops %d children %d\n",
		   &tlevel, &m_Id, &m_NumberOfResidues, &m_SimilarityIndex, &numSegments, &numFlexLoops, &numChildren);
	fscanf(fp, "Segments\n");
	for (int i=0; i<numSegments; i++)
	{
		Segment* segment = new Segment(-1,-1,0,0, residueList, m_DomainComplex);
		segment->read(fp, residueList);
		m_Segments.push_back(segment);
	}
	fscanf(fp, "Flexible loops\n");
	for (int i=0; i<numFlexLoops; i++)
	{
		FlexibleLoop* flexibleLoop = new FlexibleLoop(-1,-1,0,0, residueList, m_DomainComplex);
		flexibleLoop->read(fp, residueList);
		m_FlexibleLoops.push_back(flexibleLoop);
	}
	for (int i=0; i<numChildren; i++)
	{
		Domain* domain = new Domain(m_DomainComplex);
		domain->read(fp, residueList, level+1);
		m_Domains.push_back(domain);
	}
}

void Domain::print(FILE* fp, std::vector<int> residueList, int level)
{
	if (!fp)
	{
		return;
	}
	fprintf(fp, "Level %d Id %d residues %d similarityIndex %lf segments %lu flexloops %lu children %lu\n",
			level, m_Id, m_NumberOfResidues, m_SimilarityIndex, m_Segments.size(), m_FlexibleLoops.size(), m_Domains.size());
	fprintf(fp, "Segments\n");
	for (int i=0; i<m_Segments.size(); i++)
	{
		m_Segments[i]->print(fp, residueList);
	}
	fprintf(fp, "Flexible loops\n");
	for (int i=0; i<m_FlexibleLoops.size(); i++)
	{
		m_FlexibleLoops[i]->print(fp, residueList);
	}
	for (int i=0; i<m_Domains.size(); i++)
	{
		m_Domains[i]->print(fp, residueList, level+1);
	}
}

bool Domain::isSubDomain(Domain* domain)
{
	if (!domain)
	{
		return false;
	}
	return true;
}

bool Domain::addNewDomain(Domain* domain, int reqLevel, int curLevel)
{
	if (curLevel == reqLevel)
	{
		// if ok, add it, else return false;
		if (isSubDomain(domain))
		{
			m_Domains.push_back(domain);
			return true;
		}
		return false;
	}
	else
	{
		// try children
		bool added = false;
		for (int i=0; i<m_Domains.size(); i++)
		{
			if (addNewDomain(domain, reqLevel, curLevel+1))
			{
				added = true;
				break;
			}
		}
		return added;
	}
}

bool Domain::addLoopToDomain(FlexibleLoop* flexibleLoop, int domainId, int reqLevel, int curLevel)
{
	if (!flexibleLoop)
	{
		return false;
	}
	if (curLevel == reqLevel)
	{
		if (m_Id == domainId)
		{
			m_FlexibleLoops.push_back(flexibleLoop);
			return true;
		}
		return false;
	}
	else if (curLevel < reqLevel)   // search children
	{
		for (int i=0; i<m_Domains.size(); i++)
		{
			if (m_Domains[i]->addLoopToDomain(flexibleLoop, domainId, reqLevel, curLevel+1))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return false;    // why did I come here ?
	}
}

bool Domain::printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, int levelReq, int curLevel)
{
	if (!molecule || !fp || (levelReq<0))
	{
		return false;
	}
	if (levelReq == curLevel)
	{
		srand(m_Id*levelReq);
		double baseSegmentRed   = (double)rand()/RAND_MAX;
		double baseSegmentGreen = (double)rand()/RAND_MAX;
		double baseSegmentBlue  = (double)rand()/RAND_MAX;
		if (m_Id == 1)
		{
			baseSegmentRed = 0.45;
			baseSegmentGreen = 0.2;
			baseSegmentBlue = 0.2;
		}
		if (m_Id == 2)
		{
			baseSegmentRed = 0.2;
			baseSegmentGreen = 0.45;
			baseSegmentBlue = 0.2;
		}
		if (m_Id == 3)
		{
			baseSegmentRed = 0.2;
			baseSegmentGreen = 0.2;
			baseSegmentBlue = 0.45;
		}
		double alpha = Docking::DOMAIN_ALPHA;
		for (int s=0; s<m_Segments.size(); s++)
		{
			fprintf(fp, "#\n");
			fprintf(fp, "#        Segment %d\n", s);
			fprintf(fp, "#\n");
			double red, green, blue;
			DomainComplex::getNearbyColor(baseSegmentRed, baseSegmentGreen, baseSegmentBlue, s, &red, &green, &blue);
			m_Segments[s]->printColorMap(molecule, residueList, fp, red, green, blue, alpha);
		}
		for (int f=0; f<m_FlexibleLoops.size(); f++)
		{
			fprintf(fp, "#\n");
			fprintf(fp, "#        Flexible loop %d\n", f);
			fprintf(fp, "#\n");
			m_FlexibleLoops[f]->printColorMap(molecule, residueList, fp, baseSegmentRed, baseSegmentGreen, baseSegmentBlue);
		}
	}
	else
	{
		for (int i=0; i<m_Domains.size(); i++)
		{
			m_Domains[i]->printColorMap(molecule, residueList, fp, levelReq, curLevel+1);
		}
	}
	return true;
}

bool Domain::getAtoms(std::vector<Atom*> *atomList)
{
	atomList->clear();
	for (int s=0; s<m_Segments.size(); s++)
	{
		m_Segments[s]->appendAtoms(atomList);
	}
	for (int f=0; f<m_FlexibleLoops.size(); f++)
	{
		m_FlexibleLoops[f]->appendAtoms(atomList);
	}
	printf("Domain has %lu atoms\n", atomList->size());
	return true;
}

bool Domain::translate(Vector disp)
{
	for (int s=0; s<m_Segments.size(); s++)
	{
		m_Segments[s]->translate(disp);
	}
	for (int f=0; f<m_FlexibleLoops.size(); f++)
	{
		m_FlexibleLoops[f]->translate(disp);
	}
	return true;
}

bool Domain::rotate(Matrix rotationMatrix)
{
	for (int s=0; s<m_Segments.size(); s++)
	{
		m_Segments[s]->rotate(rotationMatrix);
	}
	for (int f=0; f<m_FlexibleLoops.size(); f++)
	{
		m_FlexibleLoops[f]->rotate(rotationMatrix);
	}
	return true;
}

void Domain::write(FILE* fp)
{
	for (int s=0; s<m_Segments.size(); s++)
	{
		m_Segments[s]->write(fp);
	}
	for (int f=0; f<m_FlexibleLoops.size(); f++)
	{
		m_FlexibleLoops[f]->write(fp);
	}
}

Vector Domain::getCenter()
{
	float x, y, z;
	int n;
	for (int s=0; s<m_Segments.size(); s++)
	{
		m_Segments[s]->getSum(&x, &y, &z, &n);
	}
	for (int f=0; f<m_FlexibleLoops.size(); f++)
	{
		m_FlexibleLoops[f]->getSum(&x, &y, &z, &n);
	}
	Vector v;
	if (n==0)
	{
		return v;
	}
	x /= n;
	y /= n;
	z /= n;
	return v.set(x, y, z, 1);
}
