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
#ifndef BALLANDSTICKRENDERINGPROPERTIESBASE_H
#define BALLANDSTICKRENDERINGPROPERTIESBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BallAndStickRenderingPropertiesBase
{
public:
    QGridLayout *gridLayout;
    Q3GroupBox *RenderGroupBox;
    QGridLayout *gridLayout1;
    Q3GroupBox *groupBox2;
    QGridLayout *gridLayout2;
    QPushButton *m_PlotAnglesPushButton;
    QPushButton *m_ColorLegendPushButton;
    QCheckBox *m_UseSolventEnlargedRadiusCheckBox;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QHBoxLayout *hboxLayout1;
    QLineEdit *m_FunctionLineEdit;
    QPushButton *m_FunctionPushButton;
    QHBoxLayout *hboxLayout2;
    QHBoxLayout *hboxLayout3;
    QLineEdit *m_ColorMapLineEdit;
    QPushButton *m_ColorMapPushButton;
    QGridLayout *gridLayout3;
    QLabel *m_SubCubeTextLabel;
    QGridLayout *gridLayout4;
    QLabel *textLabel4;
    QLabel *textLabel3;
    QLineEdit *m_OuterCubeIsoLineEdit;
    QLineEdit *m_OuterCubeBLineEdit;
    QLineEdit *m_SubCubeBLineEdit;
    QLabel *textLabel2;
    QLineEdit *m_SubCubeIsoLineEdit;
    QLabel *textLabel1;
    QLabel *m_OuterCubeTextLabel;
    QHBoxLayout *hboxLayout4;
    QComboBox *m_StructureLODComboBox;
    QLabel *m_StructureLODTextLabel;
    QHBoxLayout *hboxLayout5;
    QComboBox *m_ColorLODComboBox;
    QLabel *m_ColorLODTextLabel;
    QPushButton *m_ColorMapInterfacePushButton;
    QHBoxLayout *hboxLayout6;
    QCheckBox *m_RenderCheckBox;
    QSlider *m_FrameSlider;
    QSlider *m_ClipSlider;
    QCheckBox *m_CorrelateCheckBox;
    QLineEdit *m_CorrelateLineEdit;
    QPushButton *m_ReplicatePushButton;

    void setupUi(QWidget *BallAndStickRenderingPropertiesBase)
    {
        if (BallAndStickRenderingPropertiesBase->objectName().isEmpty())
            BallAndStickRenderingPropertiesBase->setObjectName(QString::fromUtf8("BallAndStickRenderingPropertiesBase"));
        BallAndStickRenderingPropertiesBase->resize(683, 180);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BallAndStickRenderingPropertiesBase->sizePolicy().hasHeightForWidth());
        BallAndStickRenderingPropertiesBase->setSizePolicy(sizePolicy);
        BallAndStickRenderingPropertiesBase->setMinimumSize(QSize(0, 180));
        BallAndStickRenderingPropertiesBase->setMaximumSize(QSize(32767, 180));
        gridLayout = new QGridLayout(BallAndStickRenderingPropertiesBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        RenderGroupBox = new Q3GroupBox(BallAndStickRenderingPropertiesBase);
        RenderGroupBox->setObjectName(QString::fromUtf8("RenderGroupBox"));
        RenderGroupBox->setColumnLayout(0, Qt::Vertical);
        RenderGroupBox->layout()->setSpacing(6);
        RenderGroupBox->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout1 = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(RenderGroupBox->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        groupBox2 = new Q3GroupBox(RenderGroupBox);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        groupBox2->setColumnLayout(0, Qt::Vertical);
        groupBox2->layout()->setSpacing(6);
        groupBox2->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout2 = new QGridLayout();
        QBoxLayout *boxlayout1 = qobject_cast<QBoxLayout *>(groupBox2->layout());
        if (boxlayout1)
            boxlayout1->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        m_PlotAnglesPushButton = new QPushButton(groupBox2);
        m_PlotAnglesPushButton->setObjectName(QString::fromUtf8("m_PlotAnglesPushButton"));

        gridLayout2->addWidget(m_PlotAnglesPushButton, 1, 1, 1, 1);

        m_ColorLegendPushButton = new QPushButton(groupBox2);
        m_ColorLegendPushButton->setObjectName(QString::fromUtf8("m_ColorLegendPushButton"));

        gridLayout2->addWidget(m_ColorLegendPushButton, 1, 0, 1, 1);

        m_UseSolventEnlargedRadiusCheckBox = new QCheckBox(groupBox2);
        m_UseSolventEnlargedRadiusCheckBox->setObjectName(QString::fromUtf8("m_UseSolventEnlargedRadiusCheckBox"));

        gridLayout2->addWidget(m_UseSolventEnlargedRadiusCheckBox, 0, 0, 1, 2);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_FunctionLineEdit = new QLineEdit(groupBox2);
        m_FunctionLineEdit->setObjectName(QString::fromUtf8("m_FunctionLineEdit"));
        m_FunctionLineEdit->setEnabled(false);

        hboxLayout1->addWidget(m_FunctionLineEdit);

        m_FunctionPushButton = new QPushButton(groupBox2);
        m_FunctionPushButton->setObjectName(QString::fromUtf8("m_FunctionPushButton"));

        hboxLayout1->addWidget(m_FunctionPushButton);


        hboxLayout->addLayout(hboxLayout1);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        m_ColorMapLineEdit = new QLineEdit(groupBox2);
        m_ColorMapLineEdit->setObjectName(QString::fromUtf8("m_ColorMapLineEdit"));
        m_ColorMapLineEdit->setEnabled(false);

        hboxLayout3->addWidget(m_ColorMapLineEdit);

        m_ColorMapPushButton = new QPushButton(groupBox2);
        m_ColorMapPushButton->setObjectName(QString::fromUtf8("m_ColorMapPushButton"));

        hboxLayout3->addWidget(m_ColorMapPushButton);


        hboxLayout2->addLayout(hboxLayout3);


        vboxLayout->addLayout(hboxLayout2);


        gridLayout2->addLayout(vboxLayout, 0, 2, 2, 1);

        gridLayout3 = new QGridLayout();
        gridLayout3->setSpacing(6);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        m_SubCubeTextLabel = new QLabel(groupBox2);
        m_SubCubeTextLabel->setObjectName(QString::fromUtf8("m_SubCubeTextLabel"));
        m_SubCubeTextLabel->setWordWrap(false);

        gridLayout3->addWidget(m_SubCubeTextLabel, 0, 0, 1, 1);

        gridLayout4 = new QGridLayout();
        gridLayout4->setSpacing(6);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        textLabel4 = new QLabel(groupBox2);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setWordWrap(false);

        gridLayout4->addWidget(textLabel4, 1, 0, 1, 2);

        textLabel3 = new QLabel(groupBox2);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        gridLayout4->addWidget(textLabel3, 1, 3, 1, 1);

        m_OuterCubeIsoLineEdit = new QLineEdit(groupBox2);
        m_OuterCubeIsoLineEdit->setObjectName(QString::fromUtf8("m_OuterCubeIsoLineEdit"));

        gridLayout4->addWidget(m_OuterCubeIsoLineEdit, 1, 2, 1, 1);

        m_OuterCubeBLineEdit = new QLineEdit(groupBox2);
        m_OuterCubeBLineEdit->setObjectName(QString::fromUtf8("m_OuterCubeBLineEdit"));

        gridLayout4->addWidget(m_OuterCubeBLineEdit, 1, 4, 1, 1);

        m_SubCubeBLineEdit = new QLineEdit(groupBox2);
        m_SubCubeBLineEdit->setObjectName(QString::fromUtf8("m_SubCubeBLineEdit"));

        gridLayout4->addWidget(m_SubCubeBLineEdit, 0, 4, 1, 1);

        textLabel2 = new QLabel(groupBox2);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        gridLayout4->addWidget(textLabel2, 0, 3, 1, 1);

        m_SubCubeIsoLineEdit = new QLineEdit(groupBox2);
        m_SubCubeIsoLineEdit->setObjectName(QString::fromUtf8("m_SubCubeIsoLineEdit"));

        gridLayout4->addWidget(m_SubCubeIsoLineEdit, 0, 1, 1, 2);

        textLabel1 = new QLabel(groupBox2);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        gridLayout4->addWidget(textLabel1, 0, 0, 1, 1);


        gridLayout3->addLayout(gridLayout4, 0, 1, 2, 1);

        m_OuterCubeTextLabel = new QLabel(groupBox2);
        m_OuterCubeTextLabel->setObjectName(QString::fromUtf8("m_OuterCubeTextLabel"));
        m_OuterCubeTextLabel->setWordWrap(false);

        gridLayout3->addWidget(m_OuterCubeTextLabel, 1, 0, 1, 1);


        gridLayout2->addLayout(gridLayout3, 0, 3, 2, 1);


        gridLayout1->addWidget(groupBox2, 2, 0, 1, 3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        m_StructureLODComboBox = new QComboBox(RenderGroupBox);
        m_StructureLODComboBox->setObjectName(QString::fromUtf8("m_StructureLODComboBox"));

        hboxLayout4->addWidget(m_StructureLODComboBox);

        m_StructureLODTextLabel = new QLabel(RenderGroupBox);
        m_StructureLODTextLabel->setObjectName(QString::fromUtf8("m_StructureLODTextLabel"));
        m_StructureLODTextLabel->setWordWrap(false);

        hboxLayout4->addWidget(m_StructureLODTextLabel);


        gridLayout1->addLayout(hboxLayout4, 1, 0, 1, 1);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(6);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        m_ColorLODComboBox = new QComboBox(RenderGroupBox);
        m_ColorLODComboBox->setObjectName(QString::fromUtf8("m_ColorLODComboBox"));

        hboxLayout5->addWidget(m_ColorLODComboBox);

        m_ColorLODTextLabel = new QLabel(RenderGroupBox);
        m_ColorLODTextLabel->setObjectName(QString::fromUtf8("m_ColorLODTextLabel"));
        m_ColorLODTextLabel->setWordWrap(false);

        hboxLayout5->addWidget(m_ColorLODTextLabel);


        gridLayout1->addLayout(hboxLayout5, 1, 1, 1, 1);

        m_ColorMapInterfacePushButton = new QPushButton(RenderGroupBox);
        m_ColorMapInterfacePushButton->setObjectName(QString::fromUtf8("m_ColorMapInterfacePushButton"));

        gridLayout1->addWidget(m_ColorMapInterfacePushButton, 1, 2, 1, 1);

        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setSpacing(6);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        m_RenderCheckBox = new QCheckBox(RenderGroupBox);
        m_RenderCheckBox->setObjectName(QString::fromUtf8("m_RenderCheckBox"));

        hboxLayout6->addWidget(m_RenderCheckBox);

        m_FrameSlider = new QSlider(RenderGroupBox);
        m_FrameSlider->setObjectName(QString::fromUtf8("m_FrameSlider"));
        m_FrameSlider->setOrientation(Qt::Horizontal);
        m_FrameSlider->setTickPosition(QSlider::TicksBelow);

        hboxLayout6->addWidget(m_FrameSlider);

        m_ClipSlider = new QSlider(RenderGroupBox);
        m_ClipSlider->setObjectName(QString::fromUtf8("m_ClipSlider"));
        m_ClipSlider->setOrientation(Qt::Horizontal);

        hboxLayout6->addWidget(m_ClipSlider);

        m_CorrelateCheckBox = new QCheckBox(RenderGroupBox);
        m_CorrelateCheckBox->setObjectName(QString::fromUtf8("m_CorrelateCheckBox"));

        hboxLayout6->addWidget(m_CorrelateCheckBox);

        m_CorrelateLineEdit = new QLineEdit(RenderGroupBox);
        m_CorrelateLineEdit->setObjectName(QString::fromUtf8("m_CorrelateLineEdit"));
        m_CorrelateLineEdit->setEnabled(false);

        hboxLayout6->addWidget(m_CorrelateLineEdit);

        m_ReplicatePushButton = new QPushButton(RenderGroupBox);
        m_ReplicatePushButton->setObjectName(QString::fromUtf8("m_ReplicatePushButton"));

        hboxLayout6->addWidget(m_ReplicatePushButton);


        gridLayout1->addLayout(hboxLayout6, 0, 0, 1, 3);


        gridLayout->addWidget(RenderGroupBox, 0, 0, 1, 1);


        retranslateUi(BallAndStickRenderingPropertiesBase);

        QMetaObject::connectSlotsByName(BallAndStickRenderingPropertiesBase);
    } // setupUi

    void retranslateUi(QWidget *BallAndStickRenderingPropertiesBase)
    {
        BallAndStickRenderingPropertiesBase->setWindowTitle(QApplication::translate("BallAndStickRenderingPropertiesBase", "Form2", 0, QApplication::UnicodeUTF8));
        RenderGroupBox->setTitle(QString());
        groupBox2->setTitle(QString());
        m_PlotAnglesPushButton->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Plot Angles", 0, QApplication::UnicodeUTF8));
        m_ColorLegendPushButton->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Show Legend", 0, QApplication::UnicodeUTF8));
        m_UseSolventEnlargedRadiusCheckBox->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Solvent enlarged radius", 0, QApplication::UnicodeUTF8));
        m_FunctionPushButton->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Associate function", 0, QApplication::UnicodeUTF8));
        m_ColorMapPushButton->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Color map", 0, QApplication::UnicodeUTF8));
        m_SubCubeTextLabel->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Sub cube", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "I", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "B", 0, QApplication::UnicodeUTF8));
        m_OuterCubeIsoLineEdit->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "1.0", 0, QApplication::UnicodeUTF8));
        m_OuterCubeBLineEdit->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "-2.3", 0, QApplication::UnicodeUTF8));
        m_SubCubeBLineEdit->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "-2.3", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "B", 0, QApplication::UnicodeUTF8));
        m_SubCubeIsoLineEdit->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "1.0", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "I", 0, QApplication::UnicodeUTF8));
        m_OuterCubeTextLabel->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Outer cube", 0, QApplication::UnicodeUTF8));
        m_StructureLODTextLabel->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Structure detail", 0, QApplication::UnicodeUTF8));
        m_ColorLODTextLabel->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Color detail", 0, QApplication::UnicodeUTF8));
        m_ColorMapInterfacePushButton->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Colormap Interface", 0, QApplication::UnicodeUTF8));
        m_RenderCheckBox->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Render", 0, QApplication::UnicodeUTF8));
        m_CorrelateCheckBox->setText(QString());
        m_ReplicatePushButton->setText(QApplication::translate("BallAndStickRenderingPropertiesBase", "Replicate", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(BallAndStickRenderingPropertiesBase);
    } // retranslateUi

};

namespace Ui {
    class BallAndStickRenderingPropertiesBase: public Ui_BallAndStickRenderingPropertiesBase {};
} // namespace Ui

QT_END_NAMESPACE

class BallAndStickRenderingPropertiesBase : public QWidget, public Ui::BallAndStickRenderingPropertiesBase
{
    Q_OBJECT

public:
    BallAndStickRenderingPropertiesBase(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = {});
    ~BallAndStickRenderingPropertiesBase();

public slots:
    virtual void redrawSlot();

protected slots:
    virtual void languageChange();

};

#endif // BALLANDSTICKRENDERINGPROPERTIESBASE_H
