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
#ifndef TRANSFORMATIONSDIALOGBASE_H
#define TRANSFORMATIONSDIALOGBASE_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3ListBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_TransformationsDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    Q3GroupBox *m_CurrentTransformationsGroupBox;
    QGridLayout *gridLayout1;
    Q3ListBox *m_CurrentTransformationsListBox;
    Q3GroupBox *m_NewTransformationsGroupBox;
    QGridLayout *gridLayout2;
    QHBoxLayout *hboxLayout1;
    QLineEdit *m_XTranslationLineEdit;
    QLineEdit *m_YTranslationLineEdit;
    QLineEdit *m_ZTranslationLineEdit;
    QPushButton *m_AddTranslationPushButton;
    QHBoxLayout *hboxLayout2;
    QLineEdit *m_AngleLineEdit;
    QComboBox *m_RotationAxisComboBox;
    QPushButton *m_AddRotationPushButton;
    QHBoxLayout *hboxLayout3;
    QLineEdit *m_ScaleLineEdit;
    QPushButton *m_AddScalePushButton;
    Q3GroupBox *m_GeneralRotationGroupBox;
    QGridLayout *gridLayout3;
    QLineEdit *m_R00LineEdit;
    QLineEdit *m_R01LineEdit;
    QLineEdit *m_R02LineEdit;
    QLineEdit *m_R10LineEdit;
    QLineEdit *m_R11LineEdit;
    QLineEdit *m_R12LineEdit;
    QLineEdit *m_R20LineEdit;
    QLineEdit *m_R21LineEdit;
    QLineEdit *m_R22LineEdit;
    QHBoxLayout *hboxLayout4;
    QSpacerItem *spacer2;
    QPushButton *m_GeneralRotationPushButton;
    Q3GroupBox *m_GeneralTransformationGroupBox;
    QGridLayout *gridLayout4;
    QLineEdit *m_X10LineEdit;
    QLineEdit *m_X20LineEdit;
    QLineEdit *m_X30LineEdit;
    QLineEdit *m_X31LineEdit;
    QLineEdit *m_X32LineEdit;
    QLineEdit *m_X33LineEdit;
    QLineEdit *m_X00LineEdit;
    QLineEdit *m_X01LineEdit;
    QLineEdit *m_X11LineEdit;
    QLineEdit *m_X21LineEdit;
    QLineEdit *m_X02LineEdit;
    QLineEdit *m_X12LineEdit;
    QLineEdit *m_X22LineEdit;
    QLineEdit *m_X03LineEdit;
    QLineEdit *m_X13LineEdit;
    QLineEdit *m_X23LineEdit;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *spacer3;
    QPushButton *m_GeneralTransformationPushButton;

    void setupUi(QDialog *TransformationsDialogBase)
    {
        if (TransformationsDialogBase->objectName().isEmpty())
            TransformationsDialogBase->setObjectName(QString::fromUtf8("TransformationsDialogBase"));
        TransformationsDialogBase->resize(588, 477);
        TransformationsDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(TransformationsDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(TransformationsDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(TransformationsDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(TransformationsDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 2);

        m_CurrentTransformationsGroupBox = new Q3GroupBox(TransformationsDialogBase);
        m_CurrentTransformationsGroupBox->setObjectName(QString::fromUtf8("m_CurrentTransformationsGroupBox"));
        m_CurrentTransformationsGroupBox->setColumnLayout(0, Qt::Vertical);
        m_CurrentTransformationsGroupBox->layout()->setSpacing(6);
        m_CurrentTransformationsGroupBox->layout()->setMargin(11);
        gridLayout1 = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(m_CurrentTransformationsGroupBox->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        m_CurrentTransformationsListBox = new Q3ListBox(m_CurrentTransformationsGroupBox);
        m_CurrentTransformationsListBox->setObjectName(QString::fromUtf8("m_CurrentTransformationsListBox"));

        gridLayout1->addWidget(m_CurrentTransformationsListBox, 0, 0, 1, 1);


        gridLayout->addWidget(m_CurrentTransformationsGroupBox, 0, 1, 1, 1);

        m_NewTransformationsGroupBox = new Q3GroupBox(TransformationsDialogBase);
        m_NewTransformationsGroupBox->setObjectName(QString::fromUtf8("m_NewTransformationsGroupBox"));
        m_NewTransformationsGroupBox->setColumnLayout(0, Qt::Vertical);
        m_NewTransformationsGroupBox->layout()->setSpacing(6);
        m_NewTransformationsGroupBox->layout()->setMargin(11);
        gridLayout2 = new QGridLayout();
        QBoxLayout *boxlayout1 = qobject_cast<QBoxLayout *>(m_NewTransformationsGroupBox->layout());
        if (boxlayout1)
            boxlayout1->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_XTranslationLineEdit = new QLineEdit(m_NewTransformationsGroupBox);
        m_XTranslationLineEdit->setObjectName(QString::fromUtf8("m_XTranslationLineEdit"));

        hboxLayout1->addWidget(m_XTranslationLineEdit);

        m_YTranslationLineEdit = new QLineEdit(m_NewTransformationsGroupBox);
        m_YTranslationLineEdit->setObjectName(QString::fromUtf8("m_YTranslationLineEdit"));

        hboxLayout1->addWidget(m_YTranslationLineEdit);

        m_ZTranslationLineEdit = new QLineEdit(m_NewTransformationsGroupBox);
        m_ZTranslationLineEdit->setObjectName(QString::fromUtf8("m_ZTranslationLineEdit"));

        hboxLayout1->addWidget(m_ZTranslationLineEdit);

        m_AddTranslationPushButton = new QPushButton(m_NewTransformationsGroupBox);
        m_AddTranslationPushButton->setObjectName(QString::fromUtf8("m_AddTranslationPushButton"));

        hboxLayout1->addWidget(m_AddTranslationPushButton);


        gridLayout2->addLayout(hboxLayout1, 0, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_AngleLineEdit = new QLineEdit(m_NewTransformationsGroupBox);
        m_AngleLineEdit->setObjectName(QString::fromUtf8("m_AngleLineEdit"));

        hboxLayout2->addWidget(m_AngleLineEdit);

        m_RotationAxisComboBox = new QComboBox(m_NewTransformationsGroupBox);
        m_RotationAxisComboBox->setObjectName(QString::fromUtf8("m_RotationAxisComboBox"));

        hboxLayout2->addWidget(m_RotationAxisComboBox);

        m_AddRotationPushButton = new QPushButton(m_NewTransformationsGroupBox);
        m_AddRotationPushButton->setObjectName(QString::fromUtf8("m_AddRotationPushButton"));

        hboxLayout2->addWidget(m_AddRotationPushButton);


        gridLayout2->addLayout(hboxLayout2, 1, 0, 1, 1);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        m_ScaleLineEdit = new QLineEdit(m_NewTransformationsGroupBox);
        m_ScaleLineEdit->setObjectName(QString::fromUtf8("m_ScaleLineEdit"));

        hboxLayout3->addWidget(m_ScaleLineEdit);

        m_AddScalePushButton = new QPushButton(m_NewTransformationsGroupBox);
        m_AddScalePushButton->setObjectName(QString::fromUtf8("m_AddScalePushButton"));

        hboxLayout3->addWidget(m_AddScalePushButton);


        gridLayout2->addLayout(hboxLayout3, 2, 0, 1, 1);

        m_GeneralRotationGroupBox = new Q3GroupBox(m_NewTransformationsGroupBox);
        m_GeneralRotationGroupBox->setObjectName(QString::fromUtf8("m_GeneralRotationGroupBox"));
        m_GeneralRotationGroupBox->setColumnLayout(0, Qt::Vertical);
        m_GeneralRotationGroupBox->layout()->setSpacing(6);
        m_GeneralRotationGroupBox->layout()->setMargin(11);
        gridLayout3 = new QGridLayout();
        QBoxLayout *boxlayout2 = qobject_cast<QBoxLayout *>(m_GeneralRotationGroupBox->layout());
        if (boxlayout2)
            boxlayout2->addLayout(gridLayout3);
        gridLayout3->setAlignment(Qt::AlignTop);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        m_R00LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R00LineEdit->setObjectName(QString::fromUtf8("m_R00LineEdit"));

        gridLayout3->addWidget(m_R00LineEdit, 0, 0, 1, 1);

        m_R01LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R01LineEdit->setObjectName(QString::fromUtf8("m_R01LineEdit"));

        gridLayout3->addWidget(m_R01LineEdit, 0, 1, 1, 1);

        m_R02LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R02LineEdit->setObjectName(QString::fromUtf8("m_R02LineEdit"));

        gridLayout3->addWidget(m_R02LineEdit, 0, 2, 1, 1);

        m_R10LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R10LineEdit->setObjectName(QString::fromUtf8("m_R10LineEdit"));

        gridLayout3->addWidget(m_R10LineEdit, 1, 0, 1, 1);

        m_R11LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R11LineEdit->setObjectName(QString::fromUtf8("m_R11LineEdit"));

        gridLayout3->addWidget(m_R11LineEdit, 1, 1, 1, 1);

        m_R12LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R12LineEdit->setObjectName(QString::fromUtf8("m_R12LineEdit"));

        gridLayout3->addWidget(m_R12LineEdit, 1, 2, 1, 1);

        m_R20LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R20LineEdit->setObjectName(QString::fromUtf8("m_R20LineEdit"));

        gridLayout3->addWidget(m_R20LineEdit, 2, 0, 1, 1);

        m_R21LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R21LineEdit->setObjectName(QString::fromUtf8("m_R21LineEdit"));

        gridLayout3->addWidget(m_R21LineEdit, 2, 1, 1, 1);

        m_R22LineEdit = new QLineEdit(m_GeneralRotationGroupBox);
        m_R22LineEdit->setObjectName(QString::fromUtf8("m_R22LineEdit"));

        gridLayout3->addWidget(m_R22LineEdit, 2, 2, 1, 1);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout4->addItem(spacer2);

        m_GeneralRotationPushButton = new QPushButton(m_GeneralRotationGroupBox);
        m_GeneralRotationPushButton->setObjectName(QString::fromUtf8("m_GeneralRotationPushButton"));

        hboxLayout4->addWidget(m_GeneralRotationPushButton);


        gridLayout3->addLayout(hboxLayout4, 3, 0, 1, 3);


        gridLayout2->addWidget(m_GeneralRotationGroupBox, 3, 0, 1, 1);

        m_GeneralTransformationGroupBox = new Q3GroupBox(m_NewTransformationsGroupBox);
        m_GeneralTransformationGroupBox->setObjectName(QString::fromUtf8("m_GeneralTransformationGroupBox"));
        m_GeneralTransformationGroupBox->setColumnLayout(0, Qt::Vertical);
        m_GeneralTransformationGroupBox->layout()->setSpacing(6);
        m_GeneralTransformationGroupBox->layout()->setMargin(11);
        gridLayout4 = new QGridLayout();
        QBoxLayout *boxlayout3 = qobject_cast<QBoxLayout *>(m_GeneralTransformationGroupBox->layout());
        if (boxlayout3)
            boxlayout3->addLayout(gridLayout4);
        gridLayout4->setAlignment(Qt::AlignTop);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        m_X10LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X10LineEdit->setObjectName(QString::fromUtf8("m_X10LineEdit"));

        gridLayout4->addWidget(m_X10LineEdit, 1, 0, 1, 1);

        m_X20LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X20LineEdit->setObjectName(QString::fromUtf8("m_X20LineEdit"));

        gridLayout4->addWidget(m_X20LineEdit, 2, 0, 1, 1);

        m_X30LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X30LineEdit->setObjectName(QString::fromUtf8("m_X30LineEdit"));

        gridLayout4->addWidget(m_X30LineEdit, 3, 0, 1, 1);

        m_X31LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X31LineEdit->setObjectName(QString::fromUtf8("m_X31LineEdit"));

        gridLayout4->addWidget(m_X31LineEdit, 3, 1, 1, 1);

        m_X32LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X32LineEdit->setObjectName(QString::fromUtf8("m_X32LineEdit"));

        gridLayout4->addWidget(m_X32LineEdit, 3, 2, 1, 1);

        m_X33LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X33LineEdit->setObjectName(QString::fromUtf8("m_X33LineEdit"));

        gridLayout4->addWidget(m_X33LineEdit, 3, 3, 1, 1);

        m_X00LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X00LineEdit->setObjectName(QString::fromUtf8("m_X00LineEdit"));

        gridLayout4->addWidget(m_X00LineEdit, 0, 0, 1, 1);

        m_X01LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X01LineEdit->setObjectName(QString::fromUtf8("m_X01LineEdit"));

        gridLayout4->addWidget(m_X01LineEdit, 0, 1, 1, 1);

        m_X11LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X11LineEdit->setObjectName(QString::fromUtf8("m_X11LineEdit"));

        gridLayout4->addWidget(m_X11LineEdit, 1, 1, 1, 1);

        m_X21LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X21LineEdit->setObjectName(QString::fromUtf8("m_X21LineEdit"));

        gridLayout4->addWidget(m_X21LineEdit, 2, 1, 1, 1);

        m_X02LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X02LineEdit->setObjectName(QString::fromUtf8("m_X02LineEdit"));

        gridLayout4->addWidget(m_X02LineEdit, 0, 2, 1, 1);

        m_X12LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X12LineEdit->setObjectName(QString::fromUtf8("m_X12LineEdit"));

        gridLayout4->addWidget(m_X12LineEdit, 1, 2, 1, 1);

        m_X22LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X22LineEdit->setObjectName(QString::fromUtf8("m_X22LineEdit"));

        gridLayout4->addWidget(m_X22LineEdit, 2, 2, 1, 1);

        m_X03LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X03LineEdit->setObjectName(QString::fromUtf8("m_X03LineEdit"));

        gridLayout4->addWidget(m_X03LineEdit, 0, 3, 1, 1);

        m_X13LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X13LineEdit->setObjectName(QString::fromUtf8("m_X13LineEdit"));

        gridLayout4->addWidget(m_X13LineEdit, 1, 3, 1, 1);

        m_X23LineEdit = new QLineEdit(m_GeneralTransformationGroupBox);
        m_X23LineEdit->setObjectName(QString::fromUtf8("m_X23LineEdit"));

        gridLayout4->addWidget(m_X23LineEdit, 2, 3, 1, 1);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(6);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        spacer3 = new QSpacerItem(270, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout5->addItem(spacer3);

        m_GeneralTransformationPushButton = new QPushButton(m_GeneralTransformationGroupBox);
        m_GeneralTransformationPushButton->setObjectName(QString::fromUtf8("m_GeneralTransformationPushButton"));

        hboxLayout5->addWidget(m_GeneralTransformationPushButton);


        gridLayout4->addLayout(hboxLayout5, 4, 0, 1, 4);


        gridLayout2->addWidget(m_GeneralTransformationGroupBox, 4, 0, 1, 1);


        gridLayout->addWidget(m_NewTransformationsGroupBox, 0, 0, 1, 1);

        QWidget::setTabOrder(buttonHelp, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);
        QWidget::setTabOrder(buttonCancel, m_CurrentTransformationsListBox);
        QWidget::setTabOrder(m_CurrentTransformationsListBox, m_XTranslationLineEdit);
        QWidget::setTabOrder(m_XTranslationLineEdit, m_YTranslationLineEdit);
        QWidget::setTabOrder(m_YTranslationLineEdit, m_ZTranslationLineEdit);
        QWidget::setTabOrder(m_ZTranslationLineEdit, m_AddTranslationPushButton);
        QWidget::setTabOrder(m_AddTranslationPushButton, m_AngleLineEdit);
        QWidget::setTabOrder(m_AngleLineEdit, m_RotationAxisComboBox);
        QWidget::setTabOrder(m_RotationAxisComboBox, m_AddRotationPushButton);
        QWidget::setTabOrder(m_AddRotationPushButton, m_ScaleLineEdit);
        QWidget::setTabOrder(m_ScaleLineEdit, m_AddScalePushButton);
        QWidget::setTabOrder(m_AddScalePushButton, m_R00LineEdit);
        QWidget::setTabOrder(m_R00LineEdit, m_R01LineEdit);
        QWidget::setTabOrder(m_R01LineEdit, m_R02LineEdit);
        QWidget::setTabOrder(m_R02LineEdit, m_R10LineEdit);
        QWidget::setTabOrder(m_R10LineEdit, m_R11LineEdit);
        QWidget::setTabOrder(m_R11LineEdit, m_R12LineEdit);
        QWidget::setTabOrder(m_R12LineEdit, m_R20LineEdit);
        QWidget::setTabOrder(m_R20LineEdit, m_R21LineEdit);
        QWidget::setTabOrder(m_R21LineEdit, m_R22LineEdit);
        QWidget::setTabOrder(m_R22LineEdit, m_GeneralRotationPushButton);
        QWidget::setTabOrder(m_GeneralRotationPushButton, m_X00LineEdit);
        QWidget::setTabOrder(m_X00LineEdit, m_X01LineEdit);
        QWidget::setTabOrder(m_X01LineEdit, m_X02LineEdit);
        QWidget::setTabOrder(m_X02LineEdit, m_X03LineEdit);
        QWidget::setTabOrder(m_X03LineEdit, m_X10LineEdit);
        QWidget::setTabOrder(m_X10LineEdit, m_X11LineEdit);
        QWidget::setTabOrder(m_X11LineEdit, m_X12LineEdit);
        QWidget::setTabOrder(m_X12LineEdit, m_X13LineEdit);
        QWidget::setTabOrder(m_X13LineEdit, m_X20LineEdit);
        QWidget::setTabOrder(m_X20LineEdit, m_X21LineEdit);
        QWidget::setTabOrder(m_X21LineEdit, m_X22LineEdit);
        QWidget::setTabOrder(m_X22LineEdit, m_X23LineEdit);
        QWidget::setTabOrder(m_X23LineEdit, m_X30LineEdit);
        QWidget::setTabOrder(m_X30LineEdit, m_X31LineEdit);
        QWidget::setTabOrder(m_X31LineEdit, m_X32LineEdit);
        QWidget::setTabOrder(m_X32LineEdit, m_X33LineEdit);
        QWidget::setTabOrder(m_X33LineEdit, m_GeneralTransformationPushButton);

        retranslateUi(TransformationsDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), TransformationsDialogBase, SLOT(accept()));
        QObject::connect(m_AddRotationPushButton, SIGNAL(clicked()), TransformationsDialogBase, SLOT(addRotationSlot()));
        QObject::connect(m_AddScalePushButton, SIGNAL(clicked()), TransformationsDialogBase, SLOT(addScaleSlot()));
        QObject::connect(m_AddTranslationPushButton, SIGNAL(clicked()), TransformationsDialogBase, SLOT(addTranslationSlot()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), TransformationsDialogBase, SLOT(reject()));
        QObject::connect(m_GeneralRotationPushButton, SIGNAL(clicked()), TransformationsDialogBase, SLOT(addGeneralRotationSlot()));
        QObject::connect(m_GeneralTransformationPushButton, SIGNAL(clicked()), TransformationsDialogBase, SLOT(addGeneralXFormSlot()));

        QMetaObject::connectSlotsByName(TransformationsDialogBase);
    } // setupUi

    void retranslateUi(QDialog *TransformationsDialogBase)
    {
        TransformationsDialogBase->setWindowTitle(QApplication::translate("TransformationsDialogBase", "Objects transformations", 0, QApplication::UnicodeUTF8));
        buttonHelp->setText(QApplication::translate("TransformationsDialogBase", "&Help", 0, QApplication::UnicodeUTF8));
        buttonHelp->setShortcut(QApplication::translate("TransformationsDialogBase", "F1", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("TransformationsDialogBase", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("TransformationsDialogBase", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        m_CurrentTransformationsGroupBox->setTitle(QApplication::translate("TransformationsDialogBase", "Current transformations", 0, QApplication::UnicodeUTF8));
        m_CurrentTransformationsListBox->clear();
        m_CurrentTransformationsListBox->insertItem(QApplication::translate("TransformationsDialogBase", "New Item", 0, QApplication::UnicodeUTF8));
        m_NewTransformationsGroupBox->setTitle(QApplication::translate("TransformationsDialogBase", "Add new transformation", 0, QApplication::UnicodeUTF8));
        m_AddTranslationPushButton->setText(QApplication::translate("TransformationsDialogBase", "Add translation", 0, QApplication::UnicodeUTF8));
        m_AddRotationPushButton->setText(QApplication::translate("TransformationsDialogBase", "Add rotation", 0, QApplication::UnicodeUTF8));
        m_AddScalePushButton->setText(QApplication::translate("TransformationsDialogBase", "Add scale", 0, QApplication::UnicodeUTF8));
        m_GeneralRotationGroupBox->setTitle(QString());
        m_GeneralRotationPushButton->setText(QApplication::translate("TransformationsDialogBase", "General Rotation", 0, QApplication::UnicodeUTF8));
        m_GeneralTransformationGroupBox->setTitle(QString());
        m_GeneralTransformationPushButton->setText(QApplication::translate("TransformationsDialogBase", "General Xform", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TransformationsDialogBase);
    } // retranslateUi

};

namespace Ui {
    class TransformationsDialogBase: public Ui_TransformationsDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class TransformationsDialogBase : public QDialog, public Ui::TransformationsDialogBase
{
    Q_OBJECT

public:
    TransformationsDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~TransformationsDialogBase();

public slots:
    virtual void addRotationSlot();
    virtual void deleteTransformationSlot();
    virtual void addTranslationSlot();
    virtual void addScaleSlot();
    virtual void clearAllSlot();
    virtual void addGeneralRotationSlot();
    virtual void addGeneralXFormSlot();

protected slots:
    virtual void languageChange();

};

#endif // TRANSFORMATIONSDIALOGBASE_H
