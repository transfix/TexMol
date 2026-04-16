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


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_setViewingParametersDialogBase
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

    void setupUi(QDialog *setViewingParametersDialogBase)
    {
        if (setViewingParametersDialogBase->objectName().isEmpty())
            setViewingParametersDialogBase->setObjectName(QString::fromUtf8("setViewingParametersDialogBase"));
        setViewingParametersDialogBase->resize(511, 142);
        setViewingParametersDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(setViewingParametersDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(setViewingParametersDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(setViewingParametersDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(setViewingParametersDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        translationXLineEdit = new QLineEdit(setViewingParametersDialogBase);
        translationXLineEdit->setObjectName(QString::fromUtf8("translationXLineEdit"));

        hboxLayout1->addWidget(translationXLineEdit);

        translationYLineEdit = new QLineEdit(setViewingParametersDialogBase);
        translationYLineEdit->setObjectName(QString::fromUtf8("translationYLineEdit"));

        hboxLayout1->addWidget(translationYLineEdit);

        translationZLineEdit = new QLineEdit(setViewingParametersDialogBase);
        translationZLineEdit->setObjectName(QString::fromUtf8("translationZLineEdit"));

        hboxLayout1->addWidget(translationZLineEdit);

        translationWLineEdit = new QLineEdit(setViewingParametersDialogBase);
        translationWLineEdit->setObjectName(QString::fromUtf8("translationWLineEdit"));

        hboxLayout1->addWidget(translationWLineEdit);

        translationTextLabel = new QLabel(setViewingParametersDialogBase);
        translationTextLabel->setObjectName(QString::fromUtf8("translationTextLabel"));
        translationTextLabel->setWordWrap(false);

        hboxLayout1->addWidget(translationTextLabel);


        gridLayout->addLayout(hboxLayout1, 0, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        orientationXLineEdit = new QLineEdit(setViewingParametersDialogBase);
        orientationXLineEdit->setObjectName(QString::fromUtf8("orientationXLineEdit"));

        hboxLayout2->addWidget(orientationXLineEdit);

        orientationYLineEdit = new QLineEdit(setViewingParametersDialogBase);
        orientationYLineEdit->setObjectName(QString::fromUtf8("orientationYLineEdit"));

        hboxLayout2->addWidget(orientationYLineEdit);

        orientationZLineEdit = new QLineEdit(setViewingParametersDialogBase);
        orientationZLineEdit->setObjectName(QString::fromUtf8("orientationZLineEdit"));

        hboxLayout2->addWidget(orientationZLineEdit);

        orientationWLineEdit = new QLineEdit(setViewingParametersDialogBase);
        orientationWLineEdit->setObjectName(QString::fromUtf8("orientationWLineEdit"));

        hboxLayout2->addWidget(orientationWLineEdit);

        orientationTextLabel = new QLabel(setViewingParametersDialogBase);
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
        windowSizeLineEdit = new QLineEdit(setViewingParametersDialogBase);
        windowSizeLineEdit->setObjectName(QString::fromUtf8("windowSizeLineEdit"));

        hboxLayout4->addWidget(windowSizeLineEdit);

        windowSizeTextLabel = new QLabel(setViewingParametersDialogBase);
        windowSizeTextLabel->setObjectName(QString::fromUtf8("windowSizeTextLabel"));
        windowSizeTextLabel->setWordWrap(false);

        hboxLayout4->addWidget(windowSizeTextLabel);


        hboxLayout3->addLayout(hboxLayout4);

        spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacer2);


        gridLayout->addLayout(hboxLayout3, 2, 0, 1, 1);


        retranslateUi(setViewingParametersDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), setViewingParametersDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), setViewingParametersDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(setViewingParametersDialogBase);
    } // setupUi

    void retranslateUi(QDialog *setViewingParametersDialogBase)
    {
        setViewingParametersDialogBase->setWindowTitle(QApplication::translate("setViewingParametersDialogBase", "Set viewing parameters", 0, QApplication::UnicodeUTF8));
        buttonHelp->setText(QApplication::translate("setViewingParametersDialogBase", "&Help", 0, QApplication::UnicodeUTF8));
        buttonHelp->setShortcut(QApplication::translate("setViewingParametersDialogBase", "F1", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("setViewingParametersDialogBase", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("setViewingParametersDialogBase", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        translationXLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "0", 0, QApplication::UnicodeUTF8));
        translationYLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "0", 0, QApplication::UnicodeUTF8));
        translationZLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "0", 0, QApplication::UnicodeUTF8));
        translationWLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "1", 0, QApplication::UnicodeUTF8));
        translationTextLabel->setText(QApplication::translate("setViewingParametersDialogBase", "Translation", 0, QApplication::UnicodeUTF8));
        orientationXLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "1", 0, QApplication::UnicodeUTF8));
        orientationYLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "0", 0, QApplication::UnicodeUTF8));
        orientationZLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "0", 0, QApplication::UnicodeUTF8));
        orientationWLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "0", 0, QApplication::UnicodeUTF8));
        orientationTextLabel->setText(QApplication::translate("setViewingParametersDialogBase", "Orientation", 0, QApplication::UnicodeUTF8));
        windowSizeLineEdit->setText(QApplication::translate("setViewingParametersDialogBase", "4", 0, QApplication::UnicodeUTF8));
        windowSizeTextLabel->setText(QApplication::translate("setViewingParametersDialogBase", "Window size", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(setViewingParametersDialogBase);
    } // retranslateUi

};

namespace Ui {
    class setViewingParametersDialogBase: public Ui_setViewingParametersDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class setViewingParametersDialogBase : public QDialog, public Ui::setViewingParametersDialogBase
{
    Q_OBJECT

public:
    setViewingParametersDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~setViewingParametersDialogBase();

protected slots:
    virtual void languageChange();

};

#endif // SETVIEWINGPARAMETERSDIALOGBASE_H
