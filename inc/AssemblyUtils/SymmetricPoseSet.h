/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with TexMol; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/



/**
Description of the class

* Input:
	Multiple lists of docked poses. Each list contains poses of a specific cyclic symmtry

* Maintains:
	1. Multiple lists of SymmetricPose objects (each repreesnting one pose)
	2. For each SymmetricPose object, it computes and lists all other SymmetricPose object of different symmetry order which does not clash with it. The list is maintained using an adjacency matrix, for quicker lookup while computing cliques.
	3. Compute all 3 and 4 cliques from the graph

* Output:
	All 3 and 4 cliques of SymmetricPose objects

* Algorithm:
	Straightforward use of SymmetricPose
**/



#ifndef SYMM_POSE_SET_H
#define SYMM_POSE_SET_H

#include <AssemblyUtils/SymmetricPose.h>

using namespace std;


class SymmetricPoseSet
{
	private:
		bool testMode;
		string poseListFileName;
		int symmetryOrder;
		int fileMode;
	
		vector<SymmetricPose*> poses;

		bool readPoseFile();

	public:
		SymmetricPoseSet(string fileName, int symmOrder, int fm)
		{
			testMode = true;
			poseListFileName = fileName;
			symmetryOrder = symmOrder;
			fileMode = fm;
	
			if(!readPoseFile())
			{
				cout<<"Error while reading pose list file\n";
			}
		}

		~SymmetricPoseSet() 
		{
			poses.clear(); 
		}

		int getSymmetryOrder() 
		{
			return symmetryOrder;
		}

		int getNumberOfPoses() 
		{
			return poses.size();
		}

		SymmetricPose* getPose(int index)
		{
			if(index>=0 && index<poses.size()) 
				return poses[index];
			return NULL;
		}
};



class SymmetricPoseClique
{
	private:
		bool testMode;
		int cliqueSize;
		vector<SymmetricPose*> poses;

	public:
		SymmetricPoseClique()
		{
			testMode = true;
			cliqueSize = 0;
		}

		~SymmetricPoseClique() 
		{ 
			poses.clear(); 
		}

		void setCliqueSize(int c) 
		{
			cliqueSize = c;
		}

		int getCliqueSize() 
		{
			return cliqueSize;
		}

		int getNumberOfPoses() 
		{
			return poses.size();
		}

		bool addPose(SymmetricPose* sp) 
		{
			if(poses.size()>=cliqueSize) 
				return false; 
			poses.push_back(sp); 
			return true;
		} 

		SymmetricPose* getPose(int index) 
		{
			if(index>=0 && index<poses.size()) 
				return poses[index];
			return NULL;
		}
};



class SymmetricPoseMultiGraph
{
	private:
		bool testMode;
	
		int numSymmtryTypes;
		string symmtryFileList;

		vector<SymmetricPoseSet*> poseSets;

		int totalNumberOfPoses;

		vector<int> numberOfPoses;
		vector<int> cumulativeNumberOfPoses;

		bool** adjacencyMatrix;

		vector<SymmetricPoseClique*> symmetricPoseCliques;

		bool cliquesReady;
		
		
		bool setupPoseSets();
	
		bool computeAdjacencyMatrix();

		bool findCliques();

		bool getPoseIndices(int poseId, int& setIndex, int& poseIndex);
	
		bool isValidClique(int i, int j, int k);

		bool isValidClique(int i, int j, int k, int l);
	
	public:

		SymmetricPoseMultiGraph(string sfl)
		{
			testMode = true;
			symmtryFileList = sfl;
			cliquesReady = false;

			if(setupPoseSets())
			{
				if(computeAdjacencyMatrix())
					cliquesReady = findCliques();
			}
		}

		~SymmetricPoseMultiGraph()
		{
			symmetricPoseCliques.clear();
			poseSets.clear();
			numberOfPoses.clear();

			if(adjacencyMatrix)
			{
				for(int i=0; i<totalNumberOfPoses; i++)
					if(adjacencyMatrix[i])
						delete adjacencyMatrix[i];
				delete adjacencyMatrix;		
			}
		}

		bool areCliquesReady() {return cliquesReady;}

		int getNumberOfCliques()
		{
			return symmetricPoseCliques.size();
		}

		SymmetricPoseClique* getClique(int index)
		{
			if(index>=0 && index<symmetricPoseCliques.size()) 
				return symmetricPoseCliques[index];
			return NULL;
		}	

		int getNumPoses() {return totalNumberOfPoses;}

		SymmetricPose* getPose(int poseId);
};

#endif
