#ifndef POTGEOMETRYPARSER_H
#define POTGEOMETRYPARSER_H

#include <stdio.h>
#include "geometryParser.h"
#include "trielement.h"

namespace TriElementNS
{
        class Vertex;
        class Face;
        class ASpline;
        class PotGeometryParser : public GeometryParser
        {
        public:
                PotGeometryParser();
		TriElement* getASpline(const char* fname);

		TriElement* getLinearPatch(const char* fname);

		void getPotential(TriElement* surface, double* phi);

		bool isVtxPot() { return vflag; }
	private:
		bool vflag;
		double* phi;
		TriElement* SurfaceTriangulation(TriElement* surface,FILE* fp);
        };
}

#endif //POTGEOMETRYPARSER_H

