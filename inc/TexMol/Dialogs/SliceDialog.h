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
#ifndef CCV_TEXMOL_SLICE_DIALOG_H
#define CCV_TEXMOL_SLICE_DIALOG_H

#include "SliceDialogBase.Qt3.h"

namespace PDBParser
{
	class GroupOfAtoms;
};

class SliceDialog : public SliceDialogBase
{
	public:
		SliceDialog(PDBParser::GroupOfAtoms* molecule, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		virtual ~SliceDialog();
		bool printParameterization();

	protected:
		PDBParser::GroupOfAtoms* m_Molecule;
};

#endif
