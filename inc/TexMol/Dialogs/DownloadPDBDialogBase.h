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
#ifndef DOWNLOADPDBDIALOGBASE_H
#define DOWNLOADPDBDIALOGBASE_H

#include <qvariant.h>



#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DownloadPDBDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QHBoxLayout *hboxLayout1;
    QLineEdit *m_SavePDBLineEdit;
    QPushButton *m_SavePDBPushButton;
    QCheckBox *m_LoadPDBCheckBox;
    QHBoxLayout *hboxLayout2;
    QLineEdit *m_PDBNameLineEdit;
    QLabel *m_PDBNameTextLabel;

    void setupUi(QDialog *DownloadPDBDialogBase)
    {
        if (DownloadPDBDialogBase->objectName().isEmpty())
            DownloadPDBDialogBase->setObjectName(QString::fromUtf8("DownloadPDBDialogBase"));
        DownloadPDBDialogBase->resize(511, 149);
        DownloadPDBDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(DownloadPDBDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(DownloadPDBDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(DownloadPDBDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(DownloadPDBDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_SavePDBLineEdit = new QLineEdit(DownloadPDBDialogBase);
        m_SavePDBLineEdit->setObjectName(QString::fromUtf8("m_SavePDBLineEdit"));

        hboxLayout1->addWidget(m_SavePDBLineEdit);

        m_SavePDBPushButton = new QPushButton(DownloadPDBDialogBase);
        m_SavePDBPushButton->setObjectName(QString::fromUtf8("m_SavePDBPushButton"));

        hboxLayout1->addWidget(m_SavePDBPushButton);


        gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);

        m_LoadPDBCheckBox = new QCheckBox(DownloadPDBDialogBase);
        m_LoadPDBCheckBox->setObjectName(QString::fromUtf8("m_LoadPDBCheckBox"));
        m_LoadPDBCheckBox->setChecked(true);

        gridLayout->addWidget(m_LoadPDBCheckBox, 2, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_PDBNameLineEdit = new QLineEdit(DownloadPDBDialogBase);
        m_PDBNameLineEdit->setObjectName(QString::fromUtf8("m_PDBNameLineEdit"));

        hboxLayout2->addWidget(m_PDBNameLineEdit);

        m_PDBNameTextLabel = new QLabel(DownloadPDBDialogBase);
        m_PDBNameTextLabel->setObjectName(QString::fromUtf8("m_PDBNameTextLabel"));
        m_PDBNameTextLabel->setWordWrap(false);

        hboxLayout2->addWidget(m_PDBNameTextLabel);


        gridLayout->addLayout(hboxLayout2, 0, 0, 1, 1);

        QWidget::setTabOrder(m_PDBNameLineEdit, m_SavePDBLineEdit);
        QWidget::setTabOrder(m_SavePDBLineEdit, m_SavePDBPushButton);
        QWidget::setTabOrder(m_SavePDBPushButton, m_LoadPDBCheckBox);
        QWidget::setTabOrder(m_LoadPDBCheckBox, buttonHelp);
        QWidget::setTabOrder(buttonHelp, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);

        retranslateUi(DownloadPDBDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), DownloadPDBDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), DownloadPDBDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(DownloadPDBDialogBase);
    } // setupUi

    void retranslateUi(QDialog *DownloadPDBDialogBase)
    {
        DownloadPDBDialogBase->setWindowTitle(QCoreApplication::translate("DownloadPDBDialogBase", "Download PDB"));
        buttonHelp->setText(QCoreApplication::translate("DownloadPDBDialogBase", "&Help"));
        buttonHelp->setShortcut(QCoreApplication::translate("DownloadPDBDialogBase", "F1"));
        buttonOk->setText(QCoreApplication::translate("DownloadPDBDialogBase", "&OK"));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QCoreApplication::translate("DownloadPDBDialogBase", "&Cancel"));
        buttonCancel->setShortcut(QString());
        m_SavePDBLineEdit->setText(QCoreApplication::translate("DownloadPDBDialogBase", "101M.pdb"));
        m_SavePDBPushButton->setText(QCoreApplication::translate("DownloadPDBDialogBase", "Save"));
        m_LoadPDBCheckBox->setText(QCoreApplication::translate("DownloadPDBDialogBase", "Load PDB"));
        m_PDBNameLineEdit->setText(QCoreApplication::translate("DownloadPDBDialogBase", "101M"));
        m_PDBNameTextLabel->setText(QCoreApplication::translate("DownloadPDBDialogBase", "     PDB Name     "));
        Q_UNUSED(DownloadPDBDialogBase);
    } // retranslateUi

};

namespace Ui {
    class DownloadPDBDialogBase: public Ui_DownloadPDBDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class DownloadPDBDialogBase : public QDialog, public Ui::DownloadPDBDialogBase
{
    Q_OBJECT

public:
    DownloadPDBDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~DownloadPDBDialogBase() {}

protected slots:
    virtual void languageChange() {}

};

#endif // DOWNLOADPDBDIALOGBASE_H
