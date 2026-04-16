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
#include <QPalette>
#include <QFrame>

// Qt6 helper: set widget background color via palette
static void setWidgetBgColor(QWidget* w, const QColor& c) {
	QPalette pal = w->palette();
	pal.setColor(QPalette::Window, c);
	pal.setColor(QPalette::WindowText, c);
	w->setAutoFillBackground(true);
	w->setPalette(pal);
}

ColorLegend::ColorLegend()
{
	// Create named widgets (were Qt3 Designer-generated)
	residueText0 = new QLabel(this); residueText1 = new QLabel(this); residueText2 = new QLabel(this);
	residueText3 = new QLabel(this); residueText4 = new QLabel(this); residueText5 = new QLabel(this);
	residueText6 = new QLabel(this); residueText7 = new QLabel(this); residueText8 = new QLabel(this);
	residueText9 = new QLabel(this); residueText10 = new QLabel(this); residueText11 = new QLabel(this);
	residueText12 = new QLabel(this); residueText13 = new QLabel(this); residueText14 = new QLabel(this);
	residueText15 = new QLabel(this); residueText16 = new QLabel(this); residueText17 = new QLabel(this);
	residueText18 = new QLabel(this); residueText19 = new QLabel(this); residueText20 = new QLabel(this);
	residueText21 = new QLabel(this); residueText22 = new QLabel(this); residueText23 = new QLabel(this);
	residueText24 = new QLabel(this); residueText25 = new QLabel(this); residueText26 = new QLabel(this);
	residueText27 = new QLabel(this); residueText28 = new QLabel(this);
	residueColor0 = new QFrame(this); residueColor1 = new QFrame(this); residueColor2 = new QFrame(this);
	residueColor3 = new QFrame(this); residueColor4 = new QFrame(this); residueColor5 = new QFrame(this);
	residueColor6 = new QFrame(this); residueColor7 = new QFrame(this); residueColor8 = new QFrame(this);
	residueColor9 = new QFrame(this); residueColor10 = new QFrame(this); residueColor11 = new QFrame(this);
	residueColor12 = new QFrame(this); residueColor13 = new QFrame(this); residueColor14 = new QFrame(this);
	residueColor15 = new QFrame(this); residueColor16 = new QFrame(this); residueColor17 = new QFrame(this);
	residueColor18 = new QFrame(this); residueColor19 = new QFrame(this); residueColor20 = new QFrame(this);
	residueColor21 = new QFrame(this); residueColor22 = new QFrame(this); residueColor23 = new QFrame(this);
	residueColor24 = new QFrame(this); residueColor25 = new QFrame(this); residueColor26 = new QFrame(this);
	residueColor27 = new QFrame(this); residueColor28 = new QFrame(this);
	ssText0 = new QLabel(this); ssText1 = new QLabel(this); ssText2 = new QLabel(this); ssText3 = new QLabel(this);
	ssColor0 = new QFrame(this); ssColor1 = new QFrame(this); ssColor2 = new QFrame(this); ssColor3 = new QFrame(this);
	moleculeText = new QLabel(this);
	moleculeColorFrame = new QFrame(this);

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
		setWidgetBgColor(residueColors[i], currColor);
	}

	// Set ss colors
	n = PDBParser::TOTAL_SS_TYPE;

	for(i = 0; i < n; i++)
	{
		r = PDBParser::ssColorR[i] * 255;
		g = PDBParser::ssColorG[i] * 255;
		b = PDBParser::ssColorB[i] * 255;
		currColor.setRgb(r,g,b);
		setWidgetBgColor(ssColors[i], currColor);
	}

	// Set molecule color
	r = PDBParser::moleculeColor[0] * 255;
	g = PDBParser::moleculeColor[1] * 255;
	b = PDBParser::moleculeColor[2] * 255;
	currColor.setRgb(r,g,b);
	setWidgetBgColor(moleculeColorFrame, currColor);
}
