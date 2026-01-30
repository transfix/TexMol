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
#include <F2DockClient/quadGenControl.h>
#include <string.h>

QuadGenControl::QuadGenControl()
{

}


QuadGenControl::~QuadGenControl()
{

}


void QuadGenControl::init()
{

}

bool QuadGenControl::setReceptorPDBName(string fileName)
{
	return basPar->setReceptorPDBName(fileName);
}

bool QuadGenControl::setLigandPDBName(string fileName)
{
	return basPar->setLigandPDBName(fileName);
}

bool QuadGenControl::setResolution(int index)
{
	return quadPar->setResolution(index);
}

bool QuadGenControl::setJobTitle(string name)
{
	return basPar->setJobTitle(name);
}

bool QuadGenControl::handleJob(bool r)
{
	if(basPar->getReceptorPDBAvailable() && basPar->getLigandPDBAvailable() && quadPar->prepareInputFile(basPar->getQuadGenInputFileName()))
	{
		basPar->setQuadGenInputAvailable(true);

		if(r) basPar->setStoreReceptorRAWN(true);
		if(r) basPar->setStoreLigandRAWN(true);
		basPar->setStoreReceptorQuad(true);
		basPar->setStoreLigandQuad(true);

		return true;
	}
	return false;
}

bool QuadGenControl::setParamsFromFile(string fileName)
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
		
	  		else if (strcasecmp(key, "resolution")==0) 
			{
				fprintf(fp2,"%s \n", val);
			}
		
			else
			{

			}

		} //end while

      		fclose(fp);
		fclose(fp2);

    	}	//end if. 

	//fileName2 = 'temp.inp' now has the params. the filenames are set in the basPar. 

	basPar->setStoreReceptorPQR(true);
	basPar->setStoreLigandPQR(true);	
	basPar->setStoreReceptorRAWN(true);
	basPar->setStoreLigandRAWN(true);	
	basPar->setStoreReceptorQuad(true);
	basPar->setStoreLigandQuad(true);	


	return basPar->setQuadGenInputName(fileName2,0);
}
