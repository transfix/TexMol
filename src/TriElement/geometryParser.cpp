#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include<iostream>

#include "TriElement/geometry.h"
#include "aspline.h"
#include "linearpatch.h"
#include "trielement.h"
#include "TriElement/geometryParser.h"

using namespace TriElementNS;

GeometryParser::GeometryParser()
{

}


int GeometryParser::ReadHowmanyComponents(FILE *fp)
{
    char  str[256];
    float  f0, f1, f2, f3, f4, f5, f6,f7,f8,f9,f10,f11;
    int   nvert, nface,component;

    fgets(str, 256, fp);
    sscanf(str, "%d %d", &nvert, &nface);

    fgets(str, 256, fp);
    component = sscanf(str, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
                            &f0, &f1, &f2, &f3, &f4, &f5,
                            &f6, &f7, &f8, &f9, &f10, &f11);
    return(component);
}


TriElement* GeometryParser::getASpline(const char* fname) {
  return getASpline(fname, 0);
}

TriElement* GeometryParser::getASpline(const char* fname, int permu)
{
	
        FILE* fp = fopen(fname, "r");
        if(!fp) {
                printf("could not open the rawnc file for read\n");
                exit(0);
        }
	TriElement* surface = new ASpline();
	return SurfaceTriangulation(surface,fp,permu);
}

TriElement* GeometryParser::getLinearPatch(const char* fname) {
  return getLinearPatch(fname,0);
}

TriElement* GeometryParser::getLinearPatch(const char* fname, int permu)
{
        FILE* fp = fopen(fname, "r");
        if(!fp) {
                printf("could not open the rawnc file for read\n");
                exit(0);
        }
        TriElement* surface = new LinearPatch();
        return SurfaceTriangulation(surface,fp,permu);
}


TriElement* GeometryParser::SurfaceTriangulation(TriElement* surface, FILE *fp, int permu)
{
    double maxx, maxy, maxz, minx, miny, minz;
    int           i,ii,jj,kk,i1,i2,i3,minindex,nscan;
    double        x,y,z,f,nx,ny,nz,red,green,blue;
    double         w;
    double         maxf, minf;

    howmany = ReadHowmanyComponents(fp);
    if (howmany != 3 && howmany != 4 && howmany != 6 && howmany !=9)
    {
        printf("howmany = %d\n", howmany);
        printf("Wrong data type in geometry\n");
        exit(0);
    }
    if (howmany == 3) {normalFlag = 0; colorFlag = 0;}
    if (howmany == 4) {normalFlag = 0; colorFlag = 0;}     /* have function value on vertex */
    if (howmany == 6) {normalFlag = 1; colorFlag = 0;}
    if (howmany == 9) {normalFlag = 1; colorFlag = 1;}    /* have rgb value on vertex */

    (normalFlag == 0) ? surface->normalFlag = false : surface->normalFlag = true;
    (colorFlag == 0) ? surface->colorFlag = false : surface->colorFlag = true;

    maxx = -100000.0;
    maxy = -100000.0;
    maxz = -100000.0;
    minx = 100000.0;
    miny = 100000.0;
    minz = 100000.0;
    minindex = 1000;

    rewind(fp);
    fscanf(fp,"%d %d\n",&surface->numbpts, &surface->numbtris);


    surface->vertexNeighbors.resize(surface->numbpts);
    surface->vertexStar.resize(surface->numbpts);

    if (howmany == 4)
    {
    }

    for (i = 0 ; i < surface->numbpts; i++)
    {
        if (howmany == 3)
        {
            nscan = fscanf(fp,"%lf %lf %lf\n", &x, &y, &z);
            if (nscan != 3 )
            {
                printf("Wrong data type in geometry\n");
                exit(0);
            }
        }
        if (howmany == 4)
        {
            nscan = fscanf(fp,"%lf %lf %lf %lf\n", &x, &y, &z, &f);
            if (nscan != 4 )
            {
                printf("Wrong data type in geometry\n");
                exit(0);
            }
        }
        if (normalFlag == 1)
        {
            if (howmany == 6 )
                nscan = fscanf(fp,"%lf %lf %lf %lf %lf %lf\n",
                               &x, &y, &z, &nx, &ny, &nz);
            if (howmany == 9 )
                nscan = fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
                               &x, &y, &z, &nx, &ny, &nz, &red, &green, &blue);
            if (nscan != 6 && nscan != 9)
            {
                printf("Wrong data type in geometry at %d\n", i);
                exit(0);
            }
        }
        if (x < minx) minx = x;
        if (y < miny) miny = y;
        if (z < minz) minz = z;
        if (x > maxx) maxx = x;
        if (y > maxy) maxy = y;
        if (z > maxz) maxz = z;

        Vertex* point = new Vertex();
        point->Point[0] = x;
        point->Point[1] = y;
        point->Point[2] = z;
        if (normalFlag == 1)
        {
 //           w = sqrt(nx*nx + ny*ny + nz*nz);
	    w = 1.0;
            point->Normal[0] = nx/w;
            point->Normal[1] = ny/w;
            point->Normal[2] = nz/w;
        }  else
        {
            point->Normal[0] = 0.0;
            point->Normal[1] = 0.0;
            point->Normal[2] = 0.0;
        }
	if (colorFlag == 1)
	{	
		point->Color[0] = red;
		point->Color[1] = green;
		point->Color[2] = blue;
	}
	point->weight = 1.0;	// default node weight
        surface->addPoint(point);
    } /* end point (i) loop */


    for (i = 0 ; i < surface->numbtris; i++)
    {
        fscanf(fp,"%d %d %d\n", &ii, &jj, &kk);
        if (ii < jj && jj < kk)    { i1 = ii; i2 = jj;  i3 = kk; }
        if (ii < kk && kk < jj)    { i1 = ii; i2 = kk;  i3 = jj; }
        if (jj < kk && kk < ii)    { i1 = jj; i2 = kk;  i3 = ii; }
        if (jj < ii && ii < kk)    { i1 = jj; i2 = ii;  i3 = kk; }
        if (kk < jj && jj < ii)    { i1 = kk; i2 = jj;  i3 = ii; }
        if (kk < ii && ii < jj)    { i1 = kk; i2 = ii;  i3 = jj; }  // result in i1 <= i2 <= i3
        
        Face* facet = new Face();
        facet->Index[0] = i1;
        facet->Index[1] = i2;
        facet->Index[2] = i3;

	if (permu == 2){
	  facet->IndexInRAW[0] = kk;
	  facet->IndexInRAW[1] = ii;
	  facet->IndexInRAW[2] = jj;
	} else if (permu == 1) {
	  facet->IndexInRAW[0] = jj;
	  facet->IndexInRAW[1] = kk;
	  facet->IndexInRAW[2] = ii;
	} else {
	  facet->IndexInRAW[0] = ii;
	  facet->IndexInRAW[1] = jj;
	  facet->IndexInRAW[2] = kk;
	}
        facet->AdjTri[0] = -1;
        facet->AdjTri[1] = -1;
        facet->AdjTri[2] = -1;

        facet->whichnml[0] = -1;
        facet->whichnml[1] = -1;
        facet->whichnml[2] = -1;
	//double* p1 = surface->m_Vertices[i1]->Point;
	//double* p2 = surface->m_Vertices[i2]->Point;
	//double* p3 = surface->m_Vertices[i3]->Point;

	//facet->Area = TriElement::Triangle_area(p1,p2,p3);
        surface->addFacet(facet);
        if (i1 < minindex) minindex = i1;
    }   /* end facet (i) loop */
      
    for (i = 0 ; i < surface->numbtris; i++)
    {
        surface->m_Faces[i]->Index[0] = surface->m_Faces[i]->Index[0] - minindex;
        surface->m_Faces[i]->Index[1] = surface->m_Faces[i]->Index[1] - minindex;
        surface->m_Faces[i]->Index[2] = surface->m_Faces[i]->Index[2] - minindex;
    }
    for (i = 0 ; i < surface->numbtris; i++)
    {
	for(int j=0;j<3;j++)
	{
		int index = surface->m_Faces[i]->Index[j];
		for(int k=0;k<3;k++)
		{
			surface->m_Faces[i]->Center[k] += 
				surface->m_Vertices[index]->Point[k];
			surface->m_Faces[i]->Normal[k] +=
                        	surface->m_Vertices[index]->Normal[k];
		}
	}
	for(int k=0;k<3;k++)
        {
		surface->m_Faces[i]->Center[k] /= 3.0;
		surface->m_Faces[i]->Normal[k] /= 3.0;
	}
    }
    
    //printf("minindex = %d,  numbtris = %d,  numbpts = %d\n",
    //                     minindex,surface->numbtris,surface->numbpts);
//    printf("%d points %d triangles\n", surface->m_Vertices.size(), surface->m_Faces.size());
    if (howmany == 4) /* with function value */
    {
    }

    return surface;
}

