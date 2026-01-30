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
// ColorMapInterface.h: interface for the ColorMapInterface class.

#ifndef CCV_COLOR_MAP_INTERFACE_H
#define CCV_COLOR_MAP_INTERFACE_H

#include "ColorMapInterfaceBase.Qt3.h"
#include <qstring.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

class Q3TextEdit;
class Q3ScrollView;

const int DEFAULT_MINIMUM_ROW_LENGTH = 20;
class ColorMapInterface : public ColorMapInterfaceBase
{
		Q_OBJECT

	public:
		ColorMapInterface(int maximumRowLength);
		virtual ~ColorMapInterface();
		bool setGOAData(PDBParser::GroupOfAtoms* molecule);
		void colorGOA();

		Q3GridLayout* m_ScrollViewLayout;
		Q3ScrollView* m_ScrollView;
		Q3TextEdit* m_ResidueListTextEdit;
		Q3TextEdit* m_ResidueStartIDListTextEdit;
		Q3TextEdit* m_ChainIDListTextEdit;



	signals:
		void update(QString updateString);
		void updateRenderer();

	public slots:
		void residueListTextEditSelectionColorSlot();
		void chainIDTextEditSelectionColorSlot();
		void clearColorMapSlot();

	protected:

		bool setNewHeights(int numRows);
		char getChainIdForLine(int line);
		int getChainIndexForLine(int line);
		int getNumberOfResidueInChain(int chainIndex);
		bool addResidueToColorMap(int residueIndex, int chainIndex, double r, double g, double b, double a);
		vector<int>  m_ChainStartingRow;
		int m_MaximumRowLength; // Number of residues to display on a row
		PDBParser::GroupOfAtoms* m_Molecule;
		vector<std::string> m_ColorMap;
};

#endif
