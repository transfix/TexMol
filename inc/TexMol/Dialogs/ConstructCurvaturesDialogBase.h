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
#ifndef CONSTRUCTCURVATURESDIALOGBASE_H
#define CONSTRUCTCURVATURESDIALOGBASE_H

#include <qvariant.h>


#include <QGroupBox>

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include <QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ConstructCurvaturesDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLineEdit *m_PDBFileNameLineEdit;
    QPushButton *m_PDBFIlePushButton;
    QHBoxLayout *hboxLayout1;
    QHBoxLayout *hboxLayout2;
    QSpinBox *m_Dim1SpinBox;
    QLabel *m_Dim1TextLabel;
    QHBoxLayout *hboxLayout3;
    QSpinBox *m_Dim2SpinBox;
    QLabel *m_Dim2TextLabel;
    QHBoxLayout *hboxLayout4;
    QSpinBox *m_Dim3SpinBox;
    QLabel *m_Dim3TextLabel;
    QHBoxLayout *hboxLayout5;
    QLabel *m_BlobbyTextLabel_2;
    QLineEdit *m_BlobbyLineEdit;
    QGroupBox *m_IsocontourChoiceButtonGroup;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout6;
    QRadioButton *m_IsocontourFileRadioButton;
    QLineEdit *m_IsocontourFileLineEdit;
    QPushButton *m_IsocontourFilePushButton;
    QHBoxLayout *hboxLayout7;
    QRadioButton *m_IsovalueRadioButton;
    QLineEdit *m_IsovalueLineEdit;
    QLabel *m_IsovalueTextLabel;
    QHBoxLayout *hboxLayout8;
    QHBoxLayout *hboxLayout9;
    QHBoxLayout *hboxLayout10;
    QLabel *m_GridDivisionsTextLabel;
    QLineEdit *m_NumberOfGridDivisionsLineEdit;
    QHBoxLayout *hboxLayout11;
    QLabel *m_MaxFunctionErrorTextLabel;
    QLineEdit *m_MaximumFunctionErrorLineEdit;
    QHBoxLayout *hboxLayout12;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QHBoxLayout *hboxLayout13;
    QLineEdit *m_GaussianCurvatureFileLineEdit;
    QPushButton *m_GaussianCurvaturePushButton;
    QHBoxLayout *hboxLayout14;
    QLineEdit *m_MeanCurvatureFileLineEdit;
    QPushButton *m_MeanCurvaturePushButton;
    QGroupBox *m_RadiusTypeButtonGroup;
    QGridLayout *gridLayout2;
    QRadioButton *m_AtomSolventEnlargedRadiusRadioButton;
    QRadioButton *m_AtomRadiusRadioButton;

    void setupUi(QDialog *ConstructCurvaturesDialogBase)
    {
        if (ConstructCurvaturesDialogBase->objectName().isEmpty())
            ConstructCurvaturesDialogBase->setObjectName(QString::fromUtf8("ConstructCurvaturesDialogBase"));
        ConstructCurvaturesDialogBase->resize(496, 385);
        ConstructCurvaturesDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(ConstructCurvaturesDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_PDBFileNameLineEdit = new QLineEdit(ConstructCurvaturesDialogBase);
        m_PDBFileNameLineEdit->setObjectName(QString::fromUtf8("m_PDBFileNameLineEdit"));

        hboxLayout->addWidget(m_PDBFileNameLineEdit);

        m_PDBFIlePushButton = new QPushButton(ConstructCurvaturesDialogBase);
        m_PDBFIlePushButton->setObjectName(QString::fromUtf8("m_PDBFIlePushButton"));

        hboxLayout->addWidget(m_PDBFIlePushButton);


        gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_Dim1SpinBox = new QSpinBox(ConstructCurvaturesDialogBase);
        m_Dim1SpinBox->setObjectName(QString::fromUtf8("m_Dim1SpinBox"));
        m_Dim1SpinBox->setMaximum(1025);
        m_Dim1SpinBox->setValue(64);

        hboxLayout2->addWidget(m_Dim1SpinBox);

        m_Dim1TextLabel = new QLabel(ConstructCurvaturesDialogBase);
        m_Dim1TextLabel->setObjectName(QString::fromUtf8("m_Dim1TextLabel"));
        m_Dim1TextLabel->setWordWrap(false);

        hboxLayout2->addWidget(m_Dim1TextLabel);


        hboxLayout1->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        m_Dim2SpinBox = new QSpinBox(ConstructCurvaturesDialogBase);
        m_Dim2SpinBox->setObjectName(QString::fromUtf8("m_Dim2SpinBox"));
        m_Dim2SpinBox->setMaximum(1025);
        m_Dim2SpinBox->setValue(64);

        hboxLayout3->addWidget(m_Dim2SpinBox);

        m_Dim2TextLabel = new QLabel(ConstructCurvaturesDialogBase);
        m_Dim2TextLabel->setObjectName(QString::fromUtf8("m_Dim2TextLabel"));
        m_Dim2TextLabel->setWordWrap(false);

        hboxLayout3->addWidget(m_Dim2TextLabel);


        hboxLayout1->addLayout(hboxLayout3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        m_Dim3SpinBox = new QSpinBox(ConstructCurvaturesDialogBase);
        m_Dim3SpinBox->setObjectName(QString::fromUtf8("m_Dim3SpinBox"));
        m_Dim3SpinBox->setMaximum(1025);
        m_Dim3SpinBox->setValue(64);

        hboxLayout4->addWidget(m_Dim3SpinBox);

        m_Dim3TextLabel = new QLabel(ConstructCurvaturesDialogBase);
        m_Dim3TextLabel->setObjectName(QString::fromUtf8("m_Dim3TextLabel"));
        m_Dim3TextLabel->setWordWrap(false);

        hboxLayout4->addWidget(m_Dim3TextLabel);


        hboxLayout1->addLayout(hboxLayout4);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(6);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        m_BlobbyTextLabel_2 = new QLabel(ConstructCurvaturesDialogBase);
        m_BlobbyTextLabel_2->setObjectName(QString::fromUtf8("m_BlobbyTextLabel_2"));
        m_BlobbyTextLabel_2->setWordWrap(false);

        hboxLayout5->addWidget(m_BlobbyTextLabel_2);

        m_BlobbyLineEdit = new QLineEdit(ConstructCurvaturesDialogBase);
        m_BlobbyLineEdit->setObjectName(QString::fromUtf8("m_BlobbyLineEdit"));

        hboxLayout5->addWidget(m_BlobbyLineEdit);


        hboxLayout1->addLayout(hboxLayout5);


        gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);

        m_IsocontourChoiceButtonGroup = new QGroupBox(ConstructCurvaturesDialogBase);
        m_IsocontourChoiceButtonGroup->setObjectName(QString::fromUtf8("m_IsocontourChoiceButtonGroup"));
            QVBoxLayout *_vbl1 = new QVBoxLayout(m_IsocontourChoiceButtonGroup);

            _vbl1->setSpacing(6);

            _vbl1->setContentsMargins(11, 11, 11, 11);

            gridLayout1 = new QGridLayout();

            _vbl1->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setSpacing(6);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        m_IsocontourFileRadioButton = new QRadioButton(m_IsocontourChoiceButtonGroup);
        m_IsocontourFileRadioButton->setObjectName(QString::fromUtf8("m_IsocontourFileRadioButton"));
        m_IsocontourFileRadioButton->setChecked(true);

        hboxLayout6->addWidget(m_IsocontourFileRadioButton);

        m_IsocontourFileLineEdit = new QLineEdit(m_IsocontourChoiceButtonGroup);
        m_IsocontourFileLineEdit->setObjectName(QString::fromUtf8("m_IsocontourFileLineEdit"));

        hboxLayout6->addWidget(m_IsocontourFileLineEdit);

        m_IsocontourFilePushButton = new QPushButton(m_IsocontourChoiceButtonGroup);
        m_IsocontourFilePushButton->setObjectName(QString::fromUtf8("m_IsocontourFilePushButton"));

        hboxLayout6->addWidget(m_IsocontourFilePushButton);


        gridLayout1->addLayout(hboxLayout6, 2, 0, 1, 1);

        hboxLayout7 = new QHBoxLayout();
        hboxLayout7->setSpacing(6);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        m_IsovalueRadioButton = new QRadioButton(m_IsocontourChoiceButtonGroup);
        m_IsovalueRadioButton->setObjectName(QString::fromUtf8("m_IsovalueRadioButton"));
        m_IsovalueRadioButton->setChecked(false);

        hboxLayout7->addWidget(m_IsovalueRadioButton);

        m_IsovalueLineEdit = new QLineEdit(m_IsocontourChoiceButtonGroup);
        m_IsovalueLineEdit->setObjectName(QString::fromUtf8("m_IsovalueLineEdit"));
        m_IsovalueLineEdit->setEnabled(false);

        hboxLayout7->addWidget(m_IsovalueLineEdit);

        m_IsovalueTextLabel = new QLabel(m_IsocontourChoiceButtonGroup);
        m_IsovalueTextLabel->setObjectName(QString::fromUtf8("m_IsovalueTextLabel"));
        m_IsovalueTextLabel->setEnabled(false);
        m_IsovalueTextLabel->setWordWrap(false);

        hboxLayout7->addWidget(m_IsovalueTextLabel);


        gridLayout1->addLayout(hboxLayout7, 0, 0, 1, 1);

        hboxLayout8 = new QHBoxLayout();
        hboxLayout8->setSpacing(6);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));

        gridLayout1->addLayout(hboxLayout8, 1, 0, 1, 1);


        gridLayout->addWidget(m_IsocontourChoiceButtonGroup, 2, 0, 1, 1);

        hboxLayout9 = new QHBoxLayout();
        hboxLayout9->setSpacing(6);
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        hboxLayout10 = new QHBoxLayout();
        hboxLayout10->setSpacing(6);
        hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
        m_GridDivisionsTextLabel = new QLabel(ConstructCurvaturesDialogBase);
        m_GridDivisionsTextLabel->setObjectName(QString::fromUtf8("m_GridDivisionsTextLabel"));
        m_GridDivisionsTextLabel->setWordWrap(false);

        hboxLayout10->addWidget(m_GridDivisionsTextLabel);

        m_NumberOfGridDivisionsLineEdit = new QLineEdit(ConstructCurvaturesDialogBase);
        m_NumberOfGridDivisionsLineEdit->setObjectName(QString::fromUtf8("m_NumberOfGridDivisionsLineEdit"));

        hboxLayout10->addWidget(m_NumberOfGridDivisionsLineEdit);


        hboxLayout9->addLayout(hboxLayout10);

        hboxLayout11 = new QHBoxLayout();
        hboxLayout11->setSpacing(6);
        hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
        m_MaxFunctionErrorTextLabel = new QLabel(ConstructCurvaturesDialogBase);
        m_MaxFunctionErrorTextLabel->setObjectName(QString::fromUtf8("m_MaxFunctionErrorTextLabel"));
        m_MaxFunctionErrorTextLabel->setWordWrap(false);

        hboxLayout11->addWidget(m_MaxFunctionErrorTextLabel);

        m_MaximumFunctionErrorLineEdit = new QLineEdit(ConstructCurvaturesDialogBase);
        m_MaximumFunctionErrorLineEdit->setObjectName(QString::fromUtf8("m_MaximumFunctionErrorLineEdit"));

        hboxLayout11->addWidget(m_MaximumFunctionErrorLineEdit);


        hboxLayout9->addLayout(hboxLayout11);


        gridLayout->addLayout(hboxLayout9, 3, 0, 1, 1);

        hboxLayout12 = new QHBoxLayout();
        hboxLayout12->setSpacing(6);
        hboxLayout12->setContentsMargins(0, 0, 0, 0);
        hboxLayout12->setObjectName(QString::fromUtf8("hboxLayout12"));
        buttonHelp = new QPushButton(ConstructCurvaturesDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout12->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout12->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(ConstructCurvaturesDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout12->addWidget(buttonOk);

        buttonCancel = new QPushButton(ConstructCurvaturesDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout12->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout12, 7, 0, 1, 1);

        hboxLayout13 = new QHBoxLayout();
        hboxLayout13->setSpacing(6);
        hboxLayout13->setObjectName(QString::fromUtf8("hboxLayout13"));
        m_GaussianCurvatureFileLineEdit = new QLineEdit(ConstructCurvaturesDialogBase);
        m_GaussianCurvatureFileLineEdit->setObjectName(QString::fromUtf8("m_GaussianCurvatureFileLineEdit"));

        hboxLayout13->addWidget(m_GaussianCurvatureFileLineEdit);

        m_GaussianCurvaturePushButton = new QPushButton(ConstructCurvaturesDialogBase);
        m_GaussianCurvaturePushButton->setObjectName(QString::fromUtf8("m_GaussianCurvaturePushButton"));

        hboxLayout13->addWidget(m_GaussianCurvaturePushButton);


        gridLayout->addLayout(hboxLayout13, 6, 0, 1, 1);

        hboxLayout14 = new QHBoxLayout();
        hboxLayout14->setSpacing(6);
        hboxLayout14->setObjectName(QString::fromUtf8("hboxLayout14"));
        m_MeanCurvatureFileLineEdit = new QLineEdit(ConstructCurvaturesDialogBase);
        m_MeanCurvatureFileLineEdit->setObjectName(QString::fromUtf8("m_MeanCurvatureFileLineEdit"));

        hboxLayout14->addWidget(m_MeanCurvatureFileLineEdit);

        m_MeanCurvaturePushButton = new QPushButton(ConstructCurvaturesDialogBase);
        m_MeanCurvaturePushButton->setObjectName(QString::fromUtf8("m_MeanCurvaturePushButton"));

        hboxLayout14->addWidget(m_MeanCurvaturePushButton);


        gridLayout->addLayout(hboxLayout14, 5, 0, 1, 1);

        m_RadiusTypeButtonGroup = new QGroupBox(ConstructCurvaturesDialogBase);
        m_RadiusTypeButtonGroup->setObjectName(QString::fromUtf8("m_RadiusTypeButtonGroup"));
            QVBoxLayout *_vbl2 = new QVBoxLayout(m_RadiusTypeButtonGroup);

            _vbl2->setSpacing(6);

            _vbl2->setContentsMargins(11, 11, 11, 11);

            gridLayout2 = new QGridLayout();

            _vbl2->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        m_AtomSolventEnlargedRadiusRadioButton = new QRadioButton(m_RadiusTypeButtonGroup);
        m_AtomSolventEnlargedRadiusRadioButton->setObjectName(QString::fromUtf8("m_AtomSolventEnlargedRadiusRadioButton"));

        gridLayout2->addWidget(m_AtomSolventEnlargedRadiusRadioButton, 1, 0, 1, 1);

        m_AtomRadiusRadioButton = new QRadioButton(m_RadiusTypeButtonGroup);
        m_AtomRadiusRadioButton->setObjectName(QString::fromUtf8("m_AtomRadiusRadioButton"));
        m_AtomRadiusRadioButton->setChecked(true);

        gridLayout2->addWidget(m_AtomRadiusRadioButton, 0, 0, 1, 1);


        gridLayout->addWidget(m_RadiusTypeButtonGroup, 4, 0, 1, 1);

        QWidget::setTabOrder(m_PDBFileNameLineEdit, m_PDBFIlePushButton);
        QWidget::setTabOrder(m_PDBFIlePushButton, m_Dim1SpinBox);
        QWidget::setTabOrder(m_Dim1SpinBox, m_Dim2SpinBox);
        QWidget::setTabOrder(m_Dim2SpinBox, m_Dim3SpinBox);
        QWidget::setTabOrder(m_Dim3SpinBox, m_BlobbyLineEdit);
        QWidget::setTabOrder(m_BlobbyLineEdit, m_IsovalueRadioButton);
        QWidget::setTabOrder(m_IsovalueRadioButton, m_IsovalueLineEdit);
        QWidget::setTabOrder(m_IsovalueLineEdit, m_IsocontourFileRadioButton);
        QWidget::setTabOrder(m_IsocontourFileRadioButton, m_IsocontourFileLineEdit);
        QWidget::setTabOrder(m_IsocontourFileLineEdit, m_IsocontourFilePushButton);
        QWidget::setTabOrder(m_IsocontourFilePushButton, m_NumberOfGridDivisionsLineEdit);
        QWidget::setTabOrder(m_NumberOfGridDivisionsLineEdit, m_MaximumFunctionErrorLineEdit);
        QWidget::setTabOrder(m_MaximumFunctionErrorLineEdit, m_MeanCurvatureFileLineEdit);
        QWidget::setTabOrder(m_MeanCurvatureFileLineEdit, m_MeanCurvaturePushButton);
        QWidget::setTabOrder(m_MeanCurvaturePushButton, m_GaussianCurvatureFileLineEdit);
        QWidget::setTabOrder(m_GaussianCurvatureFileLineEdit, m_GaussianCurvaturePushButton);
        QWidget::setTabOrder(m_GaussianCurvaturePushButton, buttonHelp);
        QWidget::setTabOrder(buttonHelp, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);

        retranslateUi(ConstructCurvaturesDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), ConstructCurvaturesDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), ConstructCurvaturesDialogBase, SLOT(reject()));
        QObject::connect(m_PDBFIlePushButton, SIGNAL(clicked()), ConstructCurvaturesDialogBase, SLOT(inputPDBFileSlot()));
        QObject::connect(m_IsocontourFilePushButton, SIGNAL(clicked()), ConstructCurvaturesDialogBase, SLOT(inputIsocontourFileSlot()));
        QObject::connect(m_MeanCurvaturePushButton, SIGNAL(clicked()), ConstructCurvaturesDialogBase, SLOT(outputMeanCurvatureFileSlot()));
        QObject::connect(m_GaussianCurvaturePushButton, SIGNAL(clicked()), ConstructCurvaturesDialogBase, SLOT(outputGaussianCurvatureFileSlot()));
        QObject::connect(m_IsovalueRadioButton, SIGNAL(toggled(bool)), ConstructCurvaturesDialogBase, SLOT(isovalueRadioButtonSelectedSlot(bool)));
        QObject::connect(m_IsocontourFileRadioButton, SIGNAL(toggled(bool)), ConstructCurvaturesDialogBase, SLOT(isocontourRadioButtonSelectedSlot(bool)));

        QMetaObject::connectSlotsByName(ConstructCurvaturesDialogBase);
    } // setupUi

    void retranslateUi(QDialog *ConstructCurvaturesDialogBase)
    {
        ConstructCurvaturesDialogBase->setWindowTitle(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Construct curvatures"));
        m_PDBFIlePushButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "PDB or PQR file"));
        m_Dim1TextLabel->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Dim 1"));
        m_Dim2TextLabel->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Dim 2"));
        m_Dim3TextLabel->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Dim 3"));
        m_BlobbyTextLabel_2->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Gaussian shape param"));
        m_BlobbyLineEdit->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "-2.6"));
        m_IsocontourChoiceButtonGroup->setTitle(QString());
        m_IsocontourFileRadioButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Read isocontour"));
        m_IsocontourFilePushButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Isocontour file"));
        m_IsovalueRadioButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Use isovalue"));
        m_IsovalueLineEdit->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "1.0"));
        m_IsovalueTextLabel->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Isovalue"));
        m_GridDivisionsTextLabel->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Number of grid divisions"));
        m_NumberOfGridDivisionsLineEdit->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "10"));
        m_MaxFunctionErrorTextLabel->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Max. func. error"));
        m_MaximumFunctionErrorLineEdit->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "0.1"));
        buttonHelp->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "&Help"));
        buttonHelp->setShortcut(QCoreApplication::translate("ConstructCurvaturesDialogBase", "F1"));
        buttonOk->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "&OK"));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "&Cancel"));
        buttonCancel->setShortcut(QString());
        m_GaussianCurvaturePushButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Gaussian curvature mesh"));
        m_MeanCurvaturePushButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Mean curvature mesh"));
        m_RadiusTypeButtonGroup->setTitle(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Radius type"));
        m_AtomSolventEnlargedRadiusRadioButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "Solvent enlarged"));
        m_AtomRadiusRadioButton->setText(QCoreApplication::translate("ConstructCurvaturesDialogBase", "VDV Radius"));
        Q_UNUSED(ConstructCurvaturesDialogBase);
    } // retranslateUi

};

namespace Ui {
    class ConstructCurvaturesDialogBase: public Ui_ConstructCurvaturesDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class ConstructCurvaturesDialogBase : public QDialog, public Ui::ConstructCurvaturesDialogBase
{
    Q_OBJECT

public:
    ConstructCurvaturesDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~ConstructCurvaturesDialogBase() {}

public slots:
    virtual void inputPDBFileSlot() {}
    virtual void inputIsocontourFileSlot() {}
    virtual void outputMeanCurvatureFileSlot() {}
    virtual void outputGaussianCurvatureFileSlot() {}
    virtual void isovalueRadioButtonSelectedSlot(bool) {}
    virtual void isocontourRadioButtonSelectedSlot(bool) {}
    virtual void outputVolumeFileNameSlot() {}
    virtual void outputCurvatureFileSlot() {}

protected slots:
    virtual void languageChange() {}

};

#endif // CONSTRUCTCURVATURESDIALOGBASE_H
