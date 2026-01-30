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
// BallAndStickDataManager.h: interface for the BallAndStickDataManager class.

#if !defined(AFX_BALLANDSTICKDATAMANAGER_H__8D64A672_6754_4B81_84AC_DC85209FDD4F__INCLUDED_)
#define AFX_BALLANDSTICKDATAMANAGER_H__8D64A672_6754_4B81_84AC_DC85209FDD4F__INCLUDED_

namespace PDBParser
{
	class GroupOfAtoms;
}

class BallAndStickDataManager
{
	public:
		BallAndStickDataManager();
		virtual ~BallAndStickDataManager();
		virtual PDBParser::GroupOfAtoms* readSimplePDBModels(const char* fileName);
		virtual bool evolve(const char* pdbOrPqrFileName, const char* changesFileName, const char* outputPdbOrPqrFileName);
		virtual bool writePDB(const char* inputFileName, const char* outputFileName, int outputLevel);
		virtual bool writeGOA(const char* inputFileName, const char* outputFileName);
		virtual PDBParser::GroupOfAtoms* generateFullVirus(PDBParser::GroupOfAtoms* molecule);
		virtual bool printPDBInformation(const char* pdbOrPqrFileName, const char* outputFileName, int printType);
		virtual bool getMaxDistanceFromPoint(const char* pdbOrPqrFileName, const char* outputFileName, double xOrigin, double yOrigin, double zOrigin, bool appendToFile);

	protected:
		void twistChainRecursive(PDBParser::GroupOfAtoms* currGOA, int pdbResSeq, double phi, double psi);
		void twistAndOrReplaceResidueRecursive(PDBParser::GroupOfAtoms* currGOA, int pdbResSeq, int oldResId, PDBParser::GroupOfAtoms* newRes, double chi1, double chi2, double chi3, double chi4, double chi5);
		void generateChainsCopy(PDBParser::GroupOfAtoms* newMolecule, PDBParser::GroupOfAtoms* molecule);
};

#endif // !defined(AFX_BALLANDSTICKDATAMANAGER_H__8D64A672_6754_4B81_84AC_DC85209FDD4F__INCLUDED_)
