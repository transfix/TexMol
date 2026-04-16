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
#ifndef SURFACERENDERINGPROPERTIESBASE_H
#define SURFACERENDERINGPROPERTIESBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3ButtonGroup>
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
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SurfaceRenderingPropertiesBase
{
public:
    QGridLayout *gridLayout;
    Q3GroupBox *groupBox4;
    QGridLayout *gridLayout1;
    QCheckBox *m_RenderPointsCheckBox;
    QSpinBox *m_PointThicknessSpinBox;
    Q3ButtonGroup *buttonGroup7;
    QGridLayout *gridLayout2;
    QGridLayout *gridLayout3;
    QHBoxLayout *hboxLayout;
    QRadioButton *m_UserSelectedPointColorRadioButton;
    QPushButton *m_PointColorPushButton;
    QRadioButton *m_DefaultPointColorRadioButton;
    Q3GroupBox *m_LinesGgroupBox;
    QGridLayout *gridLayout4;
    Q3ButtonGroup *buttonGroup6;
    QGridLayout *gridLayout5;
    QGridLayout *gridLayout6;
    QHBoxLayout *hboxLayout1;
    QRadioButton *m_UserSelectedLineColorRadioButton;
    QPushButton *m_LineColorPushButton;
    QRadioButton *m_DefaultLineColorRadioButton;
    QHBoxLayout *hboxLayout2;
    QCheckBox *m_RenderLinesCheckBox;
    QSpinBox *m_LineThicknessSpinBox;
    Q3GroupBox *m_WireframeGroupBox;
    QGridLayout *gridLayout7;
    QHBoxLayout *hboxLayout3;
    QCheckBox *m_RenderWireframeCheckBox;
    QHBoxLayout *hboxLayout4;
    QSpinBox *m_ThicknessSpinBox;
    Q3ButtonGroup *buttonGroup5;
    QGridLayout *gridLayout8;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout5;
    QRadioButton *m_UserSelectedWireframeColorRadioButton;
    QPushButton *m_WireframeColorPushButton;
    QRadioButton *m_DefaultWireframeColorRadioButton;
    Q3GroupBox *m_SurfaceGroupBox;
    QGridLayout *gridLayout9;
    QCheckBox *m_RenderSurfaceCheckBox;
    QHBoxLayout *hboxLayout6;
    QComboBox *m_SurfaceRenderingTypesComboBox;
    Q3ButtonGroup *buttonGroup4;
    QGridLayout *gridLayout10;
    QHBoxLayout *hboxLayout7;
    QCheckBox *m_UseSurfaceTextureCheckBox;
    QPushButton *m_SurfaceTexturePushButton;
    QHBoxLayout *hboxLayout8;
    QCheckBox *m_SurfaceColorCheckBox;
    QPushButton *m_SurfaceColorPushButton;
    QHBoxLayout *hboxLayout9;
    QCheckBox *m_RenderCheckBox;
    QPushButton *m_QSAPushButton;
    QSlider *m_FrameSlider;
    QSlider *m_ClipSlider;
    QPushButton *m_ReplicatePushButton;
    QSlider *m_TransparencySlider;

    void setupUi(QWidget *SurfaceRenderingPropertiesBase)
    {
        if (SurfaceRenderingPropertiesBase->objectName().isEmpty())
            SurfaceRenderingPropertiesBase->setObjectName(QString::fromUtf8("SurfaceRenderingPropertiesBase"));
        SurfaceRenderingPropertiesBase->resize(669, 180);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SurfaceRenderingPropertiesBase->sizePolicy().hasHeightForWidth());
        SurfaceRenderingPropertiesBase->setSizePolicy(sizePolicy);
        SurfaceRenderingPropertiesBase->setMinimumSize(QSize(0, 180));
        SurfaceRenderingPropertiesBase->setMaximumSize(QSize(32767, 180));
        gridLayout = new QGridLayout(SurfaceRenderingPropertiesBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox4 = new Q3GroupBox(SurfaceRenderingPropertiesBase);
        groupBox4->setObjectName(QString::fromUtf8("groupBox4"));
        groupBox4->setColumnLayout(0, Qt::Vertical);
        groupBox4->layout()->setSpacing(6);
        groupBox4->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout1 = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(groupBox4->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        m_RenderPointsCheckBox = new QCheckBox(groupBox4);
        m_RenderPointsCheckBox->setObjectName(QString::fromUtf8("m_RenderPointsCheckBox"));

        gridLayout1->addWidget(m_RenderPointsCheckBox, 0, 0, 1, 1);

        m_PointThicknessSpinBox = new QSpinBox(groupBox4);
        m_PointThicknessSpinBox->setObjectName(QString::fromUtf8("m_PointThicknessSpinBox"));
        m_PointThicknessSpinBox->setMinimum(1);

        gridLayout1->addWidget(m_PointThicknessSpinBox, 0, 1, 1, 1);

        buttonGroup7 = new Q3ButtonGroup(groupBox4);
        buttonGroup7->setObjectName(QString::fromUtf8("buttonGroup7"));
        buttonGroup7->setColumnLayout(0, Qt::Vertical);
        buttonGroup7->layout()->setSpacing(6);
        buttonGroup7->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout2 = new QGridLayout();
        QBoxLayout *boxlayout1 = qobject_cast<QBoxLayout *>(buttonGroup7->layout());
        if (boxlayout1)
            boxlayout1->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        gridLayout3 = new QGridLayout();
        gridLayout3->setSpacing(6);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_UserSelectedPointColorRadioButton = new QRadioButton(buttonGroup7);
        m_UserSelectedPointColorRadioButton->setObjectName(QString::fromUtf8("m_UserSelectedPointColorRadioButton"));

        hboxLayout->addWidget(m_UserSelectedPointColorRadioButton);

        m_PointColorPushButton = new QPushButton(buttonGroup7);
        m_PointColorPushButton->setObjectName(QString::fromUtf8("m_PointColorPushButton"));

        hboxLayout->addWidget(m_PointColorPushButton);


        gridLayout3->addLayout(hboxLayout, 0, 0, 1, 1);

        m_DefaultPointColorRadioButton = new QRadioButton(buttonGroup7);
        m_DefaultPointColorRadioButton->setObjectName(QString::fromUtf8("m_DefaultPointColorRadioButton"));

        gridLayout3->addWidget(m_DefaultPointColorRadioButton, 1, 0, 1, 1);


        gridLayout2->addLayout(gridLayout3, 0, 0, 1, 1);


        gridLayout1->addWidget(buttonGroup7, 1, 0, 1, 2);


        gridLayout->addWidget(groupBox4, 1, 3, 1, 1);

        m_LinesGgroupBox = new Q3GroupBox(SurfaceRenderingPropertiesBase);
        m_LinesGgroupBox->setObjectName(QString::fromUtf8("m_LinesGgroupBox"));
        m_LinesGgroupBox->setColumnLayout(0, Qt::Vertical);
        m_LinesGgroupBox->layout()->setSpacing(6);
        m_LinesGgroupBox->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout4 = new QGridLayout();
        QBoxLayout *boxlayout2 = qobject_cast<QBoxLayout *>(m_LinesGgroupBox->layout());
        if (boxlayout2)
            boxlayout2->addLayout(gridLayout4);
        gridLayout4->setAlignment(Qt::AlignTop);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        buttonGroup6 = new Q3ButtonGroup(m_LinesGgroupBox);
        buttonGroup6->setObjectName(QString::fromUtf8("buttonGroup6"));
        buttonGroup6->setColumnLayout(0, Qt::Vertical);
        buttonGroup6->layout()->setSpacing(6);
        buttonGroup6->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout5 = new QGridLayout();
        QBoxLayout *boxlayout3 = qobject_cast<QBoxLayout *>(buttonGroup6->layout());
        if (boxlayout3)
            boxlayout3->addLayout(gridLayout5);
        gridLayout5->setAlignment(Qt::AlignTop);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        gridLayout6 = new QGridLayout();
        gridLayout6->setSpacing(6);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_UserSelectedLineColorRadioButton = new QRadioButton(buttonGroup6);
        m_UserSelectedLineColorRadioButton->setObjectName(QString::fromUtf8("m_UserSelectedLineColorRadioButton"));

        hboxLayout1->addWidget(m_UserSelectedLineColorRadioButton);

        m_LineColorPushButton = new QPushButton(buttonGroup6);
        m_LineColorPushButton->setObjectName(QString::fromUtf8("m_LineColorPushButton"));

        hboxLayout1->addWidget(m_LineColorPushButton);


        gridLayout6->addLayout(hboxLayout1, 0, 0, 1, 1);

        m_DefaultLineColorRadioButton = new QRadioButton(buttonGroup6);
        m_DefaultLineColorRadioButton->setObjectName(QString::fromUtf8("m_DefaultLineColorRadioButton"));
        m_DefaultLineColorRadioButton->setChecked(true);

        gridLayout6->addWidget(m_DefaultLineColorRadioButton, 1, 0, 1, 1);


        gridLayout5->addLayout(gridLayout6, 0, 0, 1, 1);


        gridLayout4->addWidget(buttonGroup6, 1, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_RenderLinesCheckBox = new QCheckBox(m_LinesGgroupBox);
        m_RenderLinesCheckBox->setObjectName(QString::fromUtf8("m_RenderLinesCheckBox"));

        hboxLayout2->addWidget(m_RenderLinesCheckBox);

        m_LineThicknessSpinBox = new QSpinBox(m_LinesGgroupBox);
        m_LineThicknessSpinBox->setObjectName(QString::fromUtf8("m_LineThicknessSpinBox"));
        m_LineThicknessSpinBox->setMinimum(1);

        hboxLayout2->addWidget(m_LineThicknessSpinBox);


        gridLayout4->addLayout(hboxLayout2, 0, 0, 1, 1);


        gridLayout->addWidget(m_LinesGgroupBox, 1, 2, 1, 1);

        m_WireframeGroupBox = new Q3GroupBox(SurfaceRenderingPropertiesBase);
        m_WireframeGroupBox->setObjectName(QString::fromUtf8("m_WireframeGroupBox"));
        m_WireframeGroupBox->setColumnLayout(0, Qt::Vertical);
        m_WireframeGroupBox->layout()->setSpacing(6);
        m_WireframeGroupBox->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout7 = new QGridLayout();
        QBoxLayout *boxlayout4 = qobject_cast<QBoxLayout *>(m_WireframeGroupBox->layout());
        if (boxlayout4)
            boxlayout4->addLayout(gridLayout7);
        gridLayout7->setAlignment(Qt::AlignTop);
        gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        m_RenderWireframeCheckBox = new QCheckBox(m_WireframeGroupBox);
        m_RenderWireframeCheckBox->setObjectName(QString::fromUtf8("m_RenderWireframeCheckBox"));

        hboxLayout3->addWidget(m_RenderWireframeCheckBox);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        m_ThicknessSpinBox = new QSpinBox(m_WireframeGroupBox);
        m_ThicknessSpinBox->setObjectName(QString::fromUtf8("m_ThicknessSpinBox"));
        m_ThicknessSpinBox->setMinimum(1);
        m_ThicknessSpinBox->setValue(1);

        hboxLayout4->addWidget(m_ThicknessSpinBox);


        hboxLayout3->addLayout(hboxLayout4);


        gridLayout7->addLayout(hboxLayout3, 0, 0, 1, 1);

        buttonGroup5 = new Q3ButtonGroup(m_WireframeGroupBox);
        buttonGroup5->setObjectName(QString::fromUtf8("buttonGroup5"));
        buttonGroup5->setColumnLayout(0, Qt::Vertical);
        buttonGroup5->layout()->setSpacing(6);
        buttonGroup5->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout8 = new QGridLayout();
        QBoxLayout *boxlayout5 = qobject_cast<QBoxLayout *>(buttonGroup5->layout());
        if (boxlayout5)
            boxlayout5->addLayout(gridLayout8);
        gridLayout8->setAlignment(Qt::AlignTop);
        gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(6);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        m_UserSelectedWireframeColorRadioButton = new QRadioButton(buttonGroup5);
        m_UserSelectedWireframeColorRadioButton->setObjectName(QString::fromUtf8("m_UserSelectedWireframeColorRadioButton"));

        hboxLayout5->addWidget(m_UserSelectedWireframeColorRadioButton);

        m_WireframeColorPushButton = new QPushButton(buttonGroup5);
        m_WireframeColorPushButton->setObjectName(QString::fromUtf8("m_WireframeColorPushButton"));

        hboxLayout5->addWidget(m_WireframeColorPushButton);


        vboxLayout->addLayout(hboxLayout5);

        m_DefaultWireframeColorRadioButton = new QRadioButton(buttonGroup5);
        m_DefaultWireframeColorRadioButton->setObjectName(QString::fromUtf8("m_DefaultWireframeColorRadioButton"));
        m_DefaultWireframeColorRadioButton->setChecked(true);

        vboxLayout->addWidget(m_DefaultWireframeColorRadioButton);


        gridLayout8->addLayout(vboxLayout, 0, 0, 1, 1);


        gridLayout7->addWidget(buttonGroup5, 1, 0, 1, 1);


        gridLayout->addWidget(m_WireframeGroupBox, 1, 1, 1, 1);

        m_SurfaceGroupBox = new Q3GroupBox(SurfaceRenderingPropertiesBase);
        m_SurfaceGroupBox->setObjectName(QString::fromUtf8("m_SurfaceGroupBox"));
        m_SurfaceGroupBox->setColumnLayout(0, Qt::Vertical);
        m_SurfaceGroupBox->layout()->setSpacing(6);
        m_SurfaceGroupBox->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout9 = new QGridLayout();
        QBoxLayout *boxlayout6 = qobject_cast<QBoxLayout *>(m_SurfaceGroupBox->layout());
        if (boxlayout6)
            boxlayout6->addLayout(gridLayout9);
        gridLayout9->setAlignment(Qt::AlignTop);
        gridLayout9->setObjectName(QString::fromUtf8("gridLayout9"));
        m_RenderSurfaceCheckBox = new QCheckBox(m_SurfaceGroupBox);
        m_RenderSurfaceCheckBox->setObjectName(QString::fromUtf8("m_RenderSurfaceCheckBox"));
        m_RenderSurfaceCheckBox->setChecked(true);

        gridLayout9->addWidget(m_RenderSurfaceCheckBox, 0, 0, 1, 1);

        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setSpacing(6);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        m_SurfaceRenderingTypesComboBox = new QComboBox(m_SurfaceGroupBox);
        m_SurfaceRenderingTypesComboBox->setObjectName(QString::fromUtf8("m_SurfaceRenderingTypesComboBox"));

        hboxLayout6->addWidget(m_SurfaceRenderingTypesComboBox);


        gridLayout9->addLayout(hboxLayout6, 0, 1, 1, 1);

        buttonGroup4 = new Q3ButtonGroup(m_SurfaceGroupBox);
        buttonGroup4->setObjectName(QString::fromUtf8("buttonGroup4"));
        buttonGroup4->setColumnLayout(0, Qt::Vertical);
        buttonGroup4->layout()->setSpacing(6);
        buttonGroup4->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout10 = new QGridLayout();
        QBoxLayout *boxlayout7 = qobject_cast<QBoxLayout *>(buttonGroup4->layout());
        if (boxlayout7)
            boxlayout7->addLayout(gridLayout10);
        gridLayout10->setAlignment(Qt::AlignTop);
        gridLayout10->setObjectName(QString::fromUtf8("gridLayout10"));
        hboxLayout7 = new QHBoxLayout();
        hboxLayout7->setSpacing(6);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        m_UseSurfaceTextureCheckBox = new QCheckBox(buttonGroup4);
        m_UseSurfaceTextureCheckBox->setObjectName(QString::fromUtf8("m_UseSurfaceTextureCheckBox"));

        hboxLayout7->addWidget(m_UseSurfaceTextureCheckBox);

        m_SurfaceTexturePushButton = new QPushButton(buttonGroup4);
        m_SurfaceTexturePushButton->setObjectName(QString::fromUtf8("m_SurfaceTexturePushButton"));

        hboxLayout7->addWidget(m_SurfaceTexturePushButton);


        gridLayout10->addLayout(hboxLayout7, 1, 0, 1, 1);

        hboxLayout8 = new QHBoxLayout();
        hboxLayout8->setSpacing(6);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        m_SurfaceColorCheckBox = new QCheckBox(buttonGroup4);
        m_SurfaceColorCheckBox->setObjectName(QString::fromUtf8("m_SurfaceColorCheckBox"));

        hboxLayout8->addWidget(m_SurfaceColorCheckBox);

        m_SurfaceColorPushButton = new QPushButton(buttonGroup4);
        m_SurfaceColorPushButton->setObjectName(QString::fromUtf8("m_SurfaceColorPushButton"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_SurfaceColorPushButton->sizePolicy().hasHeightForWidth());
        m_SurfaceColorPushButton->setSizePolicy(sizePolicy1);

        hboxLayout8->addWidget(m_SurfaceColorPushButton);


        gridLayout10->addLayout(hboxLayout8, 0, 0, 1, 1);


        gridLayout9->addWidget(buttonGroup4, 1, 0, 1, 2);


        gridLayout->addWidget(m_SurfaceGroupBox, 1, 0, 1, 1);

        hboxLayout9 = new QHBoxLayout();
        hboxLayout9->setSpacing(6);
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        m_RenderCheckBox = new QCheckBox(SurfaceRenderingPropertiesBase);
        m_RenderCheckBox->setObjectName(QString::fromUtf8("m_RenderCheckBox"));

        hboxLayout9->addWidget(m_RenderCheckBox);

        m_QSAPushButton = new QPushButton(SurfaceRenderingPropertiesBase);
        m_QSAPushButton->setObjectName(QString::fromUtf8("m_QSAPushButton"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_QSAPushButton->sizePolicy().hasHeightForWidth());
        m_QSAPushButton->setSizePolicy(sizePolicy2);
        m_QSAPushButton->setMinimumSize(QSize(24, 24));
        m_QSAPushButton->setMaximumSize(QSize(24, 24));

        hboxLayout9->addWidget(m_QSAPushButton);

        m_FrameSlider = new QSlider(SurfaceRenderingPropertiesBase);
        m_FrameSlider->setObjectName(QString::fromUtf8("m_FrameSlider"));
        m_FrameSlider->setOrientation(Qt::Horizontal);
        m_FrameSlider->setTickPosition(QSlider::TicksBelow);

        hboxLayout9->addWidget(m_FrameSlider);

        m_ClipSlider = new QSlider(SurfaceRenderingPropertiesBase);
        m_ClipSlider->setObjectName(QString::fromUtf8("m_ClipSlider"));
        m_ClipSlider->setOrientation(Qt::Horizontal);

        hboxLayout9->addWidget(m_ClipSlider);

        m_ReplicatePushButton = new QPushButton(SurfaceRenderingPropertiesBase);
        m_ReplicatePushButton->setObjectName(QString::fromUtf8("m_ReplicatePushButton"));

        hboxLayout9->addWidget(m_ReplicatePushButton);

        m_TransparencySlider = new QSlider(SurfaceRenderingPropertiesBase);
        m_TransparencySlider->setObjectName(QString::fromUtf8("m_TransparencySlider"));
        m_TransparencySlider->setMaximum(100);
        m_TransparencySlider->setValue(100);
        m_TransparencySlider->setOrientation(Qt::Horizontal);

        hboxLayout9->addWidget(m_TransparencySlider);


        gridLayout->addLayout(hboxLayout9, 0, 0, 1, 4);


        retranslateUi(SurfaceRenderingPropertiesBase);

        QMetaObject::connectSlotsByName(SurfaceRenderingPropertiesBase);
    } // setupUi

    void retranslateUi(QWidget *SurfaceRenderingPropertiesBase)
    {
        SurfaceRenderingPropertiesBase->setWindowTitle(QApplication::translate("SurfaceRenderingPropertiesBase", "SurfaceRenderingPropertiesBase", 0, QApplication::UnicodeUTF8));
        groupBox4->setTitle(QApplication::translate("SurfaceRenderingPropertiesBase", "Points", 0, QApplication::UnicodeUTF8));
        m_RenderPointsCheckBox->setText(QString());
        buttonGroup7->setTitle(QString());
        m_UserSelectedPointColorRadioButton->setText(QString());
        m_PointColorPushButton->setText(QString());
        m_DefaultPointColorRadioButton->setText(QApplication::translate("SurfaceRenderingPropertiesBase", "Default", 0, QApplication::UnicodeUTF8));
        m_LinesGgroupBox->setTitle(QApplication::translate("SurfaceRenderingPropertiesBase", "Lines", 0, QApplication::UnicodeUTF8));
        buttonGroup6->setTitle(QString());
        m_UserSelectedLineColorRadioButton->setText(QString());
        m_LineColorPushButton->setText(QString());
        m_DefaultLineColorRadioButton->setText(QApplication::translate("SurfaceRenderingPropertiesBase", "Default", 0, QApplication::UnicodeUTF8));
        m_RenderLinesCheckBox->setText(QString());
        m_WireframeGroupBox->setTitle(QApplication::translate("SurfaceRenderingPropertiesBase", "Wireframe", 0, QApplication::UnicodeUTF8));
        m_RenderWireframeCheckBox->setText(QString());
        buttonGroup5->setTitle(QString());
        m_UserSelectedWireframeColorRadioButton->setText(QString());
        m_WireframeColorPushButton->setText(QString());
        m_DefaultWireframeColorRadioButton->setText(QApplication::translate("SurfaceRenderingPropertiesBase", "Default", 0, QApplication::UnicodeUTF8));
        m_SurfaceGroupBox->setTitle(QApplication::translate("SurfaceRenderingPropertiesBase", "Surface", 0, QApplication::UnicodeUTF8));
        m_RenderSurfaceCheckBox->setText(QString());
        buttonGroup4->setTitle(QString());
        m_UseSurfaceTextureCheckBox->setText(QString());
        m_SurfaceTexturePushButton->setText(QApplication::translate("SurfaceRenderingPropertiesBase", "Tex Img", 0, QApplication::UnicodeUTF8));
        m_SurfaceColorCheckBox->setText(QString());
        m_SurfaceColorPushButton->setText(QString());
        m_RenderCheckBox->setText(QApplication::translate("SurfaceRenderingPropertiesBase", "Render", 0, QApplication::UnicodeUTF8));
        m_QSAPushButton->setText(QApplication::translate("SurfaceRenderingPropertiesBase", "S", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_FrameSlider->setProperty("toolTip", QVariant(QApplication::translate("SurfaceRenderingPropertiesBase", "Frame", "Frame", QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        m_ClipSlider->setProperty("toolTip", QVariant(QApplication::translate("SurfaceRenderingPropertiesBase", "Clip", "Clip", QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        m_ReplicatePushButton->setText(QApplication::translate("SurfaceRenderingPropertiesBase", "Replicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_TransparencySlider->setProperty("toolTip", QVariant(QApplication::translate("SurfaceRenderingPropertiesBase", "Transparency", "Transparency", QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        Q_UNUSED(SurfaceRenderingPropertiesBase);
    } // retranslateUi

};

namespace Ui {
    class SurfaceRenderingPropertiesBase: public Ui_SurfaceRenderingPropertiesBase {};
} // namespace Ui

QT_END_NAMESPACE

class SurfaceRenderingPropertiesBase : public QWidget, public Ui::SurfaceRenderingPropertiesBase
{
    Q_OBJECT

public:
    SurfaceRenderingPropertiesBase(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = {});
    ~SurfaceRenderingPropertiesBase();

protected slots:
    virtual void languageChange();

};

#endif // SURFACERENDERINGPROPERTIESBASE_H
