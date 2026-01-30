#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"
#include "GBEnergy/PQRParser.h"
#include "GBEnergy/geometry.h"
#include "GBEnergy/surface.h"
#include "GBEnergy/geometryParser.h"
#include "GBEnergy/energy.h"

using namespace MOLECULE;
using namespace GBGEOMETRY;
using namespace std;

void quicksort(int t, double *F, int *order, int N)
{
  int lpos=0;
  int rpos=N-1;
  /*double pivot=x[((N-1)/2)*d+t];*/
  double pivot=F[N/2+t];

  int k;
  double temp1;
  int temp2;

  while (lpos<=rpos)
  {
    while (F[lpos+t]>pivot)
      lpos++;
    while (F[rpos+t]<pivot)
      rpos--;
    if (lpos<=rpos)
    {
        temp1=F[lpos];
        F[lpos]=F[rpos];
        F[rpos]=temp1;

      temp2=order[lpos];
      order[lpos]=order[rpos];
      order[rpos]=temp2;

      lpos++;
      rpos--;
    }
  }
  if (0<rpos)
    quicksort(t,F,order,rpos+1);
  if (lpos<N-1)
    quicksort(t,F+lpos,order+lpos, N-lpos);
}


int visualizeforce(int argc, char* argv[])
{
        FILE *fp;
        char fileName1[256], fileName2[256], fileName3[256];

	if (argc != 5)
	{
		printf("Usage: ./visualizForce original.rawnc force force.rawnc\n");
		exit(0);
	}

        fp = loadFile_read(argv[1]);
        GroupOfGBAtoms* molecule;
        molecule = GBParsePQR(fp, PQR_RADIUS);
        cout << molecule->numOfAtoms << " atoms" << endl;
        fclose(fp);

        fp = loadFile_read(argv[2]);
        Surface* surface;
        surface = ParseGeometry(fp); 
        fclose(fp);

//        surface->Triangle_Bezier_Patch();

	double *F = new double[molecule->numOfAtoms];
	int *order = new int[molecule->numOfAtoms];
	double fx, fy, fz, t, r, x, y, z;
	int i, j, N;

	fp = loadFile_read(argv[3]);
	for (i = 0; i < molecule->numOfAtoms; i++)
	{
		fscanf(fp, "%lf %lf %lf %lf\n", &fx, &fy, &fz, &t);
		F[i] = sqrt(fx*fx + fy*fy + fz*fz);
		order[i] = i;
	}

	quicksort(0, F, order, molecule->numOfAtoms);
/*
	for (i = 0; i < 20; i++)
	{
		printf("%d %f\n", order[i], F[i]);
	}
*/
	bool pick;

	// arand: why divide by 10?

	N = (int)molecule->numOfAtoms/10;
	printf("N = %d\n", N);

	fp = loadFile_write(argv[4]);
	fprintf(fp, "%d %d\n", surface->numbpts, surface->numbtris);
	// points
	for (i = 0; i < surface->numbpts; i++)
	{
		x = surface->m_Vertices[i]->Point[0];
		y = surface->m_Vertices[i]->Point[1];
		z = surface->m_Vertices[i]->Point[2];
		pick = false;

		for (j = 0; j < N; j++)
		{
			fx = molecule->m_Atoms[order[j]]->position[0];
			fy = molecule->m_Atoms[order[j]]->position[1];
			fz = molecule->m_Atoms[order[j]]->position[2];
			r = molecule->m_Atoms[order[j]]->radius;
			double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
			if (d < (r+0.8)*(r+0.8))
			{
				pick = true;
				fprintf(fp, "%f %f %f %f %f %f 1.0 0.0 0.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
				break;
			}
		}
		if (pick == true) continue;

		for (j = 9*N; j < molecule->numOfAtoms; j++)
                {
                        fx = molecule->m_Atoms[order[j]]->position[0];
                        fy = molecule->m_Atoms[order[j]]->position[1];
                        fz = molecule->m_Atoms[order[j]]->position[2];
                        r = molecule->m_Atoms[order[j]]->radius;
                        double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
                        if (d < (r+0.2)*(r+0.2))
                        {
                                pick = true;
                                fprintf(fp, "%f %f %f %f %f %f 0.0 0.0 1.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
                                break;
                        }
                }

                if (pick == false) 
		{
		//	printf("something wrong\n");
			fprintf(fp, "%f %f %f %f %f %f 0.9 0.9 0.9\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
		}
	}

	// face
        for (i = 0; i < surface->numbtris; i++)
        {
		fprintf(fp, "%d %d %d\n", surface->m_Faces[i]->Index[0], surface->m_Faces[i]->Index[1], surface->m_Faces[i]->Index[2]);
	}
        fclose(fp);
        return 0;
}


/*
		if (pick == true) continue;

                for (j = N; j < 2*N; j++)
                {
                        fx = molecule->m_Atoms[order[j]]->position[0];
                        fy = molecule->m_Atoms[order[j]]->position[1];
                        fz = molecule->m_Atoms[order[j]]->position[2];
                        r = molecule->m_Atoms[order[j]]->radius;
                        double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
                        if (d < (r+0.2)*(r+0.2))
                        {
                                pick = true;
                                fprintf(fp, "%f %f %f %f %f %f 1.0 0.5 0.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
                                break;
                        }
                }
                if (pick == true) continue;
 
                for (j = 2*N; j < 3*N; j++)
                {
                        fx = molecule->m_Atoms[order[j]]->position[0];
                        fy = molecule->m_Atoms[order[j]]->position[1];
                        fz = molecule->m_Atoms[order[j]]->position[2];
                        r = molecule->m_Atoms[order[j]]->radius;
                        double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
                        if (d < (r+0.2)*(r+0.2))
                        {
                                pick = true;
                                fprintf(fp, "%f %f %f %f %f %f 1.0 1.0 0.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
                                break;
                        }
                }
                if (pick == true) continue;

                for (j = 3*N; j < 4*N; j++)
                {
                        fx = molecule->m_Atoms[order[j]]->position[0];
                        fy = molecule->m_Atoms[order[j]]->position[1];
                        fz = molecule->m_Atoms[order[j]]->position[2];
                        r = molecule->m_Atoms[order[j]]->radius;
                        double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
                        if (d < (r+0.2)*(r+0.2))
                        {
                                pick = true;
                                fprintf(fp, "%f %f %f %f %f %f 0.0 1.0 0.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
                                break;
                        }
                }
                if (pick == true) continue;

                for (j = 9*N; j < molecule->numOfAtoms; j++)
                {
                        fx = molecule->m_Atoms[order[j]]->position[0];
                        fy = molecule->m_Atoms[order[j]]->position[1];
                        fz = molecule->m_Atoms[order[j]]->position[2];
                        r = molecule->m_Atoms[order[j]]->radius;
                        double d = (x-fx)*(x-fx) + (y-fy)*(y-fy) + (z-fz)*(z-fz);
                        if (d < (r+0.2)*(r+0.2))
                        {
                                pick = true;
                                fprintf(fp, "%f %f %f %f %f %f 0.0 0.0 1.0\n", x, y, z, surface->m_Vertices[i]->Normal[0], surface->m_Vertices[i]->Normal[1], surface->m_Vertices[i]->Normal[2]);
                                break;
                        }
                }
*/
