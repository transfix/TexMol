#include <vector>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "geometry.h"
#include "linearpatch.h"
#include "quadrature.h"

using std::vector;

using namespace TriElementNS;


LinearPatch::LinearPatch()
{
}

void LinearPatch::Triangle_Patch_Nodes(char *type, int numOfPts)
{
    int i;
    printf("linear patch\n");

    my_quadr = new Quadrature(type, numOfPts);

    for (i = 0; i < numbtris; i++)
    {

        double p1[3], p2[3], p3[3], n1[3], n2[3], n3[3];
        int j;
        for (j = 0; j<3; j++)
        {
            p1[j] = m_Vertices[m_Faces[i]->IndexInRAW[0]]->Point[j];
            p2[j] = m_Vertices[m_Faces[i]->IndexInRAW[1]]->Point[j];
            p3[j] = m_Vertices[m_Faces[i]->IndexInRAW[2]]->Point[j];

            n1[j] = m_Vertices[m_Faces[i]->IndexInRAW[0]]->Normal[j];
            n2[j] = m_Vertices[m_Faces[i]->IndexInRAW[1]]->Normal[j];
            n3[j] = m_Vertices[m_Faces[i]->IndexInRAW[2]]->Normal[j];

        }

	Triangle_Patch_Nodes(i,p1,p2,p3,n1,n2,n3);
    }
}



void LinearPatch::Triangle_Patch_Nodes
(
	int i, 
	double *p1, double *p2, double *p3,
	double *n1, double *n2, double *n3
)
{
    int j;

    int k;
    double b1,b2,b3;

    double area = Triangle_area(p1,p2,p3);
    //printf("area:%f\n",area);
    for (k = 0; k < my_quadr->numOfPts_perTri; k++)
    {
	b1 = my_quadr->xi1[k];
	b2 = my_quadr->xi2[k];
	b3 = my_quadr->xi3[k];

	Vertex *node = new Vertex();
	for (j = 0; j < 3; j++)
        {
                node->Point[j] = b1*p1[j] + b2*p2[j] + b3*p3[j];
                node->Normal[j] = b1*n1[j] + b2*n2[j] + b3*n3[j];
        }
        node->weight = my_quadr->w[k] * area;

	m_Faces[i]->Node.push_back(node);
    }
}

