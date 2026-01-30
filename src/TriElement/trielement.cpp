#include <vector>
#include <set>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "geometry.h"
#include "trielement.h"
#include "quadrature.h"

using namespace TriElementNS;
using namespace std;




TriElement::TriElement()
{
}


void TriElement::addPoint(Vertex* point)
{
    Vertex* p = new Vertex();

    p->Point[0] = point->Point[0];
    p->Point[1] = point->Point[1];
    p->Point[2] = point->Point[2];

    if (normalFlag == true)
    {
        p->Normal[0] = point->Normal[0];
        p->Normal[1] = point->Normal[1];
        p->Normal[2] = point->Normal[2];
    }

    if (colorFlag == true)
    {
        p->Color[0] = point->Color[0];
        p->Color[1] = point->Color[1];
        p->Color[2] = point->Color[2];
    }
    p->weight = point->weight;
    p->lambda = point->lambda;

    m_Vertices.push_back(p);
}

void TriElement::addFacet(Face* facet)
{
    Face* f = new Face();

    f->Index[0] = facet->Index[0];
    f->Index[1] = facet->Index[1];
    f->Index[2] = facet->Index[2];

    f->IndexInRAW[0] = facet->IndexInRAW[0];
    f->IndexInRAW[1] = facet->IndexInRAW[1];
    f->IndexInRAW[2] = facet->IndexInRAW[2];

    f->AdjTri[0] = facet->AdjTri[0];
    f->AdjTri[1] = facet->AdjTri[1];
    f->AdjTri[2] = facet->AdjTri[2];

    f->whichnml[0] = facet->whichnml[0];
    f->whichnml[1] = facet->whichnml[1];
    f->whichnml[2] = facet->whichnml[2];

    //f->Area = facet->Area;

    m_Faces.push_back(f);

    int id = m_Faces.size() -1;

    vertexStar[facet->Index[0]].push_back(id);
    vertexStar[facet->Index[1]].push_back(id);
    vertexStar[facet->Index[2]].push_back(id);
    
    int found1=0;
    int found2=0;
    for (int i=0; i<vertexNeighbors[facet->Index[0]].size(); i++) {
      if (vertexNeighbors[facet->Index[0]][i] == facet->Index[1]) {
	found1 = 1;
      }
      if (vertexNeighbors[facet->Index[0]][i] == facet->Index[2]) {
	found2 = 1;
      }
    }
    if (found1 == 0) {
      vertexNeighbors[facet->Index[0]].push_back(facet->Index[1]);
    }
    if (found2 == 0) {
      vertexNeighbors[facet->Index[0]].push_back(facet->Index[2]);
    }


    int found0=0;
    found2=0;
    for (int i=0; i<vertexNeighbors[facet->Index[1]].size(); i++) {
      if (vertexNeighbors[facet->Index[1]][i] == facet->Index[0]) {
	found0 = 1;
      }
      if (vertexNeighbors[facet->Index[1]][i] == facet->Index[2]) {
	found2 = 1;
      }
    }
    if (found0 == 0) {
      vertexNeighbors[facet->Index[1]].push_back(facet->Index[0]);
    }
    if (found2 == 0) {
      vertexNeighbors[facet->Index[1]].push_back(facet->Index[2]);
    }

    found0=0;
    found1=0;
    for (int i=0; i<vertexNeighbors[facet->Index[2]].size(); i++) {
      if (vertexNeighbors[facet->Index[2]][i] == facet->Index[0]) {
	found0 = 1;
      }
      if (vertexNeighbors[facet->Index[2]][i] == facet->Index[1]) {
	found1 = 1;
      }
    }
    if (found0 == 0) {
      vertexNeighbors[facet->Index[2]].push_back(facet->Index[0]);
    }
    if (found1 == 0) {
      vertexNeighbors[facet->Index[2]].push_back(facet->Index[1]);
    }



}

void TriElement::NormalizeNormal()
{
    
    for(int i=0;i<m_Vertices.size();i++)
    {
	Vertex* v = m_Vertices[i];
	float dis = 0.0;
	for(int j=0;j<3;j++)
		dis += v->Normal[j]*v->Normal[j];
	for(int j=0;j<3;j++)
		v->Normal[j] /= sqrt(dis);
    }
    for(int i=0;i<m_Faces.size();i++)
    {
	Face* f = m_Faces[i];
	float dis = 0.0;
	for(int j=0;j<3;j++)
		dis += f->Normal[j]*f->Normal[j];
	for(int j=0;j<3;j++)
		f->Normal[j] /= sqrt(dis);
	for(int j=0;j<f->Node.size();j++)
	{
		Vertex* v = f->Node[j];
	        float dis = 0.0;
        	for(int j=0;j<3;j++)
                	dis += v->Normal[j]*v->Normal[j];
	        for(int j=0;j<3;j++)
        	        v->Normal[j] /= sqrt(dis);
	}
    }
}

void TriElement::BoundingBox()
{
        int i, j;
        min[0] = 100000.0;      min[1] = 100000.0;      min[2] = 100000.0;
        max[0] = -100000.0;     max[1] = -100000.0;     max[2] = -100000.0;

        for ( i = 0; i < numbtris; i++)
        {
                double p[3];
                for (j = 0; j < 3; j++)
                {
                        p[j] = m_Vertices[m_Faces[i]->Index[0]]->Point[j];
                        if (p[j] < min[j]) min[j] = p[j];
                        if (p[j] > max[j]) max[j] = p[j];
                }
        }
}

double TriElement::DotProduct(double *v1, double *v2)
{
    return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}

void TriElement::CrossProduct(double *v1, double *v2, double *v)
{
   v[0] = v1[1]*v2[2] - v1[2]*v2[1];
   v[1] = v1[2]*v2[0] - v1[0]*v2[2];
   v[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
}

double TriElement::Determinant(double *v1, double *v2, double *v3)
{
    return v1[0]*v2[1]*v3[2] + v2[0]*v3[1]*v1[2] + v3[0]*v1[1]*v2[2] -
           v1[2]*v2[1]*v3[0] - v2[2]*v3[1]*v1[0] - v3[2]*v1[1]*v2[0];
}

double TriElement::Triangle_area(double *v1, double *v2, double *v3)
{
        /* Heron's formula  */
    double a,b,c,s;
    a = sqrt( (v1[0]-v2[0])*(v1[0]-v2[0]) + (v1[1]-v2[1])*(v1[1]-v2[1]) + (v1[2]-v2[2])*(v1[2]-v2[2]) );
    b = sqrt( (v2[0]-v3[0])*(v2[0]-v3[0]) + (v2[1]-v3[1])*(v2[1]-v3[1]) + (v2[2]-v3[2])*(v2[2]-v3[2]) );
    c = sqrt( (v1[0]-v3[0])*(v1[0]-v3[0]) + (v1[1]-v3[1])*(v1[1]-v3[1]) + (v1[2]-v3[2])*(v1[2]-v3[2]) );
    s = (a+b+c)/2.0;
    return sqrt( s*(s-a)*(s-b)*(s-c) );
}
    
void  TriElement::PolyDerivative(double *p, int n, double *dp)
{
    int   i;
    for (i = 1; i <= n; i++ ) 
    {
        dp[i-1] = i*p[i];
    }
}

void  TriElement::PolyProduct(double *p1, int m, double *p2, int n, double *p3)
{
    int   k, i;
    double sum;

    for (k = 0; k <= m+n; k++) 
    {
        sum = 0.0;
        for (i = 0; i <= k; i++) 
        {
            if (i <= m && k - i <= n) 
            {
                sum = sum + p1[i]*p2[k-i];
            }
        }
        p3[k] = sum;
    }
}

double TriElement::HornerEvaluate(double *p, int n, double t)
{
    double result;
    int i;

    result = p[n];
    for (i = 0; i < n; i++) 
    {
        result = result*t + p[n-i-1];
    }
    return result;
}

int TriElement::computeTriIndex(int i, int j, int resolution)
{
	int index = (2*resolution + 3 - j) * j / 2 + i;
	return index;
}

float TriElement::get_aspect_ratio(Face* f)
{
        float a, b, c, p, s, r_in, r_out;
        int i;

        a = 0.0;                b = 0.0;                c = 0.0;
	Vertex* v0 = m_Vertices[f->Index[0]]; 
	Vertex* v1 = m_Vertices[f->Index[1]]; 
	Vertex* v2 = m_Vertices[f->Index[2]]; 

	for(int i=0;i<3;i++)
	{
	  a += (v1->Point[i] - v0->Point[i])*(v1->Point[i] - v0->Point[i]);
	  b += (v2->Point[i] - v1->Point[i])*(v2->Point[i] - v1->Point[i]);
	  c += (v0->Point[i] - v2->Point[i])*(v0->Point[i] - v2->Point[i]);
	}
        a = (float)sqrt(a);             b = (float)sqrt(b);             c = (float)sqrt(c);
        p = (a + b + c) / 2.0f;
        s = (float)sqrt(p * (p - a) * (p - b) * (p - c));
        r_in = s / p;
        r_out = a * b * c / (4.0f * s);
        return (r_in / r_out);
}


void TriElement::calculateAspectRatio()
{
  float avg_aspect,max_aspect;
  avg_aspect = max_aspect = 0.0;
  float tmp_aspect = 0.0;
  int vert1,vert2,vert3;
  printf("calculate aspect ratio\n");
  for(int i=0;i<m_Faces.size();i++)
  {
    Face* f = m_Faces[i];
    tmp_aspect = get_aspect_ratio(f);
    if(tmp_aspect > max_aspect)
      max_aspect = tmp_aspect;
    avg_aspect += tmp_aspect;
  }
  avg_aspect /= (float)m_Faces.size();
  printf("max aspect ratio:%f avg aspect ratio:%f\n",max_aspect,avg_aspect);
}


//std::vector< std::set<int> > vertexNearField;

void TriElement::SetUpNearField(int depth) {

  vertexNearField.resize(vertexStar.size());

  
  for (int iV=0; iV<vertexStar.size(); iV++) {
  
    set<int> finishedTris;
    set<int> finishedVerts;

    set<int> nextVerts;

    vector<int> currentVerts;

    finishedVerts.insert(iV);

    finishedVerts.insert(iV);
    for (int iT=0; iT<vertexStar[iV].size(); iT++) {
      finishedTris.insert(vertexStar[iV][iT]);
    }

    for (int iN=0; iN<vertexNeighbors[iV].size(); iN++) {
      currentVerts.push_back(vertexNeighbors[iV][iN]);
      finishedVerts.insert(vertexNeighbors[iV][iN]);
    }
    

    for (int iD=0; iD<depth; iD++) {
      
      for (int iC=0; iC<currentVerts.size(); iC++) {
	for (int iT=0; iT<vertexStar[currentVerts[iC]].size(); iT++) {

	  int myT = vertexStar[currentVerts[iC]][iT];

	  if (finishedTris.find(myT)== finishedTris.end()) {
	    vertexNearField[iV].insert(myT);
	    finishedTris.insert(myT);
	  }
	}

	for (int iN=0; iN<vertexNeighbors[currentVerts[iC]].size(); iN++) {

	  int myN = vertexNeighbors[currentVerts[iC]][iN];	  

	  if (finishedVerts.find(myN) == finishedVerts.end()) {
	    nextVerts.insert(myN);
	    finishedVerts.insert(myN);
	  }
	} 
      }

      currentVerts.clear();
      for (set<int>::iterator sIt=nextVerts.begin(); sIt != nextVerts.end(); sIt++) {
	currentVerts.push_back(*sIt);
      }
    }
    
  }    

}


void TriElement::SetUpNearField2(int depth) {

  vertexNearField.resize(numbtris);


  for (int iT=0; iT<numbtris; iT++) {
    set<int> finishedTris;
    set<int> finishedVerts;
    
    set<int> nextVerts;
    
    vector<int> currentVerts;

    finishedTris.insert(iT);

    currentVerts.push_back(m_Faces[iT]->Index[0]);
    currentVerts.push_back(m_Faces[iT]->Index[1]);
    currentVerts.push_back(m_Faces[iT]->Index[2]);
    finishedVerts.insert(m_Faces[iT]->Index[0]);
    finishedVerts.insert(m_Faces[iT]->Index[1]);
    finishedVerts.insert(m_Faces[iT]->Index[2]);

    for (int iD=0; iD<depth; iD++) {
      
      for (int iC=0; iC<currentVerts.size(); iC++) {
	
	for (int iT1=0; iT1<vertexStar[currentVerts[iC]].size(); iT1++) {
	  
	  int myT = vertexStar[currentVerts[iC]][iT1];
	  
	  if (finishedTris.find(myT)== finishedTris.end()) {
	    vertexNearField[iT].insert(myT);
	    finishedTris.insert(myT);
	  }
	}


	for (int iN=0; iN<vertexNeighbors[currentVerts[iC]].size(); iN++) {
	  
	  int myN = vertexNeighbors[currentVerts[iC]][iN];	  
	  
	  if (finishedVerts.find(myN) == finishedVerts.end()) {
	    nextVerts.insert(myN);
	    finishedVerts.insert(myN);
	  }
	} 
      }
      
      currentVerts.clear();
      for (set<int>::iterator sIt=nextVerts.begin(); sIt != nextVerts.end(); sIt++) {
	currentVerts.push_back(*sIt);
      }
    }

  }


}
