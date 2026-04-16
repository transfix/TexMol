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
#ifndef ANGLESDIALOG_H
#define ANGLESDIALOG_H

#include <qvariant.h>


#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_AnglesDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    Q3Frame *frame3;

    void setupUi(QDialog *AnglesDialog)
    {
        if (AnglesDialog->objectName().isEmpty())
            AnglesDialog->setObjectName(QString::fromUtf8("AnglesDialog"));
        AnglesDialog->resize(411, 432);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AnglesDialog->sizePolicy().hasHeightForWidth());
        AnglesDialog->setSizePolicy(sizePolicy);
        AnglesDialog->setMinimumSize(QSize(400, 400));
        AnglesDialog->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(AnglesDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(AnglesDialog);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(AnglesDialog);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

        frame3 = new Q3Frame(AnglesDialog);
        frame3->setObjectName(QString::fromUtf8("frame3"));
        frame3->setMinimumSize(QSize(200, 200));
        frame3->setFrameShape(QFrame::StyledPanel);
        frame3->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frame3, 0, 0, 1, 1);


        retranslateUi(AnglesDialog);
        QObject::connect(buttonOk, SIGNAL(clicked()), AnglesDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(AnglesDialog);
    } // setupUi

    void retranslateUi(QDialog *AnglesDialog)
    {
        AnglesDialog->setWindowTitle(QApplication::translate("AnglesDialog", "Ramachandran Plot", 0, QApplication::UnicodeUTF8));
        buttonHelp->setText(QApplication::translate("AnglesDialog", "&Help", 0, QApplication::UnicodeUTF8));
        buttonOk->setWindowTitle(QString());
        buttonOk->setText(QApplication::translate("AnglesDialog", "&Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AnglesDialog);
    } // retranslateUi

};

namespace Ui {
    class AnglesDialog: public Ui_AnglesDialog {};
} // namespace Ui

QT_END_NAMESPACE

class AnglesDialog : public QDialog, public Ui::AnglesDialog
{
    Q_OBJECT

public:
    AnglesDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~AnglesDialog();

protected slots:
    virtual void languageChange();

};

#endif // ANGLESDIALOG_H
