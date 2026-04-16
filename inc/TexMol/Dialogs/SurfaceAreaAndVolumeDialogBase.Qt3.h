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


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

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
        SurfaceAreaAndVolumeDialogBase->setWindowTitle(QApplication::translate("SurfaceAreaAndVolumeDialogBase", "Surface area and volume", 0, QApplication::UnicodeUTF8));
        outputVolume->setText(QString());
        outputArea->setText(QString());
        textLabel2->setText(QApplication::translate("SurfaceAreaAndVolumeDialogBase", "\303\205\302\263", 0, QApplication::UnicodeUTF8));
        labelVolume->setText(QApplication::translate("SurfaceAreaAndVolumeDialogBase", "Volume:", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("SurfaceAreaAndVolumeDialogBase", "\303\205\302\262", 0, QApplication::UnicodeUTF8));
        labelArea->setText(QApplication::translate("SurfaceAreaAndVolumeDialogBase", "Area:", 0, QApplication::UnicodeUTF8));
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
    SurfaceAreaAndVolumeDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~SurfaceAreaAndVolumeDialogBase();

protected slots:
    virtual void languageChange();

};

#endif // SURFACEAREAANDVOLUMEDIALOGBASE_H
