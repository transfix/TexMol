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
// ColorMapInterface.cpp: implementation of the ColorMapInterface class.

#include <DataManager/BallAndStickDataManager/ColorMapInterface.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/CollectionData.h>
#include <PDBParser/GOAColor.h>
#include <PDBParser/Residues.h>
#include <q3textedit.h>
#include <q3scrollview.h>
#include <q3groupbox.h>
#include <qlayout.h>
#include <qcolordialog.h>
//Added by qt3to4:
#include <Q3GridLayout>

ColorMapInterface::ColorMapInterface(int maximumRowLength)
{
	m_MaximumRowLength = maximumRowLength;

	if(m_MaximumRowLength < DEFAULT_MINIMUM_ROW_LENGTH)
	{
		m_MaximumRowLength = DEFAULT_MINIMUM_ROW_LENGTH;
	}

	m_ScrollView = new Q3ScrollView(m_MainGroupBox, "m_ScrollView");
	m_ScrollView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	m_ScrollView->setMinimumWidth(200);
	m_ScrollView->setMinimumHeight(200);
	m_MainGroupBox->setColumnLayout(0, Qt::Vertical);
	m_MainGroupBox->layout()->setSpacing(6);
	m_MainGroupBox->layout()->setMargin(11);
	Q3GridLayout* m_MainGroupBoxLayout = new Q3GridLayout(m_MainGroupBox->layout());
	m_MainGroupBoxLayout->setAlignment(Qt::AlignTop);
	m_MainGroupBoxLayout->addWidget(m_ScrollView, 0, 0);
	m_ChainIDListTextEdit = new Q3TextEdit(m_ScrollView->viewport(), "m_ChainIDListTextEdit");
	m_ChainIDListTextEdit->setFont(QFont("Courier"));
	//m_ChainIDListTextEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_ChainIDListTextEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_ChainIDListTextEdit->setMinimumWidth(40);
	m_ChainIDListTextEdit->setMaximumWidth(40);
	m_ChainIDListTextEdit->setReadOnly(true);
	m_ScrollView->addChild(m_ChainIDListTextEdit, 10, 10);
	m_ResidueStartIDListTextEdit = new Q3TextEdit(m_ScrollView->viewport(), "m_ResidueStartIDListTextEdit");
	m_ResidueStartIDListTextEdit->setFont(QFont("Courier"));
	m_ResidueStartIDListTextEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_ResidueStartIDListTextEdit->setMinimumWidth(80);
	m_ResidueStartIDListTextEdit->setMaximumWidth(80);
	m_ResidueStartIDListTextEdit->setReadOnly(true);
	m_ScrollView->addChild(m_ResidueStartIDListTextEdit, 60, 10);
	m_ResidueListTextEdit = new Q3TextEdit(m_ScrollView->viewport(), "m_ResidueListTextEdit");
	m_ResidueListTextEdit->setFont(QFont("Courier"));
	//m_ResidueListTextEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_ResidueListTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	int cW = (m_ResidueListTextEdit->fontMetrics()).width('W');
	int widthRequired = (m_ResidueListTextEdit->fontMetrics()).width('W') * (m_MaximumRowLength+1);
	// arand, 9-6-2011: fixing the widths in these boxes...
	//m_ResidueListTextEdit->setWidth( widthRequired );

	m_ResidueListTextEdit->setMinimumWidth(widthRequired);
	m_ResidueListTextEdit->setMaximumWidth(widthRequired);

	m_ResidueListTextEdit->setReadOnly(true);
	m_ScrollView->addChild(m_ResidueListTextEdit, 150, 10);
	m_ScrollView->resizeContents(150 + m_ResidueListTextEdit->width() + 10, 50+40);
	connect(m_ResidueListTextEdit, SIGNAL(selectionChanged()), this, SLOT(residueListTextEditSelectionColorSlot()));
	connect(m_ChainIDListTextEdit, SIGNAL(selectionChanged()), this, SLOT(chainIDTextEditSelectionColorSlot()));
	connect((QWidget*)m_ClearColorMapPushButton, SIGNAL(clicked()), this, SLOT(clearColorMapSlot()));
}

ColorMapInterface::~ColorMapInterface()
{
}

bool ColorMapInterface::setNewHeights(int numRows)
{
	if(!m_ChainIDListTextEdit || !m_ResidueStartIDListTextEdit || !m_ResidueListTextEdit || (numRows<1))
	{
		return false;
	}

	int height = (m_ResidueListTextEdit->fontMetrics()).lineSpacing() * (numRows+1);
	m_ScrollView->resizeContents(150 + m_ResidueListTextEdit->width() + 10, height+40);
	m_ChainIDListTextEdit->setMinimumHeight(height);
	m_ResidueStartIDListTextEdit->setMinimumHeight(height);
	m_ResidueListTextEdit->setMinimumHeight(height);
	return true;
}

bool ColorMapInterface::setGOAData(PDBParser::GroupOfAtoms* molecule)
{
	if(!molecule)
	{
		return false;
	}

	if(!molecule->m_CollectionData)
	{
		return false;
	}

	m_Molecule = molecule;
	{
		/*int i,n = molecule->m_CollectionData->m_ChainIds.size();
		int r = 0;
		for( i=0; i<n/2; i++ )
		{
			printf( "%c %d\n", (char)(molecule->m_CollectionData->m_ChainIds[2*i+0]), molecule->m_CollectionData->m_ChainIds[2*i+1] );
			int j;
			for( j=0; j<molecule->m_CollectionData->m_ChainIds[2*i+1]; j++ )
			{
				printf( "[%c at <%d %c>]\t",
					(char)(molecule->m_CollectionData->m_ResidueIds[r+0]),
					(molecule->m_CollectionData->m_ResidueIds[r+1]),
					(char)(molecule->m_CollectionData->m_ResidueIds[r+2]) );
				r += 3;
			}
			printf("\n");
		}*/
	}

	if(m_MaximumRowLength < 1)
	{
		m_MaximumRowLength = DEFAULT_MINIMUM_ROW_LENGTH;
	}

	int numRows = 1;
	{
		QString resList;
		int i,n = molecule->m_CollectionData->m_ChainIds.size();
		int r = 0;

		for(i=0; i<n/2; i++)
		{
			int j;
			int chainR = 0;

			for(j=0; j<molecule->m_CollectionData->m_ChainIds[2*i+1]; j++)
			{
				if((chainR/3+1)%m_MaximumRowLength == 0)
				{
					numRows++;
				}

				r += 3;
				chainR += 3;
			}

			if(((chainR-3)/3+1)%m_MaximumRowLength != 0)
			{
				numRows++;
			}
		}
	}

	setNewHeights(numRows);
	{
		QString resList;
		QString chainIDList;
		QString resStartingRowID;
		int i,n = molecule->m_CollectionData->m_ChainIds.size();
		int r = 0;
		int rowNumber = 0;

		for(i=0; i<n/2; i++)
		{

		  //printf("%c %d\n", (char)(molecule->m_CollectionData->m_ChainIds[2*i+0]), molecule->m_CollectionData->m_ChainIds[2*i+1]);

			QChar qChain((char)(molecule->m_CollectionData->m_ChainIds[2*i+0]));
			chainIDList += qChain;
			m_ChainStartingRow.push_back(rowNumber);

			int j;
			int chainR = 0;
			bool addNewPair = true;

			for(j=0; j<molecule->m_CollectionData->m_ChainIds[2*i+1]; j++)
			{
#ifdef CCV_TEXMOL_PRINT_ALL
				printf("[%c at <%d %c>]\t",
					   (char)(molecule->m_CollectionData->m_ResidueIds[r+0]),
					   (molecule->m_CollectionData->m_ResidueIds[r+1]),
					   (char)(molecule->m_CollectionData->m_ResidueIds[r+2]));
#endif

				if(addNewPair)
				{
					int int_resStartingRowID = molecule->m_CollectionData->m_ResidueIds[r+1];
					resStartingRowID += QString::number(molecule->m_CollectionData->m_ResidueIds[r+1]);
					QChar resAltID((char)(molecule->m_CollectionData->m_ResidueIds[r+2]));
					resStartingRowID += resAltID;
					resStartingRowID += "\n";
					addNewPair = false;
				}

				QChar qc((char)(molecule->m_CollectionData->m_ResidueIds[r+0]));
				resList += qc;

				if((chainR/3+1)%m_MaximumRowLength == 0)
				{
					resList += "\n";
					chainIDList += "\n";
					rowNumber++;
					addNewPair = true;
				}

				r += 3;
				chainR += 3;
			}

			//printf("\n");

			if(((chainR-3)/3+1)%m_MaximumRowLength != 0)
			{
				resList += "\n";
				chainIDList += "\n";
				rowNumber++;
			}

			int whatever = ((chainR-3)/3+1)%m_MaximumRowLength;
		}

		m_ChainIDListTextEdit->append(chainIDList);
		m_ResidueStartIDListTextEdit->append(resStartingRowID);
		m_ResidueListTextEdit->append(resList);
	}
	return true;
}

char ColorMapInterface::getChainIdForLine(int line)
{
	// the line is either inbetween two chains, or belongs to the last chain.
	int i = 0;

	for(i=0; i<m_Molecule->m_CollectionData->m_ChainIds.size()/2-1; i++)
	{
		if(m_ChainStartingRow[i] <= line && m_ChainStartingRow[i+1] >= line)
		{
			return (char)(m_Molecule->m_CollectionData->m_ChainIds[i*2]);
		}
	}

	return (char)(m_Molecule->m_CollectionData->m_ChainIds[i*2]);
}

int ColorMapInterface::getChainIndexForLine(int line)
{
	// the line is either inbetween two chains, or belongs to the last chain.
	int i = 0;

	for(i=0; i<m_Molecule->m_CollectionData->m_ChainIds.size()/2-1; i++)
	{
		if(m_ChainStartingRow[i] <= line && m_ChainStartingRow[i+1] >= line)
		{
			return i;
		}
	}

	return i;
}

int ColorMapInterface::getNumberOfResidueInChain(int chainIndex)
{
	if(chainIndex*2+1 >= m_Molecule->m_CollectionData->m_ChainIds.size())
	{
		return 0;    // error!
	}

	return m_Molecule->m_CollectionData->m_ChainIds[chainIndex*2+1];
}

bool ColorMapInterface::addResidueToColorMap(int residueIndex, int chainIndex, double r, double g, double b, double a)
{
	//         <mol> <chainNum chainID> <ignored> <resSeq resName> <atomName index> <r    g     b     a>
	// example "0"   "1"       "B"      "2"       "6"     "LYS" "  N "       "3"    "0.0" "0.0" "1.0" "1.0"
	if(chainIndex >= m_Molecule->m_CollectionData->m_ChainIds.size()/2)
	{
		return false;    // error!
	}

	// get cumulative residue Index
	int cumulativeResidueIndex = 0;
	{
		int c;

		for(c=0; c<chainIndex; c++)
		{
			cumulativeResidueIndex += m_Molecule->m_CollectionData->m_ChainIds[c*2+1];
		}

		cumulativeResidueIndex += residueIndex;
	}

	if(cumulativeResidueIndex*3+2 >= m_Molecule->m_CollectionData->m_ResidueIds.size())
	{
		return false;    // error!
	}

	char chainID = m_Molecule->m_CollectionData->m_ChainIds[chainIndex*2+0];
	QChar resShortId((char)(m_Molecule->m_CollectionData->m_ResidueIds[cumulativeResidueIndex*3+0]));
/*Q3Err:CS
	const char* resNameC = PDBParser::Residues::lookupLongResID(resShortId);
*/
	const char* resNameC = PDBParser::Residues::lookupLongResID(resShortId.toAscii());	//by cha

	QString resName(resNameC);
	int resSeq = m_Molecule->m_CollectionData->m_ResidueIds[cumulativeResidueIndex*3+1];
	QString colorMapLine;
	colorMapLine +=	"\"0\"";
	colorMapLine +=	" \"";
	colorMapLine +=	QString::number(chainIndex);
	colorMapLine +=	"\"";
	colorMapLine +=	" \"";
	colorMapLine +=	chainID;
	colorMapLine +=	"\"";
	int dummy = 1;
	colorMapLine +=	" \"";
	colorMapLine +=	QString::number(dummy);
	colorMapLine +=	"\"";
	colorMapLine +=	" \"";
	colorMapLine +=	QString::number(resSeq);
	colorMapLine +=	"\"";
	colorMapLine +=	" \"";
	colorMapLine +=	resName;
	colorMapLine +=	"\"";
	colorMapLine +=	" \"\"";
	colorMapLine +=	" \"\"";
	colorMapLine +=	" \"";
	colorMapLine +=	QString::number(r);
	colorMapLine +=	"\"";
	colorMapLine +=	" \"";
	colorMapLine +=	QString::number(g);
	colorMapLine +=	"\"";
	colorMapLine +=	" \"";
	colorMapLine +=	QString::number(b);
	colorMapLine +=	"\"";
	colorMapLine +=	" \"";
	colorMapLine +=	QString::number(a);
	colorMapLine +=	"\"";
	m_ColorMap.push_back(string(colorMapLine.latin1()));
	//printf("%s\n", colorMapLine.latin1());
	return true;
}

void ColorMapInterface::residueListTextEditSelectionColorSlot()
{
	if(!m_ResidueListTextEdit->hasSelectedText())
	{
		return;
	}

	QColor newColor = QColorDialog::getColor(Qt::white, this);

	if(!newColor.isValid())
	{
		return;
	}
/*Q3Err:C
	double r = newColor.Qt::red() / 255.0;
	double g = newColor.Qt::green() / 255.0;
	double b = newColor.Qt::blue() / 255.0;
*/
	double r = newColor.red() / 255.0;
	double g = newColor.green() / 255.0;
	double b = newColor.blue() / 255.0;
	double a = 1.0;
	int paraFrom = 0;
	int indexFrom = 0;
	int paraTo = 0;
	int indexTo = 0;
	m_ResidueListTextEdit->getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
	int startChainIndex = 0;
	int endChainIndex = 0;
	// find the start and end chain Index
	{
		bool foundChain = false;
		//printf("Chains selected:\n");
		int line;

		for(line = paraFrom; line <= paraTo; line++)
		{
			int chainIndex = getChainIndexForLine(line);

			if(!foundChain)
			{
				startChainIndex = chainIndex;
				foundChain = true;
			}

			endChainIndex = chainIndex;
			//printf("%d\n", chainIndex);
		}
	}
	// find start and end residue index
	int startResIndex = (paraFrom - m_ChainStartingRow[startChainIndex])*m_MaximumRowLength + indexFrom;
	int endResIndex = (paraTo - m_ChainStartingRow[endChainIndex])*m_MaximumRowLength + indexTo;
	// loop through in parts to get all residues
	int curChainIndex = startChainIndex;

	while(curChainIndex != endChainIndex)
	{
		int i;
		int numResInCurChain = getNumberOfResidueInChain(curChainIndex);

		for(i=startResIndex; i<numResInCurChain; i++)
		{
			// add this residue to selection
			addResidueToColorMap(i, curChainIndex, r, g, b, a);
		}

		startResIndex = 0;
		curChainIndex++;
	}

	{
		int i;

		for(i=startResIndex; i<endResIndex; i++)
		{
			// add this residue to selection
			addResidueToColorMap(i, curChainIndex, r, g, b, a);
		}
	}

	{
	  //printf("New color map is:\n");
	  //int i;

		//for(i=0; i<m_ColorMap.size(); i++)
		//	{
		//	printf("%s\n", m_ColorMap[i].c_str());
		//}
	}

	// <mol> <chainNum chainID> <ignored> <resSeq resName> <atomName index> <r g b a>
	// example "0" "1" "B" "2" "6" "LYS" " N " "3" "0.0" "0.0" "1.0" "1.0"


	colorGOA();
}
	
void ColorMapInterface::colorGOA() {
	PDBParser::GOAColor* goaColor = new PDBParser::GOAColor();
	goaColor->parseColormapString(m_ColorMap);
	goaColor->ApplyColormap(m_Molecule);
	delete goaColor;
	emit updateRenderer();
}

void ColorMapInterface::chainIDTextEditSelectionColorSlot()
{
	if(!m_ChainIDListTextEdit->hasSelectedText())
	{
		return;
	}

	QColor newColor = QColorDialog::getColor(Qt::white, this);

	if(!newColor.isValid())
	{
		return;
	}
/*Q3Err:C
	double r = newColor.Qt::red() / 255.0;
	double g = newColor.Qt::green() / 255.0;
	double b = newColor.Qt::blue() / 255.0;
*/
	double r = newColor.red() / 255.0;
	double g = newColor.green() / 255.0;
	double b = newColor.blue() / 255.0;
	double a = 1.0;
	int paraFrom = 0;
	int indexFrom = 0;
	int paraTo = 0;
	int indexTo = 0;
	m_ChainIDListTextEdit->getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
	// go through the list of chain positions and see if any are selected
	{
	  //printf("Chains selected:\n");
		int i;

		for(i=0; i<m_Molecule->m_CollectionData->m_ChainIds.size()/2; i++)
		{
			if(m_ChainStartingRow[i] >= paraFrom && m_ChainStartingRow[i] <= paraTo)
			{
				char c_chainID = (char)(m_Molecule->m_CollectionData->m_ChainIds[i*2]);
				//printf("%c\n", (char)(m_Molecule->m_CollectionData->m_ChainIds[i*2]));
				QString colorMapLine;
				colorMapLine +=	"\"0\"";
				colorMapLine +=	" \"";
				colorMapLine +=	QString::number(i);
				colorMapLine +=	"\"";
				colorMapLine +=	" \"";
				colorMapLine +=	c_chainID;
				colorMapLine +=	"\"";
				colorMapLine +=	" \"\"";
				colorMapLine +=	" \"\"";
				colorMapLine +=	" \"\"";
				colorMapLine +=	" \"\"";
				colorMapLine +=	" \"\"";
				colorMapLine +=	" \"";
				colorMapLine +=	QString::number(r);
				colorMapLine +=	"\"";
				colorMapLine +=	" \"";
				colorMapLine +=	QString::number(g);
				colorMapLine +=	"\"";
				colorMapLine +=	" \"";
				colorMapLine +=	QString::number(b);
				colorMapLine +=	"\"";
				colorMapLine +=	" \"";
				colorMapLine +=	QString::number(a);
				colorMapLine +=	"\"";
				m_ColorMap.push_back(string(colorMapLine.latin1()));
			}
		}
	}
	{
	  //printf("New color map is:\n");
	  //int i;

	  //for(i=0; i<m_ColorMap.size(); i++)
	  //{
	  //	printf("%s\n", m_ColorMap[i].c_str());
	  //}
	}
	// <mol> <chainNum chainID> <ignored> <resSeq resName> <atomName index> <r g b a>
	// example "0" "1" "B" "2" "6" "LYS" " N " "3" "0.0" "0.0" "1.0" "1.0"

	colorGOA();
}


/*
	PDBParser::GOAColor* goaColor = new PDBParser::GOAColor();
	goaColor->parseColormapString(m_ColorMap);
	goaColor->ApplyColormap(m_Molecule);
	delete goaColor;
	emit updateRenderer();
}
*/


void ColorMapInterface::clearColorMapSlot()
{
	m_ColorMap.clear();
	

	PDBParser::GOAColor* goaColor = new PDBParser::GOAColor();
	goaColor->ApplyColorsByGOALevel(m_Molecule,PDBParser::GroupOfAtoms::RESIDUE);
	delete goaColor;
	emit updateRenderer();
}
