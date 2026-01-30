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
#include <F2DockClient/rerankControl.h>
#include <string.h>

RerankControl::RerankControl()
{

}


RerankControl::~RerankControl()
{

}


void RerankControl::init()
{

}

string RerankControl::getNumsol()
{
	ostringstream out;
	out << rerankPar->getNumSol();
	return out.str();
}

string RerankControl::getGpolWeight()
{
	ostringstream out;
	out << rerankPar->getGpolWeight();
	return out.str();
}

string RerankControl::getNonpolWeight()
{
	ostringstream out;
	out << rerankPar->getGnonpolWeight();
	return out.str();
}

string RerankControl::getF2dockWeight()
{
	ostringstream out;
	out << rerankPar->getF2dockWeight();
	return out.str();
}

string RerankControl::getEpsilonBR()
{
	ostringstream out;
	out << rerankPar->getEpsilonBR();
	return out.str();
}

string RerankControl::getEpsilonGpol()
{
	ostringstream out;
	out << rerankPar->getEpsilonGpol();
	return out.str();
}


bool RerankControl::reset()
{	
	return rerankPar->reset();
}

bool RerankControl::setJobTitle(string name)
{
	return basPar->setJobTitle(name);
}

bool RerankControl::setReceptorPDBName(string fileName)
{
	return basPar->setReceptorPDBName(fileName);
}

bool RerankControl::setLigandPDBName(string fileName)
{
	return basPar->setLigandPDBName(fileName);
}

bool RerankControl::setReceptorPQRName(string fileName)
{
	return basPar->setReceptorPQRName(fileName);
}

bool RerankControl::setLigandPQRName(string fileName)
{
	return basPar->setLigandPQRName(fileName);
}

bool RerankControl::setReceptorRAWNName(string fileName)
{
	return basPar->setReceptorRAWNName(fileName);
}

bool RerankControl::setLigandRAWNName(string fileName)
{
	return basPar->setLigandRAWNName(fileName);
}

bool RerankControl::setReceptorQuadName(string fileName)
{
	return basPar->setReceptorQuadName(fileName);
}

bool RerankControl::setLigandQuadName(string fileName)
{
	return basPar->setLigandQuadName(fileName);
}

bool RerankControl::setDockingOutputName(string fileName)
{
	return basPar->setDockingOutputName(fileName);
}

bool RerankControl::setNumsol(int n)
{
	return rerankPar->setNumSol(n);
}

bool RerankControl::setGpolWeight(double w)
{
	return rerankPar->setGpolWeight(w);
}

bool RerankControl::setNonpolWeight(double w)
{
	return rerankPar->setGnonpolWeight(w);
}

bool RerankControl::setF2dockWeight(double w)
{
	return rerankPar->setF2dockWeight(w);
}

bool RerankControl::setEpsilonBR(double e)
{
	return rerankPar->setEpsilonBR(e);
}

bool RerankControl::setEpsilonGpol(double e)
{
	return rerankPar->setEpsilonGpol(e);
}

void RerankControl::setStoreReceptorPQR(){basPar->setStoreReceptorPQR(true);}
void RerankControl::setStoreLigandPQR(){basPar->setStoreLigandPQR(true);}
void RerankControl::setStoreReceptorRAWN(){basPar->setStoreReceptorRAWN(true);}
void RerankControl::setStoreLigandRAWN(){basPar->setStoreLigandRAWN(true);}
void RerankControl::setStoreReceptorQuad(){basPar->setStoreReceptorQuad(true);}
void RerankControl::setStoreLigandQuad(){basPar->setStoreLigandQuad(true);}

bool RerankControl::handleJob()
{
	bool rpdb = basPar->getReceptorPDBAvailable();
	bool rpqr = basPar->getReceptorPQRAvailable();
	bool rraw = basPar->getReceptorRAWNAvailable();
	bool rqua = basPar->getReceptorQuadAvailable();

	bool lpdb = basPar->getLigandPDBAvailable();
	bool lpqr = basPar->getLigandPQRAvailable();
	bool lraw = basPar->getLigandRAWNAvailable();
	bool lqua = basPar->getLigandQuadAvailable();

	if( ! ( rpdb || (rpqr && (rqua||rraw)) )  ) return false;
	if( ! ( lpdb || (lpqr && (lqua||lraw)) )  ) return false;

	if(rerankPar->prepareInputFile(basPar->getRerankingInputFileName()))
	{
		basPar->setRerankingInputAvailable(true);
		return true;
	}
	return false;
}


bool RerankControl::setParamsFromFile(string fileName)
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

			if (strcasecmp(key, "staticMoleculePQR")==0) 
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

	  		else if (strcasecmp(key, "F2DockOutputFile")==0) 
			{
				test = fopen(val, "r");

				if(test == NULL)
				{
					printf("\n\nError: Failed to open docking output file %s!\n\n",  val);
					return false;
				}
				else
				{	
					fclose(test);
					string filepath(val);

					if(!basPar->setDockingOutputName(filepath,0))
					{
						printf("\n\nError: Failed to update params %s!\n\n",  val);
						return false;
					}
				}	     
			}

			else if (strcasecmp(key, "rerankedOutputFile")==0) 
			{
				string filepath(val);

				if(!basPar->setRerankingOutputName(filepath,1))
				{
					printf("\n\nError: Failed to update params %s!\n\n",  val);
					return false;
				}	     
			}

			else
			{
				fprintf(fp2,"%s %s\n", key, val);
			}

		} //end while

      		fclose(fp);
		fclose(fp2);

    	}	//end if. 

	//fileName2 = 'temp.inp' now has the params. the filenames are set in the basPar. 

	if(!basPar->getReceptorQuadAvailable()) basPar->setStoreReceptorQuad(true);
	if(!basPar->getLigandQuadAvailable()) basPar->setStoreLigandQuad(true);

	return basPar->setRerankingInputName(fileName2,0);
}

