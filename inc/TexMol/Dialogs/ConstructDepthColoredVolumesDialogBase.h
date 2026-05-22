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
#ifndef CONSTRUCTDEPTHCOLOREDVOLUMESDIALOGBASE_H
#define CONSTRUCTDEPTHCOLOREDVOLUMESDIALOGBASE_H

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
#include "ColorTable.h"

QT_BEGIN_NAMESPACE

class Ui_ConstructDepthColoredVolumesDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLineEdit *m_RawVFileNameLineEdit;
    QPushButton *m_RawVPushButton;
    QHBoxLayout *hboxLayout1;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    CVCColorTable::ColorTable *m_ColorTable;
    QCheckBox *m_LoadDataCheckBox;
    QHBoxLayout *hboxLayout2;
    QLabel *m_InputVolumeFileNameLextLabel;
    QPushButton *m_VolumePushButton;

    void setupUi(QDialog *ConstructDepthColoredVolumesDialogBase)
    {
        if (ConstructDepthColoredVolumesDialogBase->objectName().isEmpty())
            ConstructDepthColoredVolumesDialogBase->setObjectName(QString::fromUtf8("ConstructDepthColoredVolumesDialogBase"));
        ConstructDepthColoredVolumesDialogBase->resize(511, 305);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConstructDepthColoredVolumesDialogBase->sizePolicy().hasHeightForWidth());
        ConstructDepthColoredVolumesDialogBase->setSizePolicy(sizePolicy);
        ConstructDepthColoredVolumesDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(ConstructDepthColoredVolumesDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_RawVFileNameLineEdit = new QLineEdit(ConstructDepthColoredVolumesDialogBase);
        m_RawVFileNameLineEdit->setObjectName(QString::fromUtf8("m_RawVFileNameLineEdit"));

        hboxLayout->addWidget(m_RawVFileNameLineEdit);

        m_RawVPushButton = new QPushButton(ConstructDepthColoredVolumesDialogBase);
        m_RawVPushButton->setObjectName(QString::fromUtf8("m_RawVPushButton"));

        hboxLayout->addWidget(m_RawVPushButton);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        buttonHelp = new QPushButton(ConstructDepthColoredVolumesDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout1->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(ConstructDepthColoredVolumesDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout1->addWidget(buttonOk);

        buttonCancel = new QPushButton(ConstructDepthColoredVolumesDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout1->addWidget(buttonCancel);

        gridLayout->addLayout(hboxLayout1, 4, 0, 1, 1);
/*Q3Err:CS
        m_ColorTable = new ColorTable(ConstructDepthColoredVolumesDialogBase);
*/
        m_ColorTable = new CVCColorTable::ColorTable();
        gridLayout->addWidget(m_ColorTable, 2, 0, 1, 1);
  	m_ColorTable->interactiveUpdates(true);
	m_ColorTable->update();
        
	m_LoadDataCheckBox = new QCheckBox(ConstructDepthColoredVolumesDialogBase);
        m_LoadDataCheckBox->setObjectName(QString::fromUtf8("m_LoadDataCheckBox"));
        m_LoadDataCheckBox->setChecked(true);

        gridLayout->addWidget(m_LoadDataCheckBox, 3, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_InputVolumeFileNameLextLabel = new QLabel(ConstructDepthColoredVolumesDialogBase);
        m_InputVolumeFileNameLextLabel->setObjectName(QString::fromUtf8("m_InputVolumeFileNameLextLabel"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(5));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_InputVolumeFileNameLextLabel->sizePolicy().hasHeightForWidth());
        m_InputVolumeFileNameLextLabel->setSizePolicy(sizePolicy1);
        m_InputVolumeFileNameLextLabel->setWordWrap(false);

        hboxLayout2->addWidget(m_InputVolumeFileNameLextLabel);

        m_VolumePushButton = new QPushButton(ConstructDepthColoredVolumesDialogBase);
        m_VolumePushButton->setObjectName(QString::fromUtf8("m_VolumePushButton"));
        m_VolumePushButton->setEnabled(false);

        hboxLayout2->addWidget(m_VolumePushButton);


        gridLayout->addLayout(hboxLayout2, 0, 0, 1, 1);


        retranslateUi(ConstructDepthColoredVolumesDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), ConstructDepthColoredVolumesDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), ConstructDepthColoredVolumesDialogBase, SLOT(reject()));
        QObject::connect(m_RawVPushButton, SIGNAL(clicked()), ConstructDepthColoredVolumesDialogBase, SLOT(outputRawVFileNameSlot()));

        QMetaObject::connectSlotsByName(ConstructDepthColoredVolumesDialogBase);

    } // setupUi

    void retranslateUi(QDialog *ConstructDepthColoredVolumesDialogBase)
    {
        ConstructDepthColoredVolumesDialogBase->setWindowTitle(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "Construct depth colored volumes"));
        m_RawVPushButton->setText(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "Output rawV file"));
        buttonHelp->setText(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "&Help"));
        buttonHelp->setShortcut(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "F1"));
        buttonOk->setText(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "&OK"));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "&Cancel"));
        buttonCancel->setShortcut(QString());
        m_LoadDataCheckBox->setText(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "Load new volume"));
        m_InputVolumeFileNameLextLabel->setText(QString());
        m_VolumePushButton->setText(QCoreApplication::translate("ConstructDepthColoredVolumesDialogBase", "Input volume file"));
        Q_UNUSED(ConstructDepthColoredVolumesDialogBase);
    } // retranslateUi


protected:
    enum IconID
    {
        image0_ID,
        unknown_ID
    };
    static QPixmap qt_get_icon(IconID id)
    {
    static const char* const image0_data[] = { 
"0 0 0 1"};


    switch (id) {
        case image0_ID: return QPixmap((const char**)image0_data);
        default: return QPixmap();
    } // switch
    } // icon

};

namespace Ui {
    class ConstructDepthColoredVolumesDialogBase: public Ui_ConstructDepthColoredVolumesDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class ConstructDepthColoredVolumesDialogBase : public QDialog, public Ui::ConstructDepthColoredVolumesDialogBase
{
    Q_OBJECT

public:
    ConstructDepthColoredVolumesDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~ConstructDepthColoredVolumesDialogBase() {}

public slots:
    virtual void inputFileNameSlot() {}
    virtual void outputRawVFileNameSlot() {}

protected slots:
    virtual void languageChange() {}

};

#endif // CONSTRUCTDEPTHCOLOREDVOLUMESDIALOGBASE_H
