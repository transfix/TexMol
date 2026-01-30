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
	cout<<"testAssembleVirus <input filename> <layoutFileName> <capsidpdbName>"<<endl;
}

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		printUsage();
		return -1;		
	}

	string inputfilename(argv[1]);

	SymmetricPoseMultiGraph* spg = new SymmetricPoseMultiGraph(inputfilename);

	if(spg->areCliquesReady())
	{
		cout << "Found " << spg->getNumberOfCliques() << " cliques\n";

		if(spg->getNumberOfCliques() > 0)
		{
			SymmetricPoseClique* clique = spg->getClique(0);

			cout << "Size of clique = " << clique->getCliqueSize() << endl;

			for(int j=0; j<clique->getCliqueSize(); j++)
			{
				SymmetricPose* sp = clique->getPose(j);
				cout<< "Pose " << j << " in the clique has order " <<  sp->getSymmetryOrder() << endl;
				
				char newpdbname[100];
				sprintf(newpdbname, "clique_pose_%d.pdb", j );

				sp->writeAlignedOligomer(string(newpdbname));

				for(int k=0; k<clique->getCliqueSize(); k++)
				{
					if(k==j) continue;

					SymmetricPose* other = clique->getPose(k);
				
					char newpdbname2[100];
					sprintf(newpdbname2, "clique_pose_%d_and_pose_%d.pdb", j, k );

					sp->writeMergedAndAlignedOligomer(string(newpdbname2), other);
				}
			}

			AssembleVirus* av = new AssembleVirus(clique, string(argv[2]));
			
			vector < vector<PDBParser::Atom*> > capsids;

			if(av->getCapsid(capsids))
			{
				for(int c=0; c<capsids.size(); c++)
				{
					char capName[100];
			
					sprintf(capName, "test_capsid_%d.pdb", c);

					FILE *fp = fopen(capName, "wt");

					if(!fp)
					{
						cout<<"Could not write pdb\n";
						return false;
					}

					for(int i = 0; i < capsids[c].size(); i++)
						fprintf(fp, "ATOM%7d %-3s %3s %c%4d    %8.3f%8.3f%8.3f%8.4f%7.4f\n", i+1,
								capsids[c][i]->getName(),
								capsids[c][i]->getResidueName(),
								capsids[c][i]->getChain(),
								capsids[c][i]->getResidueNum(),
								capsids[c][i]->getX(),
								capsids[c][i]->getY(),
								capsids[c][i]->getZ(),
								capsids[c][i]->getCharge(),
								capsids[c][i]->getRadius());

					if(fp) fclose(fp);
				}
			}
			
		}
		else
		{
			int numPoses = spg->getNumPoses();
		
			for(int i=0; i< numPoses; i++)
			{
				SymmetricPose* sp = spg->getPose(i);
				char name[40];
				sprintf(name, "oligomer_for_pose_%d.pdb", i);
				sp->writeAlignedOligomer(name);
			}
		}
	}
}
