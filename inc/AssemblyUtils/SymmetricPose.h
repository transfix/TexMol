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

Given a particular docking pose which corresponds to a cyclic symmetric 
oligomer, this class maintains the parameters of the pose. 

Given a relative transformation between two proteins, it provides methods to-
1) Detect the axis and center of symmetry, as well as the angle
2) Compute the whole oligomer
3) Align the oligomer's symmetry axis and center with the z-axis and the origin
4) Transform the aligned oligomer such that the centroid of the anchor protein lies on the X-axis

Given an oligomer aligned to the z-axis and the origin as mentioned above, it provides methods to-
5) Align an anchor copy of the protein with the coordinate based on principal components
6) Compute the relative transformation between a copy of the protein with the anchor

Given another instance of this class, it can-
7) Compute a 3-way assembly (anchor protein + a copy from this instance + a copy from the other instance)
8) Compute different scores for the 3-way assembly, specially computing clash, LJ, interface prop etc. between the two copies
9) Let the order of symmetries are n1 and n2, then it can compute a whole oligomer of n1 + n2 -1 proteins
10) Align this whole oligomer to the z-axis and the origin


There are some duplication of code which could have been avoided, but I feel they make the code more understandable and decided to keep it this way.
**/



#ifndef SYMM_POSE_H
#define SYMM_POSE_H

#include <PDBParser/Atom.h>
#include <GOAFileTypes/GOALoader.h>
#include <AssemblyUtils/MyVector.h>
#include <AssemblyUtils/MyMatrix.h>

#include <fast-clash/clashFilter.h>
#include <fast-LJ/fastLJ.h>
#include <Utility/utility.h>
#include <Utility/utils.h>


using namespace std;



class SymmetricPose
{
	private:
		bool testMode;

		bool oligomerGiven;
		bool relXformGiven;

		double distanceToCentroid;
		double maxRad;
		double minRad;
		double approxRad;

		string pdbFileName;

		vector<PDBParser::Atom*> givenOligomer;
		MyMatrix givenXform;

		vector<PDBParser::Atom*> anchorProtAtoms;
		vector<PDBParser::Atom*> copyProtAtoms;

		int symmetryOrder;

		bool atomsReady;
		bool anchored;
		bool relXformComputed;
		bool inverseRelXformComputed;
		bool alignToCoordComputed;

		bool cFilterReady;
		bool ljFilterReady;
		bool ipFilterReady;

		MyMatrix makeAnchor;			// transformation needed to align the anchor protein to the coord system
		MyMatrix relativeTransformation;		// relative motion to be applied to a copy of the anchored protein to get it to a symmetric pose w.r.t. to the anchor, which is stationary
		MyMatrix inverseRelativeTransformation;	// inverse of the above, makes a copy in the other direction (same as above if 2-fold symmetry)
		MyMatrix alignToCoord;			// transformation which maps the symmetry axis with the z-axis and the center to the origin. Dislodges the anchor, but keeps it centroid on X-axis

		/* Note on the matrices above-
			step 1: Given either a relative pose (as output by F2Dock, ZDock etc.) or an oligomer (as output by M-ZDock or any pdb containing multiple chains in symmetry), we immediately compute makeAnchor.
			step 2: Apply makeAnchor to the relative copy or one protein of the oligomer. This step is redundant in some cases, but it is still here to ensure that the next two steps do not depend on the input type
			step 3: compute relativeTransformation
			step 4: compute alignToCoord
		Details are available in the corresponding methods	
		*/


		clashFilter *cFilter;			// Used to compute copy-vs-copy LJ and Clash. Static Octree is populated with a copy after makeAnchor is applied to it. Moving Octree is populated with anchor.
							// When another SymmetricPose is given, the relativeTransformation from that is used to move the Moving Octree and compute the scores
							// I have not included pseudoGSol and LJ filters yet 

		void init();

		bool readAndDecomposePDB();
		bool readAndDecomposePDB(MyMatrix relXform);

		void computeCentroid(double &x, double &y, double &z);
		void computeCentroidForCopy(double &x, double &y, double &z);
		void computeGivenOligomerCentroid(double &x, double &y, double &z);
		void computeGivenOligomerCentroid(double &x, double &y, double &z, int index);
		void computeOligomerCentroidUsingGivenXform(double &x, double &y, double &z);
		void computeOligomerCentroid(double &x, double &y, double &z);

		// the following two methods are deprecated to avoid depending on LAPACK. Use selectAnchorAtomsAndVectors instead.
		bool computeLeastSquaresFit(double centroidX, double centroidY, double centroidZ, MyVector& PC1, MyVector& PC2, MyVector& PC3);

		bool computeEigenValuesAndVectors(double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22, double& w0, double& w1, double& w2, MyVector& PC1, MyVector& PC2, MyVector& PC3);

		void selectAnchorAtomsAndVectors(double centroidX, double centroidY, double centroidZ, MyVector& PC1, MyVector& PC2);

		void selectAnchorAtomsAndVectorsForCopy(double centroidX, double centroidY, double centroidZ, MyVector& PC1, MyVector& PC2);

		void anchorPDB();

		bool computeXforms();

		bool computeRads();

		bool computeMakeAnchor();
		bool computeMakeAnchorWithoutEigen();
		bool computeRelativeTransformation();
		bool computeInverseRelativeTransformation();
		bool computeAlignToCoord();

		bool writePDB(vector<PDBParser::Atom*>& mol, string fileName);

		void cleanup();


	public:
		SymmetricPose(string pdbFileName, int symmOrder); // the PDB contains the oligomer. the first 2 chains are used to define the anchor and its neighbor. number of chains is assumed to be equal to the symmetry order
		SymmetricPose(string pdbFileName, int symmOrder, MyMatrix relXform); 	// the PDB contains one copy only

		~SymmetricPose();

		int getSymmetryOrder() {return symmetryOrder;}

		MyMatrix getMakeAnchor();
		MyMatrix getRelativeTransformation();
		MyMatrix getInverseRelativeTransformation();
		MyMatrix getAlignToCoord();

		bool writeOligomer(string newPDBName);
		bool writeOligomerFromInverse(string newPDBName);
		bool writeAlignedOligomer(string newPDBName);

		bool computeClashBetweenCopies(SymmetricPose* otherPose, int& clash, int& severe, double& score);		// Does not verify if the other SymmetricPose instance is for the same proteins, it simply uses the relativeTransformation matrix from the other

		bool computeLJBetweenCopies(SymmetricPose* otherPose, double& score) {score = 0; return true;} // same caveat. shall be implemented soon

		bool computeInterfacePropBetweenCopies(SymmetricPose* otherPose, double& score) {score = 0; return true;} // same caveat. shall be implemented soon

		bool computeAlignedOligomer(vector<PDBParser::Atom*>& oligomer, int firstChainNo =0, bool includeAnchor = true);

		bool computeMergedOligomer(vector<PDBParser::Atom*>& mergedpdb, vector<PDBParser::Atom*>& oligomer, int firstChainNo =0);

		bool computeMergedAndAlignedOligomer(vector<PDBParser::Atom*>& mergedpdb, vector<PDBParser::Atom*>& oligomer, int firstChainNo =0);

		bool computeOligomer(vector<PDBParser::Atom*>& oligomer, int firstChainNo =0, bool includeAnchor = true);

		bool computeOligomerFromInverse(vector<PDBParser::Atom*>& oligomer, int firstChainNo =0, bool includeAnchor = true);


		bool writeMergedOligomer(string newPDBName, SymmetricPose* otherPose);	//same caveat

		bool writeMergedAndAlignedOligomer(string newPDBName, SymmetricPose* otherPose);	//same caveat

		double getMaxRad() {return maxRad;}
		double getMinRad() {return minRad;}


		bool setupFilters();

		void destroyFilters();
};




#endif
