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

#include <QFrame>
#include <QWidget>
#include <PDBParser/ResidueInformation.h>
#include <PDBParser/GroupOfAtoms.h>
#include <QLabel>
#include <QFrame>

// ColorLegendBase was a Qt3 Designer class — stub it as QWidget for Qt6
typedef QWidget ColorLegendBase;

class ColorLegend : public ColorLegendBase
{
	public:
		ColorLegend();
		virtual ~ColorLegend();
		void setText();
		void setColors();
		// All the residue objects
		QLabel* residueTexts[PDBParser::TOTAL_RES_TYPE];
		QFrame* residueColors[PDBParser::TOTAL_RES_TYPE];
		// All the ss objects
		QLabel* ssTexts[PDBParser::TOTAL_SS_TYPE];
		QFrame* ssColors[PDBParser::TOTAL_SS_TYPE];
		// Named widget members (were Qt3 Designer-generated)
		QLabel* residueText0; QLabel* residueText1; QLabel* residueText2;
		QLabel* residueText3; QLabel* residueText4; QLabel* residueText5;
		QLabel* residueText6; QLabel* residueText7; QLabel* residueText8;
		QLabel* residueText9; QLabel* residueText10; QLabel* residueText11;
		QLabel* residueText12; QLabel* residueText13; QLabel* residueText14;
		QLabel* residueText15; QLabel* residueText16; QLabel* residueText17;
		QLabel* residueText18; QLabel* residueText19; QLabel* residueText20;
		QLabel* residueText21; QLabel* residueText22; QLabel* residueText23;
		QLabel* residueText24; QLabel* residueText25; QLabel* residueText26;
		QLabel* residueText27; QLabel* residueText28;
		QFrame* residueColor0; QFrame* residueColor1; QFrame* residueColor2;
		QFrame* residueColor3; QFrame* residueColor4; QFrame* residueColor5;
		QFrame* residueColor6; QFrame* residueColor7; QFrame* residueColor8;
		QFrame* residueColor9; QFrame* residueColor10; QFrame* residueColor11;
		QFrame* residueColor12; QFrame* residueColor13; QFrame* residueColor14;
		QFrame* residueColor15; QFrame* residueColor16; QFrame* residueColor17;
		QFrame* residueColor18; QFrame* residueColor19; QFrame* residueColor20;
		QFrame* residueColor21; QFrame* residueColor22; QFrame* residueColor23;
		QFrame* residueColor24; QFrame* residueColor25; QFrame* residueColor26;
		QFrame* residueColor27; QFrame* residueColor28;
		QLabel* ssText0; QLabel* ssText1; QLabel* ssText2; QLabel* ssText3;
		QFrame* ssColor0; QFrame* ssColor1; QFrame* ssColor2; QFrame* ssColor3;
		QLabel* moleculeText;
		QFrame* moleculeColorFrame;
};

#endif
