#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

namespace TriElementNS{
    class Vertex
    {
    public:
        Vertex()
        {
		weight = 0.0; lambda = 0.0;
        }
        double   Point[3];
        double   Normal[3];
        int	 NumFaces;
	double	 Color[3];
	double   weight;
	double   lambda;

//      double   *Hessian;    /* the hansian at the vertices  */
//      double   *SharpNormal;   /* the normal for sharp vertex */

    };

    class Face
    {
    public:
        Face()
        {
        }
        int Index[3];      		/* the indexs of a triangle-increase order*/
	int IndexInRAW[3]; 		/* the indices of a triangle in the origianl raw file */
        std::vector<Vertex *> Node;	/* Gaussian integration nodes             */ 
        int Orien;        		/* 1--orientaioned, othwise no            */
        int AdjTri[3];    		/* neighbor triangles index               */
        double Center[3];		/* the center of the triangle             */
        double Normal[3];		/* face normal                            */
        int whichnml[3];		/* Index[0] normal index                  */
        int Tou;			/* the begin index of the insided points  */
        int Wei;			/* the end index+1 of the insided points  */
        int State;			/* 1--removed; 2-- new, 0--old;           */
	//float Area;			/* area */ // Albert
	std::vector<Face *> subFacets;
    };
}
#endif //GEOMETRY_H

