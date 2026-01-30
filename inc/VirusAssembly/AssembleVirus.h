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
	1. A clique of consistent cyclic symmetric poses. 
	2. A suggested layout for the capsid

* Maintains:
	1. Arranges the cyclic poses based on the layout
	2. Searches the space of 1D rotation around the symetry axis and the 1D translation along the axis
		a) Try to remove clash but ensure there exists some interface (i.e. components touch but not penetrate)
		b) Search uses Newton-Raphson iterations
		c) Initial guesses are artitrary now, but will be updated later to improve performance

* Output:
	Capsid

**/



#ifndef ASSEMBLE_VIRUS_H
#define ASSEMBLE_VIRUS_H

#include <AssemblyUtils/SymmetricPoseSet.h>
#include <fast-clash/clashFilter.h>
#include <fast-LJ/fastLJ.h>
#include <Utility/utility.h>
#include <Utility/utils.h>

using namespace std;


class SymmetryAxis
{
	private:
		int order;
		int orderOther;
		double xlationScale;
		MyVector primaryDirection;
		MyVector secondaryDirection;
		MyVector centroid;

	public:
		SymmetryAxis(int ord, int ord2,  double xls, double cx, double cy, double cz, double dx, double dy, double dz)
		{
			order = ord;
			orderOther = ord2;
			xlationScale = xls;
			primaryDirection = MyVector(cx,cy,cz);
			primaryDirection.normalize();
			secondaryDirection = MyVector(dx,dy,dz);
			secondaryDirection.normalize();
			centroid = MyVector(cx,cy,cz);
			centroid.normalize();
		}

		~SymmetryAxis(){}

		int getOrder() {return order;}
		int getOtherOrder() {return orderOther;}

		MyVector getPrimaryDirection() {return primaryDirection;}
		MyVector getSecondaryDirection() {return secondaryDirection;}
		MyVector getCentroid() {return centroid;}

		MyMatrix getXformToAlignTheZAxisToThisAxis() 
		{
			return MyMatrix::alignZAndXWith(primaryDirection, secondaryDirection);
		}

		MyMatrix getXformToAlignTheZAxisToThisAxis(double objectScale) 
		{
			double scale = objectScale*xlationScale;
			MyVector newCentroid(centroid[0]*scale, centroid[1]*scale, centroid[2]*scale); 
			return MyMatrix::alignZAndXWithAndMove(primaryDirection, secondaryDirection, newCentroid);
		}

		MyMatrix getXformToAlignThisAxisToTheZAxis() 
		{
			return MyMatrix::alignWithZAndX(primaryDirection, secondaryDirection);
		}

		MyMatrix getXformToAlignThisAxisToTheZAxis(double objectScale) 
		{
			double scale = objectScale*xlationScale;
			MyVector newCentroid(centroid[0]*scale, centroid[1]*scale, centroid[2]*scale); 
			return MyMatrix::centerAndAlignWithZAndX(primaryDirection, secondaryDirection, newCentroid);
		}

};

class Layout
{
	private:
		int numAxes;
		vector<SymmetryAxis*> axes;

		int numNeighbors;
		vector<int> neighbor1;	// Pairs of items in these two vectors
		vector<int> neighbor2;	// are indices to two neighboring axes.
					// These are used for the scoring.

		void readLayoutFile(string layoutFileName);

	public:
		Layout(string layoutFileName)
		{
			readLayoutFile(layoutFileName);
		}

		~Layout() {axes.clear();} 

		int getNumAxes() {return numAxes;}

		SymmetryAxis* getAxis(int index) {return axes[index];}

		int getNeighbor1(int index) {return neighbor1[index];}
		int getNeighbor2(int index) {return neighbor2[index];}
};



class AssembleVirus
{
	private:
		bool testMode;

		SymmetricPoseClique* clique;

		Layout* layout;

		clashFilter *cFilter;


		bool formCapsidUnit(vector<PDBParser::Atom*>& capsid, double scale, double rotZ, int poseIndex);

		bool formCapsid(vector<PDBParser::Atom*>& capsid, double scale, double rotZ, int poseIndex);

		bool searchScaleAndRotation(vector<double>& scale, vector<double>& rotZ, int& poseToUse);

		bool computeClashBetweenCopies(double objectScale, double angles, int poseIndex, int& clash, int & severe, double& score);


	public:
		AssembleVirus(SymmetricPoseClique* cl, string layoutFileName)
		{
			testMode = true;
			clique = cl;
			layout = new Layout(layoutFileName);
		}

		~AssembleVirus()
		{
			if(layout) delete layout;
		}

		bool testAxisXform(double scale, double rotZ, int poseIndex);

		bool getCapsid(vector< vector<PDBParser::Atom*> >& capsids);
};


#endif
