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
#ifndef CONSTRUCTVOLUMESDIALOGBASE_H
#define CONSTRUCTVOLUMESDIALOGBASE_H

#include <qvariant.h>


#include <QGroupBox>
#include <QGroupBox>

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
#include <QRadioButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConstructVolumesDialogBase
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QSpinBox *m_Dim1SpinBox;
    QLabel *m_Dim1TextLabel;
    QHBoxLayout *hboxLayout1;
    QSpinBox *m_Dim2SpinBox;
    QLabel *m_Dim2TextLabel;
    QHBoxLayout *hboxLayout2;
    QSpinBox *m_Dim3SpinBox;
    QLabel *m_Dim3TextLabel;
    QLineEdit *m_BlobbyLineEdit;
    QLabel *m_BlobbyTextLabel;
    QGroupBox *m_RadiusTypeButtonGroup;
    QGridLayout *gridLayout1;
    QRadioButton *m_AtomSolventEnlargedRadiusRadioButton;
    QRadioButton *m_AtomRadiusRadioButton;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QHBoxLayout *hboxLayout4;
    QCheckBox *m_UseHardCutOffCheckBox;
    QGroupBox *m_RawVGroupBox;
    QGridLayout *gridLayout2;
    QCheckBox *m_RawVCheckBox;
    QHBoxLayout *hboxLayout5;
    QLineEdit *m_ColormapFileLineEdit;
    QPushButton *m_ColorMapFilePushButton;
    QGroupBox *m_ColoringTypeButtonGroup;
    QGridLayout *gridLayout3;
    QRadioButton *m_ResidueRadioButton;
    QRadioButton *m_SSRadioButton;
    QRadioButton *m_AtomRadioButton;
    QRadioButton *m_ChainRadioButton;
    QRadioButton *m_ProteinRadioButton;
    QRadioButton *m_CollectionRadioButton;
    QGroupBox *functionTypeButtonGroup;
    QGridLayout *gridLayout4;
    QGroupBox *m_HydrophobicityTypeGroupBox;
    QGridLayout *gridLayout5;
    QRadioButton *m_PerAtomHydrophobicityRadioButton;
    QRadioButton *m_PerResidueHydrophobicityRadioButton;
    QGridLayout *gridLayout6;

    QRadioButton *m_ElectrostaticPotentialRadioButton;
    QRadioButton *electronDensityRadioButton;
    QRadioButton *hydrophobicityRadioButton;

    void setupUi(QDialog *ConstructVolumesDialogBase)
    {
        if (ConstructVolumesDialogBase->objectName().isEmpty())
            ConstructVolumesDialogBase->setObjectName(QString::fromUtf8("ConstructVolumesDialogBase"));
        ConstructVolumesDialogBase->resize(472, 441);
        ConstructVolumesDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(ConstructVolumesDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_Dim1SpinBox = new QSpinBox(ConstructVolumesDialogBase);
        m_Dim1SpinBox->setObjectName(QString::fromUtf8("m_Dim1SpinBox"));
        m_Dim1SpinBox->setMaximum(1025);
        m_Dim1SpinBox->setValue(128);

        hboxLayout->addWidget(m_Dim1SpinBox);

        m_Dim1TextLabel = new QLabel(ConstructVolumesDialogBase);
        m_Dim1TextLabel->setObjectName(QString::fromUtf8("m_Dim1TextLabel"));
        m_Dim1TextLabel->setWordWrap(false);

        hboxLayout->addWidget(m_Dim1TextLabel);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_Dim2SpinBox = new QSpinBox(ConstructVolumesDialogBase);
        m_Dim2SpinBox->setObjectName(QString::fromUtf8("m_Dim2SpinBox"));
        m_Dim2SpinBox->setMaximum(1025);
        m_Dim2SpinBox->setValue(128);

        hboxLayout1->addWidget(m_Dim2SpinBox);

        m_Dim2TextLabel = new QLabel(ConstructVolumesDialogBase);
        m_Dim2TextLabel->setObjectName(QString::fromUtf8("m_Dim2TextLabel"));
        m_Dim2TextLabel->setWordWrap(false);

        hboxLayout1->addWidget(m_Dim2TextLabel);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_Dim3SpinBox = new QSpinBox(ConstructVolumesDialogBase);
        m_Dim3SpinBox->setObjectName(QString::fromUtf8("m_Dim3SpinBox"));
        m_Dim3SpinBox->setMaximum(1025);
        m_Dim3SpinBox->setValue(128);

        hboxLayout2->addWidget(m_Dim3SpinBox);

        m_Dim3TextLabel = new QLabel(ConstructVolumesDialogBase);
        m_Dim3TextLabel->setObjectName(QString::fromUtf8("m_Dim3TextLabel"));
        m_Dim3TextLabel->setWordWrap(false);

        hboxLayout2->addWidget(m_Dim3TextLabel);


        vboxLayout->addLayout(hboxLayout2);


        gridLayout->addLayout(vboxLayout, 0, 1, 1, 2);

        m_BlobbyLineEdit = new QLineEdit(ConstructVolumesDialogBase);
        m_BlobbyLineEdit->setObjectName(QString::fromUtf8("m_BlobbyLineEdit"));

        gridLayout->addWidget(m_BlobbyLineEdit, 1, 1, 1, 1);

        m_BlobbyTextLabel = new QLabel(ConstructVolumesDialogBase);
        m_BlobbyTextLabel->setObjectName(QString::fromUtf8("m_BlobbyTextLabel"));
        m_BlobbyTextLabel->setWordWrap(false);

        gridLayout->addWidget(m_BlobbyTextLabel, 1, 2, 1, 1);

        m_RadiusTypeButtonGroup = new QGroupBox(ConstructVolumesDialogBase);
        m_RadiusTypeButtonGroup->setObjectName(QString::fromUtf8("m_RadiusTypeButtonGroup"));
            QVBoxLayout *_vbl1 = new QVBoxLayout(m_RadiusTypeButtonGroup);

            _vbl1->setSpacing(6);

            _vbl1->setContentsMargins(11, 11, 11, 11);

            gridLayout1 = new QGridLayout();

            _vbl1->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        m_AtomSolventEnlargedRadiusRadioButton = new QRadioButton(m_RadiusTypeButtonGroup);
        m_AtomSolventEnlargedRadiusRadioButton->setObjectName(QString::fromUtf8("m_AtomSolventEnlargedRadiusRadioButton"));

        gridLayout1->addWidget(m_AtomSolventEnlargedRadiusRadioButton, 1, 0, 1, 1);

        m_AtomRadiusRadioButton = new QRadioButton(m_RadiusTypeButtonGroup);
        m_AtomRadiusRadioButton->setObjectName(QString::fromUtf8("m_AtomRadiusRadioButton"));
        m_AtomRadiusRadioButton->setChecked(true);

        gridLayout1->addWidget(m_AtomRadiusRadioButton, 0, 0, 1, 1);


        gridLayout->addWidget(m_RadiusTypeButtonGroup, 2, 1, 1, 2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(ConstructVolumesDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout3->addWidget(buttonOk);

        buttonCancel = new QPushButton(ConstructVolumesDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout3->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout3, 5, 0, 1, 3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        m_UseHardCutOffCheckBox = new QCheckBox(ConstructVolumesDialogBase);
        m_UseHardCutOffCheckBox->setObjectName(QString::fromUtf8("m_UseHardCutOffCheckBox"));

        hboxLayout4->addWidget(m_UseHardCutOffCheckBox);


        gridLayout->addLayout(hboxLayout4, 4, 0, 1, 3);

        m_RawVGroupBox = new QGroupBox(ConstructVolumesDialogBase);
        m_RawVGroupBox->setObjectName(QString::fromUtf8("m_RawVGroupBox"));
            QVBoxLayout *_vbl2 = new QVBoxLayout(m_RawVGroupBox);

            _vbl2->setSpacing(6);

            _vbl2->setContentsMargins(11, 11, 11, 11);

            gridLayout2 = new QGridLayout();

            _vbl2->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        m_RawVCheckBox = new QCheckBox(m_RawVGroupBox);
        m_RawVCheckBox->setObjectName(QString::fromUtf8("m_RawVCheckBox"));

        gridLayout2->addWidget(m_RawVCheckBox, 0, 0, 1, 1);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(6);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        m_ColormapFileLineEdit = new QLineEdit(m_RawVGroupBox);
        m_ColormapFileLineEdit->setObjectName(QString::fromUtf8("m_ColormapFileLineEdit"));
        m_ColormapFileLineEdit->setEnabled(false);

        hboxLayout5->addWidget(m_ColormapFileLineEdit);

        m_ColorMapFilePushButton = new QPushButton(m_RawVGroupBox);
        m_ColorMapFilePushButton->setObjectName(QString::fromUtf8("m_ColorMapFilePushButton"));
        m_ColorMapFilePushButton->setEnabled(false);

        hboxLayout5->addWidget(m_ColorMapFilePushButton);


        gridLayout2->addLayout(hboxLayout5, 1, 0, 1, 2);

        m_ColoringTypeButtonGroup = new QGroupBox(m_RawVGroupBox);
        m_ColoringTypeButtonGroup->setObjectName(QString::fromUtf8("m_ColoringTypeButtonGroup"));
        m_ColoringTypeButtonGroup->setEnabled(false);
            QVBoxLayout *_vbl3 = new QVBoxLayout(m_ColoringTypeButtonGroup);

            _vbl3->setSpacing(6);

            _vbl3->setContentsMargins(11, 11, 11, 11);

            gridLayout3 = new QGridLayout();

            _vbl3->addLayout(gridLayout3);
        gridLayout3->setAlignment(Qt::AlignTop);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        m_ResidueRadioButton = new QRadioButton(m_ColoringTypeButtonGroup);
        m_ResidueRadioButton->setObjectName(QString::fromUtf8("m_ResidueRadioButton"));

        gridLayout3->addWidget(m_ResidueRadioButton, 1, 0, 1, 1);

        m_SSRadioButton = new QRadioButton(m_ColoringTypeButtonGroup);
        m_SSRadioButton->setObjectName(QString::fromUtf8("m_SSRadioButton"));

        gridLayout3->addWidget(m_SSRadioButton, 2, 0, 1, 1);

        m_AtomRadioButton = new QRadioButton(m_ColoringTypeButtonGroup);
        m_AtomRadioButton->setObjectName(QString::fromUtf8("m_AtomRadioButton"));
        m_AtomRadioButton->setChecked(true);

        gridLayout3->addWidget(m_AtomRadioButton, 0, 0, 1, 1);

        m_ChainRadioButton = new QRadioButton(m_ColoringTypeButtonGroup);
        m_ChainRadioButton->setObjectName(QString::fromUtf8("m_ChainRadioButton"));

        gridLayout3->addWidget(m_ChainRadioButton, 0, 1, 1, 1);

        m_ProteinRadioButton = new QRadioButton(m_ColoringTypeButtonGroup);
        m_ProteinRadioButton->setObjectName(QString::fromUtf8("m_ProteinRadioButton"));

        gridLayout3->addWidget(m_ProteinRadioButton, 1, 1, 1, 1);

        m_CollectionRadioButton = new QRadioButton(m_ColoringTypeButtonGroup);
        m_CollectionRadioButton->setObjectName(QString::fromUtf8("m_CollectionRadioButton"));

        gridLayout3->addWidget(m_CollectionRadioButton, 2, 1, 1, 1);


        gridLayout2->addWidget(m_ColoringTypeButtonGroup, 0, 1, 1, 1);


        gridLayout->addWidget(m_RawVGroupBox, 3, 0, 1, 3);

        functionTypeButtonGroup = new QGroupBox(ConstructVolumesDialogBase);
        functionTypeButtonGroup->setObjectName(QString::fromUtf8("functionTypeButtonGroup"));
            QVBoxLayout *_vbl4 = new QVBoxLayout(functionTypeButtonGroup);

            _vbl4->setSpacing(6);

            _vbl4->setContentsMargins(11, 11, 11, 11);

            gridLayout4 = new QGridLayout();

            _vbl4->addLayout(gridLayout4);
        gridLayout4->setAlignment(Qt::AlignTop);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        m_HydrophobicityTypeGroupBox = new QGroupBox(functionTypeButtonGroup);
        m_HydrophobicityTypeGroupBox->setObjectName(QString::fromUtf8("m_HydrophobicityTypeGroupBox"));
            QVBoxLayout *_vbl5 = new QVBoxLayout(m_HydrophobicityTypeGroupBox);

            _vbl5->setSpacing(6);

            _vbl5->setContentsMargins(11, 11, 11, 11);

            gridLayout5 = new QGridLayout();

            _vbl5->addLayout(gridLayout5);
        gridLayout5->setAlignment(Qt::AlignTop);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        m_PerAtomHydrophobicityRadioButton = new QRadioButton(m_HydrophobicityTypeGroupBox);
        m_PerAtomHydrophobicityRadioButton->setObjectName(QString::fromUtf8("m_PerAtomHydrophobicityRadioButton"));
        m_PerAtomHydrophobicityRadioButton->setChecked(true);

        gridLayout5->addWidget(m_PerAtomHydrophobicityRadioButton, 0, 0, 1, 1);

        m_PerResidueHydrophobicityRadioButton = new QRadioButton(m_HydrophobicityTypeGroupBox);
        m_PerResidueHydrophobicityRadioButton->setObjectName(QString::fromUtf8("m_PerResidueHydrophobicityRadioButton"));

        gridLayout5->addWidget(m_PerResidueHydrophobicityRadioButton, 1, 0, 1, 1);


        gridLayout4->addWidget(m_HydrophobicityTypeGroupBox, 1, 0, 1, 1);

        gridLayout6 = new QGridLayout();
        gridLayout6->setSpacing(6);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        m_ElectrostaticPotentialRadioButton = new QRadioButton(functionTypeButtonGroup);
        m_ElectrostaticPotentialRadioButton->setObjectName(QString::fromUtf8("m_ElectrostaticPotentialRadioButton"));

        gridLayout6->addWidget(m_ElectrostaticPotentialRadioButton, 2, 0, 1, 1);

        electronDensityRadioButton = new QRadioButton(functionTypeButtonGroup);
        electronDensityRadioButton->setObjectName(QString::fromUtf8("electronDensityRadioButton"));
        electronDensityRadioButton->setChecked(true);

        gridLayout6->addWidget(electronDensityRadioButton, 0, 0, 1, 1);

        hydrophobicityRadioButton = new QRadioButton(functionTypeButtonGroup);
        hydrophobicityRadioButton->setObjectName(QString::fromUtf8("hydrophobicityRadioButton"));

        gridLayout6->addWidget(hydrophobicityRadioButton, 1, 0, 1, 1);


        gridLayout4->addLayout(gridLayout6, 0, 0, 1, 1);


        gridLayout->addWidget(functionTypeButtonGroup, 0, 0, 3, 1);


        QWidget::setTabOrder(electronDensityRadioButton, hydrophobicityRadioButton);
        QWidget::setTabOrder(hydrophobicityRadioButton, m_Dim1SpinBox);
        QWidget::setTabOrder(m_Dim1SpinBox, m_Dim2SpinBox);
        QWidget::setTabOrder(m_Dim2SpinBox, m_Dim3SpinBox);
        QWidget::setTabOrder(m_Dim3SpinBox, m_BlobbyLineEdit);
        QWidget::setTabOrder(m_BlobbyLineEdit, m_RawVCheckBox);
        QWidget::setTabOrder(m_RawVCheckBox, m_AtomRadioButton);
        QWidget::setTabOrder(m_AtomRadioButton, m_ColormapFileLineEdit);
        QWidget::setTabOrder(m_ColormapFileLineEdit, m_ColorMapFilePushButton);
        QWidget::setTabOrder(m_ColorMapFilePushButton, m_UseHardCutOffCheckBox);
        QWidget::setTabOrder(m_UseHardCutOffCheckBox, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);

        retranslateUi(ConstructVolumesDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), ConstructVolumesDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), ConstructVolumesDialogBase, SLOT(reject()));
        QObject::connect(m_ColorMapFilePushButton, SIGNAL(clicked()), ConstructVolumesDialogBase, SLOT(colorMapFileSlot()));
        QObject::connect(m_RawVCheckBox, SIGNAL(toggled(bool)), ConstructVolumesDialogBase, SLOT(constructRawVSlot(bool)));

        QObject::connect(functionTypeButtonGroup, SIGNAL(clicked(int)), ConstructVolumesDialogBase, SLOT(functionTypeChangedSlot(int)));
       
	QMetaObject::connectSlotsByName(ConstructVolumesDialogBase);
    } // setupUi

    void retranslateUi(QDialog *ConstructVolumesDialogBase)
    {
        ConstructVolumesDialogBase->setWindowTitle(QCoreApplication::translate("ConstructVolumesDialogBase", "ConstructVolumesDialogBase"));
        m_Dim1TextLabel->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Dim 1"));
        m_Dim2TextLabel->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Dim 2"));
        m_Dim3TextLabel->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Dim 3"));
        m_BlobbyLineEdit->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "-2.3"));
        m_BlobbyTextLabel->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Gaussian shape param"));
        m_RadiusTypeButtonGroup->setTitle(QCoreApplication::translate("ConstructVolumesDialogBase", "Radius type"));
        m_AtomSolventEnlargedRadiusRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Solvent enlarged"));
        m_AtomRadiusRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "VDV Radius"));
        buttonOk->setWindowTitle(QString());
        buttonOk->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "&OK"));
        buttonOk->setShortcut(QCoreApplication::translate("ConstructVolumesDialogBase", "Alt+O"));
        buttonCancel->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "&Cancel"));
        m_UseHardCutOffCheckBox->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Use hard cutoff"));
        m_RawVGroupBox->setTitle(QCoreApplication::translate("ConstructVolumesDialogBase", "RawV parameters"));
        m_RawVCheckBox->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Construct RawV"));
        m_ColorMapFilePushButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Colormap file"));
        m_ColoringTypeButtonGroup->setTitle(QCoreApplication::translate("ConstructVolumesDialogBase", "Color level"));
        m_ResidueRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Residues"));
        m_SSRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "SS"));
        m_AtomRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Atoms"));
        m_ChainRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Chain"));
        m_ProteinRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Protein"));
        m_CollectionRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Collection"));
        functionTypeButtonGroup->setTitle(QCoreApplication::translate("ConstructVolumesDialogBase", "Function type"));
        m_HydrophobicityTypeGroupBox->setTitle(QCoreApplication::translate("ConstructVolumesDialogBase", "Hydrophobicity type"));
        m_PerAtomHydrophobicityRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Atom"));
        m_PerResidueHydrophobicityRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Residue"));
        m_ElectrostaticPotentialRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Electrostatic potential "));
        electronDensityRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Electron density"));
        hydrophobicityRadioButton->setText(QCoreApplication::translate("ConstructVolumesDialogBase", "Hydrophobicity"));
        Q_UNUSED(ConstructVolumesDialogBase);
    } // retranslateUi

};

namespace Ui {
    class ConstructVolumesDialogBase: public Ui_ConstructVolumesDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class ConstructVolumesDialogBase : public QDialog, public Ui::ConstructVolumesDialogBase
{
    Q_OBJECT

public:
    ConstructVolumesDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~ConstructVolumesDialogBase() {}

public slots:
    virtual void outputVolumeFileSlot() {}
    virtual void colorMapFileSlot() {}
    virtual void constructRawVSlot(bool) {}
    virtual void functionTypeChangedSlot(int) {}
    virtual void outputSurfaceFileSlot() {}

protected slots:
    virtual void languageChange() {}

};

#endif // CONSTRUCTVOLUMESDIALOGBASE_H
