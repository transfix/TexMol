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
#if !defined(AFX_COLORLEGEND_H__2FAABA56_E763_48C0_BBE7_3E9523714FB0__INCLUDED_)
#define AFX_COLORLEGEND_H__2FAABA56_E763_48C0_BBE7_3E9523714FB0__INCLUDED_

#include "ColorLegendBase.Qt3.h"
#include <PDBParser/ResidueInformation.h>
#include <PDBParser/GroupOfAtoms.h>
#include <qlabel.h>
#include <q3frame.h>

class ColorLegend : public ColorLegendBase
{
	public:
		ColorLegend();
		virtual ~ColorLegend();
		void setText();
		void setColors();
		// All the residue objects
		QLabel* residueTexts[PDBParser::TOTAL_RES_TYPE];
		Q3Frame* residueColors[PDBParser::TOTAL_RES_TYPE];
		// All the ss objects
		QLabel* ssTexts[PDBParser::TOTAL_SS_TYPE];
		Q3Frame* ssColors[PDBParser::TOTAL_SS_TYPE];
};

#endif
