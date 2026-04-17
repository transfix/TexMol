
#include <AssemblyUtils/SymmetricPose.h>
#include <PDBParser/FlattenGOA.h>
#include <UsefulMath/Matrix.h>
#include <malloc.h>

/**
Using LAPACK for least squares solving
**/

extern "C" {void dsyev_( char* jobz, char* uplo, int* n, double* a, int* lda,
                double* w, double* work, int* lwork, int* info );
	   }
#define N 3
#define LDA N


/** ********* **
PUBLIC METHODS
** ********* **/



SymmetricPose::SymmetricPose(string pdb, int symm)			
{
	testMode = false;

	symmetryOrder = symm;

	pdbFileName = pdb;

	oligomerGiven = false;	// hack

	relXformGiven = false;

	init();
	atomsReady = readAndDecomposePDB();
	
	if(!atomsReady)
	{
		cout<<"Cout not parse PDB\n";
		return;
	}

	if(!computeXforms())
	{
		cout<<"Cout not compute xforms\n";
		return;
	}

	if(!computeRads())
	{
		cout<<"Could not compute approximate radii\n";
		return;
	}

/*	if(!setupFilters())
	{
		cout<<"Cout not setup filters\n";
		return;
	}*/
}




SymmetricPose::SymmetricPose(string pdb, int symm, MyMatrix relXform)
{
	testMode = false;

	symmetryOrder = symm;

	pdbFileName = pdb;

	oligomerGiven = false;

	relXformGiven = true;

	givenXform = relXform;

	init();
	atomsReady = readAndDecomposePDB(relXform);
	
	if(!atomsReady)
	{
		cout<<"Cout not parse PDB\n";
		return;
	}

	if(!computeXforms())
	{
		cout<<"Cout not compute xforms\n";
		return;
	}

/*	if(!setupFilters())
	{
		cout<<"Cout not setup filters\n";
		return;
	}*/
}




SymmetricPose::~SymmetricPose()
{
	cleanup();
}




MyMatrix SymmetricPose::getMakeAnchor() {return makeAnchor;}
MyMatrix SymmetricPose::getRelativeTransformation() {return relativeTransformation;}
MyMatrix SymmetricPose::getInverseRelativeTransformation() {return inverseRelativeTransformation;}
MyMatrix SymmetricPose::getAlignToCoord() {return alignToCoord;}




bool SymmetricPose::writePDB(vector<PDBParser::Atom*>& mol, string fileName)
{
	FILE *fp = fopen(fileName.c_str(), "wt");

	if(!fp)
	{
		cout<<"Could not write pdb\n";
		return false;
	}

	for(int i = 0; i < mol.size(); i++)
		fprintf(fp, "ATOM%7d %-3s %3s %c%4d    %8.3f%8.3f%8.3f%8.4f%7.4f\n", i+1,
				mol[i]->getName(),
				mol[i]->getResidueName(),
				mol[i]->getChain(),
				mol[i]->getResidueNum(),
				mol[i]->getX(),
				mol[i]->getY(),
				mol[i]->getZ(),
				mol[i]->getCharge(),
				mol[i]->getRadius());

	if(fp) fclose(fp);

	if(testMode)
		cout<<"PDB written\n";

	return true;	

}




bool SymmetricPose::writeOligomerFromInverse(string newPDBName)
{
	if(testMode)
		cout<<"Writing aligned oligomer\n";

	vector<PDBParser::Atom*> oligomer;
	
	if(!computeOligomerFromInverse(oligomer, 0, true))
	{
		cout<<"Could not prepare oligomer coordinates\n";
		return false;
	}

	if(testMode)
		cout<<"Oligomer coords computed\n";

	bool success = writePDB(oligomer, newPDBName);

	oligomer.clear();

	return success;
}





bool SymmetricPose::writeOligomer(string newPDBName)
{
	if(testMode)
		cout<<"Writing aligned oligomer\n";

	vector<PDBParser::Atom*> oligomer;
	
	if(!computeOligomer(oligomer, 0, true))
	{
		cout<<"Could not prepare oligomer coordinates\n";
		return false;
	}

	if(testMode)
		cout<<"Oligomer coords computed\n";

	bool success = writePDB(oligomer, newPDBName);

	oligomer.clear();

	return success;
}




bool SymmetricPose::writeAlignedOligomer(string newPDBName)
{
	if(testMode)
		cout<<"Writing aligned oligomer\n";

	vector<PDBParser::Atom*> oligomer;
	
	if(!computeAlignedOligomer(oligomer, 0, true))
	{
		cout<<"Could not prepare oligomer coordinates\n";
		return false;
	}

	if(testMode)
		cout<<"Oligomer coords computed\n";

	bool success = writePDB(oligomer, newPDBName);

	oligomer.clear();

	return success;
}




bool SymmetricPose::computeClashBetweenCopies(SymmetricPose* otherPose, int& clash, int& severe, double& score)
{
	if(testMode)
		cout<<"Computing clash\n";

	if(!cFilterReady)
	{
		cout<<"Cannot compute clash score. Filter not initialized\n";
		return false;
	}

	if(!otherPose->relXformComputed)
	{
		cout<<"Cannot compute clash score. Relative transformation is not available in the other pose\n";
		return false;
	}

	MyMatrix xform;
	int cl = 0, sev = 0;
	double sc = 0.0;

	clash = 0;
	severe = 0;
	score = 0;

	for(int i=0; i < otherPose->symmetryOrder-1; i++)
	{
		xform = otherPose->relativeTransformation*xform;

		CCVOpenGLMath::Matrix mtx(	xform.get(0,0), xform.get(0,1), xform.get(0,2), xform.get(0,3),
						xform.get(1,0), xform.get(1,1), xform.get(1,2), xform.get(1,3),
						xform.get(2,0), xform.get(2,1), xform.get(2,2), xform.get(2,3),
						xform.get(3,0), xform.get(3,1), xform.get(3,2), xform.get(3,3)
					);

		if(!cFilter->computeInteractions( mtx, &cl, &sev, &sc ))
		{
			cout<<"could not compute clash scores\n";
			return false;
		}

		clash += cl;
		severe += sev;
		score += sc;

		if(testMode)
		{
			cout<<"Clash scores computed for one copy\n";
			cout<<"clash score = [soft, severe, score] = [" << cl << ", " << sev << ", " << sc << "]\n" ;
		}
	}
 
	return true;
}




/*bool SymmetricPose::computeLJBetweenCopies(SymmetricPose* otherPose, double& score)
{


}*/




/*bool SymmetricPose::computeInterfacePropBetweenCopies(SymmetricPose* otherPose, double& score)
{


}*/





bool SymmetricPose::writeMergedOligomer(string newPDBName, SymmetricPose* otherPose)
{
	if(testMode)
		cout<<"Writing merged oligomer\n";

	vector<PDBParser::Atom*> mergedpdb;
	
	if(!otherPose->computeOligomer(mergedpdb, 0, true))
	{
		cout<<"Could not prepare oligomer coordinates\n";
		return false;
	}

	if(testMode)
		cout<<"Coords for other oligomer computed\n";


	vector<PDBParser::Atom*> oligomer;
	
	if(!computeMergedOligomer(mergedpdb, oligomer, 0))
	{
		cout<<"Could not prepare oligomer coordinates\n";
		return false;
	}

	if(testMode)
		cout<<"Oligomers merged\n";	
	
	bool success = writePDB(oligomer, newPDBName);

	oligomer.clear();
	mergedpdb.clear();

	return success;
}




bool SymmetricPose::writeMergedAndAlignedOligomer(string newPDBName, SymmetricPose* otherPose)
{
	if(testMode)
		cout<<"Writing merged oligomer\n";

	vector<PDBParser::Atom*> mergedpdb;
	
	if(!otherPose->computeOligomer(mergedpdb, 0, true))
	{
		cout<<"Could not prepare oligomer coordinates\n";
		return false;
	}

	if(testMode)
		cout<<"Coords for other oligomer computed\n";


	vector<PDBParser::Atom*> oligomer;
	
	if(!computeMergedAndAlignedOligomer(mergedpdb, oligomer, 0))
	{
		cout<<"Could not prepare oligomer coordinates\n";
		return false;
	}

	if(testMode)
		cout<<"Oligomers merged\n";	
	
	bool success = writePDB(oligomer, newPDBName);

	oligomer.clear();
	mergedpdb.clear();

	return success;
}




/** ********* **
PRIVATE METHODS
** ********* **/

bool SymmetricPose::readAndDecomposePDB()
{
	if(testMode)
		cout<<"Reading oligomer pdb " << pdbFileName << "\n";

	PDBParser::GroupOfAtoms* input = 0;
	GOALoader* gLoader = new GOALoader();
	input = gLoader->loadFile(pdbFileName.c_str());
	delete gLoader;

	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, 0);
	
	PDBParser::CollectionData* collectionData = 0;
	if(input->type == PDBParser::COLLECTION_TYPE)
	{
		collectionData = input->m_CollectionData;
	}
	PDBParser::FlattenGOA(input, givenOligomer, collectionData, 0, 0, 0, radiusType, PDBParser::ATOM_TYPE, false);

	if(testMode)
		cout<<"Oligomer read. " << givenOligomer.size() <<" atoms found. Decomposing it now.\n";

	char tmpChainID = givenOligomer[0]->getChain();

	int chainCounter = 1;

	for(int i=0; i< givenOligomer.size(); i++)
	{
		PDBParser::Atom *at = new PDBParser::Atom(*givenOligomer[i]);
		if(at->getChain() != tmpChainID)
		{	
			tmpChainID = at->getChain();
			chainCounter ++;
		}
		
		if(chainCounter==1) anchorProtAtoms.push_back(at);
		if(chainCounter==2) copyProtAtoms.push_back(at);
	}

	if(testMode)
		cout<<"Oligomer decomposed\n";

	if(testMode)
	{
		writePDB(anchorProtAtoms, string("anchor_initial.pdb"));
		writePDB(copyProtAtoms, string("copy_initial.pdb"));
	}

	return chainCounter == symmetryOrder;
}




bool SymmetricPose::readAndDecomposePDB(MyMatrix relXform)
{
	if(testMode)
		cout<<"Reading pdb and relative xform\n";

	PDBParser::GroupOfAtoms* input = 0;
	GOALoader* gLoader = new GOALoader();
	input = gLoader->loadFile(pdbFileName.c_str());
	delete gLoader;

	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, 0);

	PDBParser::CollectionData* collectionData = 0;
	if(input->type == PDBParser::COLLECTION_TYPE)
	{
		collectionData = input->m_CollectionData;
	}
	PDBParser::FlattenGOA(input, givenOligomer, collectionData, 0, 0, 0, radiusType, PDBParser::ATOM_TYPE, false);

	if(testMode)
		cout<<"pdb read " << givenOligomer.size() <<" atoms found.\n";

	int numAtoms = givenOligomer.size();

	// prepare copies for anchoring

	for(int i=0; i< numAtoms; i++)
	{
		PDBParser::Atom *at = new PDBParser::Atom(*givenOligomer[i]);
		
		anchorProtAtoms.push_back(at);

		PDBParser::Atom *at2 = new PDBParser::Atom(*at);
		
		MyVector vec(at->getX(), at->getY(), at->getZ());

		vec = relXform*vec;

		at2->setPosition(vec[0], vec[1], vec[2]);

		copyProtAtoms.push_back(at2);
	}

	if(testMode)
		cout<<"relative xform applied\n";

	if(testMode)
	{
		writePDB(anchorProtAtoms, string("anchor_initial.pdb"));
		writePDB(copyProtAtoms, string("copy_initial.pdb"));
	}

	return true;
}




bool SymmetricPose::computeXforms()
{
	if(testMode)
		cout<<"Computing all xforms\n";

	if(!atomsReady) 
	{
		cout<<"ERROR: computeXforms called when atoms are not ready\n";
		return false;
	}

	anchored = computeMakeAnchorWithoutEigen();

	if(!anchored)
	{
		cout<<"ERROR: failed to anchor proteins\n";
		return false;
	}

	if(testMode)
		cout<<"Anchored\n";

	if(testMode)
	{
		writePDB(anchorProtAtoms, string("anchor_anchored.pdb"));
		writePDB(copyProtAtoms, string("copy_anchored.pdb"));
	}

	inverseRelXformComputed = computeInverseRelativeTransformation();

	if(!inverseRelXformComputed)
	{
		cout<<"ERROR: failed to invert relative xform\n";
		return false;
	}

	if(testMode)
		cout<<"Inverse xforms computed\n";

	if(testMode)
	{
		writeOligomerFromInverse(string("oligomer_from_inverse.pdb"));
	}


	relXformComputed = computeRelativeTransformation();

	if(!relXformComputed)
	{
		cout<<"ERROR: failed to compute relative xform\n";
		return false;
	}

	if(testMode)
		cout<<"relative xform computed\n";

	if(testMode)
	{
		writeOligomer(string("oligomer.pdb"));
	}


	alignToCoordComputed = computeAlignToCoord();

	if(!alignToCoordComputed)
	{
		cout<<"ERROR: failed to align to coord\n";
		return false;
	}

	if(testMode)
		cout<<"Alignment xform computed\n";

	return true;
}




void SymmetricPose::computeCentroid(double &x, double &y, double &z)
{
	if(testMode)
		cout<<"Computing centroid\n";

	x = 0.0;
	y = 0.0;
	z = 0.0;

	int numAtoms = anchorProtAtoms.size();

	for(int j=0; j<numAtoms; j++)
	{
		x += anchorProtAtoms[j]->getX();
		y += anchorProtAtoms[j]->getY();
		z += anchorProtAtoms[j]->getZ();
	}

	x /= (double)numAtoms;
	y /= (double)numAtoms;
	z /= (double)numAtoms;

	if(testMode)
		cout<<"centroid computed "<< x << " " << y << " " << z << "\n";
}




void SymmetricPose::computeCentroidForCopy(double &x, double &y, double &z)
{
	if(testMode)
		cout<<"Computing centroid for copy\n";

	x = 0.0;
	y = 0.0;
	z = 0.0;

	int numAtoms = copyProtAtoms.size();

	for(int j=0; j<numAtoms; j++)
	{
		x += copyProtAtoms[j]->getX();
		y += copyProtAtoms[j]->getY();
		z += copyProtAtoms[j]->getZ();
	}

	x /= (double)numAtoms;
	y /= (double)numAtoms;
	z /= (double)numAtoms;

	if(testMode)
		cout<<"copy centroid computed"<< x << " " << y << " " << z << "\n";
}




void SymmetricPose::computeGivenOligomerCentroid(double &x, double &y, double &z)
{
	if(testMode)
		cout<<"Computing centroid for given oligomer\n";

	x = 0.0;
	y = 0.0;
	z = 0.0;

	int numAtoms = givenOligomer.size();

	if(testMode)
		cout<<"Number of atoms "<<numAtoms<<"\n";

	for(int j=0; j<numAtoms; j++)
	{
		x += givenOligomer[j]->getX();
		y += givenOligomer[j]->getY();
		z += givenOligomer[j]->getZ();
	}

	x /= (double)numAtoms;
	y /= (double)numAtoms;
	z /= (double)numAtoms;

	if(testMode)
		cout<<"given oligomer centroid computed"<< x << " " << y << " " << z << "\n";
}




void SymmetricPose::computeGivenOligomerCentroid(double &x, double &y, double &z, int index)
{
	if(testMode)
		cout<<"Computing centroid for given oligomer chain \n";

	x = 0.0;
	y = 0.0;
	z = 0.0;

	int numAtoms = givenOligomer.size();

	int numAtomsInChain = numAtoms/symmetryOrder;

	int start = numAtomsInChain*index;

	int end =  start + numAtomsInChain;

	for(int j=start; j<end; j++)
	{
		x += givenOligomer[j]->getX();
		y += givenOligomer[j]->getY();
		z += givenOligomer[j]->getZ();
	}

	x /= (double)numAtoms;
	y /= (double)numAtoms;
	z /= (double)numAtoms;

	if(testMode)
		cout<<"given oligomer centroid computed"<< x << " " << y << " " << z << "\n";
}




void SymmetricPose::computeOligomerCentroidUsingGivenXform(double &x, double &y, double &z)
{
	if(testMode)
		cout<<"Computing oligomer centroid using given rel xform\n";

	x = 0.0;
	y = 0.0;
	z = 0.0;

	int numAtoms = givenOligomer.size();

	for(int j=0; j<numAtoms; j++)
	{
		x += givenOligomer[j]->getX();
		y += givenOligomer[j]->getY();
		z += givenOligomer[j]->getZ();
	}

	x /= (double)numAtoms;
	y /= (double)numAtoms;
	z /= (double)numAtoms;

	MyVector vec(x, y, z);

	for(int k=0; k<symmetryOrder-1; k++ )
	{
		vec = givenXform*vec;


		if(testMode)
		{
			vec.print();
		}

		x += vec[0];
		y += vec[1];
		z += vec[2];
	}

	x /= (double) symmetryOrder;
	y /= (double) symmetryOrder;
	z /= (double) symmetryOrder;


	if(testMode)
		cout<<"centroid computed "<< x << " " << y << " " << z << "\n";
}




void SymmetricPose::computeOligomerCentroid(double &x, double &y, double &z)
{
	if(testMode)
		cout<<"Computing oligomer centroid\n";

	MyVector vec(0.0, 0.0, 0.0);	// already anchored, so centroid is 0,0,0
	x = 0.0;
	y = 0.0;
	z = 0.0;
	
	for(int k=0; k<symmetryOrder-1; k++ )
	{
		vec = relativeTransformation*vec;

		if(testMode)
		{
			vec.print();
		}

		x += vec[0];
		y += vec[1];
		z += vec[2];
	}

	x /= (double) symmetryOrder;
	y /= (double) symmetryOrder;
	z /= (double) symmetryOrder;

	if(testMode)
		cout<<"oligomer centroid computed "<< x << " " << y << " " << z << "\n";
}




bool SymmetricPose::computeLeastSquaresFit(double centroidX, double centroidY, double centroidZ, MyVector& PC1, MyVector& PC2, MyVector& PC3)
{
	if(testMode)
		cout<<"Computing LSQ fitting\n";

	double a00 = 0.0, a01 = 0.0, a02 = 0.0, a10 = 0.0, a11 = 0.0, a12 = 0.0, a20 = 0.0, a21 = 0.0, a22 = 0.0;

	int numAtoms = anchorProtAtoms.size();
	double xc, yc, zc;

	for(int i=0; i<numAtoms; i++)
	{
		xc = anchorProtAtoms[i]->getX() - centroidX;
		yc = anchorProtAtoms[i]->getY() - centroidY;
		zc = anchorProtAtoms[i]->getZ() - centroidZ;

		a00 += xc*xc;
		a01 += xc*yc;
		a02 += xc*zc;
		a11 += yc*yc;		
		a12 += yc*zc;
		a22 += zc*zc;
	}

	a10 = a01;
	a20 = a02;
	a21 = a12;

	double w0, w1, w2;

	if(testMode)
		cout<<"Computed Lsq fit\n";

	return computeEigenValuesAndVectors(a00, a01, a02, a10, a11, a12, a20, a21, a22, w0, w1, w2, PC1, PC2, PC3);
}




bool SymmetricPose::computeEigenValuesAndVectors(double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22, double& w0, double& w1, double& w2, MyVector& PC1, MyVector& PC2, MyVector& PC3)
{
	if(testMode)
		cout<<"Computing eigen values and vectors using LAPACK\n";

	// set up and execute LAPACK's eigenvalue solver

	int n = N, lda = LDA, info, lwork;
	double wkopt;
	double* work;
	double w[N];

	double a[LDA*N] = {	a00, 	0.00, 	0.00,
			   	a10, 	a11, 	0.0,
				a20, 	a21, 	a22 };

	lwork = -1;
	char jobz[] = "Vectors", uplo[] = "Upper";
	dsyev_(jobz, uplo, &n, a, &lda, w, &wkopt, &lwork, &info);
	lwork = (int)wkopt;
	work = (double*)malloc(lwork*sizeof(double));
	dsyev_(jobz, uplo, &n, a, &lda, w, work, &lwork, &info);

	if(testMode)
		cout<<"eigen computation complete\n";

	
	if(info > 0)
	{
		cout << "The algorithm failed to compute eigenvalues.\n" <<endl;
		return false;
	}
	else 
	{   
		// w[0] is the minimal eignevalue, a[0-2] is corresponding unit eigen vector. This is the least squares solution

		cout<<"eigen values "<<w[0]<<" "<<w[1]<<" "<<w[2]<<endl;
		cout<<"eigen vector 0 "<<a[0]<<" "<<a[1]<<" "<<a[2]<<endl;
		cout<<"eigen vector 1 "<<a[3]<<" "<<a[4]<<" "<<a[5]<<endl;
		cout<<"eigen vector 2 "<<a[6]<<" "<<a[7]<<" "<<a[8]<<endl;

		w0 = w[0];
		w1 = w[1];
		w2 = w[2];
		PC1.set(a[0], a[1], a[2]);	
		PC2.set(a[3], a[4], a[5]);	
		PC3.set(a[6], a[7], a[8]);	

		return true;
	}	
}




void SymmetricPose::selectAnchorAtomsAndVectors(double x, double y, double z, MyVector& PC1, MyVector& PC2)
{
	if(testMode)
		cout<<"Computing anchoring coord system\n";

	int atom1, atom2;
	bool atom1selected = false, atom2selected = false;
	double dx, dy, dz, dist;

	int numAtoms = anchorProtAtoms.size();


		// atom 1 is the first atom which is at least 5A away from the centroid

	for(int i=0; i<numAtoms && !atom1selected; i++)	
	{
		dx = x - anchorProtAtoms[i]->getX();
		dy = y - anchorProtAtoms[i]->getY();
		dz = z - anchorProtAtoms[i]->getZ();

		dist = dx*dx + dy*dy + dz*dz;

		if(dist > 25)
		{
			atom1 = i;
			atom1selected = true;
		}
	}

		// atom 2 is the first atom after atom 1 which is at least 5A away from both 
		// the centroid and atom1, hence making a numerically stable choice for a 
		// triangle which is used to the alignment

	double ax = anchorProtAtoms[atom1]->getX();
	double ay = anchorProtAtoms[atom1]->getY();
	double az = anchorProtAtoms[atom1]->getZ();

	if(testMode)
		cout<<"Identified atom 1 at index "<< atom1 << " " << ax << " " << ay << " " << az << " " << "\n";
	
	for(int i=atom1+1; i<numAtoms && !atom2selected; i++)	
	{
		dx = x - anchorProtAtoms[i]->getX();
		dy = y - anchorProtAtoms[i]->getY();
		dz = z - anchorProtAtoms[i]->getZ();

		dist = dx*dx + dy*dy + dz*dz;

		if(dist > 25)
		{
			dx = ax - anchorProtAtoms[i]->getX();
			dy = ay - anchorProtAtoms[i]->getY();
			dz = az - anchorProtAtoms[i]->getZ();

			dist = dx*dx + dy*dy + dz*dz;

			if(dist > 25)
			{
				atom2 = i;
				atom2selected = true;
			}
		}
	}

	if(testMode)
		cout<<"Identified atom 2 at index "<< atom2 << " " << anchorProtAtoms[atom2]->getX() << " " << anchorProtAtoms[atom2]->getY() << " " << anchorProtAtoms[atom2]->getZ() << " " << "\n";

	double vx1 = ax - x;
	double vy1 = ay - y;
	double vz1 = az - z;

	double vx2 = anchorProtAtoms[atom2]->getX() - x;
	double vy2 = anchorProtAtoms[atom2]->getY() - y;
	double vz2 = anchorProtAtoms[atom2]->getZ() - z;

	PC1.set(vx1, vy1, vz1);
	PC2.set(vx2, vy2, vz2);

	if(testMode)
		cout<<"selected coord system\n";
}




void SymmetricPose::selectAnchorAtomsAndVectorsForCopy(double x, double y, double z, MyVector& PC1, MyVector& PC2)
{
	if(testMode)
		cout<<"Computing anchoring coord system for copy\n";

	int atom1, atom2;
	bool atom1selected = false, atom2selected = false;
	double dx, dy, dz, dist;

	int numAtoms = copyProtAtoms.size();


		// atom 1 is the first atom which is at least 5A away from the centroid

	for(int i=0; i<numAtoms && !atom1selected; i++)	
	{
		dx = x - copyProtAtoms[i]->getX();
		dy = y - copyProtAtoms[i]->getY();
		dz = z - copyProtAtoms[i]->getZ();

		dist = dx*dx + dy*dy + dz*dz;

		if(dist > 25)
		{
			atom1 = i;
			atom1selected = true;
		}
	}

		// atom 2 is the first atom after atom 1 which is at least 5A away from both 
		// the centroid and atom1, hence making a numerically stable choice for a 
		// triangle which is used to the alignment

	double ax = copyProtAtoms[atom1]->getX();
	double ay = copyProtAtoms[atom1]->getY();
	double az = copyProtAtoms[atom1]->getZ();

	if(testMode)
		cout<<"Identified atom 1 at index "<< atom1 << " " << ax << " " << ay << " " << az << " " << "\n";
	
	for(int i=atom1+1; i<numAtoms && !atom2selected; i++)	
	{
		dx = x - copyProtAtoms[i]->getX();
		dy = y - copyProtAtoms[i]->getY();
		dz = z - copyProtAtoms[i]->getZ();

		dist = dx*dx + dy*dy + dz*dz;

		if(dist > 25)
		{
			dx = ax - copyProtAtoms[i]->getX();
			dy = ay - copyProtAtoms[i]->getY();
			dz = az - copyProtAtoms[i]->getZ();

			dist = dx*dx + dy*dy + dz*dz;

			if(dist > 25)
			{
				atom2 = i;
				atom2selected = true;
			}
		}
	}

	if(testMode)
		cout<<"Identified atom 2 at index "<< atom2 << " " << anchorProtAtoms[atom2]->getX() << " " << anchorProtAtoms[atom2]->getY() << " " << anchorProtAtoms[atom2]->getZ() << " " << "\n";

	double vx1 = ax - x;
	double vy1 = ay - y;
	double vz1 = az - z;

	double vx2 = copyProtAtoms[atom2]->getX() - x;
	double vy2 = copyProtAtoms[atom2]->getY() - y;
	double vz2 = copyProtAtoms[atom2]->getZ() - z;


	PC1.set(vx1, vy1, vz1);
	PC2.set(vx2, vy2, vz2);

	if(testMode)
		cout<<"selected coord system\n";
}




void SymmetricPose::anchorPDB()
{
	if(testMode)
		cout<<"Anchoring pdb\n";

	int numAtoms = anchorProtAtoms.size();

	for(int j=0; j<numAtoms; j++)
	{
		MyVector vec(anchorProtAtoms[j]->getX(), anchorProtAtoms[j]->getY(), anchorProtAtoms[j]->getZ());

		MyVector newVec = makeAnchor*vec;

		anchorProtAtoms[j]->setPosition(newVec[0], newVec[1], newVec[2]);
	}

	if(testMode)
		cout<<"original pdb anchored\n";


	numAtoms = copyProtAtoms.size();

	for(int j=0; j<numAtoms; j++)
	{
		MyVector vec(copyProtAtoms[j]->getX(), copyProtAtoms[j]->getY(), copyProtAtoms[j]->getZ());

		MyVector newVec = makeAnchor*vec;

		copyProtAtoms[j]->setPosition(newVec[0], newVec[1], newVec[2]);
	}	

	if(testMode)
		cout<<"copy anchored\n";
}


/**
To anchor a PDB to the coord system. We do the following-
1) Compute the centroid. translate so that the centroid lies on the origin
2) compute the principal components of the entire protein. Align the longest eigenvector to the Z axis and the shortest to the X axis.
	2a. Align the shortest eigenvector first by rotation around X-axis to move it to XZ plane, and then rotate around the Y axis to map it to the Z-axis
	2b. The longest eigenvector should now be lying on the XY plane. Rotate around Z-axis to align it to the X axis
	2c. The other eigenvector should now coincide with the Y axis.
[The alignment procedure takes idea from http://www.fundza.com/mel/axis_to_vector/align_axis_to_vector.html]
**/

bool SymmetricPose::computeMakeAnchor()
{
	if(testMode)
		cout<<"Starting anchoring process\n";

	if(!atomsReady) 
	{
		cout<<"ERROR: atoms are not ready. Cannot anchor.\n";
		return false;
	}


	// computing centroid

	double x, y, z;
	computeCentroid(x, y, z);

	MyMatrix translateToOrigin;
	translateToOrigin.set(0,3,-x);
	translateToOrigin.set(1,3,-y);
	translateToOrigin.set(2,3,-z);

	if(testMode)
	{
		cout<<"Translate computed\n";
		translateToOrigin.print();
	}


	// computing the eigenvectors

	MyVector PC1, PC2, PC3;	
	computeLeastSquaresFit(x, y, z, PC1, PC2, PC3);		// PC1 is the smallest, PC3 is the longest

	if(testMode)
	{
		cout<<"Vectors computed\n";
		PC1.print();
		PC3.print();
	}


	// computing the rotation axis and angles for alignment

	MyMatrix moveToXZ;
	MyMatrix moveToZ;

	double u, v, w;

	u = PC1[0];
	v = PC1[1];
	w = PC1[2];

	double u2v2 = sqrt(u*u+v*v);
	double u2v2w2 = sqrt(u*u+v*v+w*w);

	moveToXZ.set(0,0, u/u2v2);
	moveToXZ.set(0,1, v/u2v2);
	moveToXZ.set(1,0, -v/u2v2);
	moveToXZ.set(1,1, y/u2v2);

	moveToZ.set(0,0, w/u2v2w2);
	moveToZ.set(0,2, -u2v2/u2v2w2);
	moveToZ.set(2,0, u2v2/u2v2w2);
	moveToZ.set(2,2, w/u2v2w2);

	MyMatrix combinedRotate = moveToZ*moveToXZ;

	if(testMode)
	{
		cout<<"Alignment for vector 1 computed\n";
		moveToZ.print();
		moveToXZ.print();
		combinedRotate.print();
	}

	if(testMode)
	{
		MyVector PC1_2 = combinedRotate*PC1;

		PC1_2.print();

		if(fabs(PC1_2[0]) > 0.001 || fabs(PC1_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on Z axis\n";
		}
	}


	// so far we have found how to translate to the origin and align the shortest axis to the z axis. Now we apply this to the longest and then align it to the X-axis

	MyMatrix temp1 = moveToXZ*translateToOrigin;
	MyMatrix temp2 = moveToZ*temp1;
	MyVector PC3_1 = temp2*PC3;
	
	if(testMode)
	{
		PC3.print();
		PC3_1.print();

		if(fabs(PC3_1[2]) > 0.001)
		{
			cout<<"ERROR: should be on the XY plane\n";
		}
	}

	MyMatrix moveToXZ2;	// this would bring it onto the X-axis, as Z is already 0

	u = PC3_1[0];
	v = PC3_1[1];
	w = PC3_1[2];

	u2v2 = sqrt(u*u+v*v);

	moveToXZ2.set(0,0, u/u2v2);
	moveToXZ2.set(0,1, v/u2v2);
	moveToXZ2.set(1,0, -v/u2v2);
	moveToXZ2.set(1,1, u/u2v2);

	if(testMode)
	{
		cout<<"Alignment for vector 2 computed\n";
		moveToXZ2.print();
	}

	if(testMode)
	{
		MyVector PC3_2 = moveToXZ2*PC3_1;

		PC3.print();
		PC3_1.print();
		PC3_2.print();


		if(fabs(PC3_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on XZ plane\n";
		}
	}

	// finally computing the overall transformation

	makeAnchor = moveToXZ2*temp2;

	if(testMode)
	{
		cout<<"Complete xform computed\n";
		makeAnchor.print();
	}

	anchorPDB();
	return true;
}


/**
Same as computeMakeAnchor. However, instead of using PCA to find a unique and uniform placement of the anchor, it uses a specific and repeatable rule for selecting two distinct atoms which are used to define the two vectors. The first is aligned with the z-axis and the second is placed on the XZ plane using a rotation around Z-axis.
**/

bool SymmetricPose::computeMakeAnchorWithoutEigen()
{
	if(testMode)
		cout<<"Starting anchoring process\n";

	if(!atomsReady) 
	{
		cout<<"ERROR: atoms are not ready. Cannot anchor.\n";
		return false;
	}


	// computing centroid

	double x, y, z;
	computeCentroid(x, y, z);

	MyMatrix translateToOrigin;
	translateToOrigin.set(0,3,-x);
	translateToOrigin.set(1,3,-y);
	translateToOrigin.set(2,3,-z);

	if(testMode)
	{
		cout<<"Translate computed\n";
		translateToOrigin.print();
	}


	// computing the vectors using a selection rule

	MyVector PC1, PC3;	
	selectAnchorAtomsAndVectors(x, y, z, PC1, PC3);	

	if(testMode)
	{
		cout<<"Vectors computed\n";
		PC1.print();
		PC3.print();
	}

	// computing the rotation axis and angles for alignment

	MyMatrix moveToXZ;
	MyMatrix moveToZ;

	double u, v, w;

	u = PC1[0];
	v = PC1[1];
	w = PC1[2];

	double u2v2 = sqrt(u*u+v*v);
	double u2v2w2 = sqrt(u*u+v*v+w*w);

	moveToXZ.set(0,0, u/u2v2);
	moveToXZ.set(0,1, v/u2v2);
	moveToXZ.set(1,0, -v/u2v2);
	moveToXZ.set(1,1, u/u2v2);

	moveToZ.set(0,0, w/u2v2w2);
	moveToZ.set(0,2, -u2v2/u2v2w2);
	moveToZ.set(2,0, u2v2/u2v2w2);
	moveToZ.set(2,2, w/u2v2w2);

	MyMatrix combinedRotate = moveToZ*moveToXZ;

	if(testMode)
	{
		cout<<"Alignment for vector 1 computed\n";
		moveToZ.print();
		moveToXZ.print();
		combinedRotate.print();
	}

	if(testMode)
	{
		MyVector PC1_2 = combinedRotate*PC1;

		PC1_2.print();

		if(fabs(PC1_2[0]) > 0.001 || fabs(PC1_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on Z axis\n";
		}
	}

	// so far we have found how to translate to the origin and align the first vector to the z axis. Now we apply this to the other and then map it onto the XZ-plane

	MyMatrix temp1 = moveToXZ*translateToOrigin;
	MyMatrix temp2 = moveToZ*temp1;

	if(testMode)
	{
		MyVector PC2(x,y,z);
		MyVector PC2_1 = temp2*PC2;
		cout<<"Centroid moved to ";
		PC2_1.print();

		if(fabs(PC2_1[0]) > 0.001 || fabs(PC2_1[1]) > 0.001 || fabs(PC2_1[2]) > 0.001)
		{
			cout<<"ERROR: Centroid should be at the origin\n";
		}
	}

	MyVector PC3_1 = combinedRotate*PC3;

	if(testMode)
	{
		cout<<"Second vector transformed\n";
		PC3.print();
		PC3_1.print();
	}

	MyMatrix moveToXZ2;

	u = PC3_1[0];
	v = PC3_1[1];
	w = PC3_1[2];

	u2v2 = sqrt(u*u+v*v);

	moveToXZ2.set(0,0, u/u2v2);
	moveToXZ2.set(0,1, v/u2v2);
	moveToXZ2.set(1,0, -v/u2v2);
	moveToXZ2.set(1,1, u/u2v2);

	if(testMode)
	{
		cout<<"Alignment for vector 2 computed\n";
		moveToXZ2.print();
	}

	if(testMode)
	{
		MyVector PC3_2 = moveToXZ2*PC3_1;

		PC3.print();
		PC3_1.print();
		PC3_2.print();


		if(fabs(PC3_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on XZ plane\n";
		}
	}

	// finally computing the overall transformation

	makeAnchor = moveToXZ2*temp2;

	if(testMode)
	{
		cout<<"Complete xform computed\n";
		makeAnchor.print();
	}

	anchorPDB();
	return true;	
}




/**
Since we actually compute the inverse first, the relative transform is computed as inverse of the inverse.

If an affine transformation matrix is 
| A  b |
| 0  1 |
where, A is the rotation matrix and b is the translation vector

Then the inverse is-
| B -B(b) | 
| 0   1   |

where B = A^(-1) = A^T
**/

bool SymmetricPose::computeRelativeTransformation()
{
	if(testMode)
		cout<<"Computing relative xform\n";


	if(!(atomsReady && anchored && inverseRelXformComputed))
	{
		cout<<"Cannot compute relative xform\n";
		return false;
	}

	MyMatrix B;

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			B.set(i,j, inverseRelativeTransformation.get(j,i));

	MyVector b(inverseRelativeTransformation.get(0,3), inverseRelativeTransformation.get(1,3), inverseRelativeTransformation.get(2,3));

	MyVector Bb = B*b;

	relativeTransformation = B;
	relativeTransformation.set(0,3, -Bb[0]);
	relativeTransformation.set(1,3, -Bb[1]);
	relativeTransformation.set(2,3, -Bb[2]);

	if(testMode)
	{
		cout<<"relative xform computed\n";
		relativeTransformation.print();
	}

	return true;
}




/**
Computing the inverse relative transformation is the same as computing the transformation that aligns the copy with the anchor.
**/

bool SymmetricPose::computeInverseRelativeTransformation()
{
	if(testMode)
		cout<<"Computing inverse relative xform\n";

	if(!(atomsReady && anchored))
	{
		cout<<"ERROR: atoms are not ready or anchored. Cannot compute relative xform.\n";
		return false;
	}


	// computing centroid

	double x, y, z;
	computeCentroidForCopy(x, y, z);

	MyMatrix translateToOrigin;
	translateToOrigin.set(0,3,-x);
	translateToOrigin.set(1,3,-y);
	translateToOrigin.set(2,3,-z);

	if(testMode)
	{
		cout<<"Translate computed\n";
		translateToOrigin.print();
	}

	// computing the vectors using a selection rule

	MyVector PC1, PC3;	
	selectAnchorAtomsAndVectorsForCopy(x, y, z, PC1, PC3);	

	if(testMode)
	{
		cout<<"Vectors computed\n";
		PC1.print();
		PC3.print();
	}

	// computing the rotation axis and angles for alignment

	MyMatrix moveToXZ;
	MyMatrix moveToZ;

	double u, v, w;

	u = PC1[0];
	v = PC1[1];
	w = PC1[2];

	double u2v2 = sqrt(u*u+v*v);
	double u2v2w2 = sqrt(u*u+v*v+w*w);

	moveToXZ.set(0,0, u/u2v2);
	moveToXZ.set(0,1, v/u2v2);
	moveToXZ.set(1,0, -v/u2v2);
	moveToXZ.set(1,1, u/u2v2);

	moveToZ.set(0,0, w/u2v2w2);
	moveToZ.set(0,2, -u2v2/u2v2w2);
	moveToZ.set(2,0, u2v2/u2v2w2);
	moveToZ.set(2,2, w/u2v2w2);

	MyMatrix combinedRotate = moveToZ*moveToXZ;

	if(testMode)
	{
		cout<<"Alignment for vector 1 computed\n";
		moveToZ.print();
		moveToXZ.print();
		combinedRotate.print();
	}

	if(testMode)
	{
		MyVector PC1_2 = combinedRotate*PC1;

		PC1_2.print();

		if(fabs(PC1_2[0]) > 0.001 || fabs(PC1_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on Z axis\n";
		}
	}


		// so far we have found how to translate to the origin and align the first vector to the z axis. Now we apply this to the other and then map it onto the XZ-plane

	MyMatrix temp1 = moveToXZ*translateToOrigin;
	MyMatrix temp2 = moveToZ*temp1;

	if(testMode)
	{
		MyVector PC2(x,y,z);
		MyVector PC2_1 = temp2*PC2;
		cout<<"Centroid moved to ";
		PC2_1.print();

		if(fabs(PC2_1[0]) > 0.001 || fabs(PC2_1[1]) > 0.001 || fabs(PC2_1[2]) > 0.001)
		{
			cout<<"ERROR: Centroid should be at the origin\n";
		}
	}

	MyVector PC3_1 = combinedRotate*PC3;

	if(testMode)
	{
		cout<<"Second vector transformed\n";
		PC3.print();
		PC3_1.print();
	}

	MyMatrix moveToXZ2;

	u = PC3_1[0];
	v = PC3_1[1];
	w = PC3_1[2];

	u2v2 = sqrt(u*u+v*v);

	moveToXZ2.set(0,0, u/u2v2);
	moveToXZ2.set(0,1, v/u2v2);
	moveToXZ2.set(1,0, -v/u2v2);
	moveToXZ2.set(1,1, u/u2v2);

	if(testMode)
	{
		cout<<"Alignment for vector 2 computed\n";
		moveToXZ2.print();
	}

	if(testMode)
	{
		MyVector PC3_2 = moveToXZ2*PC3_1;

		PC3.print();
		PC3_1.print();
		PC3_2.print();


		if(fabs(PC3_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on XZ plane\n";
		}
	}

	// finally computing the overall transformation

	inverseRelativeTransformation = moveToXZ2*temp2;

	if(testMode)
	{
		cout<<"Inverse relative xform computed\n";
		inverseRelativeTransformation.print();
	}


	return true;
}




/**
To align the oligomer with the coord system we must place the center of the rotation at the origin and align the axis of rotation to the z-axis. Additionally we require that the centroid of one of the chains falls on the x-axis

1) Approximate the center of rotation as the average of the centroids. (works if the oligomer is perfectly symmetric)

2) Compute the axis as the eigenvector corresponding to the real eigenvalue of the rotation matrix (from the relative xformation matrix).
	-We can compute the eigen vector quite easily for rotation matrices
		- Let A is the rotation matrix
		- Then B = A - A^T is a skew-symmetric matrix
		- The eigen vector is then defined as B21, B02, B10
		- see reference (http://math.stackexchange.com/questions/155078/closed-form-for-eigenvectors-of-rotation-matrix)
		- verified the computation using a general purpose eigen solver server available at 
			(http://www.arndt-bruenner.de/mathe/scripts/engl_eigenwert2.htm)

3) Once the axis and center is found, we perform the same type of transformations we performed for the anchoring exercise to align these.

4) Now the centroids of all the copies should lie on the XY plane. We now apply another rotation around Z axis
**/


bool SymmetricPose::computeAlignToCoord()
{
	if(testMode)
		cout<<"Computing alignment xforms\n";


	if(!(atomsReady && anchored && relXformComputed))
	{
		cout<<"Cannot align oligomer to coords\n";
		return false;
	}

	// computing the center

	double x, y, z;
	if(oligomerGiven)
		computeGivenOligomerCentroid(x,y,z);
	else if(relXformGiven)
		computeOligomerCentroidUsingGivenXform(x,y,z);
	else	
		computeOligomerCentroid(x,y,z);

	MyMatrix translateToOrigin;
	translateToOrigin.set(0,3,-x);
	translateToOrigin.set(1,3,-y);
	translateToOrigin.set(2,3,-z);

	if(testMode)
	{
		cout<<"Computed translation\n";
		translateToOrigin.print();
	}

	// computing the rotation axis

	MyVector PC1;
	double x1, x2, y1, y2, z1, z2;

	if(oligomerGiven)
	{
		computeGivenOligomerCentroid(x1,y1,z1, 0);
		computeGivenOligomerCentroid(x2,y2,z2, 1);

		MyVector V1(x1-x, y1-y, z1-z);
		MyVector V2(x2-x, y2-y, z2-z);

		PC1 = V1.cross(V2);
	}

	else if(relXformGiven)
	{
		double ev0 = givenXform.get(2,1) - givenXform.get(1,2);
		double ev1 = givenXform.get(0,2) - givenXform.get(2,0);
		double ev2 = givenXform.get(1,0) - givenXform.get(0,1);
		PC1.set(ev0, ev1, ev2);
	}

	else
	{
		double ev0 = relativeTransformation.get(2,1) - relativeTransformation.get(1,2);
		double ev1 = relativeTransformation.get(0,2) - relativeTransformation.get(2,0);
		double ev2 = relativeTransformation.get(1,0) - relativeTransformation.get(0,1);
		PC1.set(ev0, ev1, ev2);
	}

	if(testMode)
	{
		cout<<"Computed first vector\n";
		PC1.print();
	}

	// aligning the rotation axis to the z -axis

	MyMatrix moveToXZ;
	MyMatrix moveToZ;

	double u, v, w;

	u = PC1[0];
	v = PC1[1];
	w = PC1[2];

	double u2v2 = sqrt(u*u+v*v);
	double u2v2w2 = sqrt(u*u+v*v+w*w);

	moveToXZ.set(0,0, u/u2v2);
	moveToXZ.set(0,1, v/u2v2);
	moveToXZ.set(1,0, -v/u2v2);
	moveToXZ.set(1,1, u/u2v2);

	moveToZ.set(0,0, w/u2v2w2);
	moveToZ.set(0,2, -u2v2/u2v2w2);
	moveToZ.set(2,0, u2v2/u2v2w2);
	moveToZ.set(2,2, w/u2v2w2);

	MyMatrix combinedRotate = moveToZ*moveToXZ;

	if(testMode)
	{
		cout<<"Alignment for vector 1 computed\n";
		moveToZ.print();
		moveToXZ.print();
		combinedRotate.print();
	}

	if(testMode)
	{
		MyVector PC1_2 = combinedRotate*PC1;

		PC1_2.print();

		if(fabs(PC1_2[0]) > 0.001 || fabs(PC1_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on Z axis\n";
		}
	}	

	// putting the anchor prot's centroid onto the x-axis

	MyMatrix temp1 = moveToXZ*translateToOrigin;
	MyMatrix temp2 = moveToZ*temp1;

	if(testMode)
	{
		MyVector PC2(x,y,z);
		MyVector PC2_1 = temp2*PC2;
		cout<<"Oligomer centroid moved to ";
		PC2_1.print();

		if(fabs(PC2_1[0]) > 0.001 || fabs(PC2_1[1]) > 0.001 || fabs(PC2_1[2]) > 0.001)
		{
			cout<<"ERROR: oligomer centroid should be at the origin\n";
		}
	}


	
	MyVector PC3;

	if(oligomerGiven)
		PC3.set(x1,y1,z1);
	else
	{
		computeCentroid(x,y,z);
		PC3.set(x,y,z);
	}

	MyVector PC3_1 = temp2*PC3;
	
	if(fabs(PC3_1[2]) > 0.001)
	{
		cout<<"ERROR: protein centroid should be on the XY plane\n";
	}

	if(testMode)
	{
		cout<<"Centroid moved to ";
		PC3_1.print();
	}


	MyMatrix moveToXZ2;	// this would bring it onto the X-axis, as Z is already 0

	u = PC3_1[0];
	v = PC3_1[1];
	w = PC3_1[2];

	u2v2 = sqrt(u*u+v*v);

	moveToXZ2.set(0,0, u/u2v2);
	moveToXZ2.set(0,1, v/u2v2);
	moveToXZ2.set(1,0, -v/u2v2);
	moveToXZ2.set(1,1, u/u2v2);

	if(testMode)
	{
		cout<<"Alignment for vector 2 computed\n";
		moveToXZ2.print();
	}

	MyVector PC3_2 = moveToXZ2*PC3_1;

	distanceToCentroid = PC3_2[0];

	if(testMode)
	{
		PC3.print();
		PC3_1.print();
		PC3_2.print();


		if(fabs(PC3_2[1]) > 0.001)
		{
			cout<<"ERROR:: should be on XZ plane\n";
		}
	}

	// finally computing the overall transformation

	alignToCoord = moveToXZ2*temp2;
	alignToCoord = temp2;

	if(testMode)
	{
		cout<<"Computed all xforms\n";
		moveToXZ2.print();
		alignToCoord.print();

	}

	return true;
}




bool SymmetricPose::setupFilters()
{
	if(testMode || 1)
		cout<<"Setting up filters\n";

	if(!(atomsReady && anchored))
	{
		cout<<"Cannot setup filter. proteins are not anchored yet\n";
		return false;
	}


	int numStaticAtoms = anchorProtAtoms.size()*symmetryOrder;	// entire oligomer
	int numMovingAtoms = anchorProtAtoms.size();

	if(testMode || 1)
		cout<<"Allocating memory\n";

	double *staticAtoms = ( double * ) malloc( 5 * numStaticAtoms * sizeof( double ) );
	double *movingAtoms = ( double * ) malloc( 5 * numMovingAtoms * sizeof( double ) );

	if ( ( staticAtoms == NULL ) || ( movingAtoms == NULL ) ) return false;

	printf( "\nnumStaticAtoms = %d , numMovingAtoms = %d \n", numStaticAtoms, numMovingAtoms );

	MyMatrix xform;

	if(testMode || 1)
		cout<<"Copying static atoms\n";

	int j = 0;

	for(int k = 0; k < symmetryOrder; k++)
	{	
		for ( int i = 0; i < anchorProtAtoms.size(); i++ )
		{
			MyVector vec(anchorProtAtoms[i]->getX(), anchorProtAtoms[i]->getY(), anchorProtAtoms[i]->getZ());

			vec = xform*vec;

			staticAtoms[ j++ ] = vec[0];
			staticAtoms[ j++ ] = vec[1];
			staticAtoms[ j++ ] = vec[2];
			staticAtoms[ j++ ] = anchorProtAtoms[i]->getCharge();
			staticAtoms[ j++ ] = anchorProtAtoms[i]->getRadius();
		}

		xform = relativeTransformation*xform;
	}

	if(testMode || 1)
		cout<<"Copying moving atoms\n";

	j = 0;

	for ( int i = 0; i < numMovingAtoms; i++ )
	{
		movingAtoms[ j++ ] = anchorProtAtoms[i]->getX();
		movingAtoms[ j++ ] = anchorProtAtoms[i]->getY();
		movingAtoms[ j++ ] = anchorProtAtoms[i]->getZ();
		movingAtoms[ j++ ] = anchorProtAtoms[i]->getCharge();
		movingAtoms[ j++ ] = anchorProtAtoms[i]->getRadius();
	}

	if(testMode || 1)
		cout<<"Creating filter\n";


	cFilter = new clashFilter( numStaticAtoms, staticAtoms, numMovingAtoms, movingAtoms, false );

	cFilterReady = true;

	if(testMode || 1)
		cout<<"Filter ready. cleaning up allocation\n";

	free( staticAtoms );
	free( movingAtoms );

	if(testMode || 1)
		cout<<"Memory freed\n";

	return true;
}




bool SymmetricPose::computeOligomerFromInverse(vector<PDBParser::Atom*>& oligomer, int firstChainNo, bool includeAnchor)
{
	if(testMode)
		cout<<"Computing oligomers coords\n";


	if(!(atomsReady && anchored && inverseRelXformComputed))
	{
		cout<<"Cannot prepare oligomer.\n";
		return false;
	}

	int numAtoms = copyProtAtoms.size();

	MyMatrix compoundXform;	// initialized to identity
	
	for(int k=0; k<symmetryOrder; k++ )
	{
		if(k==0)
		{
			if(!includeAnchor)
			{
				compoundXform = inverseRelativeTransformation*compoundXform;
				continue;
			}
		}

		MyMatrix xform = compoundXform;

		for(int j=0; j<numAtoms; j++)
		{
			MyVector vec(copyProtAtoms[j]->getX(), copyProtAtoms[j]->getY(), copyProtAtoms[j]->getZ());

			MyVector newVec = xform*vec;

			PDBParser::Atom* atm = new PDBParser::Atom(*copyProtAtoms[j]);

			atm->setPosition(newVec[0], newVec[1], newVec[2]);

			atm->setChain((char)(65+k+firstChainNo));

			oligomer.push_back(atm);
		}

		compoundXform = inverseRelativeTransformation*compoundXform;

		if(testMode)
			cout<<"One copy computed\n";

	}
	return true;
}




bool SymmetricPose::computeMergedOligomer(vector<PDBParser::Atom*>& mergedpdb, vector<PDBParser::Atom*>& oligomer, int firstChainNo)
{
	if(testMode)
		cout<<"Computing merged oligomers coords\n";


	if(!(atomsReady && relXformComputed))
	{
		cout<<"Cannot prepare oligomer.\n";
		return false;
	}



	int numAtoms = mergedpdb.size();

	MyMatrix compoundXform;	// initialized to identity

	for(int k=0; k<symmetryOrder; k++ )
	{
		MyMatrix xform = compoundXform;

		for(int j=0; j<numAtoms; j++)
		{
			MyVector vec(mergedpdb[j]->getX(), mergedpdb[j]->getY(), mergedpdb[j]->getZ());

			MyVector newVec = xform*vec;

			PDBParser::Atom* atm = new PDBParser::Atom(*mergedpdb[j]);

			atm->setPosition(newVec[0], newVec[1], newVec[2]);

			atm->setChain((char)(65+k+firstChainNo));

			oligomer.push_back(atm);
		}

		compoundXform = relativeTransformation*compoundXform;

		if(testMode)
			cout<<"One copy computed\n";
	}

	return true;
}





bool SymmetricPose::computeMergedAndAlignedOligomer(vector<PDBParser::Atom*>& mergedpdb, vector<PDBParser::Atom*>& oligomer, int firstChainNo)
{
	if(testMode)
		cout<<"Computing merged oligomers coords\n";


	if(!(atomsReady && relXformComputed))
	{
		cout<<"Cannot prepare oligomer.\n";
		return false;
	}



	int numAtoms = mergedpdb.size();

	MyMatrix compoundXform;	// initialized to identity

	for(int k=0; k<symmetryOrder; k++ )
	{
		MyMatrix xform = alignToCoord*compoundXform;

		for(int j=0; j<numAtoms; j++)
		{
			MyVector vec(mergedpdb[j]->getX(), mergedpdb[j]->getY(), mergedpdb[j]->getZ());

			MyVector newVec = xform*vec;

			PDBParser::Atom* atm = new PDBParser::Atom(*mergedpdb[j]);

			atm->setPosition(newVec[0], newVec[1], newVec[2]);

			atm->setChain((char)(65+k+firstChainNo));

			oligomer.push_back(atm);
		}

		compoundXform = relativeTransformation*compoundXform;

		if(testMode)
			cout<<"One copy computed\n";
	}

	return true;
}






bool SymmetricPose::computeOligomer(vector<PDBParser::Atom*>& oligomer, int firstChainNo, bool includeAnchor)
{
	if(testMode)
		cout<<"Computing oligomers coords\n";


	if(!(atomsReady && anchored && relXformComputed))
	{
		cout<<"Cannot prepare oligomer.\n";
		return false;
	}



	if(relXformGiven)
	{
		if(testMode)
			cout<<"Using given xform\n";

		int numAtoms = givenOligomer.size();

		MyMatrix compoundXform;	// initialized to identity

		MyVector vec(19.8935, 16.5605, -50.8205);
	
		for(int k=0; k<symmetryOrder; k++ )
		{
			MyVector vec2 = compoundXform*vec;
			vec2.print();

			if(k==0)
			{
				if(!includeAnchor)
				{
					compoundXform = givenXform*compoundXform;
					continue;
				}
			}

			MyMatrix xform = compoundXform;

			for(int j=0; j<numAtoms; j++)
			{
				MyVector vec(givenOligomer[j]->getX(), givenOligomer[j]->getY(), givenOligomer[j]->getZ());

				MyVector newVec = xform*vec;

				PDBParser::Atom* atm = new PDBParser::Atom(*givenOligomer[j]);

				atm->setPosition(newVec[0], newVec[1], newVec[2]);

				atm->setChain((char)(65+k+firstChainNo));

				oligomer.push_back(atm);
			}

			compoundXform = givenXform*compoundXform;

			if(testMode)
				cout<<"One copy computed\n";
		}
	}
	
	else
	{
		int numAtoms = anchorProtAtoms.size();

		MyMatrix compoundXform;	// initialized to identity
	
		for(int k=0; k<symmetryOrder; k++ )
		{
			if(k==0)
			{
				if(!includeAnchor)
				{
					compoundXform = compoundXform*relativeTransformation;
					continue;
				}
			}

			MyMatrix xform = compoundXform;

			for(int j=0; j<numAtoms; j++)
			{
				MyVector vec(anchorProtAtoms[j]->getX(), anchorProtAtoms[j]->getY(), anchorProtAtoms[j]->getZ());

				MyVector newVec = xform*vec;

				PDBParser::Atom* atm = new PDBParser::Atom(*anchorProtAtoms[j]);

				atm->setPosition(newVec[0], newVec[1], newVec[2]);

				atm->setChain((char)(65+k+firstChainNo));

				oligomer.push_back(atm);
			}

			compoundXform = relativeTransformation*compoundXform;

			if(testMode)
				cout<<"One copy computed\n";
		}
	}

	return true;
}




bool SymmetricPose::computeAlignedOligomer(vector<PDBParser::Atom*>& oligomer, int firstChainNo, bool includeAnchor)
{
	if(testMode)
		cout<<"Computing oligomers coords\n";


	if(!(atomsReady && anchored && relXformComputed && alignToCoordComputed))
	{
		cout<<"Cannot prepare oligomer.\n";
		return false;
	}

	
	if(oligomerGiven)
	{
		int numAtoms = givenOligomer.size();

		for(int j=0; j<numAtoms; j++)
		{
			MyVector vec(givenOligomer[j]->getX(), givenOligomer[j]->getY(), givenOligomer[j]->getZ());

			MyVector newVec = alignToCoord*vec;

			PDBParser::Atom* atm = new PDBParser::Atom(*givenOligomer[j]);

			atm->setPosition(newVec[0], newVec[1], newVec[2]);

			oligomer.push_back(atm);
		}
	
	}

	else if(relXformGiven)
	{
		if(testMode)
			cout<<"Using given xform\n";

		int numAtoms = givenOligomer.size();

		MyMatrix compoundXform;	// initialized to identity

		MyVector vec(19.8935, 16.5605, -50.8205);
	
		for(int k=0; k<symmetryOrder; k++ )
		{
			MyVector vec2 = compoundXform*vec;
			vec2.print();

			if(k==0)
			{
				if(!includeAnchor)
				{
					compoundXform = givenXform*compoundXform;
					continue;
				}
			}

			MyMatrix xform = alignToCoord*compoundXform;

			for(int j=0; j<numAtoms; j++)
			{
				MyVector vec(givenOligomer[j]->getX(), givenOligomer[j]->getY(), givenOligomer[j]->getZ());

				MyVector newVec = xform*vec;

				PDBParser::Atom* atm = new PDBParser::Atom(*givenOligomer[j]);

				atm->setPosition(newVec[0], newVec[1], newVec[2]);

				atm->setChain((char)(65+k+firstChainNo));

				oligomer.push_back(atm);
			}

			compoundXform = givenXform*compoundXform;

			if(testMode)
				cout<<"One copy computed\n";
		}
	}

	else
	{
		int numAtoms = anchorProtAtoms.size();

		MyMatrix compoundXform;	// initialized to identity
	
		for(int k=0; k<symmetryOrder; k++ )
		{
			if(k==0)
			{
				if(!includeAnchor)
				{
					compoundXform = compoundXform*relativeTransformation;
					continue;
				}
			}

			MyMatrix xform = alignToCoord*compoundXform;

			for(int j=0; j<numAtoms; j++)
			{
				MyVector vec(anchorProtAtoms[j]->getX(), anchorProtAtoms[j]->getY(), anchorProtAtoms[j]->getZ());

				MyVector newVec = xform*vec;

				PDBParser::Atom* atm = new PDBParser::Atom(*anchorProtAtoms[j]);

				atm->setPosition(newVec[0], newVec[1], newVec[2]);

				atm->setChain((char)(65+k+firstChainNo));

				oligomer.push_back(atm);
			}

			compoundXform = relativeTransformation*compoundXform;

			if(testMode)
				cout<<"One copy computed\n";

		}
	}

	return true;
}




/** 
1. First we prepare the aligned oligomer. The symmtry axis for the oligomer should be the z-axis. Secondary axis of one component should be aligned to the x-axis.
2. Then, we project all the points onto the XY plane (just remove the z-coord). 
3. Let radius be defined as the distnace from the origin to one corner of a symmetric p-gon where p is the order of symmtry.
4. We compute 3 versions on radii
	a. maxRad = A crude approximate is when the area of the p-gon equals 1.5 times the area of the bbox
	b. minRad = approximated as the distance to a centroid from the origin

* Area of p-gon = 0.25 * p * rad^2 * sin (2*M_PI/p)
* Note that we do not attempt to rotate the p-gon. we assume that the corners always lie on the axes going from the origin through the centroids of each of the objects.
**/


bool SymmetricPose::computeRads()
{
	if(testMode)
		cout<<"Computing oligomers coords\n";


	if(!(atomsReady && anchored && relXformComputed && alignToCoordComputed))
	{
		cout<<"Cannot prepare oligomer.\n";
		return false;
	}

	vector<PDBParser::Atom*> oligomer;

	computeAlignedOligomer(oligomer, 0, true);	

	// computing bounding box

	double xmin = 0, ymin = 0, xmax = 0, ymax = 0;
	double x, y;

	int numAtoms = oligomer.size();

	for(int i=0; i<numAtoms; i++)
	{
		x = oligomer[i]->getX(); 
		if(x < xmin) xmin = x;
		else if(x > xmax) xmax = x;

		y = oligomer[i]->getY(); 
		if(y < ymin) ymin = y;
		else if(y > ymax) ymax = y;
	}

	int p = symmetryOrder;
	int angle = 2*M_PI/(double)p;
	double pgonArea = 1.5 * (xmax - xmin) * (ymax - ymin);

	maxRad = sqrt((pgonArea*4.0)/(p*angle));

	minRad = distanceToCentroid;	

	return true;
}




void SymmetricPose::init()
{
	if(testMode)
		cout<<"Initializing\n";

	atomsReady = false;
	anchored = false;
	relXformComputed = false;
	inverseRelXformComputed = false;
	alignToCoordComputed = false;

	cFilterReady = false;
	ljFilterReady = false;
	ipFilterReady = false;

	if(testMode)
		cout<<"Initialization complete\n";

}




void SymmetricPose::destroyFilters()
{
	if(cFilter) delete cFilter;
}




void SymmetricPose::cleanup()
{
	if(testMode)
		cout<<"cleaning up\n";

	anchorProtAtoms.clear();
	copyProtAtoms.clear();
	givenOligomer.clear();

	if(testMode)
		cout<<"Completed clean up\n";
}

