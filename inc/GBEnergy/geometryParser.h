#ifndef GBGEOMETRYPARSER_H
#define GBGEOMETRYPARSER_H

#include <stdio.h>

namespace GBGEOMETRY 
{
        class Vertex;
        class Face;
        class Suface;
        class GeometryParser
        {
        public:
                GeometryParser();
		Surface* SurfaceTriangulation(FILE* fp);
                int ReadHowmanyComponents(FILE* fp);
        private:
                int howmany, normalFlag;
        };
}

#endif //GEOMETRYPARSER_H

