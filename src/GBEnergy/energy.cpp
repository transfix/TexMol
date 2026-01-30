#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
#include <vector>
//#include <time.h>
 
#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"
#include "GBEnergy/PQRParser.h"
#include "GBEnergy/geometry.h"
#include "GBEnergy/surface.h"
#include "GBEnergy/geometryParser.h"
#include "GBEnergy/polEnergy.h"
#include "GBEnergy/polForce.h"
#include "GBEnergy/CoarseGrain/coarseGrain.h"
#include "MolEnergyUtil/utils.h"
//#include "optimizer.h"

using namespace MOLECULE;
using namespace GBGEOMETRY;
using namespace FUNCTION;
using namespace std;

void printUsage()
{
        printf("\tGB -aspline -raw <inputSurfaceFileName> <outputSurfaceFileName> <int resolution>\n");
        printf("\tGB -aspline -quadrature <inputSurfaceFileName> <const char *quadratureRule (eg gaussian)> <int numOfPtsPerTriangle> <outputNodeFileName>\n");
        printf("\tGB -BornRadius <radiusType(vdw/pqr> <inputPQRFileName> <inputSurfaceFileName> <outputRadiusFileName>\n");
        printf("\tGB -energy <radiusType(vdw/pqr> <inputPQRFileName> <inputSurfaceFileName> <inputSurfaceAreaFileName> <outputEnergyFileName> <double eps> <int threshold>\n");
        printf("\tGB -force <radiusType(vdw/pqr> <inputPQRFileName> <inputSurfaceFileName> <inputSurfaceAreaFileName> <outputForceFileName>\n");
	printf("\tGB -dividePQR <inputPQRFileName> <output1stPQRFileName> <output2ndPQRFileName>\n");
        printf("\tGB -coarseGrain <radiusType(vdw/pqr> <inputPDB/PQRFileName> <outputPQRFileName> <outputCGInformation>\n");
	printf("\tGB -cgCluster <radiusType(vdw/pqr> <inputPDB/PQRFileName> <inputCGInformation> <outputPQRFileName>\n");
//        printf("\tGB -coarseGrainCharge <radiusType(vdw/pqr> <inputPDB/PQRFileName> <inputCGPQRFileName> <inputCGInformation> <outputOptimizedCGPQRFileName>\n");
        printf("\tGB -coarseGrainCharge <radiusType(vdw/pqr> <inputPDB/PQRFileName> <inputBornRadiusFileName> <inputCGPQRFileName> <inputCGBornRadiusFileName> <inputCGInformation> <outputOptimizedCGPQRFileName>\n");
        printf("\tGB -coarseGrainEnergy <radiusType(vdw/pqr> <inputPDB/PQRFileName> <inputCGSurfaceFileName> <inputCGSurfaceAreaFileName> <outputEnergyFileName>\n");
        printf("\tGB -coarseGrainForce\n");
}

FILE *loadFile_read(char* fileName)
{
        FILE* fp;
        fp=fopen(fileName, "r");
        if(fp == NULL)
        {
                printf("could not open file %s for read\n", fileName);
                exit(0);
        }
        return fp;
}

FILE *loadFile_write(char* fileName)
{
        FILE* fp;
        fp=fopen(fileName, "w");
        if(fp == NULL)
        {
                printf("could not open file %s for read\n", fileName);
                exit(0);
        }
        return fp;
}

GroupOfGBAtoms *GBParsePQR(FILE *fp, MOLECULE::RADIUS_TYPE radiusType)
{
        PQRParser* parser = new PQRParser();
        GroupOfGBAtoms* molecule = parser->ParserPQRtoGOA(fp, radiusType);
        delete parser;
        return molecule;
}

GroupOfGBAtoms *GBParseOptimalPQR(FILE *fp, MOLECULE::RADIUS_TYPE radiusType)
{
        PQRParser* parser = new PQRParser();
        GroupOfGBAtoms* molecule = parser->ParserOptimalPQRtoGOA(fp, radiusType);
        delete parser;
        return molecule;
}


Surface *ParseGeometry(FILE *fp)
{
        GeometryParser* parser = new GeometryParser();
        Surface* surface = parser->SurfaceTriangulation(fp);
	return surface;
}

void loadBornRadius(GroupOfGBAtoms *molecule, FILE *fp)
{
	double br;
	for (int i = 0; i < molecule->numOfAtoms; i++)
	{
		fscanf(fp, "%lf\n", &br);
		molecule->m_Atoms[i]->Born_radius = br;
	}
}

void ParseCGInfo(FILE *fp, GroupOfGBAtoms *groupOfBeads)
{
        ssize_t read;
        size_t len = 0;
        char* token = NULL;
        char* line = NULL;
        char* delim = (char *)" \t\n";
	vector<int> atomList;

        while((read = getline(&line,&len,fp)) != -1 )
        {
                token = strtok(line,delim);
                if(token == NULL)
                        break;
                if(!strcmp(token,"Bead") )
                {
                        token = strtok(NULL,delim);
                        int beadIndex = atoi(token);
                        //printf("index:%d\n",index);

                        if((read = getline(&line,&len,fp)) != -1 )
                        {
                                token = strtok(line,delim);
                                while(token != NULL)
                                {
					atomList.push_back(atoi(token));
                                        token = strtok(NULL,delim);
                                }
				groupOfBeads->m_Atoms[beadIndex]->child = new int[atomList.size()];
				for (int i = 0; i < atomList.size(); i++)
					groupOfBeads->m_Atoms[beadIndex]->child[i] = atomList[i];
				groupOfBeads->m_Atoms[beadIndex]->numOfChildren = atomList.size();
				atomList.clear();
                        }
                }
        }
}

/*
ATOM      3  C   ASP     1      63.870  35.622  28.099  0.5621 1.9080
ATOM      4  O   ASP     1      63.953  36.489  27.216 -0.5889 1.6612
ATOM      5  CB  ASP     1      65.899  34.294  27.517 -0.0235 1.9080
ATOM      6  CG  ASP     1      66.807  33.170  28.019  0.8194 1.9080
ATOM      7  OD1 ASP     1      66.825  32.872  29.277 -0.8084 1.6612
*/
void savePQR(GroupOfGBAtoms *goa, FILE *fp)
{
        for (int i = 0; i < goa->numOfAtoms; i++) 
                fprintf(fp, "ATOM%7d  C   %3s %c%4d    %8.3f%8.3f%8.3f%8.4f%7.4f\n", i+1,
                                goa->m_Atoms[i]->resName,
				goa->m_Atoms[i]->chainID,
                                goa->m_Atoms[i]->resSeq,
                                goa->m_Atoms[i]->position[0],
                                goa->m_Atoms[i]->position[1],
                                goa->m_Atoms[i]->position[2],
                                goa->m_Atoms[i]->charge,
                                goa->m_Atoms[i]->radius);
}


#include <time.h>
#ifdef _WIN32
        #include <sys/types.h>
        #include <sys/timeb.h>
#else
        #include <sys/time.h>
#endif

/*****************************************************************************************************************/
/*                                                                                                               */
/*  Get the current time in seconds as a double value                                                            */
/*                                                                                                               */
/*****************************************************************************************************************/

/*
double getTime2()
{
#ifdef _WIN32
        time_t ltime;
        time( &ltime );
        return (double) ltime;
#else
    struct timeval t;
    gettimeofday( &t, NULL );
    return (double)(t.tv_sec + 1e-6*t.tv_usec);
#endif
}
*/

void aspline(int argc, char* argv[])
{
}

double evaluate(double *x, double *p, double r, double c)
{
    return exp(c*((x[0]-p[0])*(x[0]-p[0])+(x[1]-p[1])*(x[1]-p[1])+(x[2]-p[2])*(x[2]-p[2])-r*r));
}

void surfaceError(int argc, char* argv[])
{
        FILE *fp;
        double start, total_time_1, total_time_2, total_time_3;

        MOLECULE::RADIUS_TYPE radiusType;
	if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
		radiusType = VDW_RADIUS;
	else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
	else { printUsage(); exit(0); }

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        molecule = GBParsePQR(fp, radiusType);
        cout << molecule->numOfAtoms << " atoms" << endl;
        fclose(fp);

        fp = loadFile_read(argv[4]);
        Surface* surface;
        surface = ParseGeometry(fp);
        fclose(fp);

	fp = loadFile_write(argv[5]);
	double x[3];
	double p[3];
	double r;
	double c;
	double sum1, sum2[3], t;
	double error;
	int i, j;
	double red, green, blue;
	double avg;
	double rmin = 999.0;

	for (i = 0; i < molecule->numOfAtoms; i++)
	{
		r = molecule->m_Atoms[i]->radius;
		if (r < rmin)	rmin = r;
	}

	fprintf(fp, "%d %d\n", (int) surface->m_Vertices.size(), (int) surface->m_Faces.size());

	avg = 0.0;
	for (j = 0; j < surface->m_Vertices.size(); j++)
	{
		x[0] = surface->m_Vertices[j]->Point[0];
		x[1] = surface->m_Vertices[j]->Point[1];
		x[2] = surface->m_Vertices[j]->Point[2];

		sum1 = 0.0;	sum2[0] = sum2[1] = sum2[2] = 0.0;
		for (i =0; i < molecule->numOfAtoms; i++)
		{
	    		p[0] = molecule->m_Atoms[i]->position[0];
			p[1] = molecule->m_Atoms[i]->position[1];
		    	p[2] = molecule->m_Atoms[i]->position[2];
	    		r =  molecule->m_Atoms[i]->radius;
			//c = -2.3;
			//c = -5.0*log(3)/r;
			// c = -5.0*log(3)/rmin;
			// c = -4.5; // Xu1
			 c = -1.8; // Xu2
			t = evaluate(x, p, r, c);
			sum1 += t;
			sum2[0] += t*(p[0]-x[0]);
			sum2[1] += t*(p[1]-x[1]);
			sum2[2] += t*(p[2]-x[2]);
	    	}
		error = fabs((sum1-1)/(2*c*(sqrt(sum2[0]*sum2[0]+sum2[1]*sum2[1]+sum2[2]*sum2[2]))));
		if (error < 0.05)
		{
		    red = 45; green = 213; blue = 62;
		}
		else if (error < 0.1)
		{
			red = 	43; green = 215; blue = 128;
		}
		else if (error < 0.2)
		{
		    red =  44; green = 141; blue = 214;
		}
		else if (error < 0.5)
		{
		    red =  46; green= 41; blue= 216;
		}
		else if (error < 1)
		{
		    red = 173; green = 39; blue = 218;
		}
		else if (error < 10)
		{
		    red = 220; green =  37; blue = 192;
		}
		else if (error < 50)
		{
		    red = 224; green =  33; blue = 129;
		}
		else 
		{
		    red = 223; green = 34; blue = 53;
		} 
		    fprintf(fp, "%f %f %f %f %f %f\n", x[0], x[1], x[2], red/255.0, green/255.0, blue/255.0);
		avg += error;
	}
	avg /= surface->m_Vertices.size();
	printf("average error = %f\n", avg);

	for (i = 0; i < surface->m_Faces.size(); i++)
	{
	    fprintf(fp, "%d %d %d\n", surface->m_Faces[i]->Index[0], surface->m_Faces[i]->Index[1],surface->m_Faces[i]->Index[2]);
	}

	fclose(fp);
}

void energy(int argc, char* argv[])
{
        FILE *fp;
        double start, total_time_1, total_time_2, total_time_3;


/* below is the modification of the ion rawn
	fp = loadFile_read(argv[2]);
        Surface* surface;
        surface = ParseGeometry(fp);
        fclose(fp);
//	printf("%d %d\n", surface->numbpts, surface->numbtris);

	int i, j;
	for (i = 0; i < surface->numbpts; i++)
	{
		double v[3];
		double n[3];
		for (j = 0; j < 3; j++)
		{
			v[j] = surface->m_Vertices[i]->Point[j];
			n[j] = surface->m_Vertices[i]->Normal[j];
		}
		if (v[2] > 0.0)
			v[2] = sqrt(2.0*2.0 - v[0]*v[0] - v[1]*v[1]);
		else
			v[2] = -1.0*sqrt(2.0*2.0 - v[0]*v[0] - v[1]*v[1]);
		surface->m_Vertices[i]->Point[2] = v[2];

		for (j = 0; j < 3; j++)
			surface->m_Vertices[i]->Normal[j] = v[j] / 4.0;

//		double norm1 = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
//		double norm2 = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
//		printf("%f %f %f %f\n", v[0]/norm1, v[1]/norm1, v[2]/norm1, norm1);
//		printf("%f %f %f %f\n", n[0]/norm2, n[1]/norm2, n[2]/norm2, norm2);
	}
	fp = loadFile_write(argv[3]);
	fprintf(fp, "%d %d\n", surface->numbpts, surface->numbtris);
	for (i = 0; i < surface->numbpts; i++)
		fprintf(fp, "%f %f %f %f %f %f\n", surface->m_Vertices[i]->Point[0], surface->m_Vertices[i]->Point[1], surface->m_Vertices[i]->Point[2],
						   surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
	for (i = 0; i < surface->numbtris; i++)
		fprintf(fp, "%d %d %d\n", surface->m_Faces[i]->Index[0], surface->m_Faces[i]->Index[1], surface->m_Faces[i]->Index[2]);
	fclose(fp);
*/

        MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        molecule = GBParsePQR(fp, radiusType);
        cout << molecule->numOfAtoms << " atoms" << endl;
        fclose(fp);

        fp = loadFile_read(argv[4]);
        Surface* surface;
        surface = ParseGeometry(fp);
        fclose(fp);

        #ifdef CHECK_RUN_TIME
        cout << "check average of 10 times running" << endl;

        double avg_time = 0.0;
        double max_time = 0.0;
        double min_time = 10000000.0;
        for( int i = 0; i < 10; i++ ) {
        #endif

        start = getTime2();
        surface->Triangle_Bezier_Patch();
        total_time_1 = getTime2() - start;
      
        #ifdef CHECK_RUN_TIME
        avg_time += total_time_1;
        if( max_time < total_time_1 ) max_time = total_time_1;
        if( min_time > total_time_1 ) min_time = total_time_1;
        }
        cout << "surface->Triangle_Bezier_Patch(): " << (avg_time - max_time - min_time)/8.0 << "(s)" << endl;
        #else
        cout << "surface->Triangle_Bezier_Patch(): " << total_time_1 << "(s)" << endl;
        #endif
     
        #ifdef CHECK_RUN_TIME
        avg_time = 0.0;
        max_time = 0.0;
        min_time = 10000000.0;
        for( int i = 0; i < 10; i++ ) {
        #endif

        start = getTime2();
        molecule->getBornRadii(surface);
        total_time_2 = getTime2() - start;

        #ifdef CHECK_RUN_TIME
        avg_time += total_time_2;
        if( max_time < total_time_2 ) max_time = total_time_2;
        if( min_time > total_time_2 ) min_time = total_time_2;
        }
        cout << "molecule->getBornRadii(): " << total_time_2 << (avg_time - max_time - min_time)/8.0 << "(s)" << endl;
        #else
        cout << "molecule->getBornRadii(): " << total_time_2 << "(s)" << endl;
        #endif

        fp = fopen("checkAtomBR.txt", "w");
        for (int i = 0; i < molecule->numOfAtoms; i++)
                fprintf(fp, "%d %f\n", i, molecule->m_Atoms[i]->Born_radius);
        fclose(fp);

	double eps = atof(argv[7]);
	int threshold = atoi(argv[8]);
        PolEnergy *m_Gpol= new PolEnergy(molecule);

        #ifdef CHECK_RUN_TIME
        avg_time = 0.0;
        max_time = 0.0;
        min_time = 10000000.0;
        for( int i = 0; i < 10; i++ ) {
        #endif

        start = getTime2();
//        molecule->getPolEnergy();
        m_Gpol->getPolEnergy(eps, threshold);
        total_time_3 = getTime2() - start;

        #ifdef CHECK_RUN_TIME
        avg_time += total_time_3;
        if( max_time < total_time_3 ) max_time = total_time_3;
        if( min_time > total_time_3 ) min_time = total_time_3;
        }
        cout << "m_Gpol->getPolEnergy(): " << (avg_time - max_time - min_time)/8.0 << "(s)" << endl;
        #else
        cout << "m_Gpol->getPolEnergy(): " << total_time_3 << "(s)" << endl;
        #endif

        const double surface_tension = 19*0.001; // Simonson94 or 7.2*10^(-3) in Still90
        double area;
        fp =loadFile_read(argv[5]);
        fscanf(fp, "%lf\n", &area);
        double nonpol_energy = surface_tension*area;

        double sol_energy = m_Gpol->pol_energy + nonpol_energy;

        cout << "pol_energy: " << m_Gpol->pol_energy << "(kcal/mol), sol_energy:" << sol_energy << "(kcal/mol)" << endl;

        fp = loadFile_write(argv[6]);
        fprintf(fp, "%f kcal/mol, %f kcal/mol, %fs, %fs, %fs, total:%fs\n", m_Gpol->pol_energy, sol_energy, total_time_1, total_time_2, total_time_3,
                total_time_1+total_time_2+total_time_3);
	fclose(fp);
}


void force(int argc, char* argv[])
{
        FILE *fp;
	double start, total_time_1, total_time_2, total_time_3, force_time;

        MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        molecule = GBParsePQR(fp, radiusType);
        cout << molecule->numOfAtoms << " atoms" << endl;
        fclose(fp);

        fp = loadFile_read(argv[4]);
        Surface* surface;
        surface = ParseGeometry(fp);
        fclose(fp);

        start = getTime2();
        surface->Triangle_Bezier_Patch();
        total_time_1 = getTime2() - start;

        start = getTime2();
        molecule->getBornRadii(surface);
        total_time_2 = getTime2() - start;

        PolEnergy *m_Gpol= new PolEnergy(molecule);
        start = getTime2();
        m_Gpol->getPolEnergy(0.1, 100); // eps = 0.1, theshold = 100
        total_time_3 = getTime2() - start;

        start = getTime2();
        PolForce *m_Fpol = new PolForce();
	m_Fpol->getPolForce(molecule, surface);
        force_time = getTime2() - start;

        const double surface_tension = 19*0.001; // Simonson94 or 7.2*10^(-3) in Still90
        double area;
        fp =loadFile_read(argv[5]);
        fscanf(fp, "%lf\n", &area);
        double nonpol_energy = surface_tension*area;

        double sol_energy = m_Gpol->pol_energy + nonpol_energy;

        fp = loadFile_write(argv[6]);
	fprintf(fp, "Force Time: %f\n", force_time);
        for (int i = 0; i < m_Fpol->force.size(); i++)
                fprintf(fp, "%f %f %f\n", m_Fpol->force[i].fx, m_Fpol->force[i].fy, m_Fpol->force[i].fz);
        fclose(fp);
}

void dividePQR(int argc, char* argv[])
{
	FILE *fp, *fp2;
	int i;
        MOLECULE::RADIUS_TYPE radiusType = PQR_RADIUS;

        fp = loadFile_read(argv[2]);
        GroupOfGBAtoms* molecule;
        molecule = GBParsePQR(fp, radiusType);
        cout << molecule->numOfAtoms << " atoms" << endl;
        fclose(fp);

	fp = loadFile_write(argv[3]);
	fp2 = loadFile_write(argv[4]);
	int numres = (int)(molecule->numOfRes/2);
	for (i = 0; i < molecule->numOfAtoms; i++)
	{
		if (molecule->m_Atoms[i]->resSeq < numres)
			fprintf(fp, "ATOM%7d%3s   %3s%6d    %8.3f%8.3f%8.3f %8.4f %6.4f\n", i+1,
				molecule->m_Atoms[i]->atomName,
                                molecule->m_Atoms[i]->resName,
                                molecule->m_Atoms[i]->resSeq,
                                molecule->m_Atoms[i]->position[0],
                                molecule->m_Atoms[i]->position[1],
                                molecule->m_Atoms[i]->position[2],
                                molecule->m_Atoms[i]->charge,
                                molecule->m_Atoms[i]->radius);
		if (molecule->m_Atoms[i]->resSeq >= numres)
                        fprintf(fp2, "ATOM%7d%3s   %3s%6d    %8.3f%8.3f%8.3f %8.4f %6.4f\n", i+1,
                                molecule->m_Atoms[i]->atomName,
                                molecule->m_Atoms[i]->resName,
                                molecule->m_Atoms[i]->resSeq,
                                molecule->m_Atoms[i]->position[0],
                                molecule->m_Atoms[i]->position[1],
                                molecule->m_Atoms[i]->position[2],
                                molecule->m_Atoms[i]->charge,
                                molecule->m_Atoms[i]->radius);
	}
	fclose(fp);
	fclose(fp2);
}

void mergePQR(int argc, char* argv[])
{
        FILE *fp;
        int i;
        MOLECULE::RADIUS_TYPE radiusType = PQR_RADIUS;

        fp = loadFile_read(argv[2]);
        GroupOfGBAtoms* molecule1;
        molecule1 = GBParsePQR(fp, radiusType);
        cout << molecule1->numOfAtoms << " atoms" << endl;
        fclose(fp);

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule2;
        molecule2 = GBParsePQR(fp, radiusType);
        cout << molecule2->numOfAtoms << " atoms" << endl;
        fclose(fp);

	fp = loadFile_write(argv[4]);
	int j = 0;
        for (i = 0; i < molecule1->numOfAtoms; i++)
        {
		fprintf(fp, "ATOM%7d%3s   %3s%6d    %8.3f%8.3f%8.3f %8.4f %6.4f\n", j+1,
                                molecule1->m_Atoms[i]->atomName,
                                molecule1->m_Atoms[i]->resName,
                                molecule1->m_Atoms[i]->resSeq,
                                molecule1->m_Atoms[i]->position[0],
                                molecule1->m_Atoms[i]->position[1],
                                molecule1->m_Atoms[i]->position[2],
                                molecule1->m_Atoms[i]->charge,
                                molecule1->m_Atoms[i]->radius);
		j++;
	}
	for (i = 0; i < molecule2->numOfAtoms; i++)
        {
                fprintf(fp, "ATOM%7d%3s   %3s%6d    %8.3f%8.3f%8.3f %8.4f %6.4f\n", j+1,
                                molecule2->m_Atoms[i]->atomName,
                                molecule2->m_Atoms[i]->resName,
                                molecule2->m_Atoms[i]->resSeq,
                                molecule2->m_Atoms[i]->position[0],
                                molecule2->m_Atoms[i]->position[1],
                                molecule2->m_Atoms[i]->position[2],
                                molecule2->m_Atoms[i]->charge,
                                molecule2->m_Atoms[i]->radius);
                j++;
        }
	fclose(fp);
}


/*
void coarseGrain(int argc, char* argv[])
{
        FILE *fp;
        char fileName1[256], fileName2[256], fileName3[256];
	int i;

	MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        if (strstr(argv[3], "optimal"))
                molecule = ParseOptimalPQR(fp, radiusType);
        else
                molecule = ParsePQR(fp, radiusType);

//	printf("%f %f %f %f %f", molecule->m_Atoms[1]->position[0], molecule->m_Atoms[1]->position[1], molecule->m_Atoms[1]->position[2], molecule->m_Atoms[1]->charge, molecule->m_Atoms[1]->radius);


        cout << molecule->numOfAtoms << " atoms, " << molecule->numOfRes << "residues" << endl;
        fclose(fp);

        //fp = loadFile_read(argv[3]);
        //Surface* surface;
        //surface = ParseGeometry(fp);
        //fclose(fp);

        CoarseGrain* cg = new CoarseGrain();
        GroupOfGBAtoms* groupOfBeads = cg->CoarseGrainMolecule(molecule);
        delete cg;

	//for (i = 0; i < groupOfBeads->numOfAtoms; i++)
	//{
	//	Optimizer *opt = Optimzer(molecule, groupOfBeads->m_Atoms[0], surface);
	//	opt->optimizeCenterAndRadius();
	//	groupOfBeads->m_Atoms[0]->position[0] = opt->adata[0]->center[0];
	//	groupOfBeads->m_Atoms[0]->position[1] = opt->adata[0]->center[1];
	//	groupOfBeads->m_Atoms[0]->position[2] = opt->adata[0]->center[2];
	//	groupOfBeads->m_Atoms[0]->radius = opt->adata[0]->radius;
	//}


        printf("%d beads\n", groupOfBeads->numOfAtoms);
        fp = loadFile_write(argv[4]);
	savePQR(groupOfBeads, fp);
        fclose(fp);

	fp = loadFile_write(argv[5]);
	for (i = 0; i < groupOfBeads->numOfAtoms; i++)
	{
		fprintf(fp, "Bead %d\n", i);
		for (int j = 0; j < groupOfBeads->m_Atoms[i]->numOfChildren; j++)
		{
			fprintf(fp, "%d ", groupOfBeads->m_Atoms[i]->child[j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
        return; 
}


void coarseGrainCluster(int argc, char* argv[])
{
        FILE *fp;
        char fileName1[256], fileName2[256], fileName3[256];
	int i;

	MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        if (strstr(argv[3], "optimal"))
                molecule = ParseOptimalPQR(fp, radiusType);
        else
                molecule = ParsePQR(fp, radiusType);
        cout << molecule->numOfAtoms << " atoms, " << molecule->numOfRes << "residues" << endl;
        fclose(fp);

        CoarseGrain* cg = new CoarseGrain(molecule);
        GroupOfGBAtoms* groupOfBeads = new GroupOfGBAtoms();	
	fp = loadFile_read(argv[4]);
	fscanf(fp, "%d\n", &groupOfBeads->numOfAtoms);
	for(i = 0; i < groupOfBeads->numOfAtoms; i++)
	{
		GBAtom *bead = new GBAtom();
		groupOfBeads->m_Atoms.push_back(bead);
	}
        ParseCGInfo(fp, groupOfBeads);
	for (i = 0; i < groupOfBeads->numOfAtoms; i++)
	{
	        char resName[5];
		int head = groupOfBeads->m_Atoms[i]->child[0];
        	strcpy(resName, molecule->m_Atoms[head]->resName);
        	int resSeq = molecule->m_Atoms[head]->resSeq;

        	groupOfBeads->m_Atoms[i]->setResName(resName);
        	groupOfBeads->m_Atoms[i]->setResIndex(resSeq);
	}
	cg->CoarseGrainGeometry(groupOfBeads);
	delete cg;
	fclose(fp);

	fp = loadFile_write(argv[5]);
        savePQR(groupOfBeads, fp);
        fclose(fp);

        return; 
}


void coarseGrainCharge(int argc, char* argv[])
{
        FILE *fp;
        char fileName1[256], fileName2[256], fileName3[256];
	int i;

	MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

	// atomic model
        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        if (strstr(argv[3], "optimal"))
                molecule = ParseOptimalPQR(fp, radiusType);
        else
                molecule = ParsePQR(fp, radiusType);
	// cg model
        fp = loadFile_read(argv[4]);
        GroupOfGBAtoms* groupOfBeads;
        groupOfBeads = ParsePQR(fp, PQR_RADIUS);
        fclose(fp);

	fp = loadFile_read(argv[5]);
	ParseCGInfo(fp, groupOfBeads);

        cout << molecule->numOfAtoms << " atoms, " << molecule->numOfRes << " residues, " << groupOfBeads->numOfAtoms << " cg beads" << endl;

	CoarseGrain *cg = new CoarseGrain(molecule);
	cg->CoarseGrainCharge(groupOfBeads);
	delete cg;

	fp = loadFile_write(argv[6]);
	savePQR(groupOfBeads, fp);
	fclose(fp);

        return; 
}

void coarseGrainChargeGB(int argc, char* argv[])
{
        FILE *fp;
        char fileName1[256], fileName2[256], fileName3[256];
        int i;

        MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

        // atomic model
        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        if (strstr(argv[3], "optimal"))
                molecule = ParseOptimalPQR(fp, radiusType);
        else
                molecule = ParsePQR(fp, radiusType);
	fclose(fp);
	fp = loadFile_read(argv[4]);
	loadBornRadius(molecule, fp);
	fclose(fp);

        // cg model
        fp = loadFile_read(argv[5]);
        GroupOfGBAtoms* groupOfBeads;
        groupOfBeads = ParsePQR(fp, PQR_RADIUS);
        fclose(fp);

//	if (groupOfBeads->numOfAtoms <= 300 || groupOfBeads->numOfAtoms > 400)
//		exit(0);

        fp = loadFile_read(argv[6]);
	loadBornRadius(groupOfBeads, fp);
	fclose(fp);

        fp = loadFile_read(argv[7]);
        ParseCGInfo(fp, groupOfBeads);

        cout << molecule->numOfAtoms << " atoms, " << molecule->numOfRes << " residues, " << groupOfBeads->numOfAtoms << " cg beads" << endl;

        CoarseGrain *cg = new CoarseGrain(molecule);
        cg->CoarseGrainChargeGB2(groupOfBeads);
        delete cg;

        fp = loadFile_write(argv[8]);
        savePQR(groupOfBeads, fp);
        fclose(fp);

        return;
}


void coarseGrainEnergy(int argc, char* argv[])
{
        FILE *fp;
        double start, cg_time, quadrature_time, BR_time, energy_time;

        MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        molecule = ParsePQR(fp, radiusType);
        cout << molecule->numOfAtoms << " atoms" << endl;
        fclose(fp);

	if (molecule->numOfAtoms > 6000)
	{
	        fp = loadFile_write(argv[6]);
        	fprintf(fp, "%d atoms, %d atoms, %f kcal/mol, %f kcal/mol, %fs, %fs, %fs, %fs, total:%fs\n", 0, 0, 0, 0, 0, 0, 0, 0, 0);
	        fclose(fp);
		return;
	}
	start = getTime2();
        CoarseGrain* cg = new CoarseGrain();
        GroupOfGBAtoms* cg_molecule = cg->CoarseGrainMolecule(molecule);
        cout << cg_molecule->numOfAtoms << " CG atoms" << endl;
        delete cg;
	cg_time = getTime2() - start;

        fp = loadFile_read(argv[4]);
        Surface* surface;
        surface = ParseGeometry(fp);
        fclose(fp);

        start = getTime2();
        surface->Triangle_Bezier_Patch();
        quadrature_time = getTime2() - start;

        start = getTime2();
        cg_molecule->getBornRadii(surface);
//	cg_molecule->assignBRtoChild(molecule);
        BR_time = getTime2() - start;

	//fp = fopen("checkAtomBR.txt", "w");
	//for (int i = 0; i < molecule->numOfAtoms; i++)
	//	fprintf(fp, "%f\n", molecule->m_Atoms[i]->Born_radius);
	//fclose(fp);

        PolEnergy *m_Gpol= new PolEnergy(cg_molecule);
        start = getTime2();
//        molecule->getPolEnergy();
        m_Gpol->getPolEnergy(0.1, 100); // eps = 0.1, threshold = 100
        energy_time = getTime2() - start;

        const double surface_tension = 19*0.001; // Simonson94 or 7.2*10^(-3) in Still90
        double area;
        fp =loadFile_read(argv[5]);
        fscanf(fp, "%lf\n", &area);
        double nonpol_energy = surface_tension*area;

        double sol_energy = m_Gpol->pol_energy + nonpol_energy;

        fp = loadFile_write(argv[6]);
        fprintf(fp, "%d atoms, %d atoms, %f kcal/mol, %f kcal/mol, %fs, %fs, %fs, %fs, total:%fs\n", molecule->numOfAtoms, cg_molecule->numOfAtoms, m_Gpol->pol_energy, sol_energy, cg_time, quadrature_time, BR_time, energy_time,
                cg_time+quadrature_time+BR_time+energy_time);
	fclose(fp);
	return;
}

void coarseGrainForce(int argc, char* argv[])
{

}
*/

void getBornRadius(int argc, char* argv[])
{
		if(argc<6)
		{
			 printUsage();
			 exit(1);
		}
			
        FILE *fp;
        double start, total_time_1, total_time_2, total_time_3;

        MOLECULE::RADIUS_TYPE radiusType;
        if (strcmp(argv[2], "vdw") == 0 || strcmp(argv[2], "VDW") == 0 )
                radiusType = VDW_RADIUS;
        else if (strcmp(argv[2], "pqr") == 0 || strcmp(argv[2], "PQR") == 0)
                radiusType = PQR_RADIUS;
        else { printUsage(); exit(0); }

        fp = loadFile_read(argv[3]);
        GroupOfGBAtoms* molecule;
        molecule = GBParsePQR(fp, radiusType);
        cout << molecule->numOfAtoms << " atoms" << endl;
        fclose(fp);

        fp = loadFile_read(argv[4]);
        Surface* surface;
        surface = ParseGeometry(fp);
        fclose(fp);

        start = getTime2();
        surface->Triangle_Bezier_Patch();
        total_time_1 = getTime2() - start;

        start = getTime2();
        molecule->getBornRadii(surface);

        fp = loadFile_write(argv[5]);
	for (int i = 0; i < molecule->numOfAtoms; i++)
	        fprintf(fp, "%f\n", molecule->m_Atoms[i]->Born_radius);
	fclose(fp);
	return;
}
