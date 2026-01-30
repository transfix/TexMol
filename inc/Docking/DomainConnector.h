#ifndef CCV_TEXMOL_DOCKING_DOMAIN_CONNECTOR_H
#define CCV_TEXMOL_DOCKING_DOMAIN_CONNECTOR_H

#include <vector>
#include <UsefulMath/Matrix.h>
#include <Docking/Complex.h>

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

class SimpleVolumeData;

namespace Docking
{
	class DomainConnector
	{
		public:
			DomainConnector();
			virtual ~DomainConnector();
	};
};

#endif
