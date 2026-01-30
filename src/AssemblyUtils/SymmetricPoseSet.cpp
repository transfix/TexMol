
#include <AssemblyUtils/SymmetricPoseSet.h>




/**
	The input file must have the following format:
	Line 1: <int: number of symmtry types>
	Line 2: <int: symmetry order of next list> <int: type of the list (1 = oligomer giver, 2 = pdb+xforms)>
	Line 3: <string listfilepath>
	Line 4-5: next list....
	....
**/

bool SymmetricPoseMultiGraph::setupPoseSets()
{
	if(testMode)
		cout<<"Setting up the poses. reading list of lists.\n";

	FILE *fp = fopen (symmtryFileList.c_str(), "rt");

	if(fp == NULL)
	{
		cout<<"Could not open symmery list file\n";	
		return false;
	}

	fscanf( fp, "%d\n", &numSymmtryTypes);

	totalNumberOfPoses = 0;

	int order, mode;
	char fileName[200];

	if(testMode)
		cout<<"Number of lists = " << numSymmtryTypes << ".\n";


	for(int i=0; i<numSymmtryTypes; i++)
	{
		fscanf( fp, "%d %d\n", &order, &mode);
//		fgets( fileName, 200, fp);
		fscanf( fp, "%s", fileName );

		if(testMode)
			cout<<"Reading list named: " << fileName << ".\n";

		SymmetricPoseSet* sps = new SymmetricPoseSet(string(fileName), order, mode);

		poseSets.push_back(sps);

		int np = sps->getNumberOfPoses();

		if(np == 0)
		{
			cout<<"Why is the number of poses 0 for list no. " << i <<" ?\n";
		}

		totalNumberOfPoses += np;

		if(i>0) cumulativeNumberOfPoses.push_back(cumulativeNumberOfPoses[i-1] + np);
		else cumulativeNumberOfPoses.push_back(np);


		if(testMode)
			cout<<"Read list. Found " << np <<" poses\n";


		numberOfPoses.push_back(np);
	}

	fclose(fp);

	return true;
}




bool SymmetricPoseMultiGraph::getPoseIndices(int poseId, int& setIndex, int& poseIndex)
{
	if(poseId < 0)
		return false; 

	for(int i=0; i<numSymmtryTypes; i++ )
	{
		if(poseId < cumulativeNumberOfPoses[i])
		{
			setIndex = i;
			poseIndex = poseId - cumulativeNumberOfPoses[i-1];

			if(testMode)
				cout<< "poseId " << poseId <<  "   setIndex " << setIndex <<  "   poseIndex " << poseIndex << endl;

			return true;
		} 
	}
	return false;
}




SymmetricPose* SymmetricPoseMultiGraph::getPose(int poseId)
{
	int setIndex, poseIndex;
	if(getPoseIndices(poseId, setIndex, poseIndex))
		return poseSets[setIndex]->getPose(poseIndex);	
	return NULL;
}



	
bool SymmetricPoseMultiGraph::computeAdjacencyMatrix()
{
	adjacencyMatrix = new bool* [totalNumberOfPoses] ;

	for( int i = 0 ; i < totalNumberOfPoses ; i++ )
		adjacencyMatrix[i] = new bool[totalNumberOfPoses];


	int setIndex1, poseIndex1;
	int setIndex2, poseIndex2;
	int clash, severe;
	double score;
	
	for( int i = 0 ; i < totalNumberOfPoses ; i++ )
	{
		if(!getPoseIndices(i, setIndex1, poseIndex1))
		{
			cout<<"Error: failed to resolve index 1\n";
			return false;
		}

		SymmetricPose* p1 = poseSets[setIndex1]->getPose(poseIndex1);
		p1->setupFilters();


		char fn0[100];
		sprintf(fn0, "anch_%d.pdb", i);
//		p1->writeOligomer(string(fn0));

		for( int j = 0 ; j < totalNumberOfPoses ; j++ )
		{
			adjacencyMatrix[i][j] = false;

			if(!getPoseIndices(j, setIndex2, poseIndex2))
			{
				cout<<"Error: failed to resolve index 2\n";
				return false;
			}

			if(setIndex1 == setIndex2)
				continue;

			SymmetricPose* p2 = poseSets[setIndex2]->getPose(poseIndex2);

			char fn[100];
			sprintf(fn, "merge_%d_%d.pdb", i, j);

//			p1->writeMergedAndAlignedOligomer(string(fn), p2);

			p1->computeClashBetweenCopies(p2, clash, severe, score);

			cout<<"Overall clash score = [soft, severe, score] = [" << clash << ", " << severe << ", " << score << "]\n" ;

			if( clash > 150 && severe > 20 )	// clash. make the thresholds user-defined later.
			if( clash > 600 && severe > 200 )	// clash. make the thresholds user-defined later.
				continue;

			if(testMode)
				cout << i <<" and " << j << " are consistent\n";

			adjacencyMatrix[i][j] = true;		// consistent. no clash. add edge
		}	

		p1->destroyFilters();
	}

	if(testMode)
	{
		for( int i = 0 ; i < totalNumberOfPoses ; i++ )
		{
			for( int j = 0 ; j < totalNumberOfPoses ; j++ )
			{
				cout<<adjacencyMatrix[i][j]<<" ";
			}
			cout<<"\n";
		}
	}

	return true;
}




bool SymmetricPoseMultiGraph::isValidClique(int i, int j, int k)
{
	int setId, poseId;

	if(!getPoseIndices(i, setId, poseId))
		return false;

	if(poseSets[setId]->getSymmetryOrder() == 5)
		return true;

        if(!getPoseIndices(j, setId, poseId))
                return false;

        if(poseSets[setId]->getSymmetryOrder() == 5)
                return true;

        if(!getPoseIndices(k, setId, poseId))
                return false;

        if(poseSets[setId]->getSymmetryOrder() == 5)
                return true;

	return false;
}




bool SymmetricPoseMultiGraph::isValidClique(int i, int j, int k, int l)
{
	if(isValidClique(i,j,k))
		return true;

        int setId, poseId;

        if(!getPoseIndices(l, setId, poseId))
                return false;

        if(poseSets[setId]->getSymmetryOrder() == 5)
                return true;

	return false;
}




bool SymmetricPoseMultiGraph::findCliques() // only finding 3 and 4 cliques. each clique must include at least 1 5-fold pose
{
	for(int i=0; i<totalNumberOfPoses; i++)
	{
		for(int j=i+1; j<totalNumberOfPoses; j++)
		{
			if(!adjacencyMatrix[i][j] || !adjacencyMatrix[j][i])
				continue;

			for(int k=j+1; k<totalNumberOfPoses; k++)
			{
				if(!adjacencyMatrix[i][k] || !adjacencyMatrix[j][k] || !adjacencyMatrix[k][i] || !adjacencyMatrix[k][j])
					continue;

				//3-clique found
				if(isValidClique(i,j,k))
				{
					SymmetricPose* p1 = getPose(i);
					SymmetricPose* p2 = getPose(j);
					SymmetricPose* p3 = getPose(k);

					SymmetricPoseClique* spc = new SymmetricPoseClique();

					spc->setCliqueSize(3);

					spc->addPose(p1);
					spc->addPose(p2);
					spc->addPose(p3);

					symmetricPoseCliques.push_back(spc);
				}

				for(int l=k+1; l<totalNumberOfPoses; l++)
				{
	                                if(!adjacencyMatrix[i][l] || !adjacencyMatrix[j][l] || !adjacencyMatrix[k][l] || !adjacencyMatrix[l][i] || !adjacencyMatrix[l][j] || !adjacencyMatrix[l][k])
						continue;

					if(isValidClique(i,j,k,l))
					{
	                                        SymmetricPose* p1 = getPose(i);
        	                                SymmetricPose* p2 = getPose(j);
                	                        SymmetricPose* p3 = getPose(k);
						SymmetricPose* p4 = getPose(l);

                        	                SymmetricPoseClique* spc = new SymmetricPoseClique();

                                	        spc->setCliqueSize(4);

                                        	spc->addPose(p1);
	                                        spc->addPose(p2);
        	                                spc->addPose(p3);
						spc->addPose(p4);

                	                        symmetricPoseCliques.push_back(spc);
					}
				}
			}
		}
	}


	return true;
}





/**
	The pose list file must have the following format:

	if mode is 1
	-------------
	Line 1: <int n : number of poses>
	Line 2 to n+1: <path to the oligomer pdb>

	if(mode is 2)
	-------------
	Line 1: <path to the pdb>
	Line 2: <int n : number of poses>
	Line 3: <18 double variables> (the first 16 gives the relative xform, the next 2 are rmsd and score respectively)
**/


bool SymmetricPoseSet::readPoseFile()
{
	char pdbName[200];
	int numPoses;

	if(testMode)
		cout<<"Reading poselist file\n";

	FILE *fp = fopen(poseListFileName.c_str(), "r");

	if(fp == NULL)
	{
		cout<<"Could not open "<<poseListFileName<<endl;
		return false;
	}

	if(fileMode == 1)
	{
		fscanf(fp, "%d\n", &numPoses);

		if(testMode)
			cout<<"Number of poses should be "<<numPoses<<"\n";
		
		for(int i=0; i<numPoses; i++)
		{
			char oligomerName[200];

//			fgets(oligomerName, 200, fp);
			fscanf(fp, "%s", oligomerName);

			if(testMode)
				cout<<"Reading oligomer from " << oligomerName << "\n";

			SymmetricPose* sp = new SymmetricPose(string(oligomerName), symmetryOrder);

			poses.push_back(sp);
		}

		fclose(fp);
	}

	else if(fileMode == 2)
	{
		fgets(pdbName, 200, fp);
		fscanf(fp, "%d\n", &numPoses);

		for(int i=0; i<numPoses; i++)
		{
			MyMatrix relXform;
			double a[4][4], s, r;

			fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &a[0][0], &a[0][1], &a[0][2], &a[0][3], &a[1][0], &a[1][1], &a[1][2], &a[1][3], &a[2][0], &a[2][1], &a[2][2], &a[2][3], &a[3][0], &a[3][1], &a[3][2], &a[3][3], &s, &r);

	                for(int j=0; j<4; j++)
        	                for(int k=0; k<4; k++)
                	                relXform.set(j,k, a[j][k]);

			if(testMode)
			{
				cout<<"Read xform\n ";
				relXform.print();
			}

	                SymmetricPose* sp = new SymmetricPose(pdbName, symmetryOrder, relXform);

			poses.push_back(sp);
		}
	
		fclose(fp);
	}

	else
	{
		cout<<"Wrong file mode\n";
		return false;
	}

	return true;
}



	


