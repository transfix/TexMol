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
#ifndef SETVIEWINGPARAMETERSDIALOGBASE_H
#define SETVIEWINGPARAMETERSDIALOGBASE_H

#include <qvariant.h>



#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_SetViewingParametersDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QHBoxLayout *hboxLayout1;
    QLineEdit *translationXLineEdit;
    QLineEdit *translationYLineEdit;
    QLineEdit *translationZLineEdit;
    QLineEdit *translationWLineEdit;
    QLabel *translationTextLabel;
    QHBoxLayout *hboxLayout2;
    QLineEdit *orientationXLineEdit;
    QLineEdit *orientationYLineEdit;
    QLineEdit *orientationZLineEdit;
    QLineEdit *orientationWLineEdit;
    QLabel *orientationTextLabel;
    QHBoxLayout *hboxLayout3;
    QHBoxLayout *hboxLayout4;
    QLineEdit *windowSizeLineEdit;
    QLabel *windowSizeTextLabel;
    QSpacerItem *spacer2;

    void setupUi(QDialog *SetViewingParametersDialogBase)
    {
        if (SetViewingParametersDialogBase->objectName().isEmpty())
            SetViewingParametersDialogBase->setObjectName(QString::fromUtf8("SetViewingParametersDialogBase"));
        SetViewingParametersDialogBase->resize(511, 142);
        SetViewingParametersDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(SetViewingParametersDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(SetViewingParametersDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(SetViewingParametersDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(SetViewingParametersDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        translationXLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        translationXLineEdit->setObjectName(QString::fromUtf8("translationXLineEdit"));

        hboxLayout1->addWidget(translationXLineEdit);

        translationYLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        translationYLineEdit->setObjectName(QString::fromUtf8("translationYLineEdit"));

        hboxLayout1->addWidget(translationYLineEdit);

        translationZLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        translationZLineEdit->setObjectName(QString::fromUtf8("translationZLineEdit"));

        hboxLayout1->addWidget(translationZLineEdit);

        translationWLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        translationWLineEdit->setObjectName(QString::fromUtf8("translationWLineEdit"));

        hboxLayout1->addWidget(translationWLineEdit);

        translationTextLabel = new QLabel(SetViewingParametersDialogBase);
        translationTextLabel->setObjectName(QString::fromUtf8("translationTextLabel"));
        translationTextLabel->setWordWrap(false);

        hboxLayout1->addWidget(translationTextLabel);


        gridLayout->addLayout(hboxLayout1, 0, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        orientationXLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        orientationXLineEdit->setObjectName(QString::fromUtf8("orientationXLineEdit"));

        hboxLayout2->addWidget(orientationXLineEdit);

        orientationYLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        orientationYLineEdit->setObjectName(QString::fromUtf8("orientationYLineEdit"));

        hboxLayout2->addWidget(orientationYLineEdit);

        orientationZLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        orientationZLineEdit->setObjectName(QString::fromUtf8("orientationZLineEdit"));

        hboxLayout2->addWidget(orientationZLineEdit);

        orientationWLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        orientationWLineEdit->setObjectName(QString::fromUtf8("orientationWLineEdit"));

        hboxLayout2->addWidget(orientationWLineEdit);

        orientationTextLabel = new QLabel(SetViewingParametersDialogBase);
        orientationTextLabel->setObjectName(QString::fromUtf8("orientationTextLabel"));
        orientationTextLabel->setWordWrap(false);

        hboxLayout2->addWidget(orientationTextLabel);


        gridLayout->addLayout(hboxLayout2, 1, 0, 1, 1);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        windowSizeLineEdit = new QLineEdit(SetViewingParametersDialogBase);
        windowSizeLineEdit->setObjectName(QString::fromUtf8("windowSizeLineEdit"));

        hboxLayout4->addWidget(windowSizeLineEdit);

        windowSizeTextLabel = new QLabel(SetViewingParametersDialogBase);
        windowSizeTextLabel->setObjectName(QString::fromUtf8("windowSizeTextLabel"));
        windowSizeTextLabel->setWordWrap(false);

        hboxLayout4->addWidget(windowSizeTextLabel);


        hboxLayout3->addLayout(hboxLayout4);

        spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacer2);


        gridLayout->addLayout(hboxLayout3, 2, 0, 1, 1);


        retranslateUi(SetViewingParametersDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), SetViewingParametersDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), SetViewingParametersDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(SetViewingParametersDialogBase);
    } // setupUi

    void retranslateUi(QDialog *SetViewingParametersDialogBase)
    {
        SetViewingParametersDialogBase->setWindowTitle(QCoreApplication::translate("SetViewingParametersDialogBase", "Set viewing parameters"));
        buttonHelp->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "&Help"));
        buttonHelp->setShortcut(QCoreApplication::translate("SetViewingParametersDialogBase", "F1"));
        buttonOk->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "&OK"));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "&Cancel"));
        buttonCancel->setShortcut(QString());
        translationXLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "0"));
        translationYLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "0"));
        translationZLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "0"));
        translationWLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "1"));
        translationTextLabel->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "Translation"));
        orientationXLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "1"));
        orientationYLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "0"));
        orientationZLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "0"));
        orientationWLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "0"));
        orientationTextLabel->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "Orientation"));
        windowSizeLineEdit->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "4"));
        windowSizeTextLabel->setText(QCoreApplication::translate("SetViewingParametersDialogBase", "Window size"));
        Q_UNUSED(SetViewingParametersDialogBase);
    } // retranslateUi

};

namespace Ui {
    class SetViewingParametersDialogBase: public Ui_SetViewingParametersDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class SetViewingParametersDialogBase : public QDialog, public Ui::SetViewingParametersDialogBase
{
    Q_OBJECT

public:
    SetViewingParametersDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~SetViewingParametersDialogBase() {}

protected slots:
    virtual void languageChange() {}

};

#endif // SETVIEWINGPARAMETERSDIALOGBASE_H
