
#include <AssemblyUtils/SymmetricPose.h>

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
	cout<<"testAlignedOligomerFormation <input PDB filename> <output aligned oligomer filename> <symmOrder> <mode: 1=pdb contains oligomer, 2=relative xform given in file (use the next three params)> [(if mode = 2)xformfile] [(if mode = 2)conf]"<<endl;
}

int main(int argc, char** argv)
{
	if(!(argc == 5 || argc == 7))
	{
		printUsage();
		return -1;		
	}

	string pdbfilename(argv[1]);
	string outputpdbname(argv[2]);

	int symmetryOrder = atoi(argv[3]);
	int mode = atoi(argv[4]);

	if(mode == 1)
	{
		SymmetricPose* sp = new SymmetricPose(pdbfilename, symmetryOrder);
		
		if(!sp->writeAlignedOligomer(outputpdbname))
		{
			cout<<"Error in symmetric pose\n";
			delete sp;
			return -2;
		}
		delete sp;
	}
	else if(mode == 2)
	{
		MyMatrix relXform;

		FILE* xforms = fopen(argv[5],"rt");
		if(!xforms)
		{
			cout<<"Could not open transformations file"<<endl;
			return -3;
		}


		int conf = atoi(argv[6]);

		int numXform;
		fscanf(xforms, "%d", &numXform);

		if(conf > numXform)
		{
			cout<<"Conformation number out of range"<<endl;
			return -4;
		}

		double a[4][4], s, r;

		for(int i=0; i<conf; i++)
		{
			fscanf(xforms, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &a[0][0], &a[0][1], &a[0][2], &a[0][3], &a[1][0], &a[1][1], &a[1][2], &a[1][3], &a[2][0], &a[2][1], &a[2][2], &a[2][3], &a[3][0], &a[3][1], &a[3][2], &a[3][3], &s, &r);
		}

		fclose(xforms);

		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				relXform.set(i,j, a[i][j]);

		relXform.print();

		SymmetricPose* sp = new SymmetricPose(pdbfilename, symmetryOrder, relXform);
		
		if(!sp->writeAlignedOligomer(outputpdbname))
		{
			cout<<"Error in symmetric pose\n";
			delete sp;
			return -5;
		}
		delete sp;
	}
	else
	{
		cout<<"Wrong mode\n";	
		return -3;
	}

	return 0;
}
