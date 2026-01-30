#ifndef TRIELEMENT_GEOMETRYPARSER_H
#define TRIELEMENT_GEOMETRYPARSER_H

#include <stdio.h>
#include "trielement.h"

namespace TriElementNS
{
        class Vertex;
        class Face;
        class ASpline;
        class GeometryParser
        {
        public:
                GeometryParser();
		TriElement* getASpline(const char* fname);
		TriElement* getASpline(const char* fname, int permu);

		TriElement* getLinearPatch(const char* fname);
		TriElement* getLinearPatch(const char* fname, int permu);

                int ReadHowmanyComponents(FILE* fp);
        private:
		TriElement* SurfaceTriangulation(TriElement* surface,FILE* fp,int permu);
	protected:
                int howmany, normalFlag, colorFlag;
        };
}

#endif //GEOMETRYPARSER_H

