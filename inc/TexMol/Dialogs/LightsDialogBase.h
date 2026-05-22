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
#ifndef LIGHTSDIALOGBASE_H
#define LIGHTSDIALOGBASE_H

#include <qvariant.h>


#include <QGroupBox>

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_LightsDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QGroupBox *m_AttenuationGroupBox;
    QGridLayout *gridLayout1;
    QLabel *m_ConstantAttenuationTextLabel;
    QLabel *m_LinearAttenuationTextLabel;
    QLabel *m_QuadraticAttenuationTextLabel;
    QLineEdit *m_ConstantAttenuationLineEdit;
    QLineEdit *m_LinearAttenuationLineEdit;
    QLineEdit *m_QuadraticAttenuationLineEdit;
    QSpacerItem *spacer3;
    QGroupBox *m_SpotLightGroupBox;
    QGridLayout *gridLayout2;
    QHBoxLayout *hboxLayout1;
    QLabel *m_SpotLightDirectionTextLabel;
    QLineEdit *m_XSpotLightDirectionLineEdit;
    QLineEdit *m_YSpotLightDirectionLineEdit;
    QLineEdit *m_ZSpotLightDirectionLineEdit;
    QHBoxLayout *hboxLayout2;
    QLabel *m_SpotLightExponentTextLabel;
    QLineEdit *m_SpotLightExponentLineEdit;
    QLabel *m_SpotLightCutOffTextLabel;
    QLineEdit *m_SpotLightCutOffLineEdit;
    QGroupBox *m_ColorsGroupBox;
    QGridLayout *gridLayout3;
    QHBoxLayout *hboxLayout3;
    QLineEdit *m_RedAmbientLineEdit;
    QLineEdit *m_GreenAmbientLineEdit;
    QLineEdit *m_BlueAmbientLineEdit;
    QLineEdit *m_AlphaAmbientLineEdit;
    QPushButton *m_AmbientPushButton;
    QHBoxLayout *hboxLayout4;
    QLineEdit *m_RedDiffuseLineEdit;
    QLineEdit *m_GreenDiffuseLineEdit;
    QLineEdit *m_BlueDiffuseLineEdit;
    QLineEdit *m_AlphaDiffuseLineEdit;
    QPushButton *m_DiffusePushButton;
    QHBoxLayout *hboxLayout5;
    QLineEdit *m_RedSpecularLineEdit;
    QLineEdit *m_GreenSpecularLineEdit;
    QLineEdit *m_BlueSpecularLineEdit;
    QLineEdit *m_AlphaSpecularLineEdit;
    QPushButton *m_SpecularPushButton;
    QCheckBox *m_EnableCheckBox;
    QGroupBox *m_PositionGroupBox;
    QGridLayout *gridLayout4;
    QHBoxLayout *hboxLayout6;
    QLabel *m_XPositionTextLabel;
    QLineEdit *m_XPositionLineEdit;
    QHBoxLayout *hboxLayout7;
    QLabel *m_YPositionTextLabel;
    QLineEdit *m_YPositionLineEdit;
    QHBoxLayout *hboxLayout8;
    QLabel *m_ZPositionTextLabel;
    QLineEdit *m_ZPositionLineEdit;
    QHBoxLayout *hboxLayout9;
    QLabel *m_WPositionTextLabel;
    QLineEdit *m_WPositionLineEdit;

    void setupUi(QDialog *LightsDialogBase)
    {
        if (LightsDialogBase->objectName().isEmpty())
            LightsDialogBase->setObjectName(QString::fromUtf8("LightsDialogBase"));
        LightsDialogBase->resize(459, 491);
        LightsDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(LightsDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(LightsDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(LightsDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 5, 0, 1, 2);

        m_AttenuationGroupBox = new QGroupBox(LightsDialogBase);
        m_AttenuationGroupBox->setObjectName(QString::fromUtf8("m_AttenuationGroupBox"));
            QVBoxLayout *_vbl1 = new QVBoxLayout(m_AttenuationGroupBox);

            _vbl1->setSpacing(6);

            _vbl1->setContentsMargins(11, 11, 11, 11);

            gridLayout1 = new QGridLayout();

            _vbl1->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        m_ConstantAttenuationTextLabel = new QLabel(m_AttenuationGroupBox);
        m_ConstantAttenuationTextLabel->setObjectName(QString::fromUtf8("m_ConstantAttenuationTextLabel"));
        m_ConstantAttenuationTextLabel->setWordWrap(false);

        gridLayout1->addWidget(m_ConstantAttenuationTextLabel, 0, 0, 1, 2);

        m_LinearAttenuationTextLabel = new QLabel(m_AttenuationGroupBox);
        m_LinearAttenuationTextLabel->setObjectName(QString::fromUtf8("m_LinearAttenuationTextLabel"));
        m_LinearAttenuationTextLabel->setWordWrap(false);

        gridLayout1->addWidget(m_LinearAttenuationTextLabel, 1, 0, 1, 1);

        m_QuadraticAttenuationTextLabel = new QLabel(m_AttenuationGroupBox);
        m_QuadraticAttenuationTextLabel->setObjectName(QString::fromUtf8("m_QuadraticAttenuationTextLabel"));
        m_QuadraticAttenuationTextLabel->setWordWrap(false);

        gridLayout1->addWidget(m_QuadraticAttenuationTextLabel, 2, 0, 1, 2);

        m_ConstantAttenuationLineEdit = new QLineEdit(m_AttenuationGroupBox);
        m_ConstantAttenuationLineEdit->setObjectName(QString::fromUtf8("m_ConstantAttenuationLineEdit"));

        gridLayout1->addWidget(m_ConstantAttenuationLineEdit, 0, 2, 1, 1);

        m_LinearAttenuationLineEdit = new QLineEdit(m_AttenuationGroupBox);
        m_LinearAttenuationLineEdit->setObjectName(QString::fromUtf8("m_LinearAttenuationLineEdit"));

        gridLayout1->addWidget(m_LinearAttenuationLineEdit, 1, 1, 1, 2);

        m_QuadraticAttenuationLineEdit = new QLineEdit(m_AttenuationGroupBox);
        m_QuadraticAttenuationLineEdit->setObjectName(QString::fromUtf8("m_QuadraticAttenuationLineEdit"));

        gridLayout1->addWidget(m_QuadraticAttenuationLineEdit, 2, 2, 1, 1);


        gridLayout->addWidget(m_AttenuationGroupBox, 4, 0, 1, 1);

        spacer3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer3, 4, 1, 1, 1);

        m_SpotLightGroupBox = new QGroupBox(LightsDialogBase);
        m_SpotLightGroupBox->setObjectName(QString::fromUtf8("m_SpotLightGroupBox"));
            QVBoxLayout *_vbl2 = new QVBoxLayout(m_SpotLightGroupBox);

            _vbl2->setSpacing(6);

            _vbl2->setContentsMargins(11, 11, 11, 11);

            gridLayout2 = new QGridLayout();

            _vbl2->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_SpotLightDirectionTextLabel = new QLabel(m_SpotLightGroupBox);
        m_SpotLightDirectionTextLabel->setObjectName(QString::fromUtf8("m_SpotLightDirectionTextLabel"));
        m_SpotLightDirectionTextLabel->setWordWrap(false);

        hboxLayout1->addWidget(m_SpotLightDirectionTextLabel);

        m_XSpotLightDirectionLineEdit = new QLineEdit(m_SpotLightGroupBox);
        m_XSpotLightDirectionLineEdit->setObjectName(QString::fromUtf8("m_XSpotLightDirectionLineEdit"));

        hboxLayout1->addWidget(m_XSpotLightDirectionLineEdit);

        m_YSpotLightDirectionLineEdit = new QLineEdit(m_SpotLightGroupBox);
        m_YSpotLightDirectionLineEdit->setObjectName(QString::fromUtf8("m_YSpotLightDirectionLineEdit"));

        hboxLayout1->addWidget(m_YSpotLightDirectionLineEdit);

        m_ZSpotLightDirectionLineEdit = new QLineEdit(m_SpotLightGroupBox);
        m_ZSpotLightDirectionLineEdit->setObjectName(QString::fromUtf8("m_ZSpotLightDirectionLineEdit"));

        hboxLayout1->addWidget(m_ZSpotLightDirectionLineEdit);


        gridLayout2->addLayout(hboxLayout1, 0, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_SpotLightExponentTextLabel = new QLabel(m_SpotLightGroupBox);
        m_SpotLightExponentTextLabel->setObjectName(QString::fromUtf8("m_SpotLightExponentTextLabel"));
        m_SpotLightExponentTextLabel->setWordWrap(false);

        hboxLayout2->addWidget(m_SpotLightExponentTextLabel);

        m_SpotLightExponentLineEdit = new QLineEdit(m_SpotLightGroupBox);
        m_SpotLightExponentLineEdit->setObjectName(QString::fromUtf8("m_SpotLightExponentLineEdit"));

        hboxLayout2->addWidget(m_SpotLightExponentLineEdit);

        m_SpotLightCutOffTextLabel = new QLabel(m_SpotLightGroupBox);
        m_SpotLightCutOffTextLabel->setObjectName(QString::fromUtf8("m_SpotLightCutOffTextLabel"));
        m_SpotLightCutOffTextLabel->setWordWrap(false);

        hboxLayout2->addWidget(m_SpotLightCutOffTextLabel);

        m_SpotLightCutOffLineEdit = new QLineEdit(m_SpotLightGroupBox);
        m_SpotLightCutOffLineEdit->setObjectName(QString::fromUtf8("m_SpotLightCutOffLineEdit"));

        hboxLayout2->addWidget(m_SpotLightCutOffLineEdit);


        gridLayout2->addLayout(hboxLayout2, 1, 0, 1, 1);


        gridLayout->addWidget(m_SpotLightGroupBox, 3, 0, 1, 2);

        m_ColorsGroupBox = new QGroupBox(LightsDialogBase);
        m_ColorsGroupBox->setObjectName(QString::fromUtf8("m_ColorsGroupBox"));
            QVBoxLayout *_vbl3 = new QVBoxLayout(m_ColorsGroupBox);

            _vbl3->setSpacing(6);

            _vbl3->setContentsMargins(11, 11, 11, 11);

            gridLayout3 = new QGridLayout();

            _vbl3->addLayout(gridLayout3);
        gridLayout3->setAlignment(Qt::AlignTop);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        m_RedAmbientLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_RedAmbientLineEdit->setObjectName(QString::fromUtf8("m_RedAmbientLineEdit"));

        hboxLayout3->addWidget(m_RedAmbientLineEdit);

        m_GreenAmbientLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_GreenAmbientLineEdit->setObjectName(QString::fromUtf8("m_GreenAmbientLineEdit"));

        hboxLayout3->addWidget(m_GreenAmbientLineEdit);

        m_BlueAmbientLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_BlueAmbientLineEdit->setObjectName(QString::fromUtf8("m_BlueAmbientLineEdit"));

        hboxLayout3->addWidget(m_BlueAmbientLineEdit);

        m_AlphaAmbientLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_AlphaAmbientLineEdit->setObjectName(QString::fromUtf8("m_AlphaAmbientLineEdit"));

        hboxLayout3->addWidget(m_AlphaAmbientLineEdit);

        m_AmbientPushButton = new QPushButton(m_ColorsGroupBox);
        m_AmbientPushButton->setObjectName(QString::fromUtf8("m_AmbientPushButton"));

        hboxLayout3->addWidget(m_AmbientPushButton);


        gridLayout3->addLayout(hboxLayout3, 0, 0, 1, 1);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        m_RedDiffuseLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_RedDiffuseLineEdit->setObjectName(QString::fromUtf8("m_RedDiffuseLineEdit"));

        hboxLayout4->addWidget(m_RedDiffuseLineEdit);

        m_GreenDiffuseLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_GreenDiffuseLineEdit->setObjectName(QString::fromUtf8("m_GreenDiffuseLineEdit"));

        hboxLayout4->addWidget(m_GreenDiffuseLineEdit);

        m_BlueDiffuseLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_BlueDiffuseLineEdit->setObjectName(QString::fromUtf8("m_BlueDiffuseLineEdit"));

        hboxLayout4->addWidget(m_BlueDiffuseLineEdit);

        m_AlphaDiffuseLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_AlphaDiffuseLineEdit->setObjectName(QString::fromUtf8("m_AlphaDiffuseLineEdit"));

        hboxLayout4->addWidget(m_AlphaDiffuseLineEdit);

        m_DiffusePushButton = new QPushButton(m_ColorsGroupBox);
        m_DiffusePushButton->setObjectName(QString::fromUtf8("m_DiffusePushButton"));

        hboxLayout4->addWidget(m_DiffusePushButton);


        gridLayout3->addLayout(hboxLayout4, 1, 0, 1, 1);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(6);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        m_RedSpecularLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_RedSpecularLineEdit->setObjectName(QString::fromUtf8("m_RedSpecularLineEdit"));

        hboxLayout5->addWidget(m_RedSpecularLineEdit);

        m_GreenSpecularLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_GreenSpecularLineEdit->setObjectName(QString::fromUtf8("m_GreenSpecularLineEdit"));

        hboxLayout5->addWidget(m_GreenSpecularLineEdit);

        m_BlueSpecularLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_BlueSpecularLineEdit->setObjectName(QString::fromUtf8("m_BlueSpecularLineEdit"));

        hboxLayout5->addWidget(m_BlueSpecularLineEdit);

        m_AlphaSpecularLineEdit = new QLineEdit(m_ColorsGroupBox);
        m_AlphaSpecularLineEdit->setObjectName(QString::fromUtf8("m_AlphaSpecularLineEdit"));

        hboxLayout5->addWidget(m_AlphaSpecularLineEdit);

        m_SpecularPushButton = new QPushButton(m_ColorsGroupBox);
        m_SpecularPushButton->setObjectName(QString::fromUtf8("m_SpecularPushButton"));

        hboxLayout5->addWidget(m_SpecularPushButton);


        gridLayout3->addLayout(hboxLayout5, 2, 0, 1, 1);


        gridLayout->addWidget(m_ColorsGroupBox, 2, 0, 1, 2);

        m_EnableCheckBox = new QCheckBox(LightsDialogBase);
        m_EnableCheckBox->setObjectName(QString::fromUtf8("m_EnableCheckBox"));

        gridLayout->addWidget(m_EnableCheckBox, 0, 0, 1, 2);

        m_PositionGroupBox = new QGroupBox(LightsDialogBase);
        m_PositionGroupBox->setObjectName(QString::fromUtf8("m_PositionGroupBox"));
            QVBoxLayout *_vbl4 = new QVBoxLayout(m_PositionGroupBox);

            _vbl4->setSpacing(6);

            _vbl4->setContentsMargins(11, 11, 11, 11);

            gridLayout4 = new QGridLayout();

            _vbl4->addLayout(gridLayout4);
        gridLayout4->setAlignment(Qt::AlignTop);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setSpacing(6);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        m_XPositionTextLabel = new QLabel(m_PositionGroupBox);
        m_XPositionTextLabel->setObjectName(QString::fromUtf8("m_XPositionTextLabel"));
        m_XPositionTextLabel->setWordWrap(false);

        hboxLayout6->addWidget(m_XPositionTextLabel);

        m_XPositionLineEdit = new QLineEdit(m_PositionGroupBox);
        m_XPositionLineEdit->setObjectName(QString::fromUtf8("m_XPositionLineEdit"));

        hboxLayout6->addWidget(m_XPositionLineEdit);


        gridLayout4->addLayout(hboxLayout6, 0, 0, 1, 1);

        hboxLayout7 = new QHBoxLayout();
        hboxLayout7->setSpacing(6);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        m_YPositionTextLabel = new QLabel(m_PositionGroupBox);
        m_YPositionTextLabel->setObjectName(QString::fromUtf8("m_YPositionTextLabel"));
        m_YPositionTextLabel->setWordWrap(false);

        hboxLayout7->addWidget(m_YPositionTextLabel);

        m_YPositionLineEdit = new QLineEdit(m_PositionGroupBox);
        m_YPositionLineEdit->setObjectName(QString::fromUtf8("m_YPositionLineEdit"));

        hboxLayout7->addWidget(m_YPositionLineEdit);


        gridLayout4->addLayout(hboxLayout7, 0, 1, 1, 1);

        hboxLayout8 = new QHBoxLayout();
        hboxLayout8->setSpacing(6);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        m_ZPositionTextLabel = new QLabel(m_PositionGroupBox);
        m_ZPositionTextLabel->setObjectName(QString::fromUtf8("m_ZPositionTextLabel"));
        m_ZPositionTextLabel->setWordWrap(false);

        hboxLayout8->addWidget(m_ZPositionTextLabel);

        m_ZPositionLineEdit = new QLineEdit(m_PositionGroupBox);
        m_ZPositionLineEdit->setObjectName(QString::fromUtf8("m_ZPositionLineEdit"));

        hboxLayout8->addWidget(m_ZPositionLineEdit);


        gridLayout4->addLayout(hboxLayout8, 1, 0, 1, 1);

        hboxLayout9 = new QHBoxLayout();
        hboxLayout9->setSpacing(6);
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        m_WPositionTextLabel = new QLabel(m_PositionGroupBox);
        m_WPositionTextLabel->setObjectName(QString::fromUtf8("m_WPositionTextLabel"));
        m_WPositionTextLabel->setWordWrap(false);

        hboxLayout9->addWidget(m_WPositionTextLabel);

        m_WPositionLineEdit = new QLineEdit(m_PositionGroupBox);
        m_WPositionLineEdit->setObjectName(QString::fromUtf8("m_WPositionLineEdit"));

        hboxLayout9->addWidget(m_WPositionLineEdit);


        gridLayout4->addLayout(hboxLayout9, 1, 1, 1, 1);


        gridLayout->addWidget(m_PositionGroupBox, 1, 0, 1, 2);


        retranslateUi(LightsDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), LightsDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), LightsDialogBase, SLOT(reject()));
        QObject::connect(m_AmbientPushButton, SIGNAL(clicked()), LightsDialogBase, SLOT(ambientColorSlot()));
        QObject::connect(m_DiffusePushButton, SIGNAL(clicked()), LightsDialogBase, SLOT(diffuseColorSlot()));
        QObject::connect(m_SpecularPushButton, SIGNAL(clicked()), LightsDialogBase, SLOT(specularColorSlot()));

        QMetaObject::connectSlotsByName(LightsDialogBase);
    } // setupUi

    void retranslateUi(QDialog *LightsDialogBase)
    {
        LightsDialogBase->setWindowTitle(QCoreApplication::translate("LightsDialogBase", "MyDialog"));
        buttonOk->setText(QCoreApplication::translate("LightsDialogBase", "&OK"));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QCoreApplication::translate("LightsDialogBase", "&Cancel"));
        buttonCancel->setShortcut(QString());
        m_AttenuationGroupBox->setTitle(QCoreApplication::translate("LightsDialogBase", "Attenuation"));
        m_ConstantAttenuationTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Constant"));
        m_LinearAttenuationTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Linear"));
        m_QuadraticAttenuationTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Quadratic"));
        m_SpotLightGroupBox->setTitle(QCoreApplication::translate("LightsDialogBase", "Spot light"));
        m_SpotLightDirectionTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Direction"));
        m_SpotLightExponentTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Exponent"));
        m_SpotLightCutOffTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Cut off"));
        m_ColorsGroupBox->setTitle(QCoreApplication::translate("LightsDialogBase", "Color"));
        m_AmbientPushButton->setText(QCoreApplication::translate("LightsDialogBase", "Ambient"));
        m_DiffusePushButton->setText(QCoreApplication::translate("LightsDialogBase", "Diffuse"));
        m_SpecularPushButton->setText(QCoreApplication::translate("LightsDialogBase", "Specular"));
        m_EnableCheckBox->setText(QCoreApplication::translate("LightsDialogBase", "Enable"));
        m_PositionGroupBox->setTitle(QCoreApplication::translate("LightsDialogBase", "Position"));
        m_XPositionTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "X"));
        m_YPositionTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Y"));
        m_ZPositionTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "Z"));
        m_WPositionTextLabel->setText(QCoreApplication::translate("LightsDialogBase", "W"));
        Q_UNUSED(LightsDialogBase);
    } // retranslateUi

};

namespace Ui {
    class LightsDialogBase: public Ui_LightsDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class LightsDialogBase : public QDialog, public Ui::LightsDialogBase
{
    Q_OBJECT

public:
    LightsDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~LightsDialogBase() {}

public slots:
    virtual void ambientColorSlot() {}
    virtual void diffuseColorSlot() {}
    virtual void specularColorSlot() {}

protected slots:
    virtual void languageChange() {}

};

#endif // LIGHTSDIALOGBASE_H
