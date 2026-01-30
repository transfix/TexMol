#include <Docking/DomainComplex.h>
#include <Docking/FlexibleLoop.h>
#include <math.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using PDBParser::GroupOfAtoms;
using PDBParser::Atom;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;
using Docking::FlexibleLoop;
using Docking::DomainComplex;

FlexibleLoop::FlexibleLoop(int startResidueIndex, int endResidueIndex, int domain1, int domain2, std::vector<int> residueList, DomainComplex* domainComplex)
	: Segment(startResidueIndex, endResidueIndex, domain1, domain2, residueList, domainComplex)
{
}

FlexibleLoop::~FlexibleLoop()
{
}

bool FlexibleLoop::printColorMap(PDBParser::GroupOfAtoms* molecule, std::vector<int> residueList, FILE* fp, double red, double green, double blue)
{
	red *= 2;
	if (red > 1)
	{
		red = 1;
	}
	green *= 2;
	if (green > 1)
	{
		green = 1;
	}
	blue *= 2;
	if (blue > 1)
	{
		blue = 1;
	}
	return Segment::printColorMap(molecule, residueList, fp, red, green, blue, Docking::FLEXIBLE_LOOP_ALPHA);
}
