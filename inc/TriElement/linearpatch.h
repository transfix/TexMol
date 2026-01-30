#ifndef LINEARPATCH_H
#define LINEARPATCH_H

#include <vector>
#include "quadrature.h"
#include "trielement.h"

namespace TriElementNS
{
        class Vertex;
        class Face;
        class LinearPatch : public TriElement
        {
        public:
                LinearPatch();
		void Triangle_Patch_Nodes(char *type, int numOfPts);

        private:
		void Triangle_Patch_Nodes
		(
		        int i,
		        double *p1, double *p2, double *p3,
		        double *n1, double *n2, double *n3
		);
        };
}

#endif //SURFACE_H

