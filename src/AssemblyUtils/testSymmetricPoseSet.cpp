#include <AssemblyUtils/SymmetricPoseSet.h>

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
	cout<<"testSymmetricPoseSet <input filename>"<<endl;
}


int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printUsage();
		return -1;		
	}

	string inputfilename(argv[1]);

	SymmetricPoseMultiGraph* spg = new SymmetricPoseMultiGraph(inputfilename);

	if(spg->areCliquesReady())
	{
		cout << "Found " << spg->getNumberOfCliques() << " cliques\n";

		for(int i=0; i<spg->getNumberOfCliques() && i<20; i++ )
		{
			SymmetricPoseClique* clique = spg->getClique(i);

			cout << "Size of clique " << i << " = " << clique->getCliqueSize() << endl;

			for(int j=0; j<clique->getCliqueSize(); j++)
			{
				SymmetricPose* sp = clique->getPose(j);
				cout<< "Pose " << j << " in the clique has order " <<  sp->getSymmetryOrder() << endl;
				
				char newpdbname[100];
				sprintf(newpdbname, "clique_%d_pose_%d.pdb", i, j );

				sp->writeAlignedOligomer(string(newpdbname));

				for(int k=0; k<clique->getCliqueSize(); k++)
				{
					if(k==j) continue;

					SymmetricPose* other = clique->getPose(k);
				
					char newpdbname2[100];
					sprintf(newpdbname2, "clique_%d_pose_%d_and_pose_%d.pdb", i, j, k );

					sp->writeMergedAndAlignedOligomer(string(newpdbname2), other);
				}
			}
		}
	}
}
