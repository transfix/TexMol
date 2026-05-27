/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <F2DockClient/dockControl.h>
#include <string.h>
#include <TexMol/compat.h>

DockControl::DockControl()
{

}


DockControl::~DockControl()
{

}


void DockControl::init()
{

}


// get methods

bool DockControl::getApplyClashFilter(void)
{
	return dockPar->getApplyClashFilter();
}

bool DockControl::getApplyVdWFilter(void)
{
	return dockPar->getApplyVdWFilter();
}

bool DockControl::getApplyPseudoGsolFilter(void)
{
	return dockPar->getApplyPseudoGsolFilter();
}

bool DockControl::getApplyDispersionFilter(void)
{
	return dockPar->getApplyDispersionFilter();
}

bool DockControl::getApplyBasicRerank(void)
{
	return dockPar->getApplyBasicRerank();
}

bool DockControl::getBound(void)
{
	return dockPar->getBound();
}

int DockControl::getComplexType(void)
{
	return dockPar->getComplexTypeInt();
}

bool DockControl::getPerformRerank(void)
{
	return basPar->getPerformRerank();
}

string DockControl::getNumSol()
{
	ostringstream out;
	out << dockPar->getNumSol();
	return out.str();
}

string DockControl::getNumRot()
{
	ostringstream out;
	out << dockPar->getNumRot();
	return out.str();
}

string DockControl::getGridSpacing()
{
	ostringstream out;
	out << dockPar->getGridSpacing();
	return out.str();
}

string DockControl::getRotSeparation()
{
	ostringstream out;
	out << dockPar->getRotSeparation();
	return out.str();
}

string DockControl::getPeaksPerRotation()
{
	ostringstream out;
	out << dockPar->getPeaksPerRotation();
	return out.str();
}

string DockControl::getSSWeight()
{
	ostringstream out;
	out << dockPar->getSSWeight();
	return out.str();
}

string DockControl::getSCWeight()
{
	ostringstream out;
	out << dockPar->getSCWeight();
	return out.str();
}

string DockControl::getCCWeight()
{
	ostringstream out;
	out << dockPar->getCCWeight();
	return out.str();
}

string DockControl::getElecWeight()
{
	ostringstream out;
	out << dockPar->getElecWeight();
	return out.str();
}

string DockControl::getHBondWeight()
{
	ostringstream out;
	out << dockPar->getHBondWeight();
	return out.str();
}

string DockControl::getHydrophobicityWeight()
{
	ostringstream out;
	out << dockPar->getHydrophobicityWeight();
	return out.str();
}

string DockControl::getSimpleChargeWeight()
{
	ostringstream out;
	out << dockPar->getSimpleChargeWeight();
	return out.str();
}


//set methods

bool DockControl::reset(int complexType)
{
	return dockPar->reset(complexType);
}

bool DockControl::setJobTitle(string name)
{
	return basPar->setJobTitle(name);
}

bool DockControl::setBound(bool b)
{
	return dockPar->setBound(b);
}

bool DockControl::setComplexType(int n)
{
	return dockPar->reset(n);
}
	
bool DockControl::setPerformRerank(bool b)
{
	return basPar->setPerformRerank(b);
}

bool DockControl::setPeaksPerRotation(int n)
{
	return dockPar->setPeaksPerRotation(n);
}

bool DockControl::setRotSeparation(int sep)
{
	return dockPar->setRotSeparation(sep);
}

bool DockControl::setGridSpacing(double gs)
{
	return dockPar->setGridSpacing(gs);
}

bool DockControl::setSSWeight(double ss)
{
	return dockPar->setSSWeight(ss);
}

bool DockControl::setSCWeight(double sc)
{
	return dockPar->setSCWeight(sc);
}

bool DockControl::setCCWeight(double cc)
{
	return dockPar->setCCWeight(cc);
}

bool DockControl::setElecWeight(double elec)
{
	return dockPar->setElecWeight(elec);
}

bool DockControl::setHBondWeight(double hw)
{
	return dockPar->setHBondWeight(hw);
}

bool DockControl::setHydrophobicityWeight(double hw)
{
	return dockPar->setHydrophobicityWeight(hw);
}

bool DockControl::setSimpleChargeWeight(double sw)
{
	return dockPar->setSimpleChargeWeight(sw);
}

void DockControl::setApplyClashFilter(bool cf)
{
	dockPar->setApplyClashFilter(cf);
}

void DockControl::setApplyVdWFilter(bool vf)
{
	dockPar->setApplyVdWFilter(vf);
}

void DockControl::setApplyPseudoGsolFilter(bool pf)
{
	dockPar->setApplyPseudoGsolFilter(pf);
}

void DockControl::setApplyDispersionFilter(bool df)
{
	dockPar->setApplyDispersionFilter(df);
}

void DockControl::setApplyBasicRerank(bool br)
{
	dockPar->setApplyBasicRerank(br);
}

bool DockControl::setReceptorPDBName(string fileName)
{
	return basPar->setReceptorPDBName(fileName);
}

bool DockControl::setLigandPDBName(string fileName)
{
	return basPar->setLigandPDBName(fileName);
}

bool DockControl::setReceptorPQRName(string fileName)
{
	return basPar->setReceptorPQRName(fileName);
}

bool DockControl::setLigandPQRName(string fileName)
{
	return basPar->setLigandPQRName(fileName);
}

bool DockControl::setReceptorF2dName(string fileName)
{
	return basPar->setReceptorF2dName(fileName);
}

bool DockControl::setLigandF2dName(string fileName)
{
	return basPar->setLigandF2dName(fileName);
}

bool DockControl::setReceptorRAWNName(string fileName)
{
	return basPar->setReceptorRAWNName(fileName);
}

bool DockControl::setLigandRAWNName(string fileName)
{
	return basPar->setLigandRAWNName(fileName);
}

bool DockControl::setReceptorQuadName(string fileName)
{
	return basPar->setReceptorQuadName(fileName);
}

bool DockControl::setLigandQuadName(string fileName)
{
	return basPar->setLigandQuadName(fileName);
}

bool DockControl::setDockingInputName(string fileName)
{
	return basPar->setDockingInputName(fileName, 0);
}

bool DockControl::setRMSDName(string fileName)
{
	return basPar->setRMSDName(fileName, 0);
}

bool DockControl::setNumSol(int n)
{
	return dockPar->setNumSol(n);
}

bool DockControl::setNumRot(int n)
{
	return dockPar->setNumRot(n);
}


void DockControl::setStoreReceptorPQR(){basPar->setStoreReceptorPQR(true);}
void DockControl::setStoreLigandPQR(){basPar->setStoreLigandPQR(true);}
void DockControl::setStoreReceptorF2d(){basPar->setStoreReceptorF2d(true);}
void DockControl::setStoreLigandF2d(){basPar->setStoreLigandF2d(true);}
void DockControl::setStoreReceptorRAWN(){basPar->setStoreReceptorRAWN(true);}
void DockControl::setStoreLigandRAWN(){basPar->setStoreLigandRAWN(true);}
void DockControl::setStoreReceptorQuad(){basPar->setStoreReceptorQuad(true);}
void DockControl::setStoreLigandQuad(){basPar->setStoreLigandQuad(true);}


// job handling

bool DockControl::handleJob()
{

	bool rpdb = basPar->getReceptorPDBAvailable();
	bool rpqr = basPar->getReceptorPQRAvailable();
	bool rraw = basPar->getReceptorRAWNAvailable();
	bool rqua = basPar->getReceptorQuadAvailable();

	bool lpdb = basPar->getLigandPDBAvailable();
	bool lpqr = basPar->getLigandPQRAvailable();
	bool lraw = basPar->getLigandRAWNAvailable();
	bool lqua = basPar->getLigandQuadAvailable();

	if( ! ( rpdb || (rpqr && (rqua||rraw)) )  )
	{
		cout<<"Need either PDB or (PQR + QUAD/RAW) for receptor"<<endl;
		return false;
	}
	if( ! ( lpdb || (lpqr && (lqua||lraw)) )  )
	{
		cout<<"Need either PDB or (PQR + QUAD/RAW) for ligand"<<endl;
		return false;
	}

	if(basPar->getDockingInputAvailable()) return true;

	if(dockPar->prepareInputFile(basPar->getDockingInputFileName()))
	{
		basPar->setDockingInputAvailable(true);
		return true;
	}

	return false;
}


bool DockControl::setParamsFromFile(string fileName)
{
	char s[ 2000 ], line[ 2000 ];  // buffer to read lines
	char *key, *val;
	char sep[] = " ";
	FILE *fp, *fp2, *test;

	int ival;
	double dval;
	bool bval;

	string fileName2("temp.inp");

	if ( fileName.length() > 0 )
	{
		fp = fopen( fileName.c_str(), "r" );

		if (  fp == NULL )
		{
			printf( "\n\nError: Failed to open parameter file %s!\n\n",  fileName.c_str() );
			return false;
		}

		fp2 = fopen( fileName2.c_str(), "w" );

		if (  fp == NULL )
		{
			printf( "\n\nError: Failed to create parameter file %s!\n\n",  fileName2.c_str() );
			return false;
		}

		// read a line
      		while ( fgets( s, 1999, fp ) != NULL )
		{
	  		if (strlen(s)<3) continue;

			strcpy( line, s );

			key = strtok(s, sep);
			val = strtok(NULL, sep);

			printf("key val = %s %s \n", key, val);

			if (val[strlen(val)-1]=='\n')  // remove unix new line
				val[strlen(val)-1] = '\0';

			if (val[strlen(val)-1]=='\r') // rmove windows new line
				val[strlen(val)-1] = '\0';

			if (strcasecmp(key, "staticMolecule")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open receptor f2d file %s!\n\n",  val);
					return false;
				}
				else
				{
					fclose(test);	
					string filepath(val);

					if(!basPar->setReceptorF2dName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}
			}

	  		else if (strcasecmp(key, "movingMolecule")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open ligand f2d file %s!\n\n",  val);
					return false;
				}
				else
				{	
					fclose(test);
					string filepath(val);

					if(!basPar->setLigandF2dName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}	     
			} 

			if (strcasecmp(key, "staticMoleculePDB")==0)
                        {
                                test = fopen(val, "r");

                                if(test == NULL)
                                {
                                        printf("\n\nError: Failed to open receptor pdb file %s!\n\n",  val);
                                        return false;
                                }
                                else
                                {
                                        fclose(test);
                                        string filepath(val);

                                        if(!basPar->setReceptorPDBName(filepath,0))
                                        {
                                                printf("\n\nError: Failed to update params %s!\n\n",  val);
                                                return false;
                                        }
                                }
                        }

                        else if (strcasecmp(key, "movingMoleculePDB")==0)
                        {
                                test = fopen(val, "r");

                                if(test == NULL)
                                {
                                        printf("\n\nError: Failed to open ligand pdb file %s!\n\n",  val);
                                        return false;
                                }
                                else
                                {
                                        fclose(test);
                                        string filepath(val);

                                        if(!basPar->setLigandPDBName(filepath,0))
                                        {
                                                printf("\n\nError: Failed to update params %s!\n\n",  val);
                                                return false;
                                        }
                                }
                        }

	  		else if (strcasecmp(key, "staticMoleculePQR")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open receptor pqr file %s!\n\n",  val);
					return false;
				}
				else
				{	
					fclose(test);
					string filepath(val);

					if(!basPar->setReceptorPQRName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}	     
			}

	  		else if (strcasecmp(key, "movingMoleculePQR")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open ligand pqr file %s!\n\n",  val);
					return false;
				}
				else
				{	
					fclose(test);
					string filepath(val);

					if(!basPar->setLigandPQRName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}	     
			}

	  		else if (strcasecmp(key, "rmsdAtoms")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open rmsd file %s!\n\n",  val);
					return false;
				}
				else
				{	
					fclose(test);
					string filepath(val);

					if(!basPar->setRMSDName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}	     
			}

	  		else if (strcasecmp(key, "staticMoleculeQUAD")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open receptor quad file %s!\n\n",  val);
					return false;
				}
				else
				{	
					fclose(test);
					string filepath(val);

					if(!basPar->setReceptorQuadName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}	     
			}

	  		else if (strcasecmp(key, "movingMoleculeQUAD")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open ligand quad file %s!\n\n",  val);
					return false;
				}
				else
				{	
					fclose(test);
					string filepath(val);

					if(!basPar->setLigandQuadName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}	     
			}

			else if (strcasecmp(key, "outFile")==0) 
			{
				string filepath(val);

				if(!basPar->setDockingOutputName(filepath,1))
				{
					printf("\n\nError: Failed to update params %s!\n\n",  val);
					return false;
				}	     
			}

			else if (strcasecmp(key, "rotSeparation")==0) 
			{
				string filepath(val);

				int rotSpace = atoi(val);

//				cout<<rotSpace<<endl;

				if(!dockPar->setRotSeparation(rotSpace))
				{
					printf("\n\nError: Failed to update params %s!\n\n",  key);
					return false;
				}
				else
				{
					fprintf(fp2, "rotFile deg%02d.mtx\n", rotSpace);
				}	     
			}

			else if (strcasecmp(key, "rotFile")==0) 
			{
				string filepath(val);

				string angle;
				angle.assign(filepath.substr(filepath.length() - 6, 2));
//				cout<<angle<<endl;

				int rotSpace = atoi(angle.c_str());

//				cout<<rotSpace<<endl;

				if(!dockPar->setRotSeparation(rotSpace))
				{
					printf("\n\nError: Failed to update params %s!\n\n",  key);
					return false;
				}
				else
				{
					fprintf(fp2, "rotFile deg%02d.mtx\n", rotSpace);
				}	     
			}

			else if (strcasecmp(key, "effGridFile")==0) 
			{
				//ignore the val
			}

		
			else
			{
				fprintf(fp2,"%s %s\n", key, val);
			}

		} //end while

		fprintf(fp2, "effGridFile fftw-rank.txt\n"); 	//this is by default used

      		fclose(fp);
		fclose(fp2);

    	}	//end if. 

	//fileName2 = 'fileName.temp' now has the params. the filenames are set in the basPar. 

	if(!basPar->getReceptorQuadAvailable()) basPar->setStoreReceptorQuad(true);
	if(!basPar->getLigandQuadAvailable()) basPar->setStoreLigandQuad(true);

	return basPar->setDockingInputName(fileName2,0);
}

