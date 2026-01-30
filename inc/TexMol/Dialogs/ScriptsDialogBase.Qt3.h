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
#ifndef SCRIPTSDIALOGBASE_H
#define SCRIPTSDIALOGBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3TextEdit>
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

class Ui_ScriptsDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    Q3TextEdit *m_CommandsTextEdit;
    QHBoxLayout *hboxLayout1;
    QPushButton *m_LoadPushButton;
    QPushButton *m_SavePushButton;
    QSpacerItem *spacer3;
    QPushButton *m_ExecutePushButton;

    void setupUi(QDialog *ScriptsDialogBase)
    {
        if (ScriptsDialogBase->objectName().isEmpty())
            ScriptsDialogBase->setObjectName(QString::fromUtf8("ScriptsDialogBase"));
        ScriptsDialogBase->resize(511, 433);
        ScriptsDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(ScriptsDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(ScriptsDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(ScriptsDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(ScriptsDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 2, 0, 1, 1);

        m_CommandsTextEdit = new Q3TextEdit(ScriptsDialogBase);
        m_CommandsTextEdit->setObjectName(QString::fromUtf8("m_CommandsTextEdit"));

        gridLayout->addWidget(m_CommandsTextEdit, 0, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_LoadPushButton = new QPushButton(ScriptsDialogBase);
        m_LoadPushButton->setObjectName(QString::fromUtf8("m_LoadPushButton"));

        hboxLayout1->addWidget(m_LoadPushButton);

        m_SavePushButton = new QPushButton(ScriptsDialogBase);
        m_SavePushButton->setObjectName(QString::fromUtf8("m_SavePushButton"));

        hboxLayout1->addWidget(m_SavePushButton);

        spacer3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer3);

        m_ExecutePushButton = new QPushButton(ScriptsDialogBase);
        m_ExecutePushButton->setObjectName(QString::fromUtf8("m_ExecutePushButton"));

        hboxLayout1->addWidget(m_ExecutePushButton);


        gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);


        retranslateUi(ScriptsDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), ScriptsDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), ScriptsDialogBase, SLOT(reject()));
        QObject::connect(m_ExecutePushButton, SIGNAL(clicked()), ScriptsDialogBase, SLOT(executeSlot()));
        QObject::connect(m_LoadPushButton, SIGNAL(clicked()), ScriptsDialogBase, SLOT(loadSlot()));
        QObject::connect(m_SavePushButton, SIGNAL(clicked()), ScriptsDialogBase, SLOT(saveSlot()));

        QMetaObject::connectSlotsByName(ScriptsDialogBase);
    } // setupUi

    void retranslateUi(QDialog *ScriptsDialogBase)
    {
        ScriptsDialogBase->setWindowTitle(QApplication::translate("ScriptsDialogBase", "Scripts", 0, QApplication::UnicodeUTF8));
        buttonHelp->setText(QApplication::translate("ScriptsDialogBase", "&Help", 0, QApplication::UnicodeUTF8));
        buttonHelp->setShortcut(QApplication::translate("ScriptsDialogBase", "F1", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("ScriptsDialogBase", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("ScriptsDialogBase", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        m_LoadPushButton->setText(QApplication::translate("ScriptsDialogBase", "Load", 0, QApplication::UnicodeUTF8));
        m_SavePushButton->setText(QApplication::translate("ScriptsDialogBase", "Save", 0, QApplication::UnicodeUTF8));
        m_ExecutePushButton->setText(QApplication::translate("ScriptsDialogBase", "Execute", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ScriptsDialogBase);
    } // retranslateUi

};

namespace Ui {
    class ScriptsDialogBase: public Ui_ScriptsDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class ScriptsDialogBase : public QDialog, public Ui::ScriptsDialogBase
{
    Q_OBJECT

public:
    ScriptsDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~ScriptsDialogBase();

public slots:
    virtual void executeSlot();
    virtual void loadSlot();
    virtual void saveSlot();

protected slots:
    virtual void languageChange();

};

#endif // SCRIPTSDIALOGBASE_H
