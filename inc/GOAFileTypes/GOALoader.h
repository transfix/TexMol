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
#ifndef CCV_GOA_LOADER_H
#define CCV_GOA_LOADER_H

#include <map>
#include <string>
#include <UsefulMath/Matrix.h>

using std::string;

class GOAFileType;

namespace PDBParser
{
	class GroupOfAtoms;
};

class GOALoader
{
	public:
		GOALoader();
		virtual ~GOALoader();
		bool saveFile(const string& fileName, const string& selectedFilter, PDBParser::GroupOfAtoms* groupOfAtoms, unsigned int level, CCVOpenGLMath::Matrix* transformation);
		PDBParser::GroupOfAtoms* loadFile(const string& fileName, bool deleteWater = false);
		bool saveFile(const string& fileName, PDBParser::GroupOfAtoms* groupOfAtoms, unsigned int level, CCVOpenGLMath::Matrix* transformation);
		string getLoadFilterString();
		string getSaveFilterString();
		bool isValidExtension(string extension);
	protected:
		string getAllExtensions();
		bool endsWith(string str, string substr);
		PDBParser::GroupOfAtoms*  tryAll(const string& fileName, bool deleteWater);
		void addGOAFileType(GOAFileType* type);
		std::map<string, GOAFileType*> m_ExtensionMap;
		std::map<string, GOAFileType*> m_FilterMap;
};

#endif
