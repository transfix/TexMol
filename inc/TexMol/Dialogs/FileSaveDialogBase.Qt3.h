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
#ifndef FILESAVEDIALOGBASE_H
#define FILESAVEDIALOGBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileSaveDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout1;
    QLineEdit *m_FileNameLineEdit;
    QPushButton *m_FileNamePushButton;
    QHBoxLayout *hboxLayout2;
    QComboBox *m_FileFormatComboBox;
    QLabel *m_FileFormatTextLabel;
    QCheckBox *m_SaveAllCheckBox;

    void setupUi(QDialog *FileSaveDialogBase)
    {
        if (FileSaveDialogBase->objectName().isEmpty())
            FileSaveDialogBase->setObjectName(QString::fromUtf8("FileSaveDialogBase"));
        FileSaveDialogBase->resize(511, 145);
        FileSaveDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(FileSaveDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(FileSaveDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(FileSaveDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(FileSaveDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_FileNameLineEdit = new QLineEdit(FileSaveDialogBase);
        m_FileNameLineEdit->setObjectName(QString::fromUtf8("m_FileNameLineEdit"));

        hboxLayout1->addWidget(m_FileNameLineEdit);

        m_FileNamePushButton = new QPushButton(FileSaveDialogBase);
        m_FileNamePushButton->setObjectName(QString::fromUtf8("m_FileNamePushButton"));

        hboxLayout1->addWidget(m_FileNamePushButton);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_FileFormatComboBox = new QComboBox(FileSaveDialogBase);
        m_FileFormatComboBox->setObjectName(QString::fromUtf8("m_FileFormatComboBox"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_FileFormatComboBox->sizePolicy().hasHeightForWidth());
        m_FileFormatComboBox->setSizePolicy(sizePolicy);

        hboxLayout2->addWidget(m_FileFormatComboBox);

        m_FileFormatTextLabel = new QLabel(FileSaveDialogBase);
        m_FileFormatTextLabel->setObjectName(QString::fromUtf8("m_FileFormatTextLabel"));
        m_FileFormatTextLabel->setWordWrap(false);

        hboxLayout2->addWidget(m_FileFormatTextLabel);


        vboxLayout->addLayout(hboxLayout2);

        m_SaveAllCheckBox = new QCheckBox(FileSaveDialogBase);
        m_SaveAllCheckBox->setObjectName(QString::fromUtf8("m_SaveAllCheckBox"));

        vboxLayout->addWidget(m_SaveAllCheckBox);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);


        retranslateUi(FileSaveDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), FileSaveDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), FileSaveDialogBase, SLOT(reject()));
        QObject::connect(m_FileNamePushButton, SIGNAL(clicked()), FileSaveDialogBase, SLOT(fileNameSlot()));

        QMetaObject::connectSlotsByName(FileSaveDialogBase);
    } // setupUi

    void retranslateUi(QDialog *FileSaveDialogBase)
    {
        FileSaveDialogBase->setWindowTitle(QApplication::translate("FileSaveDialogBase", "Save image", 0, QApplication::UnicodeUTF8));
        buttonHelp->setText(QApplication::translate("FileSaveDialogBase", "&Help", 0, QApplication::UnicodeUTF8));
        buttonHelp->setShortcut(QApplication::translate("FileSaveDialogBase", "F1", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("FileSaveDialogBase", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("FileSaveDialogBase", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        m_FileNamePushButton->setText(QApplication::translate("FileSaveDialogBase", "File name", 0, QApplication::UnicodeUTF8));
        m_FileFormatTextLabel->setText(QApplication::translate("FileSaveDialogBase", "File type", 0, QApplication::UnicodeUTF8));
        m_SaveAllCheckBox->setText(QApplication::translate("FileSaveDialogBase", "Save all split views", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FileSaveDialogBase);
    } // retranslateUi

};

namespace Ui {
    class FileSaveDialogBase: public Ui_FileSaveDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class FileSaveDialogBase : public QDialog, public Ui::FileSaveDialogBase
{
    Q_OBJECT

public:
    FileSaveDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~FileSaveDialogBase();

public slots:
    virtual void fileNameSlot();

protected slots:
    virtual void languageChange();

};

#endif // FILESAVEDIALOGBASE_H
