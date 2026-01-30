#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trielement.h"
#include "aspline.h"
#include "geometry.h"
#include "aspline.h"
#include "trielement.h"
#include "linearpatch.h"
#include "geometryParser.h"

using namespace GEOMETRY;

void saveNewRawncFile(TriElement *aspline, FILE *fp, int resolution)
{
	int i, j, t, k;

        fprintf(fp, "%d %d\n", aspline->m_Vertices.size(), aspline->m_Faces.size()*resolution*resolution);

        // vertices loop
        for (i = 0; i < aspline->m_Vertices.size(); i++)
        {
                double P[3], N[3], C[3];
                for (j = 0; j < 3; j++)
                {
                        P[j] = aspline->m_Vertices[i]->Point[j];
                        N[j] = aspline->m_Vertices[i]->Normal[j];
                }

                C[0] = aspline->m_Vertices[i]->Color[0];
                C[1] = aspline->m_Vertices[i]->Color[1];
                C[2] = aspline->m_Vertices[i]->Color[2];

		if (aspline->normalFlag == true && aspline->colorFlag == true)
			fprintf(fp, "%f %f %f %f %f %f %f %f %f\n", P[0],P[1],P[2],N[0],N[1],N[2],C[0],C[1],C[2]);
		else if (aspline->normalFlag == true && aspline->colorFlag == false)
                        fprintf(fp, "%f %f %f %f %f %f\n", P[0],P[1],P[2],N[0],N[1],N[2]);
                else if (aspline->normalFlag == false && aspline->colorFlag == true)
                        fprintf(fp, "%f %f %f %f %f %f\n", P[0],P[1],P[2],C[0],C[1],C[2]);
                else if (aspline->normalFlag == false && aspline->colorFlag == false)
			fprintf(fp, "%f %f %f\n", P[0],P[1],P[2]);
        }

	k = 0;
        // face index loop
        for (i = 0; i < aspline->m_Faces.size(); i++)
        {
                for ( t = 0; t < aspline->m_Faces[i]->subFacets.size(); t++)
                {
                        int index[3];
                        for (j = 0; j < 3; j++)
                        {
                                index[j] = aspline->m_Faces[i]->subFacets[t]->IndexInRAW[j]+k*aspline->m_Vertices.size();
                        }
                        fprintf(fp, "%d %d %d\n", index[0], index[1], index[2]);
                }
        }
	
}


void getAspect(int argc, char **argv)
{

        GeometryParser* parser = new GeometryParser();
        TriElement* surface = parser->getLinearPatch(argv[2]);
	delete parser;
	surface->calculateAspectRatio();
	
}

// ./aspline -rawnc initial.rawnc new.rawnc resolution
void getRawnc(int argc, char **argv)
{
        if (argc != 5)
        {
                printf("USAGE:\n");
                printf("./aspline -raw initial.raw new.raw resolution\n");
		exit(0);
        }

	FILE *fp1, *fp2;
	int resolution;

	/*fp1 = fopen(argv[2], "r");
        if(!fp1) {
                printf("could not open the rawnc file for read\n");
                exit(0);
        }*/

        fp2 = fopen(argv[3], "w");
        if(!fp2) {
		printf("could not open the file for write\n");
		exit(0);
	}
	
	resolution = atoi(argv[4]);

	GeometryParser* parser = new GeometryParser();
	TriElement* aspline = (ASpline*)parser->getASpline(argv[2]);
	delete parser;

	((ASpline*)aspline)->Triangle_Bezier_Patch(resolution);

	saveNewRawncFile(aspline, fp2, resolution);
	//fclose(fp1);
	fclose(fp2);
}


// ./aspline -quadrature *.rawnc rule numOfPtsPerTriangle *.pts
void getQuadrature(int argc, char **argv)
{
        if (argc != 6)
        {
                printf("USAGE:\n");
                printf("./aspline -quadrature RawFile rule numOfPtsPerTriangle NodeFile\n");
		exit(0);
        }

	FILE *fp1, *fp2;
	char type[100];
	int numOfPts;
	int i, k;

        /*fp1 = fopen(argv[2], "r");
        if(!fp1) {
                printf("could not open the rawnc file for read\n");
                exit(0);
        }*/
	strcpy(type, argv[3]);
	numOfPts = atoi(argv[4]);

        GeometryParser* parser = new GeometryParser();
        //TriElement* aspline = (ASpline*)parser->getASpline(argv[2]);
        TriElement* aspline = parser->getASpline(argv[2]);
        delete parser;

	aspline->Triangle_Patch_Nodes(type, numOfPts);

	// output
	fp2 = fopen(argv[5], "w");
        if(!fp2) {
                printf("could not open the file for write\n");
                exit(0);
        }
	for (i = 0; i < aspline->numbtris; i++)
	{
		for (k = 0; k < numOfPts; k++)
		{
			fprintf(fp2, "%f %f %f %f %f %f %f\n", 
				aspline->m_Faces[i]->Node[k]->Point[0], 
				aspline->m_Faces[i]->Node[k]->Point[1], 
				aspline->m_Faces[i]->Node[k]->Point[2], 
				aspline->m_Faces[i]->Node[k]->Normal[0], 
				aspline->m_Faces[i]->Node[k]->Normal[1], 
				aspline->m_Faces[i]->Node[k]->Normal[2], 
				aspline->m_Faces[i]->Node[k]->weight);
		}
	}

        //fclose(fp1);
        fclose(fp2);

}

void getLinearQuadrature(int argc, char **argv)
{
        if (argc != 6)
        {
                printf("USAGE:\n");
                printf("./aspline -quadrature RawFile rule numOfPtsPerTriangle NodeFile\n");
                exit(0);
        }

        FILE *fp1, *fp2;
        char type[100];
        int numOfPts;
        int i, k;

        strcpy(type, argv[3]);
        numOfPts = atoi(argv[4]);

        GeometryParser* parser = new GeometryParser();
        TriElement* lpatch = (LinearPatch*)parser->getLinearPatch(argv[2]);
        delete parser;

	((LinearPatch*)lpatch)->Triangle_Patch_Nodes(type, numOfPts);

        // output
        fp2 = fopen(argv[5], "w");
        if(!fp2) {
                printf("could not open the file for write\n");
                exit(0);
        }
        for (i = 0; i < lpatch->numbtris; i++)
        {
                for (k = 0; k < numOfPts; k++)
                {
			fprintf(fp2, "%f %f %f %f %f %f %f\n",
                                lpatch->m_Faces[i]->Node[k]->Point[0],
                                lpatch->m_Faces[i]->Node[k]->Point[1],
                                lpatch->m_Faces[i]->Node[k]->Point[2],
                                lpatch->m_Faces[i]->Node[k]->Normal[0],
                                lpatch->m_Faces[i]->Node[k]->Normal[1],
                                lpatch->m_Faces[i]->Node[k]->Normal[2],
                                lpatch->m_Faces[i]->Node[k]->weight);
                }
        }

        //fclose(fp1);
        fclose(fp2);

}


int main(int argc, char **argv)
{
	if (argc != 3 && argc != 5 && argc != 6)
        {
		printf("USAGE:\n");
                printf("./aspline -raw initial.raw new.raw resolution\n");
                printf("./aspline -quadrature RawFile rule numOfPtsPerTriangle NodeFile\n");
		printf("./aspline -lquadrature RawncFile rule numOfPtsPerTriangle NodeFile\n");
		printf("./aspline -aspect RawFile\n");
        }
	else if( strcmp(argv[1], "-aspect") == 0)
	{
		getAspect(argc, argv);
	}
	else if( strcmp(argv[1], "-raw") == 0)
	{
		getRawnc(argc, argv);
	}
        else if ( strcmp(argv[1], "-quadrature") == 0)
        {
		getQuadrature(argc, argv);
	}
	else if ( strcmp(argv[1], "-lquadrature") == 0)
        {
                getLinearQuadrature(argc, argv);
        }
	else 
	{
                printf("USAGE:\n");
		printf("./aspline -raw initial.raw new.raw resolution\n");
		printf("./aspline -quadrature RawncFile rule numOfPtsPerTriangle NodeFile\n");
		printf("./aspline -lquadrature RawncFile rule numOfPtsPerTriangle NodeFile\n");
	}
	return 0;
}
