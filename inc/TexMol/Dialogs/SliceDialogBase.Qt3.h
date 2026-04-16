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
#ifndef SLICEDIALOGBASE_H
#define SLICEDIALOGBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SliceDialogBase
{
public:
    QWidget *Layout1;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *SliceDialogBase)
    {
        if (SliceDialogBase->objectName().isEmpty())
            SliceDialogBase->setObjectName(QString::fromUtf8("SliceDialogBase"));
        SliceDialogBase->resize(511, 282);
        SliceDialogBase->setSizeGripEnabled(true);
        Layout1 = new QWidget(SliceDialogBase);
        Layout1->setObjectName(QString::fromUtf8("Layout1"));
        Layout1->setGeometry(QRect(20, 240, 476, 33));
        hboxLayout = new QHBoxLayout(Layout1);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        buttonHelp = new QPushButton(Layout1);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(Layout1);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(Layout1);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        retranslateUi(SliceDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), SliceDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), SliceDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(SliceDialogBase);
    } // setupUi

    void retranslateUi(QDialog *SliceDialogBase)
    {
        SliceDialogBase->setWindowTitle(QApplication::translate("SliceDialogBase", "2D Slice parameterization", 0, QApplication::UnicodeUTF8));
        buttonHelp->setText(QApplication::translate("SliceDialogBase", "&Help", 0, QApplication::UnicodeUTF8));
        buttonHelp->setShortcut(QApplication::translate("SliceDialogBase", "F1", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("SliceDialogBase", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("SliceDialogBase", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        Q_UNUSED(SliceDialogBase);
    } // retranslateUi

};

namespace Ui {
    class SliceDialogBase: public Ui_SliceDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class SliceDialogBase : public QDialog, public Ui::SliceDialogBase
{
    Q_OBJECT

public:
    SliceDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~SliceDialogBase();

protected slots:
    virtual void languageChange();

};

#endif // SLICEDIALOGBASE_H
