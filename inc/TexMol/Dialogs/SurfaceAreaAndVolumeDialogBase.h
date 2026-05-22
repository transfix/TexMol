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
#ifndef SURFACEAREAANDVOLUMEDIALOGBASE_H
#define SURFACEAREAANDVOLUMEDIALOGBASE_H

#include <qvariant.h>



#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_SurfaceAreaAndVolumeDialogBase
{
public:
    QWidget *layout2;
    QGridLayout *gridLayout;
    QLabel *outputVolume;
    QLabel *outputArea;
    QLabel *textLabel2;
    QLabel *labelVolume;
    QLabel *textLabel1;
    QLabel *labelArea;

    void setupUi(QDialog *SurfaceAreaAndVolumeDialogBase)
    {
        if (SurfaceAreaAndVolumeDialogBase->objectName().isEmpty())
            SurfaceAreaAndVolumeDialogBase->setObjectName(QString::fromUtf8("SurfaceAreaAndVolumeDialogBase"));
        SurfaceAreaAndVolumeDialogBase->resize(235, 99);
        layout2 = new QWidget(SurfaceAreaAndVolumeDialogBase);
        layout2->setObjectName(QString::fromUtf8("layout2"));
        layout2->setGeometry(QRect(11, 11, 210, 70));
        gridLayout = new QGridLayout(layout2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        outputVolume = new QLabel(layout2);
        outputVolume->setObjectName(QString::fromUtf8("outputVolume"));
        outputVolume->setWordWrap(false);

        gridLayout->addWidget(outputVolume, 1, 1, 1, 1);

        outputArea = new QLabel(layout2);
        outputArea->setObjectName(QString::fromUtf8("outputArea"));
        outputArea->setWordWrap(false);

        gridLayout->addWidget(outputArea, 0, 1, 1, 1);

        textLabel2 = new QLabel(layout2);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        gridLayout->addWidget(textLabel2, 1, 2, 1, 1);

        labelVolume = new QLabel(layout2);
        labelVolume->setObjectName(QString::fromUtf8("labelVolume"));
        labelVolume->setWordWrap(false);

        gridLayout->addWidget(labelVolume, 1, 0, 1, 1);

        textLabel1 = new QLabel(layout2);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        gridLayout->addWidget(textLabel1, 0, 2, 1, 1);

        labelArea = new QLabel(layout2);
        labelArea->setObjectName(QString::fromUtf8("labelArea"));
        labelArea->setWordWrap(false);

        gridLayout->addWidget(labelArea, 0, 0, 1, 1);


        retranslateUi(SurfaceAreaAndVolumeDialogBase);

        QMetaObject::connectSlotsByName(SurfaceAreaAndVolumeDialogBase);
    } // setupUi

    void retranslateUi(QDialog *SurfaceAreaAndVolumeDialogBase)
    {
        SurfaceAreaAndVolumeDialogBase->setWindowTitle(QCoreApplication::translate("SurfaceAreaAndVolumeDialogBase", "Surface area and volume"));
        outputVolume->setText(QString());
        outputArea->setText(QString());
        textLabel2->setText(QCoreApplication::translate("SurfaceAreaAndVolumeDialogBase", "\303\205\302\263"));
        labelVolume->setText(QCoreApplication::translate("SurfaceAreaAndVolumeDialogBase", "Volume:"));
        textLabel1->setText(QCoreApplication::translate("SurfaceAreaAndVolumeDialogBase", "\303\205\302\262"));
        labelArea->setText(QCoreApplication::translate("SurfaceAreaAndVolumeDialogBase", "Area:"));
        Q_UNUSED(SurfaceAreaAndVolumeDialogBase);
    } // retranslateUi

};

namespace Ui {
    class SurfaceAreaAndVolumeDialogBase: public Ui_SurfaceAreaAndVolumeDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class SurfaceAreaAndVolumeDialogBase : public QDialog, public Ui::SurfaceAreaAndVolumeDialogBase
{
    Q_OBJECT

public:
    SurfaceAreaAndVolumeDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~SurfaceAreaAndVolumeDialogBase() {}

protected slots:
    virtual void languageChange() {}

};

#endif // SURFACEAREAANDVOLUMEDIALOGBASE_H
