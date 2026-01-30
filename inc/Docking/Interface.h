#ifndef CCV_TEXMOL_DOCKING_INTERFACE_H
#define CCV_TEXMOL_DOCKING_INTERFACE_H

#include <vector>
class Geometry;
class SimpleVolumeData;

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

namespace Docking
{
	class DockComplex;

	class Interface
	{
		public:
			Interface();
			virtual ~Interface();
			double getArea(DockComplex* complex, double distanceCutOff = 2);
			double getArea(std::vector<PDBParser::Atom*> molecule1, std::vector<PDBParser::Atom*> molecule2, double distanceCutOff = 2);
			bool getInterfaceMarkedGeometry(Geometry** surface1, Geometry** surface2,
											Geometry** surface1blue, Geometry** surface2blue,
											Geometry** surface1NoInterface, Geometry** surface2NoInterface,
											Geometry** interface1, Geometry** interface2,
											DockComplex* complex, double distanceCutOff);
		protected:
			bool markRelevantTriangles(Geometry* surface, std::vector<int> *relevantTriangles,
									   SimpleVolumeData* sData, double minFuncVal);
			double triArea(Geometry* surface, int idx1, int idx2, int idx3);
			double determinant(float a1, float b1,    // 1
							   float a2, float b2,   // 1
							   float a3, float b3);  // 1
	};

};

#endif
