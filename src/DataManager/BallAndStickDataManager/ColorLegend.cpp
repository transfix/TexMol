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
// ColorLegend.cpp: implementation of the ColorLegend class.

#include <DataManager/BallAndStickDataManager/ColorLegend.h>
#include <PDBParser/ResidueInformation.h>
#include <PDBParser/GroupOfAtoms.h>
#include <qlabel.h>
#include <qcolor.h>

ColorLegend::ColorLegend()
{
	// Initialize the arrays.  This routine is kind of a hack because it
	// does not use the #define's to figure out the size of the arrays.
	residueTexts[0] = residueText0;
	residueTexts[1] = residueText1;
	residueTexts[2] = residueText2;
	residueTexts[3] = residueText3;
	residueTexts[4] = residueText4;
	residueTexts[5] = residueText5;
	residueTexts[6] = residueText6;
	residueTexts[7] = residueText7;
	residueTexts[8] = residueText8;
	residueTexts[9] = residueText9;
	residueTexts[10] = residueText10;
	residueTexts[11] = residueText11;
	residueTexts[12] = residueText12;
	residueTexts[13] = residueText13;
	residueTexts[14] = residueText14;
	residueTexts[15] = residueText15;
	residueTexts[16] = residueText16;
	residueTexts[17] = residueText17;
	residueTexts[18] = residueText18;
	residueTexts[19] = residueText19;
	residueTexts[20] = residueText20;
	residueTexts[21] = residueText21;
	residueTexts[22] = residueText22;
	residueTexts[23] = residueText23;
	residueTexts[24] = residueText24;
	residueTexts[25] = residueText25;
	residueTexts[26] = residueText26;
	residueTexts[27] = residueText27;
	residueTexts[28] = residueText28;
	residueColors[0] = residueColor0;
	residueColors[1] = residueColor1;
	residueColors[2] = residueColor2;
	residueColors[3] = residueColor3;
	residueColors[4] = residueColor4;
	residueColors[5] = residueColor5;
	residueColors[6] = residueColor6;
	residueColors[7] = residueColor7;
	residueColors[8] = residueColor8;
	residueColors[9] = residueColor9;
	residueColors[10] = residueColor10;
	residueColors[11] = residueColor11;
	residueColors[12] = residueColor12;
	residueColors[13] = residueColor13;
	residueColors[14] = residueColor14;
	residueColors[15] = residueColor15;
	residueColors[16] = residueColor16;
	residueColors[17] = residueColor17;
	residueColors[18] = residueColor18;
	residueColors[19] = residueColor19;
	residueColors[20] = residueColor20;
	residueColors[21] = residueColor21;
	residueColors[22] = residueColor22;
	residueColors[23] = residueColor23;
	residueColors[24] = residueColor24;
	residueColors[25] = residueColor25;
	residueColors[26] = residueColor26;
	residueColors[27] = residueColor27;
	residueColors[28] = residueColor28;
	ssTexts[0] = ssText0;
	ssTexts[1] = ssText1;
	ssTexts[2] = ssText2;
	ssTexts[3] = ssText3;
	ssColors[0] = ssColor0;
	ssColors[1] = ssColor1;
	ssColors[2] = ssColor2;
	ssColors[3] = ssColor3;
}

ColorLegend::~ColorLegend()
{
}

// Set the text boxes for all elements in the legend
void ColorLegend::setText()
{
	int i, n;
	// Set residue text
	n = PDBParser::TOTAL_RES_TYPE;

	for(i = 0; i < n; i++)
	{
		residueTexts[i]->setText(PDBParser::residueTable[i].residueName);
	}

	// Set ss text
	n = PDBParser::TOTAL_SS_TYPE;

	for(i = 0; i < n; i++)
	{
		ssTexts[i]->setText(PDBParser::ssString[i]);
	}

	// Set molecule text
	moleculeText->setText(PDBParser::moleculeString);
}

void ColorLegend::setColors()
{
	int r, g, b;
	int i, n;
	QColor currColor;
	// Set residue colors
	n = PDBParser::TOTAL_RES_TYPE;

	for(i = 0; i < n; i++)
	{
		r = PDBParser::resColorR[i] * 255;
		g = PDBParser::resColorG[i] * 255;
		b = PDBParser::resColorB[i] * 255;
		currColor.setRgb(r,g,b);
		residueColors[i]->setPaletteForegroundColor(currColor);
		residueColors[i]->setPaletteBackgroundColor(currColor);
	}

	// Set ss colors
	n = PDBParser::TOTAL_SS_TYPE;

	for(i = 0; i < n; i++)
	{
		r = PDBParser::ssColorR[i] * 255;
		g = PDBParser::ssColorG[i] * 255;
		b = PDBParser::ssColorB[i] * 255;
		currColor.setRgb(r,g,b);
		ssColors[i]->setPaletteForegroundColor(currColor);
		ssColors[i]->setPaletteBackgroundColor(currColor);
	}

	// Set molecule color
	r = PDBParser::moleculeColor[0] * 255;
	g = PDBParser::moleculeColor[1] * 255;
	b = PDBParser::moleculeColor[2] * 255;
	currColor.setRgb(r,g,b);
	moleculeColorFrame->setPaletteForegroundColor(currColor);
	moleculeColorFrame->setPaletteBackgroundColor(currColor);
}
