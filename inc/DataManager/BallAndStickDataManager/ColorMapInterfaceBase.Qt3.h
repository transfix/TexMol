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
#ifndef COLORMAPINTERFACEBASE_H
#define COLORMAPINTERFACEBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ColorMapInterfaceBase
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    Q3GroupBox *m_MainGroupBox;
    QPushButton *m_ClearColorMapPushButton;

    void setupUi(QDialog *ColorMapInterfaceBase)
    {
        if (ColorMapInterfaceBase->objectName().isEmpty())
            ColorMapInterfaceBase->setObjectName(QString::fromUtf8("ColorMapInterfaceBase"));
        ColorMapInterfaceBase->resize(450, 450);
        ColorMapInterfaceBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(ColorMapInterfaceBase);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        Horizontal_Spacing2 = new QSpacerItem(225, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(Horizontal_Spacing2, 1, 1, 1, 1);

        buttonOk = new QPushButton(ColorMapInterfaceBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        gridLayout->addWidget(buttonOk, 1, 2, 1, 1);

        buttonCancel = new QPushButton(ColorMapInterfaceBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        gridLayout->addWidget(buttonCancel, 1, 3, 1, 1);

        m_MainGroupBox = new Q3GroupBox(ColorMapInterfaceBase);
        m_MainGroupBox->setObjectName(QString::fromUtf8("m_MainGroupBox"));

        gridLayout->addWidget(m_MainGroupBox, 0, 0, 1, 4);

        m_ClearColorMapPushButton = new QPushButton(ColorMapInterfaceBase);
        m_ClearColorMapPushButton->setObjectName(QString::fromUtf8("m_ClearColorMapPushButton"));

        gridLayout->addWidget(m_ClearColorMapPushButton, 1, 0, 1, 1);


        retranslateUi(ColorMapInterfaceBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), ColorMapInterfaceBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), ColorMapInterfaceBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(ColorMapInterfaceBase);
    } // setupUi

    void retranslateUi(QDialog *ColorMapInterfaceBase)
    {
        ColorMapInterfaceBase->setWindowTitle(QApplication::translate("ColorMapInterfaceBase", "Color Map Interface", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("ColorMapInterfaceBase", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("ColorMapInterfaceBase", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        m_MainGroupBox->setTitle(QString());
        m_ClearColorMapPushButton->setText(QApplication::translate("ColorMapInterfaceBase", "Clear color map", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ColorMapInterfaceBase);
    } // retranslateUi

};

namespace Ui {
    class ColorMapInterfaceBase: public Ui_ColorMapInterfaceBase {};
} // namespace Ui

QT_END_NAMESPACE

class ColorMapInterfaceBase : public QDialog, public Ui::ColorMapInterfaceBase
{
    Q_OBJECT

public:
    ColorMapInterfaceBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~ColorMapInterfaceBase();

protected slots:
    virtual void languageChange();

};

#endif // COLORMAPINTERFACEBASE_H
