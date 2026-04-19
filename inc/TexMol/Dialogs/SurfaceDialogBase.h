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
#ifndef SURFACEDIALOGBASE_H
#define SURFACEDIALOGBASE_H

#include <qvariant.h>



#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SurfaceDialogBase
{
public:
    QHBoxLayout *hboxLayout;
    QGridLayout *gridLayout;
    QComboBox *inputAlgorithm;
    QPushButton *pushButtonCalculate;
    QLabel *labelLevel;
    QSpinBox *inputSize;
    QCheckBox *inputOptimizeRadii;
    QSpinBox *inputLevel;
    QLabel *labelBlobbiness;
    QLineEdit *inputWeight;
    QComboBox *inputRadiusType;
    QLabel *textLabel1;
    QLabel *labelSize;
    QSpinBox *inputIterations;
    QLabel *textIsovalue;
    QSpinBox *inputIsovalue;
    QSpacerItem *spacer1;
    QLabel *labelWeight;
    QLabel *labelRadiusType;
    QLabel *labelAlgorithm;
    QLineEdit *inputBlobbiness;
    QLabel *labelIterations;

    void setupUi(QDialog *SurfaceDialogBase)
    {
        if (SurfaceDialogBase->objectName().isEmpty())
            SurfaceDialogBase->setObjectName(QString::fromUtf8("SurfaceDialogBase"));
        SurfaceDialogBase->resize(303, 295);
        SurfaceDialogBase->setSizeGripEnabled(false);
        SurfaceDialogBase->setModal(true);
        hboxLayout = new QHBoxLayout(SurfaceDialogBase);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        inputAlgorithm = new QComboBox(SurfaceDialogBase);
        inputAlgorithm->setObjectName(QString::fromUtf8("inputAlgorithm"));

        gridLayout->addWidget(inputAlgorithm, 0, 1, 1, 2);

        pushButtonCalculate = new QPushButton(SurfaceDialogBase);
        pushButtonCalculate->setObjectName(QString::fromUtf8("pushButtonCalculate"));

        gridLayout->addWidget(pushButtonCalculate, 9, 2, 1, 1);

        labelLevel = new QLabel(SurfaceDialogBase);
        labelLevel->setObjectName(QString::fromUtf8("labelLevel"));
        labelLevel->setWordWrap(false);

        gridLayout->addWidget(labelLevel, 5, 0, 1, 1);

        inputSize = new QSpinBox(SurfaceDialogBase);
        inputSize->setObjectName(QString::fromUtf8("inputSize"));
        inputSize->setMaximum(999999);
        inputSize->setMinimum(32);
        inputSize->setSingleStep(32);
        inputSize->setValue(128);

        gridLayout->addWidget(inputSize, 1, 1, 1, 2);

        inputOptimizeRadii = new QCheckBox(SurfaceDialogBase);
        inputOptimizeRadii->setObjectName(QString::fromUtf8("inputOptimizeRadii"));

        gridLayout->addWidget(inputOptimizeRadii, 8, 1, 1, 2);

        inputLevel = new QSpinBox(SurfaceDialogBase);
        inputLevel->setObjectName(QString::fromUtf8("inputLevel"));
        inputLevel->setMaximum(999999);
        inputLevel->setMinimum(1);
        inputLevel->setSingleStep(1);
        inputLevel->setValue(1);

        gridLayout->addWidget(inputLevel, 5, 1, 1, 2);

        labelBlobbiness = new QLabel(SurfaceDialogBase);
        labelBlobbiness->setObjectName(QString::fromUtf8("labelBlobbiness"));
        labelBlobbiness->setWordWrap(false);

        gridLayout->addWidget(labelBlobbiness, 3, 0, 1, 1);

        inputWeight = new QLineEdit(SurfaceDialogBase);
        inputWeight->setObjectName(QString::fromUtf8("inputWeight"));

        gridLayout->addWidget(inputWeight, 7, 1, 1, 2);

        inputRadiusType = new QComboBox(SurfaceDialogBase);
        inputRadiusType->setObjectName(QString::fromUtf8("inputRadiusType"));

        gridLayout->addWidget(inputRadiusType, 4, 1, 1, 2);

        textLabel1 = new QLabel(SurfaceDialogBase);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        gridLayout->addWidget(textLabel1, 8, 0, 1, 1);

        labelSize = new QLabel(SurfaceDialogBase);
        labelSize->setObjectName(QString::fromUtf8("labelSize"));
        labelSize->setWordWrap(false);

        gridLayout->addWidget(labelSize, 1, 0, 1, 1);

        inputIterations = new QSpinBox(SurfaceDialogBase);
        inputIterations->setObjectName(QString::fromUtf8("inputIterations"));
        inputIterations->setMinimum(1);
        inputIterations->setValue(3);

        gridLayout->addWidget(inputIterations, 6, 1, 1, 2);

        textIsovalue = new QLabel(SurfaceDialogBase);
        textIsovalue->setObjectName(QString::fromUtf8("textIsovalue"));
        textIsovalue->setWordWrap(false);

        gridLayout->addWidget(textIsovalue, 2, 0, 1, 1);

        inputIsovalue = new QSpinBox(SurfaceDialogBase);
        inputIsovalue->setObjectName(QString::fromUtf8("inputIsovalue"));
        inputIsovalue->setValue(1);

        gridLayout->addWidget(inputIsovalue, 2, 1, 1, 2);

        spacer1 = new QSpacerItem(292, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer1, 9, 0, 1, 2);

        labelWeight = new QLabel(SurfaceDialogBase);
        labelWeight->setObjectName(QString::fromUtf8("labelWeight"));
        labelWeight->setWordWrap(false);

        gridLayout->addWidget(labelWeight, 7, 0, 1, 1);

        labelRadiusType = new QLabel(SurfaceDialogBase);
        labelRadiusType->setObjectName(QString::fromUtf8("labelRadiusType"));
        labelRadiusType->setWordWrap(false);

        gridLayout->addWidget(labelRadiusType, 4, 0, 1, 1);

        labelAlgorithm = new QLabel(SurfaceDialogBase);
        labelAlgorithm->setObjectName(QString::fromUtf8("labelAlgorithm"));
        labelAlgorithm->setWordWrap(false);

        gridLayout->addWidget(labelAlgorithm, 0, 0, 1, 1);

        inputBlobbiness = new QLineEdit(SurfaceDialogBase);
        inputBlobbiness->setObjectName(QString::fromUtf8("inputBlobbiness"));

        gridLayout->addWidget(inputBlobbiness, 3, 1, 1, 2);

        labelIterations = new QLabel(SurfaceDialogBase);
        labelIterations->setObjectName(QString::fromUtf8("labelIterations"));
        labelIterations->setWordWrap(false);

        gridLayout->addWidget(labelIterations, 6, 0, 1, 1);


        hboxLayout->addLayout(gridLayout);


        retranslateUi(SurfaceDialogBase);
        QObject::connect(pushButtonCalculate, SIGNAL(clicked()), SurfaceDialogBase, SLOT(accept()));
        QObject::connect(inputAlgorithm, SIGNAL(activated(QString)), SurfaceDialogBase, SLOT(setupSlot(QString)));

        QMetaObject::connectSlotsByName(SurfaceDialogBase);
    } // setupUi

    void retranslateUi(QDialog *SurfaceDialogBase)
    {
        SurfaceDialogBase->setWindowTitle(QCoreApplication::translate("SurfaceDialogBase", "Generate surface"));
#ifndef QT_NO_TOOLTIP
        SurfaceDialogBase->setProperty("toolTip", QVariant(QString()));
#endif // QT_NO_TOOLTIP
        inputAlgorithm->clear();
        inputAlgorithm->insertItems(0, QStringList()
         << QCoreApplication::translate("SurfaceDialogBase", "Adaptive grid")
         << QCoreApplication::translate("SurfaceDialogBase", "HLS")
         << QCoreApplication::translate("SurfaceDialogBase", "Gaussian blur")
         << QCoreApplication::translate("SurfaceDialogBase", "LBIE")
        );
#ifndef QT_NO_TOOLTIP
        inputAlgorithm->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Select an algorithm to use when generating the surface.")));
#endif // QT_NO_TOOLTIP
        pushButtonCalculate->setText(QCoreApplication::translate("SurfaceDialogBase", "Calculate"));
#ifndef QT_NO_TOOLTIP
        pushButtonCalculate->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Generate the surface.")));
#endif // QT_NO_TOOLTIP
        labelLevel->setText(QCoreApplication::translate("SurfaceDialogBase", "Level"));
#ifndef QT_NO_TOOLTIP
        inputSize->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Size of the cube containing the surface.")));
#endif // QT_NO_TOOLTIP
        inputOptimizeRadii->setText(QString());
        labelBlobbiness->setText(QCoreApplication::translate("SurfaceDialogBase", "Blobbiness"));
#ifndef QT_NO_TOOLTIP
        labelBlobbiness->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Blobbiness, in thousandths.")));
#endif // QT_NO_TOOLTIP
        inputWeight->setText(QCoreApplication::translate("SurfaceDialogBase", "0.001"));
        inputRadiusType->clear();
        inputRadiusType->insertItems(0, QStringList()
         << QCoreApplication::translate("SurfaceDialogBase", "van der Waals")
         << QCoreApplication::translate("SurfaceDialogBase", "Roux")
        );
#ifndef QT_NO_TOOLTIP
        inputRadiusType->setProperty("toolTip", QVariant(QString()));
#endif // QT_NO_TOOLTIP
        textLabel1->setText(QCoreApplication::translate("SurfaceDialogBase", "Optimize radii"));
        labelSize->setText(QCoreApplication::translate("SurfaceDialogBase", "Size"));
#ifndef QT_NO_TOOLTIP
        labelSize->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Size of the cube containing the surface.")));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        inputIterations->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Number of iterations to run algorithm.")));
#endif // QT_NO_TOOLTIP
        textIsovalue->setText(QCoreApplication::translate("SurfaceDialogBase", "Isovalue"));
        labelWeight->setText(QCoreApplication::translate("SurfaceDialogBase", "Weight"));
#ifndef QT_NO_TOOLTIP
        labelWeight->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Weight, in thousandths.")));
#endif // QT_NO_TOOLTIP
        labelRadiusType->setText(QCoreApplication::translate("SurfaceDialogBase", "Radius type"));
        labelAlgorithm->setText(QCoreApplication::translate("SurfaceDialogBase", "Algorithm"));
#ifndef QT_NO_TOOLTIP
        labelAlgorithm->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Select an algorithm to use when generating the surface.")));
#endif // QT_NO_TOOLTIP
        inputBlobbiness->setText(QCoreApplication::translate("SurfaceDialogBase", "-2.3"));
        labelIterations->setText(QCoreApplication::translate("SurfaceDialogBase", "Iterations"));
#ifndef QT_NO_TOOLTIP
        labelIterations->setProperty("toolTip", QVariant(QCoreApplication::translate("SurfaceDialogBase", "Number of iterations to run algorithm.")));
#endif // QT_NO_TOOLTIP
        Q_UNUSED(SurfaceDialogBase);
    } // retranslateUi

};

namespace Ui {
    class SurfaceDialogBase: public Ui_SurfaceDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class SurfaceDialogBase : public QDialog, public Ui::SurfaceDialogBase
{
    Q_OBJECT

public:
    SurfaceDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~SurfaceDialogBase() {}

public slots:
    virtual void setupSlot( const QString & algorithm ) {}

protected slots:
    virtual void languageChange() {}

};

#endif // SURFACEDIALOGBASE_H
