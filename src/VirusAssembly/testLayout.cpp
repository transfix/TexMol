#include <VirusAssembly/AssembleVirus.h>

#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<vector>

using namespace std;

void printUsage()
{
	cout<<"Usage:"<<endl;
	cout<<"testLayout <xyz filename> <layoutFileName> <scale>"<<endl;
}

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		printUsage();
		return -1;		
	}

	string inputfilename(argv[1]);

	FILE* fp = fopen(argv[1], "rt");

	double x, y, z;
	vector<MyVector> vecs;
	double scale = atof(argv[3]);

	/*for(int i=0; i<3;i++)
	{
		fscanf(fp, "%lf %lf %lf", &x, &y, &z);
		cout<< x << " " << y << " " <<z<<endl;
		MyVector v1(x,y,z+4.61652530564);
		vecs.push_back(v1);
	}*/

/*
	MyVector v1(2.0*26.5, 0.0, 0.0);
	MyVector v3(-1.0*26.5, 1.73205*26.5, 0.0);
	MyVector v4(-1.0*26.5, -1.73205*26.5, 0.0);
	MyVector v5(-0.24743583, 0.71428571, 0.00000000);
	MyVector v6(-0.49487166, -0.57142857, 0.00000000);
	MyVector v7(0.74230749, -0.14285714, 0.00000000);
	vecs.push_back(v1);
	vecs.push_back(v4);
	vecs.push_back(v3);
	vecs.push_back(v5);
	vecs.push_back(v6);
	vecs.push_back(v7);
*/

	MyVector v1(2.0*30.0, 0.0, 0.0);
	MyVector v2(-1.0*30.0, 1.73205*30.0, 0.0);
	MyVector v3(-1.0*30.0, -1.73205*30.0, 0.0);
	MyVector v4(0.5*30.0, 0.866025*30.0, 0.00000000);
	MyVector v5(-1.0*30.0, 0.0, 0.00000000);
	MyVector v6(0.5*30.0, -0.866025*30.0, 0.00000000);
	vecs.push_back(v1);
	vecs.push_back(v2);
	vecs.push_back(v3);
	vecs.push_back(v4);
	vecs.push_back(v5);
	vecs.push_back(v6);



	Layout* layout = new Layout(string(argv[2]));

	int numAxes = layout->getNumAxes();

//	cout<<"60 20\n";

	double r, g, b;


	for(int i=0; i<numAxes; i++)
	{
		for(int j=0; j<6;j++)
		{

			MyMatrix xform = layout->getAxis(i)->getXformToAlignTheZAxisToThisAxis(60.0);

			if(i<1){r = 1.0; g = 0.0; b = 0.0;}
			else if(i<2){r = 0.0; g = 1.0; b = 0.0;}
			else if(i<20){r = 0.0; g = 0.0; b = 1.0;}
			else {r = 1.0; g = 0.0; b = 1.0;}

			
			MyVector newVec = xform*vecs[j];
			printf("%0.4lf %0.4lf %0.4lf\n", newVec[0], newVec[1], newVec[2]);
//			cout<<newVec[0]<< " "<<newVec[1]<< " "<<newVec[2]<< " " << r << " "<< g << " "<<b<< "\n";
		}
	}

	int triangles[4][3];

	triangles[0][0] = 0;
	triangles[0][1] = 3;
	triangles[0][2] = 5;

	triangles[1][0] = 3;
	triangles[1][1] = 1;
	triangles[1][2] = 4;

	triangles[2][0] = 5;
	triangles[2][1] = 4;
	triangles[2][2] = 2;

	triangles[3][0] = 3;
	triangles[3][1] = 4;
	triangles[3][2] = 5;



	for(int i=0; i<20; i++)
	{
		for(int j=0; j<4; j++)
			cout<< triangles[j][0]+6*i << " " << triangles[j][1]+6*i << " " << triangles[j][2]+6*i << "\n"; 
	}


/*	
	for(int i=0; i<numAxes; i++)
	{
		printf("%d %d %d\n", 3*i, 3*i+1, 3*i+2);
	}
*/
}
