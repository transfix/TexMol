#include <Blurmaps/BlurMapsDataManager.h>
#include <Docking/Connector.h>
#include <Docking/DomainComplex.h>
#include <Docking/Domain.h>
#include <Docking/Flexibility.h>
#include <Docking/FlexibleLoop.h>
#include <GOAFileTypes/GOALoader.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GroupOfAtoms.h>
#include <UsefulMath/Vector.h>
#include <Utility/utility.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using PDBParser::GroupOfAtoms;
//using PDBParser::Atom;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;
using Docking::DomainComplex;
using Docking::Domain;
using Docking::FlexibleLoop;
using Docking::Flexibility;
using CCVOpenGLMath::Vector;

DomainComplex::DomainComplex(PDBParser::GroupOfAtoms* molecule)
{
	m_MaxLevelId = -1;
	m_Molecule = molecule;
	computeResidueList(molecule, 0, 0);
}

DomainComplex::~DomainComplex()
{
	clearData();
}

void DomainComplex::clearData()
{
	for (int i=0; i<m_Domains.size(); i++)
	{
		delete m_Domains[i];
	}
	m_Domains.clear();
	for (int i=0; i<m_Connectors.size(); i++)
	{
		delete m_Connectors[i];
	}
	m_Connectors.clear();
	for (int i=0; i<m_Flexibility.size(); i++)
	{
		delete m_Flexibility[i];
	}
	m_Flexibility.clear();
}

// we need subgroupIndex because domain finder puts its own ******* chain ids. (It even gets it wrong at times - 1KKL_r_u.pdb)
void DomainComplex::computeResidueList(GroupOfAtoms* molecule, int chainId, int subgroupIndex)
{
	printf("computeRes ");
	if (molecule->type == PDBParser::CHAIN_TYPE)
	{
		chainId = subgroupIndex;
		printf("CHAIN TYPE ");
	}
	else if (molecule->type == PDBParser::RESIDUE_TYPE)
	{
		m_ResidueList.push_back(molecule->m_ResSeq);
		m_ResidueList.push_back(chainId);
		//// keep the list sorted (first by chain id, and then by residue number) --- Rezaul
		int i = m_ResidueList.size() - 2, t;
		while ((i > 0) && ((m_ResidueList[ i + 1 ] < m_ResidueList[ i - 1 ])
						   || ((m_ResidueList[ i + 1 ] == m_ResidueList[ i - 1 ]) && (m_ResidueList[ i ] < m_ResidueList[ i - 2 ]))))
		{
			t = m_ResidueList[ i ];
			m_ResidueList[ i ] = m_ResidueList[ i - 2 ];
			m_ResidueList[ i - 2 ] = t;
			t = m_ResidueList[ i + 1 ];
			m_ResidueList[ i + 1 ] = m_ResidueList[ i - 1 ];
			m_ResidueList[ i - 1 ] = t;
			i -= 2;
		}
		printf("%d %d\n", molecule->m_ResSeq, chainId);
		return;
	}
	printf("SubGroups Size: %lu\n", molecule->m_SubGroups.size());
	for (int i=0; i<molecule->m_SubGroups.size(); i++)
	{
		computeResidueList(molecule->m_SubGroups[i], chainId, i+1);
	}
}

bool DomainComplex::printFCC(const char* fccFileName)
{
	FILE* fp = fopen(fccFileName, "w");
	if (!fp)
	{
		fp = stdout;
	}
	// print domains recursively
	{
		fprintf(fp, "Domains %lu\n", m_Domains.size());
		for (int i=0; i<m_Domains.size(); i++)
		{
			m_Domains[i]->print(fp, m_ResidueList, 0);
		}
	}
	// next print connectors in each level
	{
		fprintf(fp, "Connectors %lu\n", m_Connectors.size());
		for (int i=0; i<m_Connectors.size(); i++)
		{
			m_Connectors[i]->print(fp, m_ResidueList);
		}
	}
	// next print flexibilities in each level
	{
		fprintf(fp, "Flexibilities %lu\n", m_Flexibility.size());
		for (int i=0; i<m_Flexibility.size(); i++)
		{
			m_Flexibility[i]->print(fp);
		}
	}
	if (fp != stdout)
	{
		fclose(fp);
	}
	return true;
}

Domain* DomainComplex::getDomain(int id, int level)
{
	for (int i=0; i<m_Domains.size(); i++)
		if (m_Domains[i]->m_Id == id)
		{
			return m_Domains[i];
		}
	return 0;
}

bool DomainComplex::readFCC(const char* filename)
{
	clearData();
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		return false;
	}
	int numDomains;
	fscanf(fp, "Domains %d\n", &numDomains);
	for (int i=0; i<numDomains; i++)
	{
		Domain* domain = new Domain(this);
		domain->read(fp, m_ResidueList, 0);
		m_Domains.push_back(domain);
	}
	int numConnectors;
	fscanf(fp, "Connectors %d\n", &numConnectors);
	for (int i=0; i<numConnectors; i++)
	{
		Connector* connector = new Connector(-1,-1,0,0,0, m_ResidueList, this);
		connector->read(fp, m_ResidueList);
		m_Connectors.push_back(connector);
	}
	int numFlexibilities;
	fscanf(fp, "Flexibilities %d\n", &numFlexibilities);
	for (int i=0; i<numFlexibilities; i++)
	{
		int id1, id2;
		if (fscanf(fp, "Domains %d %d\n", &id1, &id2) != 2)
		{
			return false;
		}
		Domain* d1 = getDomain(id1, 0);
		Domain* d2 = getDomain(id2, 0);
		Flexibility* f = new Flexibility(this, d1, d2);
		f->read(fp);
		m_Flexibility.push_back(f);
	}
	fclose(fp);
	return true;
}

// this needs to be a recursive call so all combintations are performed.
bool DomainComplex::getConformations(int level, const char* newLevelConformationBaseFileName, int curFlexibility, GroupOfAtoms* molecule, int* counter, int recommendedNumber)
{
	if (!newLevelConformationBaseFileName || !molecule || !counter)
	{
		return false;
	}
	if (*counter > recommendedNumber)
	{
		return true;    // sanity check in recusion
	}
	if (curFlexibility >= m_Flexibility.size())
	{
		return true;
	}
	// apply for each discrete position for current flexibility and call recursively
	int t= m_Flexibility[curFlexibility]->m_TotalDiscreteConformations;
	for (int i=0; i<t; i++)
	{
		m_Flexibility[curFlexibility]->initNewGOA();
		// update GOA with current flexibility's current conformation
		// this will change the main m_Molecule parameter!!!! keep copy and reset it.
		// we should not have any pointers to member of it someplace :( or we are dead.
		GroupOfAtoms* temp = new GroupOfAtoms(*m_Molecule);
		delete m_Molecule;
		m_Molecule = new GroupOfAtoms(*molecule);
		if (!m_Flexibility[curFlexibility]->getUpdatedGOA(i))
		{
			break;    // change copyGOA
		}
		GroupOfAtoms* copyGOA = new GroupOfAtoms(*m_Molecule);
		delete m_Molecule;
		m_Molecule = new GroupOfAtoms(*temp);
		delete temp;
		temp = 0;
		// call with next flexibility
		if (curFlexibility < m_Flexibility.size()-1)
		{
			getConformations(level, newLevelConformationBaseFileName, curFlexibility+1, copyGOA, counter, recommendedNumber);
		}
		if (curFlexibility == m_Flexibility.size() -1)
		{
			// we have applied one chain of permutations, save it.
			if (GroupOfAtoms::isValidGOA(copyGOA, 10.0))
			{
				char newFileName[1024];
				sprintf(newFileName, "%s_%d.pdb", newLevelConformationBaseFileName, *counter);
				{
					GOALoader* goaLoader = new GOALoader();
					goaLoader->saveFile(newFileName, copyGOA, 0, 0);
					delete goaLoader;
				}
				(*counter)++;
			}
		}
		delete copyGOA;
		copyGOA = 0;
	}
	return true;
}

bool DomainComplex::allocateNumberOfDiscreteFlexibilitiesPerInterface(int recommendedNumber)
{
	if (m_Flexibility.size() < 1)
	{
		return false;
	}
	// do this based on relative sizes of domains
	int totalSize = 0; // this is not number of residues in molecule, can be larger due to interconnections
	int maxSize = m_Flexibility[0]->getDomainSize();
	totalSize += maxSize;
	for (int i=1; i<m_Flexibility.size(); i++)
	{
		int size = m_Flexibility[i]->getDomainSize();
		if (maxSize < size)
		{
			maxSize = size;
		}
		totalSize += size;
	}
	// distribute the recommendedNumber based on relative size
	for (int i=0; i<m_Flexibility.size(); i++)
	{
		int size = m_Flexibility[i]->getDomainSize();
		//int n = pow(recommendedNumber, (double) size / (double) maxSize);
		int n = pow(recommendedNumber, (double) size / (double) totalSize);
		if (n < 0)
		{
			n = 1;    // need atleast one!
		}
		if (!m_Flexibility[i]->allocateNumberOfDiscreteFlexibilities(n))
		{
			return false;
		}
	}
	return true;
}

// before calling, allocate the number for each flexibility
bool DomainComplex::getConformations(const char* FCCFileName, int level, const char* newLevelConformationBaseFileName, int recommendedNumber)
{
	if (!readFCC(FCCFileName))
	{
		return false;
	}
	if (!allocateNumberOfDiscreteFlexibilitiesPerInterface(recommendedNumber))
	{
		return false;
	}
	// flexibilities are computed. Now we need to provide hierarchical sampling to it
	// just do level 1 for now.
	// Then given a transformation, find flex loops and give new conformations to them
	// then improve using residue fit
	// factorial number of conformations, so sample main one more than others?
	// use 20 deg sampling file  1800 of them, make sure no steric clash
	// divide recommendedNumber among flexibility objects
	// product of all used  = recommendedNumber
	int counter = 0;
	GroupOfAtoms* tempMol = new GroupOfAtoms(*m_Molecule);
	bool ret = getConformations(0, newLevelConformationBaseFileName, 0, tempMol, &counter, recommendedNumber);
	delete tempMol;
	tempMol = 0;
	return ret;
}

void DomainComplex::addNewDomain(Domain* domain, int reqLevel, int curLevel)
{
	if (reqLevel == 0)
	{
		m_Domains.push_back(domain);
		return;
	}
	bool added = false;
	for (int i=0; i<m_Domains.size(); i++)
	{
		if (m_Domains[i]->addNewDomain(domain, reqLevel, curLevel+1))
		{
			added = true;
			break;
		}
	}
	// just add to first, right way is to split and try to add!
	if (!added)
	{
		m_Domains[0]->m_Domains.push_back(domain);
	}
}

// build the FCC complex
bool DomainComplex::readDomainFinderOpt(const char* domainListFileName, int level)
{
	// if u find domain, create new domain, and keep adding for every line beginning with chain
	// should we split domains between chains or not ?
	if (level != m_MaxLevelId+1)   // something wierd, we are not reading children?
	{
		return false;
	}
	m_MaxLevelId++;
	FILE* fp = fopen(domainListFileName, "r");
	if (!fp)
	{
		return false;
	}
	char line[1001];
	memset(line, '\0', sizeof(line));
	Domain* curDomain = 0;
	while (fgets(line, 1000, fp) != 0)
	{
		if (strstr(line, "Domain") != 0)
		{
			if (curDomain)
			{
				if (curDomain->getNumSegments() > 1)
				{
					// add to right level under appropriate domain
					addNewDomain(curDomain, level, 0);
				}
				else
				{
					delete curDomain;
				}
			}
			curDomain = new Domain(this);
			curDomain->parseProperties(line);
		}
		else if (strstr(line, "Chain") != 0)
		{
			if (!curDomain)
			{
				continue;    // wierd, domain info with no domain!
			}
			curDomain->addNewSegment(line, m_ResidueList);
			printf("num segments = %d\n", curDomain->getNumSegments());
		}
		memset(line, '\0', sizeof(line));
	}
	if (curDomain)
	{
		if (curDomain->getNumSegments() > 1)
		{
			addNewDomain(curDomain, level, 0);
		}
		else
		{
			delete curDomain;
		}
	}
	fclose(fp);
	if (m_Domains.size() < 1)
	{
		return false;
	}
	return true;
}

void DomainComplex::addDomainIds(int* domainId, int curLevel, int reqLevel, Domain* domain)
{
	if (curLevel != reqLevel)
	{
		for (int i=0; i<domain->m_Domains.size(); i++)
		{
			addDomainIds(domainId, curLevel+1, reqLevel, domain->m_Domains[i]);
		}
	}
	else
	{
		for (int s=0; s<domain->m_Segments.size(); s++)
		{
			// go from start to end residue and mark them all with current domain ID.
			Segment* segment = domain->m_Segments[s];
			bool startFound = false;
			for (int i=segment->m_StartResidueIndex; i<=segment->m_EndResidueIndex; i++)
			{
				domainId[i] = domain->m_Id;
			}
		}
	}
}

void DomainComplex::getDomainIds(int* domainId, int level)
{
	int curLevel = 0;
	for (int i=0; i<m_ResidueList.size()/2; i++)
	{
		domainId[i] = -1;
	}
	for (int i=0; i<m_Domains.size(); i++)
	{
		addDomainIds(domainId, curLevel, level, m_Domains[i]);
	}
}

bool DomainComplex::getNextUnclassifiedResidue(int* curResidueIndex, int* domainId, int* begDomainId)
{
	for (int i=(*curResidueIndex); i<m_ResidueList.size()/2; i++)
	{
		if (domainId[i] == -1)
		{
			*curResidueIndex = i;
			/// 2nd part added: residues belong to different chains
			if ((i==0) || (m_ResidueList[((i - 1) << 1) + 1 ] != m_ResidueList[(i << 1) + 1 ]))
			{
				*begDomainId = -1;
			}
			else
			{
				*begDomainId = domainId[i-1];
			}
			return true;
		}
	}
	return false;
}

// assume current index is pointing to unclassified residue!
bool DomainComplex::getLastUnclassifiedResidue(int* lastResidueIndex, int* domainId, int* endDomainId)
{
	int chainId = m_ResidueList[(*lastResidueIndex << 1) + 1 ];
	int i;
	for (i=(*lastResidueIndex); i<m_ResidueList.size()/2; i++)
	{
		if (m_ResidueList[(i << 1) + 1 ] != chainId)
		{
			break;    /// new chain starts --- Rezaul
		}
		if (domainId[i] != -1)
		{
			*lastResidueIndex = i-1;
			*endDomainId = domainId[i];
			return true;
		}
	}
	*lastResidueIndex = i - 1;
	*endDomainId = -1;
	return true;
}

bool DomainComplex::addLoopToDomain(FlexibleLoop* flexibleLoop, int domainId, int reqLevel)
{
	if (!flexibleLoop)
	{
		return false;
	}
	for (int i=0; i<m_Domains.size(); i++)
	{
		if (m_Domains[i]->addLoopToDomain(flexibleLoop, domainId, reqLevel, 0))
		{
			return true;
		}
	}
	return false;
}

bool DomainComplex::classifyFlexibleSegments()
{
	if (m_ResidueList.size()/2 < 1)
	{
		return false;
	}
	for (int l=0; l<=m_MaxLevelId; l++)
	{
		int* domainId = new int[m_ResidueList.size()/2];
		getDomainIds(domainId, l);   // for given level
		// take each residue, or list of consecutive residues, if not part of domain,
		// add to loops and connectors at each level
		{
			// if each end of segment connects to same domain, make it a loop, else a connector.
			// find first unclassified residue, keep going till end of chain or domain is found. Classify it.
			int curResidueIndex = 0;
			int curDomainId = -1;
			while (true)
			{
				int begDomainId, endDomainId;
				if (!getNextUnclassifiedResidue(&curResidueIndex, domainId, &begDomainId))
				{
					break;
				}
				int lastResidueIndex = curResidueIndex;
				getLastUnclassifiedResidue(&lastResidueIndex, domainId, &endDomainId);
				// hope that entire chain has atleast one domain in it.
				if (begDomainId == endDomainId)
				{
					// create loop, put into appropriate domain
					FlexibleLoop* flexibleLoop = new FlexibleLoop(curResidueIndex, lastResidueIndex, begDomainId, endDomainId, m_ResidueList, this);
					if (!addLoopToDomain(flexibleLoop, begDomainId, l))
					{
						delete flexibleLoop;
					}
				}
				else if (begDomainId == -1 && endDomainId != -1)
				{
					// create loop, put into appropriate domain
					FlexibleLoop* flexibleLoop = new FlexibleLoop(curResidueIndex, lastResidueIndex, begDomainId, endDomainId, m_ResidueList, this);
					if (!addLoopToDomain(flexibleLoop, endDomainId, l))
					{
						delete flexibleLoop;
					}
				}
				else if (begDomainId != -1 && endDomainId == -1)
				{
					// create loop, put into appropriate domain
					FlexibleLoop* flexibleLoop = new FlexibleLoop(curResidueIndex, lastResidueIndex, begDomainId, endDomainId, m_ResidueList, this);
					if (!addLoopToDomain(flexibleLoop, begDomainId, l))
					{
						delete flexibleLoop;
					}
				}
				else
				{
					// add to connectors list with appropriate level info
					Connector* connector = new Connector(curResidueIndex, lastResidueIndex, begDomainId, endDomainId, l, m_ResidueList, this);
					m_Connectors.push_back(connector);
				}
				curResidueIndex = lastResidueIndex+1;
			}
		}
		delete []domainId;
		domainId = 0;
	}
	return true;
}

bool DomainComplex::printColorMap(const char* fname, int level)
{
	if (!fname)
	{
		return false;
	}
	if (m_ResidueList.size() < 1 || m_Domains.size() < 1)
	{
		return false;
	}
	FILE* fp = fopen(fname, "w");
	if (!fp)
	{
		return false;
	}
	//# Make the entire molecule Red
	//"0" "" "" "" "" "" "" "" "1.0" "1.0" "1.0" "1.0"
	fprintf(fp, "# TexMol colormap file\n");
	fprintf(fp, "#\n");
	fprintf(fp, "# This file was automatically created to represent domains\n");
	fprintf(fp, "#\n");
	fprintf(fp, "# The molecule is colored gray and subsequently modified\n");
	fprintf(fp, "\"0\" \"\" \"\" \"\" \"\" \"\" \"\" \"\" \"1.0\" \"1.0\" \"1.0\" \"1.0\"\n");
	// go through domains at appropriate level, print out residue color info.
	// to get res info, first get chain id, res name.
	// use some rand func to get consistant colors always
	fprintf(fp, "#\n");
	fprintf(fp, "# Domains\n");
	fprintf(fp, "#\n");
	for (int i=0; i<m_Domains.size(); i++)
	{
		fprintf(fp, "#\n");
		fprintf(fp, "#   Domain %d\n", i);
		m_Domains[i]->printColorMap(m_Molecule, m_ResidueList, fp, level, 0);
	}
	fprintf(fp, "#\n");
	fprintf(fp, "# Connectors\n");
	fprintf(fp, "#\n");
	for (int i=0; i<m_Connectors.size(); i++)
	{
		m_Connectors[i]->printColorMap(m_Molecule, m_ResidueList, fp, level);
	}
	fclose(fp);
	fprintf(fp, "#\n");
	fprintf(fp, "# End of color map file\n");
	return true;
}

// chainId is a 1 based index. cur chain is started with 1 and incremented with every chain
bool DomainComplex::getResidueData(char* resName, char* actualChainId, int* chainNum, int chainId, int resNum, GroupOfAtoms* molecule, int curChain)
{
	if (molecule->type == PDBParser::PROTEIN_TYPE)
	{
		curChain = 0;
	}
	if (molecule->type == PDBParser::CHAIN_TYPE)
	{
		if (curChain != chainId)
		{
			return false;    // not proper chain, so return
		}
	}
	if (molecule->type == PDBParser::RESIDUE_TYPE)
	{
		if (molecule->m_ResSeq != resNum)
		{
			return false;
		}
		PDBParser::Atom* atom = molecule->m_Atoms[0]; // hope there is atleast one atom!!!
		strcpy(resName, atom->getResidueName());
		*actualChainId = atom->getChain();
		*chainNum = curChain-1;// since domain info was 1 indexed and color map is 0 indexed
		return true;
	}
	for (int i=0; i<molecule->m_SubGroups.size(); i++)
	{
		if (molecule->type == PDBParser::PROTEIN_TYPE)
		{
			if (getResidueData(resName, actualChainId, chainNum, chainId, resNum, molecule->m_SubGroups[i], i+1))
			{
				return true;
			}
		}
		else
		{
			if (getResidueData(resName, actualChainId, chainNum, chainId, resNum, molecule->m_SubGroups[i], curChain))
			{
				return true;
			}
		}
	}
	return false;
}

void DomainComplex::getNearbyColor(double baseSegmentRed, double baseSegmentGreen, double baseSegmentBlue, int randVal, double* red, double* green, double* blue)
{
	srand(randVal);
	double r = (double)rand()/RAND_MAX * COLOR_EPS;
	*red = baseSegmentRed + r;
	if (*red > 1)
	{
		*red = 1;
	}
	if (*red < 0)
	{
		*red = 0;
	}
	double g = (double)rand()/RAND_MAX * COLOR_EPS;
	*green = baseSegmentGreen + g;
	if (*green > 1)
	{
		*green = 1;
	}
	if (*green < 0)
	{
		*green = 0;
	}
	double b = (double)rand()/RAND_MAX * COLOR_EPS;
	*blue = baseSegmentBlue + b;
	if (*blue > 1)
	{
		*blue = 1;
	}
	if (*blue < 0)
	{
		*blue = 0;
	}
}

Flexibility* DomainComplex::computeDomainInteraction(Domain* d1, Domain* d2)
{
	if (!d1 || !d2)
	{
		return false;
	}
	Flexibility* f = new Flexibility(this, d1, d2);
	if (!f->computeInterfaceArea())
	{
		delete f;
		return 0;
	}
	if (!f->computeFlexibility())
	{
		delete f;
		return 0;
	}
	return f;
}

bool DomainComplex::computeFlexibility()
{
	if (m_ResidueList.size() < 1 || m_Domains.size() < 1)
	{
		return false;
	}
	// assume all domains are connected
	// keep largest one constant
	// connector may be missing, especially if more than one chain. Then do shear.
	// maybe only one domain, then do only flexible loops
	// keep constant # conformations. Weight all connections and flexible loops and sample accordingly
	// find next largest which is connected to current system. See if it forms a cut. If so, use
	// more flex, else less,
	// compute
	// domains should already be build
	//    Mark one as main one, add others incrementally
	// compute for each domain, a range of motion, classified into shear or (2 closure + 1 twist rotations ) with ranges
	// this should be printed out and read in FCC IO.
	// For each domain pair do:
	//    Compute the connector lengths, area, determine shear or rotation
	//    Using all connectors for given domain, find out if this is a cut or not.
	//    If cut, use only d1 connectors, otherwise, restrict motion
	for (int i=0; i<m_Domains.size()-1; i++)
	{
		for (int j=i+1; j<m_Domains.size(); j++)
		{
			// for each pair, determine flexibility type and ranges
			Flexibility* f = computeDomainInteraction(m_Domains[i], m_Domains[j]);
			if (f)
			{
				m_Flexibility.push_back(f);
			}
		}
	}
	for (int i=0; i<m_Flexibility.size(); i++)
	{
		m_Flexibility[i]->computeCutProperty();
	}
	return true;
}

GroupOfAtoms* DomainComplex::getResidue(int chainId, int resNum, GroupOfAtoms* molecule, int curChain)
{
	if (molecule->type == PDBParser::PROTEIN_TYPE)
	{
		curChain = 0;
	}
	if (molecule->type == PDBParser::CHAIN_TYPE)
	{
		if (curChain != chainId)
		{
			return 0;    // not proper chain, so return
		}
	}
	if (molecule->type == PDBParser::RESIDUE_TYPE)
	{
		if (molecule->m_ResSeq != resNum)
		{
			return 0;
		}
		return molecule;
	}
	for (int i=0; i<molecule->m_SubGroups.size(); i++)
	{
		if (molecule->type == PDBParser::PROTEIN_TYPE)
		{
			GroupOfAtoms* g = getResidue(chainId, resNum, molecule->m_SubGroups[i], i+1);
			if (g)
			{
				return g;
			}
		}
		else
		{
			GroupOfAtoms* g = getResidue(chainId, resNum, molecule->m_SubGroups[i], curChain);
			if (g)
			{
				return g;
			}
		}
	}
	return 0;
}

GroupOfAtoms* DomainComplex::getResidue(int residueListIndex)
{
	return getResidue(m_ResidueList[residueListIndex*2+1], m_ResidueList[residueListIndex*2+0], m_Molecule, 0);
}
