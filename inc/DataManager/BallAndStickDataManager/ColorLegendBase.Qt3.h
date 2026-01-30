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
#ifndef COLORLEGENDBASE_H
#define COLORLEGENDBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorLegendBase
{
public:
    Q3GroupBox *residueGroupBox;
    Q3Frame *residueColor3;
    Q3Frame *residueColor4;
    Q3Frame *residueColor10;
    Q3Frame *residueColor12;
    Q3Frame *residueColor5;
    Q3Frame *residueColor2;
    Q3Frame *residueColor11;
    Q3Frame *residueColor8;
    Q3Frame *residueColor9;
    Q3Frame *residueColor13;
    Q3Frame *residueColor14;
    Q3Frame *residueColor15;
    Q3Frame *residueColor16;
    Q3Frame *residueColor17;
    Q3Frame *residueColor18;
    Q3Frame *residueColor19;
    Q3Frame *residueColor20;
    Q3Frame *residueColor21;
    Q3Frame *residueColor22;
    Q3Frame *residueColor23;
    Q3Frame *residueColor24;
    Q3Frame *residueColor25;
    Q3Frame *residueColor27;
    Q3Frame *residueColor26;
    Q3Frame *residueColor28;
    Q3Frame *residueColor7;
    Q3Frame *residueColor1;
    QLabel *residueText1;
    QLabel *residueText2;
    QLabel *residueText3;
    QLabel *residueText4;
    QLabel *residueText5;
    QLabel *residueText6;
    Q3Frame *residueColor6;
    QLabel *residueText7;
    QLabel *residueText8;
    QLabel *residueText9;
    QLabel *residueText10;
    QLabel *residueText11;
    QLabel *residueText12;
    QLabel *residueText13;
    QLabel *residueText14;
    QLabel *residueText15;
    QLabel *residueText16;
    QLabel *residueText17;
    QLabel *residueText18;
    QLabel *residueText19;
    QLabel *residueText20;
    QLabel *residueText21;
    QLabel *residueText22;
    QLabel *residueText23;
    QLabel *residueText24;
    QLabel *residueText25;
    QLabel *residueText26;
    QLabel *residueText27;
    QLabel *residueText28;
    Q3Frame *residueColor0;
    QLabel *residueText0;
    Q3GroupBox *ssGroupBox;
    QLabel *ssText3;
    QLabel *ssText1;
    QLabel *ssText0;
    Q3Frame *ssColor3;
    Q3Frame *ssColor2;
    Q3Frame *ssColor1;
    Q3Frame *ssColor0;
    QLabel *ssText2;
    Q3GroupBox *moleculeGroupBox;
    Q3Frame *moleculeColorFrame;
    QLabel *moleculeText;

    void setupUi(QWidget *ColorLegendBase)
    {
        if (ColorLegendBase->objectName().isEmpty())
            ColorLegendBase->setObjectName(QString::fromUtf8("ColorLegendBase"));
        ColorLegendBase->resize(366, 350);
        residueGroupBox = new Q3GroupBox(ColorLegendBase);
        residueGroupBox->setObjectName(QString::fromUtf8("residueGroupBox"));
        residueGroupBox->setGeometry(QRect(10, 10, 180, 330));
        residueColor3 = new Q3Frame(residueGroupBox);
        residueColor3->setObjectName(QString::fromUtf8("residueColor3"));
        residueColor3->setGeometry(QRect(40, 80, 32, 16));
        residueColor3->setFrameShape(QFrame::Box);
        residueColor3->setFrameShadow(QFrame::Plain);
        residueColor4 = new Q3Frame(residueGroupBox);
        residueColor4->setObjectName(QString::fromUtf8("residueColor4"));
        residueColor4->setGeometry(QRect(40, 100, 32, 16));
        residueColor4->setFrameShape(QFrame::Box);
        residueColor4->setFrameShadow(QFrame::Plain);
        residueColor10 = new Q3Frame(residueGroupBox);
        residueColor10->setObjectName(QString::fromUtf8("residueColor10"));
        residueColor10->setGeometry(QRect(40, 220, 32, 16));
        residueColor10->setFrameShape(QFrame::Box);
        residueColor10->setFrameShadow(QFrame::Plain);
        residueColor12 = new Q3Frame(residueGroupBox);
        residueColor12->setObjectName(QString::fromUtf8("residueColor12"));
        residueColor12->setGeometry(QRect(40, 260, 32, 16));
        residueColor12->setFrameShape(QFrame::Box);
        residueColor12->setFrameShadow(QFrame::Plain);
        residueColor5 = new Q3Frame(residueGroupBox);
        residueColor5->setObjectName(QString::fromUtf8("residueColor5"));
        residueColor5->setGeometry(QRect(40, 120, 32, 16));
        residueColor5->setFrameShape(QFrame::Box);
        residueColor5->setFrameShadow(QFrame::Plain);
        residueColor2 = new Q3Frame(residueGroupBox);
        residueColor2->setObjectName(QString::fromUtf8("residueColor2"));
        residueColor2->setGeometry(QRect(40, 60, 32, 16));
        residueColor2->setFrameShape(QFrame::Box);
        residueColor2->setFrameShadow(QFrame::Plain);
        residueColor11 = new Q3Frame(residueGroupBox);
        residueColor11->setObjectName(QString::fromUtf8("residueColor11"));
        residueColor11->setGeometry(QRect(40, 240, 32, 16));
        residueColor11->setFrameShape(QFrame::Box);
        residueColor11->setFrameShadow(QFrame::Plain);
        residueColor8 = new Q3Frame(residueGroupBox);
        residueColor8->setObjectName(QString::fromUtf8("residueColor8"));
        residueColor8->setGeometry(QRect(40, 180, 32, 16));
        residueColor8->setFrameShape(QFrame::Box);
        residueColor8->setFrameShadow(QFrame::Plain);
        residueColor9 = new Q3Frame(residueGroupBox);
        residueColor9->setObjectName(QString::fromUtf8("residueColor9"));
        residueColor9->setGeometry(QRect(40, 200, 32, 16));
        residueColor9->setFrameShape(QFrame::Box);
        residueColor9->setFrameShadow(QFrame::Plain);
        residueColor13 = new Q3Frame(residueGroupBox);
        residueColor13->setObjectName(QString::fromUtf8("residueColor13"));
        residueColor13->setGeometry(QRect(40, 280, 32, 16));
        residueColor13->setFrameShape(QFrame::Box);
        residueColor13->setFrameShadow(QFrame::Plain);
        residueColor14 = new Q3Frame(residueGroupBox);
        residueColor14->setObjectName(QString::fromUtf8("residueColor14"));
        residueColor14->setGeometry(QRect(40, 300, 32, 16));
        residueColor14->setFrameShape(QFrame::Box);
        residueColor14->setFrameShadow(QFrame::Plain);
        residueColor15 = new Q3Frame(residueGroupBox);
        residueColor15->setObjectName(QString::fromUtf8("residueColor15"));
        residueColor15->setGeometry(QRect(130, 20, 32, 16));
        residueColor15->setFrameShape(QFrame::Box);
        residueColor15->setFrameShadow(QFrame::Plain);
        residueColor16 = new Q3Frame(residueGroupBox);
        residueColor16->setObjectName(QString::fromUtf8("residueColor16"));
        residueColor16->setGeometry(QRect(130, 40, 32, 16));
        residueColor16->setFrameShape(QFrame::Box);
        residueColor16->setFrameShadow(QFrame::Plain);
        residueColor17 = new Q3Frame(residueGroupBox);
        residueColor17->setObjectName(QString::fromUtf8("residueColor17"));
        residueColor17->setGeometry(QRect(130, 60, 32, 16));
        residueColor17->setFrameShape(QFrame::Box);
        residueColor17->setFrameShadow(QFrame::Plain);
        residueColor18 = new Q3Frame(residueGroupBox);
        residueColor18->setObjectName(QString::fromUtf8("residueColor18"));
        residueColor18->setGeometry(QRect(130, 80, 32, 16));
        residueColor18->setFrameShape(QFrame::Box);
        residueColor18->setFrameShadow(QFrame::Plain);
        residueColor19 = new Q3Frame(residueGroupBox);
        residueColor19->setObjectName(QString::fromUtf8("residueColor19"));
        residueColor19->setGeometry(QRect(130, 100, 32, 16));
        residueColor19->setFrameShape(QFrame::Box);
        residueColor19->setFrameShadow(QFrame::Plain);
        residueColor20 = new Q3Frame(residueGroupBox);
        residueColor20->setObjectName(QString::fromUtf8("residueColor20"));
        residueColor20->setGeometry(QRect(130, 120, 32, 16));
        residueColor20->setFrameShape(QFrame::Box);
        residueColor20->setFrameShadow(QFrame::Plain);
        residueColor21 = new Q3Frame(residueGroupBox);
        residueColor21->setObjectName(QString::fromUtf8("residueColor21"));
        residueColor21->setGeometry(QRect(130, 140, 32, 16));
        residueColor21->setFrameShape(QFrame::Box);
        residueColor21->setFrameShadow(QFrame::Plain);
        residueColor22 = new Q3Frame(residueGroupBox);
        residueColor22->setObjectName(QString::fromUtf8("residueColor22"));
        residueColor22->setGeometry(QRect(130, 160, 32, 16));
        residueColor22->setFrameShape(QFrame::Box);
        residueColor22->setFrameShadow(QFrame::Plain);
        residueColor23 = new Q3Frame(residueGroupBox);
        residueColor23->setObjectName(QString::fromUtf8("residueColor23"));
        residueColor23->setGeometry(QRect(130, 180, 32, 16));
        residueColor23->setFrameShape(QFrame::Box);
        residueColor23->setFrameShadow(QFrame::Plain);
        residueColor24 = new Q3Frame(residueGroupBox);
        residueColor24->setObjectName(QString::fromUtf8("residueColor24"));
        residueColor24->setGeometry(QRect(130, 200, 32, 16));
        residueColor24->setFrameShape(QFrame::Box);
        residueColor24->setFrameShadow(QFrame::Plain);
        residueColor25 = new Q3Frame(residueGroupBox);
        residueColor25->setObjectName(QString::fromUtf8("residueColor25"));
        residueColor25->setGeometry(QRect(130, 220, 32, 16));
        residueColor25->setFrameShape(QFrame::Box);
        residueColor25->setFrameShadow(QFrame::Plain);
        residueColor27 = new Q3Frame(residueGroupBox);
        residueColor27->setObjectName(QString::fromUtf8("residueColor27"));
        residueColor27->setGeometry(QRect(130, 260, 32, 16));
        residueColor27->setFrameShape(QFrame::Box);
        residueColor27->setFrameShadow(QFrame::Plain);
        residueColor26 = new Q3Frame(residueGroupBox);
        residueColor26->setObjectName(QString::fromUtf8("residueColor26"));
        residueColor26->setGeometry(QRect(130, 240, 32, 16));
        residueColor26->setFrameShape(QFrame::Box);
        residueColor26->setFrameShadow(QFrame::Plain);
        residueColor28 = new Q3Frame(residueGroupBox);
        residueColor28->setObjectName(QString::fromUtf8("residueColor28"));
        residueColor28->setGeometry(QRect(130, 280, 32, 16));
        residueColor28->setFrameShape(QFrame::Box);
        residueColor28->setFrameShadow(QFrame::Plain);
        residueColor7 = new Q3Frame(residueGroupBox);
        residueColor7->setObjectName(QString::fromUtf8("residueColor7"));
        residueColor7->setGeometry(QRect(40, 160, 32, 16));
        residueColor7->setFrameShape(QFrame::Box);
        residueColor7->setFrameShadow(QFrame::Plain);
        residueColor1 = new Q3Frame(residueGroupBox);
        residueColor1->setObjectName(QString::fromUtf8("residueColor1"));
        residueColor1->setGeometry(QRect(40, 40, 32, 16));
        residueColor1->setFrameShape(QFrame::Box);
        residueColor1->setFrameShadow(QFrame::Plain);
        residueText1 = new QLabel(residueGroupBox);
        residueText1->setObjectName(QString::fromUtf8("residueText1"));
        residueText1->setGeometry(QRect(10, 40, 30, 16));
        residueText1->setWordWrap(false);
        residueText2 = new QLabel(residueGroupBox);
        residueText2->setObjectName(QString::fromUtf8("residueText2"));
        residueText2->setGeometry(QRect(10, 60, 30, 16));
        residueText2->setWordWrap(false);
        residueText3 = new QLabel(residueGroupBox);
        residueText3->setObjectName(QString::fromUtf8("residueText3"));
        residueText3->setGeometry(QRect(10, 80, 30, 16));
        residueText3->setWordWrap(false);
        residueText4 = new QLabel(residueGroupBox);
        residueText4->setObjectName(QString::fromUtf8("residueText4"));
        residueText4->setGeometry(QRect(10, 100, 30, 16));
        residueText4->setWordWrap(false);
        residueText5 = new QLabel(residueGroupBox);
        residueText5->setObjectName(QString::fromUtf8("residueText5"));
        residueText5->setGeometry(QRect(10, 120, 30, 16));
        residueText5->setWordWrap(false);
        residueText6 = new QLabel(residueGroupBox);
        residueText6->setObjectName(QString::fromUtf8("residueText6"));
        residueText6->setGeometry(QRect(10, 140, 30, 16));
        residueText6->setWordWrap(false);
        residueColor6 = new Q3Frame(residueGroupBox);
        residueColor6->setObjectName(QString::fromUtf8("residueColor6"));
        residueColor6->setGeometry(QRect(40, 140, 32, 16));
        residueColor6->setFrameShape(QFrame::Box);
        residueColor6->setFrameShadow(QFrame::Plain);
        residueText7 = new QLabel(residueGroupBox);
        residueText7->setObjectName(QString::fromUtf8("residueText7"));
        residueText7->setGeometry(QRect(10, 160, 30, 16));
        residueText7->setWordWrap(false);
        residueText8 = new QLabel(residueGroupBox);
        residueText8->setObjectName(QString::fromUtf8("residueText8"));
        residueText8->setGeometry(QRect(10, 180, 30, 16));
        residueText8->setWordWrap(false);
        residueText9 = new QLabel(residueGroupBox);
        residueText9->setObjectName(QString::fromUtf8("residueText9"));
        residueText9->setGeometry(QRect(10, 200, 30, 16));
        residueText9->setWordWrap(false);
        residueText10 = new QLabel(residueGroupBox);
        residueText10->setObjectName(QString::fromUtf8("residueText10"));
        residueText10->setGeometry(QRect(10, 220, 30, 16));
        residueText10->setWordWrap(false);
        residueText11 = new QLabel(residueGroupBox);
        residueText11->setObjectName(QString::fromUtf8("residueText11"));
        residueText11->setGeometry(QRect(10, 240, 30, 16));
        residueText11->setWordWrap(false);
        residueText12 = new QLabel(residueGroupBox);
        residueText12->setObjectName(QString::fromUtf8("residueText12"));
        residueText12->setGeometry(QRect(10, 260, 30, 16));
        residueText12->setWordWrap(false);
        residueText13 = new QLabel(residueGroupBox);
        residueText13->setObjectName(QString::fromUtf8("residueText13"));
        residueText13->setGeometry(QRect(10, 280, 30, 16));
        residueText13->setWordWrap(false);
        residueText14 = new QLabel(residueGroupBox);
        residueText14->setObjectName(QString::fromUtf8("residueText14"));
        residueText14->setGeometry(QRect(10, 300, 30, 16));
        residueText14->setWordWrap(false);
        residueText15 = new QLabel(residueGroupBox);
        residueText15->setObjectName(QString::fromUtf8("residueText15"));
        residueText15->setGeometry(QRect(100, 20, 30, 16));
        residueText15->setWordWrap(false);
        residueText16 = new QLabel(residueGroupBox);
        residueText16->setObjectName(QString::fromUtf8("residueText16"));
        residueText16->setGeometry(QRect(100, 40, 30, 16));
        residueText16->setWordWrap(false);
        residueText17 = new QLabel(residueGroupBox);
        residueText17->setObjectName(QString::fromUtf8("residueText17"));
        residueText17->setGeometry(QRect(100, 60, 30, 16));
        residueText17->setWordWrap(false);
        residueText18 = new QLabel(residueGroupBox);
        residueText18->setObjectName(QString::fromUtf8("residueText18"));
        residueText18->setGeometry(QRect(100, 80, 30, 16));
        residueText18->setWordWrap(false);
        residueText19 = new QLabel(residueGroupBox);
        residueText19->setObjectName(QString::fromUtf8("residueText19"));
        residueText19->setGeometry(QRect(100, 100, 30, 16));
        residueText19->setWordWrap(false);
        residueText20 = new QLabel(residueGroupBox);
        residueText20->setObjectName(QString::fromUtf8("residueText20"));
        residueText20->setGeometry(QRect(100, 120, 30, 16));
        residueText20->setWordWrap(false);
        residueText21 = new QLabel(residueGroupBox);
        residueText21->setObjectName(QString::fromUtf8("residueText21"));
        residueText21->setGeometry(QRect(100, 140, 30, 16));
        residueText21->setWordWrap(false);
        residueText22 = new QLabel(residueGroupBox);
        residueText22->setObjectName(QString::fromUtf8("residueText22"));
        residueText22->setGeometry(QRect(100, 160, 30, 16));
        residueText22->setWordWrap(false);
        residueText23 = new QLabel(residueGroupBox);
        residueText23->setObjectName(QString::fromUtf8("residueText23"));
        residueText23->setGeometry(QRect(100, 180, 30, 16));
        residueText23->setWordWrap(false);
        residueText24 = new QLabel(residueGroupBox);
        residueText24->setObjectName(QString::fromUtf8("residueText24"));
        residueText24->setGeometry(QRect(100, 200, 30, 16));
        residueText24->setWordWrap(false);
        residueText25 = new QLabel(residueGroupBox);
        residueText25->setObjectName(QString::fromUtf8("residueText25"));
        residueText25->setGeometry(QRect(100, 220, 30, 16));
        residueText25->setWordWrap(false);
        residueText26 = new QLabel(residueGroupBox);
        residueText26->setObjectName(QString::fromUtf8("residueText26"));
        residueText26->setGeometry(QRect(100, 240, 30, 16));
        residueText26->setWordWrap(false);
        residueText27 = new QLabel(residueGroupBox);
        residueText27->setObjectName(QString::fromUtf8("residueText27"));
        residueText27->setGeometry(QRect(100, 260, 30, 16));
        residueText27->setWordWrap(false);
        residueText28 = new QLabel(residueGroupBox);
        residueText28->setObjectName(QString::fromUtf8("residueText28"));
        residueText28->setGeometry(QRect(100, 280, 30, 16));
        residueText28->setWordWrap(false);
        residueColor0 = new Q3Frame(residueGroupBox);
        residueColor0->setObjectName(QString::fromUtf8("residueColor0"));
        residueColor0->setGeometry(QRect(40, 20, 32, 16));
        residueColor0->setFrameShape(QFrame::Box);
        residueColor0->setFrameShadow(QFrame::Plain);
        residueText0 = new QLabel(residueGroupBox);
        residueText0->setObjectName(QString::fromUtf8("residueText0"));
        residueText0->setGeometry(QRect(10, 20, 30, 16));
        residueText0->setWordWrap(false);
        ssGroupBox = new Q3GroupBox(ColorLegendBase);
        ssGroupBox->setObjectName(QString::fromUtf8("ssGroupBox"));
        ssGroupBox->setGeometry(QRect(210, 10, 120, 110));
        ssText3 = new QLabel(ssGroupBox);
        ssText3->setObjectName(QString::fromUtf8("ssText3"));
        ssText3->setGeometry(QRect(10, 80, 50, 16));
        ssText3->setWordWrap(false);
        ssText1 = new QLabel(ssGroupBox);
        ssText1->setObjectName(QString::fromUtf8("ssText1"));
        ssText1->setGeometry(QRect(10, 40, 50, 16));
        ssText1->setWordWrap(false);
        ssText0 = new QLabel(ssGroupBox);
        ssText0->setObjectName(QString::fromUtf8("ssText0"));
        ssText0->setGeometry(QRect(10, 20, 50, 16));
        ssText0->setWordWrap(false);
        ssColor3 = new Q3Frame(ssGroupBox);
        ssColor3->setObjectName(QString::fromUtf8("ssColor3"));
        ssColor3->setGeometry(QRect(70, 80, 32, 16));
        ssColor3->setFrameShape(QFrame::Box);
        ssColor3->setFrameShadow(QFrame::Plain);
        ssColor2 = new Q3Frame(ssGroupBox);
        ssColor2->setObjectName(QString::fromUtf8("ssColor2"));
        ssColor2->setGeometry(QRect(70, 60, 32, 16));
        ssColor2->setFrameShape(QFrame::Box);
        ssColor2->setFrameShadow(QFrame::Plain);
        ssColor1 = new Q3Frame(ssGroupBox);
        ssColor1->setObjectName(QString::fromUtf8("ssColor1"));
        ssColor1->setGeometry(QRect(70, 40, 32, 16));
        ssColor1->setFrameShape(QFrame::Box);
        ssColor1->setFrameShadow(QFrame::Plain);
        ssColor0 = new Q3Frame(ssGroupBox);
        ssColor0->setObjectName(QString::fromUtf8("ssColor0"));
        ssColor0->setGeometry(QRect(70, 20, 32, 16));
        ssColor0->setFrameShape(QFrame::Box);
        ssColor0->setFrameShadow(QFrame::Plain);
        ssText2 = new QLabel(ssGroupBox);
        ssText2->setObjectName(QString::fromUtf8("ssText2"));
        ssText2->setGeometry(QRect(10, 60, 50, 16));
        ssText2->setWordWrap(false);
        moleculeGroupBox = new Q3GroupBox(ColorLegendBase);
        moleculeGroupBox->setObjectName(QString::fromUtf8("moleculeGroupBox"));
        moleculeGroupBox->setGeometry(QRect(210, 130, 120, 50));
        moleculeColorFrame = new Q3Frame(moleculeGroupBox);
        moleculeColorFrame->setObjectName(QString::fromUtf8("moleculeColorFrame"));
        moleculeColorFrame->setGeometry(QRect(70, 20, 32, 16));
        moleculeColorFrame->setFrameShape(QFrame::Box);
        moleculeColorFrame->setFrameShadow(QFrame::Plain);
        moleculeText = new QLabel(moleculeGroupBox);
        moleculeText->setObjectName(QString::fromUtf8("moleculeText"));
        moleculeText->setGeometry(QRect(10, 20, 60, 16));
        moleculeText->setWordWrap(false);

        retranslateUi(ColorLegendBase);

        QMetaObject::connectSlotsByName(ColorLegendBase);
    } // setupUi

    void retranslateUi(QWidget *ColorLegendBase)
    {
        ColorLegendBase->setWindowTitle(QApplication::translate("ColorLegendBase", "Color Legend", 0, QApplication::UnicodeUTF8));
        residueGroupBox->setTitle(QApplication::translate("ColorLegendBase", "Residue", 0, QApplication::UnicodeUTF8));
        residueText1->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText2->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText3->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText4->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText5->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText6->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText7->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText8->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText9->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText10->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText11->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText12->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText13->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText14->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText15->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText16->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText17->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText18->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText19->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText20->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText21->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText22->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText23->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText24->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText25->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText26->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText27->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText28->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        residueText0->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        ssGroupBox->setTitle(QApplication::translate("ColorLegendBase", "Secondary Structure", 0, QApplication::UnicodeUTF8));
        ssText3->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        ssText1->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        ssText0->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        ssText2->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        moleculeGroupBox->setTitle(QApplication::translate("ColorLegendBase", "Molecule", 0, QApplication::UnicodeUTF8));
        moleculeText->setText(QApplication::translate("ColorLegendBase", "---", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ColorLegendBase);
    } // retranslateUi

};

namespace Ui {
    class ColorLegendBase: public Ui_ColorLegendBase {};
} // namespace Ui

QT_END_NAMESPACE

class ColorLegendBase : public QWidget, public Ui::ColorLegendBase
{
    Q_OBJECT

public:
    ColorLegendBase(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0);
    ~ColorLegendBase();

protected slots:
    virtual void languageChange();

};

#endif // COLORLEGENDBASE_H
