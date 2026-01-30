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
#ifndef NURBSRENDERINGPROPERTIESBASE_H
#define NURBSRENDERINGPROPERTIESBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NURBSRenderingPropertiesBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QCheckBox *m_RenderCheckBox;
    QPushButton *m_QSAPushButton;
    QSlider *m_FrameSlider;
    QSlider *m_ClipSlider;
    QPushButton *m_ReplicatePushButton;

    void setupUi(QWidget *NURBSRenderingPropertiesBase)
    {
        if (NURBSRenderingPropertiesBase->objectName().isEmpty())
            NURBSRenderingPropertiesBase->setObjectName(QString::fromUtf8("NURBSRenderingPropertiesBase"));
        NURBSRenderingPropertiesBase->resize(624, 180);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NURBSRenderingPropertiesBase->sizePolicy().hasHeightForWidth());
        NURBSRenderingPropertiesBase->setSizePolicy(sizePolicy);
        NURBSRenderingPropertiesBase->setMinimumSize(QSize(0, 180));
        NURBSRenderingPropertiesBase->setMaximumSize(QSize(32767, 180));
        gridLayout = new QGridLayout(NURBSRenderingPropertiesBase);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_RenderCheckBox = new QCheckBox(NURBSRenderingPropertiesBase);
        m_RenderCheckBox->setObjectName(QString::fromUtf8("m_RenderCheckBox"));

        hboxLayout->addWidget(m_RenderCheckBox);

        m_QSAPushButton = new QPushButton(NURBSRenderingPropertiesBase);
        m_QSAPushButton->setObjectName(QString::fromUtf8("m_QSAPushButton"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_QSAPushButton->sizePolicy().hasHeightForWidth());
        m_QSAPushButton->setSizePolicy(sizePolicy1);
        m_QSAPushButton->setMinimumSize(QSize(24, 24));
        m_QSAPushButton->setMaximumSize(QSize(24, 24));

        hboxLayout->addWidget(m_QSAPushButton);

        m_FrameSlider = new QSlider(NURBSRenderingPropertiesBase);
        m_FrameSlider->setObjectName(QString::fromUtf8("m_FrameSlider"));
        m_FrameSlider->setOrientation(Qt::Horizontal);
        m_FrameSlider->setTickPosition(QSlider::TicksBelow);

        hboxLayout->addWidget(m_FrameSlider);

        m_ClipSlider = new QSlider(NURBSRenderingPropertiesBase);
        m_ClipSlider->setObjectName(QString::fromUtf8("m_ClipSlider"));
        m_ClipSlider->setOrientation(Qt::Horizontal);

        hboxLayout->addWidget(m_ClipSlider);

        m_ReplicatePushButton = new QPushButton(NURBSRenderingPropertiesBase);
        m_ReplicatePushButton->setObjectName(QString::fromUtf8("m_ReplicatePushButton"));

        hboxLayout->addWidget(m_ReplicatePushButton);


        gridLayout->addLayout(hboxLayout, 0, 0, 1, 4);


        retranslateUi(NURBSRenderingPropertiesBase);

        QMetaObject::connectSlotsByName(NURBSRenderingPropertiesBase);
    } // setupUi

    void retranslateUi(QWidget *NURBSRenderingPropertiesBase)
    {
        NURBSRenderingPropertiesBase->setWindowTitle(QApplication::translate("NURBSRenderingPropertiesBase", "NURBSRenderingPropertiesBase", 0, QApplication::UnicodeUTF8));
        m_RenderCheckBox->setText(QApplication::translate("NURBSRenderingPropertiesBase", "Render", 0, QApplication::UnicodeUTF8));
        m_QSAPushButton->setText(QApplication::translate("NURBSRenderingPropertiesBase", "S", 0, QApplication::UnicodeUTF8));
        m_ReplicatePushButton->setText(QApplication::translate("NURBSRenderingPropertiesBase", "Replicate", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(NURBSRenderingPropertiesBase);
    } // retranslateUi

};

namespace Ui {
    class NURBSRenderingPropertiesBase: public Ui_NURBSRenderingPropertiesBase {};
} // namespace Ui

QT_END_NAMESPACE

class NURBSRenderingPropertiesBase : public QWidget, public Ui::NURBSRenderingPropertiesBase
{
    Q_OBJECT

public:
    NURBSRenderingPropertiesBase(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0);
    ~NURBSRenderingPropertiesBase();

protected slots:
    virtual void languageChange();

};

#endif // NURBSRENDERINGPROPERTIESBASE_H
