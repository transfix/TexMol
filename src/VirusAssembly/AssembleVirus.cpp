
#include <VirusAssembly/AssembleVirus.h>


void Layout::readLayoutFile(string layoutFileName)
{
	double xlationScale;
	double cx,cy,cz;
	double dx,dy,dz;
	int o, o2;

	FILE* fp = fopen(layoutFileName.c_str(), "rt");

	fscanf(fp, "%d %d", &numAxes, &numNeighbors);

	for(int i=0; i< numAxes; i++)
	{
		fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %d %d", &cx, &cy, &cz, &dx, &dy, &dz, &xlationScale, &o, &o2);
		SymmetryAxis* axis = new SymmetryAxis(o, o2, xlationScale, cx, cy, cz, dx, dy, dz);
		axes.push_back(axis);
	}

	int n1, n2;
	for(int i=0; i< numNeighbors; i++)
	{
		fscanf(fp, "%d %d", &n1, &n2);
		neighbor1.push_back(n1);
		neighbor2.push_back(n2);
	}
}




bool AssembleVirus::formCapsidUnit(vector<PDBParser::Atom*>& capsid, double scale, double rotZ, int poseIndex)
{
	if(testMode)
		cout<<"Forming 1 unit. At scale: " << scale << "  and rotation: "<< rotZ << "  based on pose: "<< poseIndex <<"\n";


	// computing the xform

	MyMatrix translate;
	translate.set(2,3,scale);

	MyMatrix rotate = MyMatrix::rotationZ(rotZ);

	MyMatrix xform = rotate;


	// checking if the asymmetric unit is also an oligomer

	int orderOther = layout->getAxis(0)->getOtherOrder();
	int otherPoseIndex = -1;


	if(testMode)
		cout<<"Order of symmetry required by the layout for forming asymmetric unit: " << orderOther << "\n";


	for(int i=0 ; i<clique->getCliqueSize(); i++)
	{
		if(clique->getPose(i)->getSymmetryOrder() == orderOther)
		{
			otherPoseIndex = i;
		}
	}

	if(otherPoseIndex == -1 && orderOther != 1)
	{
		cout<<"Could not find any pose in the clique suitable for this asymmetric unit\n";
		return false;
	}


	// getting the structure

	SymmetricPose* sp = clique->getPose(poseIndex);

	vector<PDBParser::Atom*> tempAtomList;

	if(orderOther > 1 && otherPoseIndex != -1)
	{
		SymmetricPose* sp2 = clique->getPose(otherPoseIndex);

		vector<PDBParser::Atom*> asymmetricUnit;

		sp2->computeOligomer(asymmetricUnit, 0, true);

		sp->computeMergedAndAlignedOligomer(asymmetricUnit, tempAtomList, 0);
	}
	else
	{
		if(!sp->computeAlignedOligomer(tempAtomList, 0, true))
			return false;
	}
	

	int numAtoms = tempAtomList.size();

	// applying the xform

	capsid.clear();

	for(int i=0; i<numAtoms; i++)
	{
		MyVector vec(tempAtomList[i]->getX(), tempAtomList[i]->getY(), tempAtomList[i]->getZ());

		MyVector newVec = xform*vec;

		PDBParser::Atom* atm = new PDBParser::Atom(*tempAtomList[i]);

		atm->setPosition(newVec[0], newVec[1], newVec[2]);

		capsid.push_back(atm);		
	}


	return true;
}




bool AssembleVirus::formCapsid(vector<PDBParser::Atom*>& capsid, double scale, double rotZ, int poseIndex)
{
	if(testMode)
		cout<<"Forming capsid\n";	

	int order = clique->getPose(poseIndex)->getSymmetryOrder();


	if(testMode)
		cout<<"Order of symmetry required by the layout: " << order << "\n";	


	// making one unit
	
	vector<PDBParser::Atom*> capsidUnit;

	if(!formCapsidUnit(capsidUnit, scale, rotZ, poseIndex))
	{
		cout<<"Could not form a single unit of the capsid\n";
		return false;
	}


	// replicating

	capsid.clear();

	int numAxes = layout->getNumAxes();
	int numAtoms = capsidUnit.size();

	if(testMode)
		cout<<"Layout describes " << numAxes << " copies \n";	


	for(int i=0 ; i<numAxes; i++)
	{
		if(layout->getAxis(i)->getOrder() != order)
			continue;
 
		MyMatrix xform = layout->getAxis(i)->getXformToAlignTheZAxisToThisAxis(scale);

		for(int j=0; j<numAtoms; j++)
		{
			MyVector vec(capsidUnit[j]->getX(), capsidUnit[j]->getY(), capsidUnit[j]->getZ());

			MyVector newVec = xform*vec;

			PDBParser::Atom* atm = new PDBParser::Atom(*capsidUnit[j]);

			atm->setPosition(newVec[0], newVec[1], newVec[2]);

			capsid.push_back(atm);		
		}	
	}

	if(testMode)
		cout<<"Capsid ready \n";

	return true;
}




bool AssembleVirus::testAxisXform(double scale, double rotZ, int poseIndex)
{
	if(testMode)
		cout<<"Forming capsid\n";	

	int order = clique->getPose(poseIndex)->getSymmetryOrder();


	if(testMode)
		cout<<"Order of symmetry required by the layout: " << order << "\n";	


	// making one unit
	
	vector<PDBParser::Atom*> capsidUnit;

	if(!formCapsidUnit(capsidUnit, scale, rotZ, poseIndex))
	{
		cout<<"Could not form a single unit of the capsid\n";
		return false;
	}


	// moving

	vector<PDBParser::Atom*> capsid;

	int numAtoms = capsidUnit.size();

	for(int i=0 ; i<1; i++)
	{
		if(layout->getAxis(i)->getOrder() != order)
			continue;
 
		MyMatrix xform = layout->getAxis(i)->getXformToAlignTheZAxisToThisAxis();

		for(int j=0; j<numAtoms; j++)
		{
			MyVector vec(capsidUnit[j]->getX(), capsidUnit[j]->getY(), capsidUnit[j]->getZ());

			MyVector newVec = xform*vec;

			PDBParser::Atom* atm = new PDBParser::Atom(*capsidUnit[j]);

			atm->setPosition(newVec[0], newVec[1], newVec[2]);

			capsid.push_back(atm);		
		}	
	}

	if(testMode)
		cout<<"Capsid unit moved \n";

	FILE *fp = fopen("unit1.pdb", "wt");

	if(!fp)
	{
		cout<<"Could not write pdb\n";
		return false;
	}

	for(int i = 0; i < capsid.size(); i++)
		fprintf(fp, "ATOM%7d %-3s %3s %c%4d    %8.3f%8.3f%8.3f%8.4f%7.4f\n", i+1,
				capsid[i]->getName(),
				capsid[i]->getResidueName(),
				capsid[i]->getChain(),
				capsid[i]->getResidueNum(),
				capsid[i]->getX(),
				capsid[i]->getY(),
				capsid[i]->getZ(),
				capsid[i]->getCharge(),
				capsid[i]->getRadius());

	if(fp) fclose(fp);


	// moving back

	vector<PDBParser::Atom*> capsid2;

	numAtoms = capsid.size();

	for(int i=0 ; i<1; i++)
	{
		if(layout->getAxis(i)->getOrder() != order)
			continue;
 
		MyMatrix xform = layout->getAxis(i)->getXformToAlignThisAxisToTheZAxis();

		for(int j=0; j<numAtoms; j++)
		{
			MyVector vec(capsid[j]->getX(), capsid[j]->getY(), capsid[j]->getZ());

			MyVector newVec = xform*vec;

			PDBParser::Atom* atm = new PDBParser::Atom(*capsid[j]);

			atm->setPosition(newVec[0], newVec[1], newVec[2]);

			capsid2.push_back(atm);		
		}	
	}

	if(testMode)
		cout<<"Capsid unit moved back \n";

	fp = fopen("unit2.pdb", "wt");

	if(!fp)
	{
		cout<<"Could not write pdb\n";
		return false;
	}

	for(int i = 0; i < capsid.size(); i++)
		fprintf(fp, "ATOM%7d %-3s %3s %c%4d    %8.3f%8.3f%8.3f%8.4f%7.4f\n", i+1,
				capsid2[i]->getName(),
				capsid2[i]->getResidueName(),
				capsid2[i]->getChain(),
				capsid2[i]->getResidueNum(),
				capsid2[i]->getX(),
				capsid2[i]->getY(),
				capsid2[i]->getZ(),
				capsid2[i]->getCharge(),
				capsid2[i]->getRadius());

	if(fp) fclose(fp);

	
	capsid.clear();
	capsid2.clear();
	capsidUnit.clear();

	return true;
}




bool AssembleVirus::searchScaleAndRotation(vector<double>& scale, vector<double>& rotZ, int& poseToUse)
{
	int numIter = 0;

	if(testMode)
		cout<<"Searching for best params\n";

	// deciding which pose to use

	int order = layout->getAxis(0)->getOrder();

	if(testMode)
		cout<<"Order of symmetry required by the layout: " << order << "\n";

	poseToUse = -1;

	if(testMode)
		cout<<"Size of the clique: " << clique->getCliqueSize() << "\n";

	for(int i=0 ; i<clique->getCliqueSize(); i++)
	{
		if(testMode)
			cout<<"Order of the pose: " << clique->getPose(i)->getSymmetryOrder() << "\n";

		if(clique->getPose(i)->getSymmetryOrder() == order)
		{
			poseToUse = i;
		}
	}

	if(poseToUse == -1)
	{
		cout<<"Could not find any pose in the clique suitable for this layout\n";
		return false;
	}

	if(testMode)
		cout<<"Selected pose: " << poseToUse << "\n";

	double largeScale = clique->getPose(poseToUse)->getMaxRad();
	double smallScale = clique->getPose(poseToUse)->getMinRad();


	int rotMax = 180/order;	// only need to rotate within the symmtry band
	int rotMin = -rotMax;


	
	/* 
		search procedure: [Similar to gradient descent where the gradient is toward a pose with no clash]

			1. Start with a large scale
			2. For each rotational sample
				2.1. Check if one copy clashes with another
				2.2. If it clashes for less than 80% rotations, then decrease the scale and repeat
				2.3. If it clashes for more than 80% rotations, then decrease the scale by a smaller amount and repeat
				2.4. If it clashes for all rotations, then increase the scale by an even smaller amount
				2.5 Stop when the increase/decrease amount is less than 1 and with less than 100% clash
			3. For that particular scale
				3.1. Use a finer rotational sampling to find the best rotation (This time, use more scoring functions)
	*/

	double objectScale = largeScale;
	int clash, severe;
	double score;

	double objectScaleDecrementAmount = 32;
	bool decreaseMode = true;

	bool scaleFound = false;
	bool rotationFound = false;

	if(testMode)
		cout<<"Looking for scale\n";

	while ( ! scaleFound )
	{
		int clashFound = 0, angleTried = 0;

		if(testMode)
			cout<<"Current scale: " << objectScale << "\n";
		if(testMode)
			cout<<"Decreasing mode?: " << decreaseMode << "\n";


		for(int ang = rotMin; ang <= rotMax; ang+= 5)
		{
			angleTried ++;
			numIter++;

			if(testMode)
				cout<<"		Current angle: " << ang << "\n";


			computeClashBetweenCopies(objectScale, (double)ang*M_PI/180.0, poseToUse, clash, severe, score);

			if( clash > 500 || severe > 100 )	// clash. make the thresholds user-defined later.
				clashFound++;
		}

		if(testMode)
			cout<< clashFound << " clashes found in " << angleTried << "attempts\n";		
	
		if (clashFound >= angleTried)
		{
			objectScaleDecrementAmount /= 2.0;
			decreaseMode = false;
		}
		else if (clashFound > angleTried*0.8)
		{
			objectScaleDecrementAmount /= 2.0;
		}
		else
		{
			decreaseMode = true;
		}
		

		if(objectScaleDecrementAmount < 2)
		{
			if(clashFound < angleTried)
			{
				scaleFound = true;
			}
			else
			{
				decreaseMode = false;
				objectScale += objectScaleDecrementAmount;
			}
		}
		else
		{
			if(decreaseMode) objectScale -= objectScaleDecrementAmount;
			else objectScale += objectScaleDecrementAmount;
		}

		if(objectScale > largeScale || objectScale < smallScale)
		{
			cout<<"Error in search"<<endl;
			break;
		}

	}

	if(testMode)
		cout<<"Decided to use scale: " << objectScale << "\n";	

	int angle = rotMin;
	int bestClash = 500;
	int bestSevere = 50;
	
	for(int ang = rotMin; ang <= rotMax; ang+= 1)
	{
		computeClashBetweenCopies(objectScale, (double)ang*M_PI/180.0, poseToUse, clash, severe, score);
		
		if(severe < bestSevere)
		{
			angle = ang;
			bestSevere = severe;
		}

		else if( severe == bestSevere && clash < bestClash)
		{
			angle = ang;
			bestClash = clash;
		}

		numIter++;
	}

	if(testMode)
		cout<<"Decided to use angle: " << angle << "\n";

	scale.push_back(objectScale);
	rotZ.push_back((double)angle*M_PI/180.0);




	objectScale = largeScale;
	objectScaleDecrementAmount = 32;
	decreaseMode = true;

	scaleFound = false;
	rotationFound = false;

	if(testMode)
		cout<<"Looking for scale2\n";

	while ( ! scaleFound )
	{
		int clashFound = 0, angleTried = 0;

		if(testMode)
			cout<<"Current scale: " << objectScale << "\n";
		if(testMode)
			cout<<"Decreasing mode?: " << decreaseMode << "\n";


		for(int ang = rotMin; ang <= rotMax; ang+= 5)
		{
			angleTried ++;
			numIter++;

			if(testMode)
				cout<<"		Current angle: " << ang << "\n";


			computeClashBetweenCopies(-objectScale, (double)ang*M_PI/180.0, poseToUse, clash, severe, score);

			if( clash > 500 || severe > 100)	// clash. make the thresholds user-defined later.
				clashFound++;
		}

		if(testMode)
			cout<< clashFound << " clashes found in " << angleTried << "attempts\n";		
	
		if (clashFound >= angleTried)
		{
			objectScaleDecrementAmount /= 2.0;
			decreaseMode = false;
		}
		else if (clashFound > angleTried*0.8)
		{
			objectScaleDecrementAmount /= 2.0;
			decreaseMode = true;
		}
		else
		{
			decreaseMode = true;
		}
		

		if(objectScaleDecrementAmount < 1)
		{
			if(clashFound < angleTried)
			{
				scaleFound = true;
			}
			else
			{
				decreaseMode = false;
				objectScale += objectScaleDecrementAmount;
			}
		}
		else
		{
			if(decreaseMode) objectScale -= objectScaleDecrementAmount;
			else objectScale += objectScaleDecrementAmount;
		}

		if(objectScale > largeScale || objectScale < smallScale)
		{
			cout<<"Error in search"<<endl;
			break;
		}

	}

	if(testMode)
		cout<<"Decided to use scale: " << objectScale << "\n";	

	angle = rotMin;
	bestClash = 500;
	bestSevere = 50;
	
	for(int ang = rotMin; ang <= rotMax; ang+= 2)
	{
		computeClashBetweenCopies(-objectScale, (double)ang*M_PI/180.0, poseToUse, clash, severe, score);

		if(severe < bestSevere)
		{
			angle = ang;
			bestSevere = severe;
		}

		else if( severe == bestSevere && clash < bestClash)
		{
			angle = ang;
			bestClash = clash;
		}

		numIter++;
	}

	if(testMode)
		cout<<"Decided to use angle: " << angle << "\n";

	scale.push_back(-objectScale);
	rotZ.push_back((double)angle*M_PI/180.0);


//	Use for making videos and for test purposes

/*	
	for(int i = -largeScale; i < largeScale; i+=10)
	{
		if(i > -smallScale && i<smallScale) continue;

		for(int j = rotMin; j < rotMax; j += 10 )
		{
			vector<PDBParser::Atom*> capsid;
			formCapsid(capsid, i, (double)j*M_PI/180.0, poseToUse);

			if(testMode)
			{
				char fileName[100];	

				sprintf(fileName, "test_cap_%d_%d.pdb", i, j);

				FILE *fp = fopen(fileName, "wt");

				if(!fp)
				{
					cout<<"Could not write pdb\n";
					return false;
				}

				for(int i = 0; i < capsid.size(); i++)
					fprintf(fp, "ATOM%7d %-3s %3s %c%4d    %8.3f%8.3f%8.3f%8.4f%7.4f\n", i+1,
							capsid[i]->getName(),
							capsid[i]->getResidueName(),
							capsid[i]->getChain(),
							capsid[i]->getResidueNum(),
							capsid[i]->getX(),
							capsid[i]->getY(),
							capsid[i]->getZ(),
							capsid[i]->getCharge(),
							capsid[i]->getRadius());

				if(fp) fclose(fp);				
			}
		}
	}
*/

	cout<<"Number of iterations "<<numIter<<endl;

	if(testMode)
		cout<<"Parameter selection compete\n";

	return true;
}




bool AssembleVirus::getCapsid(vector< vector<PDBParser::Atom*> >& capsids)
{
	if(testMode)
		cout<<"Starting getCapsid\n";

	vector<double> scale;
	vector<double> rotZ;
	int poseToUse;

	vector<PDBParser::Atom*> capsidUnit;
	formCapsidUnit(capsidUnit, 0, 0, 0);
	capsids.push_back(capsidUnit);


	if(!searchScaleAndRotation(scale, rotZ, poseToUse))
	{
		cout<<"Could not find any suitable capsid\n";
		return false;
	}

/*
//	testAxisXform(scale, rotZ, poseToUse);

	for(int i=0; i<scale.size(); i++)
	{
		vector<PDBParser::Atom*> capsidUnit;
		vector<PDBParser::Atom*> capsidUnitR;
		vector<PDBParser::Atom*> capsidUnit1;
		vector<PDBParser::Atom*> capsidUnit2;	
		vector<PDBParser::Atom*> capsidUnit3;

		formCapsidUnit(capsidUnit, scale[i], rotZ[i], poseToUse);

		formCapsidUnit(capsidUnitR, scale[i], rotZ[i]+0.5, poseToUse);

		MyMatrix xform1 = layout->getAxis(0)->getXformToAlignTheZAxisToThisAxis(scale[i]);
		MyMatrix xform2 = layout->getAxis(1)->getXformToAlignTheZAxisToThisAxis(scale[i]);
		MyMatrix xform3 = layout->getAxis(2)->getXformToAlignTheZAxisToThisAxis(scale[i]);


		for(int j=0; j<capsidUnit.size(); j++)
		{
			MyVector vec(capsidUnit[j]->getX(), capsidUnit[j]->getY(), capsidUnit[j]->getZ());

			MyVector newVec1 = xform1*vec;
			MyVector newVec2 = xform2*vec;
			MyVector newVec3 = xform3*vec;

			PDBParser::Atom* atm1 = new PDBParser::Atom(*capsidUnit[j]);
			PDBParser::Atom* atm2 = new PDBParser::Atom(*capsidUnit[j]);
			PDBParser::Atom* atm3 = new PDBParser::Atom(*capsidUnit[j]);

			atm1->setPosition(newVec1[0], newVec1[1], newVec1[2]);
			atm2->setPosition(newVec2[0], newVec2[1], newVec2[2]);
			atm3->setPosition(newVec3[0], newVec3[1], newVec3[2]);

			capsidUnit1.push_back(atm1);
			capsidUnit2.push_back(atm2);
			capsidUnit3.push_back(atm3);

		
			MyVector vecR(capsidUnitR[j]->getX(), capsidUnitR[j]->getY(), capsidUnitR[j]->getZ());
			MyVector newVecR = xform1*vecR;
			capsidUnitR[j]->setPosition(newVecR[0], newVecR[1], newVecR[2]);
		}

		capsids.push_back(capsidUnit1);
		capsids.push_back(capsidUnit2);
		capsids.push_back(capsidUnit3);
		capsids.push_back(capsidUnitR);
	

		vector<PDBParser::Atom*> capsid;
		formCapsid(capsid, scale[i], rotZ[i], poseToUse);
		capsids.push_back(capsid);


		vector<PDBParser::Atom*> capsid2;
		formCapsid(capsid2, scale[i]*2.0, rotZ[i], poseToUse);
		capsids.push_back(capsid2);

		vector<PDBParser::Atom*> capsid3;
		formCapsid(capsid3, scale[i]*0.5, rotZ[i], poseToUse);
		capsids.push_back(capsid3);

		vector<PDBParser::Atom*> capsid4;
		formCapsid(capsid4, scale[i], rotZ[i]+0.5, poseToUse);
		capsids.push_back(capsid4);
	}
*/
	if(testMode)
		cout<<"getCapsid done\n";
	
	return true;
}



bool AssembleVirus::computeClashBetweenCopies(double scale, double angle, int poseIndex, int& clash, int & severe, double& score)
{

	vector<PDBParser::Atom*> capsidUnit;

	if(!formCapsidUnit(capsidUnit, scale, angle, poseIndex))
	{
		cout<<"Could not form a single unit of the capsid\n";
		return false;
	}

	int numStaticAtoms = capsidUnit.size();
	int numMovingAtoms = capsidUnit.size();		

	if(testMode)
		cout<<"Allocating memory\n";

	double *staticAtoms = ( double * ) malloc( 5 * numStaticAtoms * sizeof( double ) );
	double *movingAtoms = ( double * ) malloc( 5 * numMovingAtoms * sizeof( double ) );

	if ( ( staticAtoms == NULL ) || ( movingAtoms == NULL ) ) return false;

	printf( "\nnumStaticAtoms = %d , numMovingAtoms = %d \n", numStaticAtoms, numMovingAtoms );



	int axis1 = layout->getNeighbor1(0);
	int axis2 = layout->getNeighbor2(0);

	MyMatrix xform1 = layout->getAxis(axis1)->getXformToAlignTheZAxisToThisAxis(scale);
	MyMatrix xform2 = layout->getAxis(axis2)->getXformToAlignTheZAxisToThisAxis(scale);


	int j = 0, k = 0;

	for ( int i = 0; i < numStaticAtoms; i++ )
	{
		MyVector vec(capsidUnit[i]->getX(), capsidUnit[i]->getY(), capsidUnit[i]->getZ());

		MyVector vec2 = xform1*vec;

		staticAtoms[ j++ ] = vec2[0];
		staticAtoms[ j++ ] = vec2[1];
		staticAtoms[ j++ ] = vec2[2];
		staticAtoms[ j++ ] = capsidUnit[i]->getCharge();
		staticAtoms[ j++ ] = capsidUnit[i]->getRadius();


		movingAtoms[ k++ ] = vec2[0];
		movingAtoms[ k++ ] = vec2[1];
		movingAtoms[ k++ ] = vec2[2];
		movingAtoms[ k++ ] = capsidUnit[i]->getCharge();
		movingAtoms[ k++ ] = capsidUnit[i]->getRadius();
	}


	cFilter = new clashFilter( numStaticAtoms, staticAtoms, numMovingAtoms, movingAtoms, false );


	free( staticAtoms );
	free( movingAtoms );

	clash = 0;
	severe = 0;
	score = 0;

	MyMatrix invXform = layout->getAxis(axis1)->getXformToAlignThisAxisToTheZAxis(scale);
	MyMatrix relXform = xform2*invXform;

	CCVOpenGLMath::Matrix mtx(	relXform.get(0,0), relXform.get(0,1), relXform.get(0,2), relXform.get(0,3),
						relXform.get(1,0), relXform.get(1,1), relXform.get(1,2), relXform.get(1,3),
						relXform.get(2,0), relXform.get(2,1), relXform.get(2,2), relXform.get(2,3),
						relXform.get(3,0), relXform.get(3,1), relXform.get(3,2), relXform.get(3,3)
					);

	if(!cFilter->computeInteractions( mtx, &clash, &severe, &score ))
	{
		cout<<"could not compute clash scores\n";
		return false;
	}


	if(testMode)
	{
		cout<<"Clash scores computed for one copy\n";
		cout<<"clash score = [soft, severe, score] = [" << clash << ", " << severe << ", " << score << "]\n" ;
	}

	delete cFilter;
 
	return true;
}

