#ifndef TRIELEMENT_H
#define TRIELEMENT_H

#include <vector>
#include <set>

#include "quadrature.h"

namespace TriElementNS
{
  
  typedef enum{
    TRI_LINEAR 	= 0,
    TRI_ASPLINE 	= 1,
    TRI_QUAD	= 2,
  } TriElementType;
  
  class Vertex;
  class Face;
  class TriElement
  {
  public:
    TriElement();
    void addPoint(Vertex* point);
    void addFacet(Face* facet);
    virtual void Triangle_Patch_Nodes(char *type, int numOfPts) = 0;
    float get_aspect_ratio(Face* f);
    void calculateAspectRatio();
    
    TriElementNS::Quadrature *my_quadr;
    std::vector<Vertex*> m_Vertices;
    std::vector<Face*> m_Faces;                
    int numbpts, numbtris; 
    double min[3], max[3];
    bool normalFlag, colorFlag;
    void BoundingBox();
    void NormalizeNormal();
    
    void SetUpNearField(int depth);
    void SetUpNearField2(int depth);


    // arand: added these to give easy access to mesh connectivity
    std::vector< std::vector<int> > vertexNeighbors;
    std::vector< std::vector<int> > vertexStar;
    std::vector< std::set<int> > vertexNearField;

    static double DotProduct(double *v1, double *v2);
    static void CrossProduct(double *v1, double *v2, double *v);
    static double Determinant(double *v1, double *v2, double *v3);
    static double Triangle_area(double *v1, double *v2, double *v3);
    static void PolyDerivative(double *p, int n, double *dp);
    static void PolyProduct(double *p1, int m, double *p2, int n, double *p3);
    static double HornerEvaluate(double *p, int n, double t);
    static int computeTriIndex(int i, int j, int resolution);
  };
}

#endif //SURFACE_H

