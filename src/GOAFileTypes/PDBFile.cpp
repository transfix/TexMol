/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
//#include <Blurmaps/BlurMapsDataManager.h>
#include <GOAFileTypes/PDBFile.h>
#include <math.h>
#include <PDBParser/Atom.h>
#include <PDBParser/CollectionData.h>
#include <PDBParser/GOAFileIO.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/parserPDBtoGOA.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

PDBFile PDBFile::ms_PDBFileRepresentative;

PDBFile::PDBFile()
{
}

PDBFile::~PDBFile()
{
}

PDBParser::GroupOfAtoms* PDBFile::loadFile(const string& fileName, bool deleteWater)
{
	PDBParser::parserPDBtoGOA* p = new PDBParser::parserPDBtoGOA();
	PDBParser::GroupOfAtoms* molecule = p->parsePDBModels(fileName.c_str(), deleteWater);
	delete p;
	return molecule;
}

bool PDBFile::checkType(const string& fileName)
{
	return false;
}

bool PDBFile::saveFile(PDBParser::GroupOfAtoms* molecule, const string& fileName, unsigned int level, CCVOpenGLMath::Matrix* transformation)
{
	FILE* fp = fopen(fileName.c_str(), "w");
	if(!fp)
	{
		return false;
	}
	bool ret = PDBParser::writeGOA2PDB(fp, molecule, level, transformation);
	fclose(fp);
	return ret;
}

GOAFileType* PDBFile::getRepresentative()
{
	return &ms_PDBFileRepresentative;
}
