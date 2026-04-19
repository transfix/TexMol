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
#ifndef MOUSEHANDLERDIALOGBASE_H
#define MOUSEHANDLERDIALOGBASE_H

#include <qvariant.h>


#include <QGroupBox>

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QFrame>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_MouseHandlerDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QGroupBox *groupBox4;
    QGridLayout *gridLayout1;
    QFrame *line1;
    QFrame *line2;
    QGridLayout *gridLayout2;
    QLabel *textLabel5;
    QComboBox *m_Key_View_Translate_ComboBox;
    QComboBox *m_Key_View_Zoom_ComboBox;
    QLabel *textLabel2;
    QLabel *textLabel3;
    QComboBox *m_Button_View_Zoom_ComboBox;
    QComboBox *m_Key_View_Rotate_ComboBox;
    QComboBox *m_Button_View_Rotate_ComboBox;
    QLabel *textLabel6;
    QLabel *textLabel1;
    QComboBox *m_Button_View_Translate_ComboBox;
    QLabel *textLabel4;
    QGridLayout *gridLayout3;
    QComboBox *m_Button_Object_Zoom_ComboBox;
    QComboBox *m_Key_Object_Rotate_ComboBox;
    QComboBox *m_Button_Object_Rotate_ComboBox;
    QComboBox *m_Key_Object_Translate_ComboBox;
    QLabel *textLabel8;
    QComboBox *m_Key_Object_Zoom_ComboBox;
    QComboBox *m_Button_Object_Translate_ComboBox;
    QLabel *textLabel7;
    QLabel *textLabel9;
    QGridLayout *gridLayout4;
    QComboBox *m_Key_Object_Select_ComboBox;
    QComboBox *m_Button_Object_Select_ComboBox;
    QLabel *textLabel10;

    void setupUi(QDialog *MouseHandlerDialogBase)
    {
        if (MouseHandlerDialogBase->objectName().isEmpty())
            MouseHandlerDialogBase->setObjectName(QString::fromUtf8("MouseHandlerDialogBase"));
        MouseHandlerDialogBase->resize(386, 307);
        MouseHandlerDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(MouseHandlerDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(MouseHandlerDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(MouseHandlerDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

        groupBox4 = new QGroupBox(MouseHandlerDialogBase);
        groupBox4->setObjectName(QString::fromUtf8("groupBox4"));
            QVBoxLayout *_vbl1 = new QVBoxLayout(groupBox4);

            _vbl1->setSpacing(6);

            _vbl1->setContentsMargins(11, 11, 11, 11);

            gridLayout1 = new QGridLayout();

            _vbl1->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        line1 = new QFrame(groupBox4);
        line1->setObjectName(QString::fromUtf8("line1"));
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);

        gridLayout1->addWidget(line1, 3, 0, 1, 1);

        line2 = new QFrame(groupBox4);
        line2->setObjectName(QString::fromUtf8("line2"));
        line2->setFrameShape(QFrame::HLine);
        line2->setFrameShadow(QFrame::Sunken);

        gridLayout1->addWidget(line2, 1, 0, 1, 1);

        gridLayout2 = new QGridLayout();
        gridLayout2->setSpacing(6);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        textLabel5 = new QLabel(groupBox4);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        textLabel5->setWordWrap(false);

        gridLayout2->addWidget(textLabel5, 2, 0, 1, 1);

        m_Key_View_Translate_ComboBox = new QComboBox(groupBox4);
        m_Key_View_Translate_ComboBox->setObjectName(QString::fromUtf8("m_Key_View_Translate_ComboBox"));

        gridLayout2->addWidget(m_Key_View_Translate_ComboBox, 1, 2, 1, 1);

        m_Key_View_Zoom_ComboBox = new QComboBox(groupBox4);
        m_Key_View_Zoom_ComboBox->setObjectName(QString::fromUtf8("m_Key_View_Zoom_ComboBox"));

        gridLayout2->addWidget(m_Key_View_Zoom_ComboBox, 2, 2, 1, 1);

        textLabel2 = new QLabel(groupBox4);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        QFont font;
        font.setBold(true);
        textLabel2->setFont(font);
        textLabel2->setWordWrap(false);

        gridLayout2->addWidget(textLabel2, 0, 2, 1, 1);

        textLabel3 = new QLabel(groupBox4);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setFont(font);
        textLabel3->setWordWrap(false);

        gridLayout2->addWidget(textLabel3, 0, 0, 1, 1);

        m_Button_View_Zoom_ComboBox = new QComboBox(groupBox4);
        m_Button_View_Zoom_ComboBox->setObjectName(QString::fromUtf8("m_Button_View_Zoom_ComboBox"));

        gridLayout2->addWidget(m_Button_View_Zoom_ComboBox, 2, 1, 1, 1);

        m_Key_View_Rotate_ComboBox = new QComboBox(groupBox4);
        m_Key_View_Rotate_ComboBox->setObjectName(QString::fromUtf8("m_Key_View_Rotate_ComboBox"));

        gridLayout2->addWidget(m_Key_View_Rotate_ComboBox, 3, 2, 1, 1);

        m_Button_View_Rotate_ComboBox = new QComboBox(groupBox4);
        m_Button_View_Rotate_ComboBox->setObjectName(QString::fromUtf8("m_Button_View_Rotate_ComboBox"));

        gridLayout2->addWidget(m_Button_View_Rotate_ComboBox, 3, 1, 1, 1);

        textLabel6 = new QLabel(groupBox4);
        textLabel6->setObjectName(QString::fromUtf8("textLabel6"));
        textLabel6->setWordWrap(false);

        gridLayout2->addWidget(textLabel6, 3, 0, 1, 1);

        textLabel1 = new QLabel(groupBox4);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setFont(font);
        textLabel1->setWordWrap(false);

        gridLayout2->addWidget(textLabel1, 0, 1, 1, 1);

        m_Button_View_Translate_ComboBox = new QComboBox(groupBox4);
        m_Button_View_Translate_ComboBox->setObjectName(QString::fromUtf8("m_Button_View_Translate_ComboBox"));

        gridLayout2->addWidget(m_Button_View_Translate_ComboBox, 1, 1, 1, 1);

        textLabel4 = new QLabel(groupBox4);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setWordWrap(false);

        gridLayout2->addWidget(textLabel4, 1, 0, 1, 1);


        gridLayout1->addLayout(gridLayout2, 0, 0, 1, 1);

        gridLayout3 = new QGridLayout();
        gridLayout3->setSpacing(6);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        m_Button_Object_Zoom_ComboBox = new QComboBox(groupBox4);
        m_Button_Object_Zoom_ComboBox->setObjectName(QString::fromUtf8("m_Button_Object_Zoom_ComboBox"));

        gridLayout3->addWidget(m_Button_Object_Zoom_ComboBox, 1, 1, 1, 1);

        m_Key_Object_Rotate_ComboBox = new QComboBox(groupBox4);
        m_Key_Object_Rotate_ComboBox->setObjectName(QString::fromUtf8("m_Key_Object_Rotate_ComboBox"));

        gridLayout3->addWidget(m_Key_Object_Rotate_ComboBox, 2, 2, 1, 1);

        m_Button_Object_Rotate_ComboBox = new QComboBox(groupBox4);
        m_Button_Object_Rotate_ComboBox->setObjectName(QString::fromUtf8("m_Button_Object_Rotate_ComboBox"));

        gridLayout3->addWidget(m_Button_Object_Rotate_ComboBox, 2, 1, 1, 1);

        m_Key_Object_Translate_ComboBox = new QComboBox(groupBox4);
        m_Key_Object_Translate_ComboBox->setObjectName(QString::fromUtf8("m_Key_Object_Translate_ComboBox"));

        gridLayout3->addWidget(m_Key_Object_Translate_ComboBox, 0, 2, 1, 1);

        textLabel8 = new QLabel(groupBox4);
        textLabel8->setObjectName(QString::fromUtf8("textLabel8"));
        textLabel8->setWordWrap(false);

        gridLayout3->addWidget(textLabel8, 1, 0, 1, 1);

        m_Key_Object_Zoom_ComboBox = new QComboBox(groupBox4);
        m_Key_Object_Zoom_ComboBox->setObjectName(QString::fromUtf8("m_Key_Object_Zoom_ComboBox"));

        gridLayout3->addWidget(m_Key_Object_Zoom_ComboBox, 1, 2, 1, 1);

        m_Button_Object_Translate_ComboBox = new QComboBox(groupBox4);
        m_Button_Object_Translate_ComboBox->setObjectName(QString::fromUtf8("m_Button_Object_Translate_ComboBox"));

        gridLayout3->addWidget(m_Button_Object_Translate_ComboBox, 0, 1, 1, 1);

        textLabel7 = new QLabel(groupBox4);
        textLabel7->setObjectName(QString::fromUtf8("textLabel7"));
        textLabel7->setWordWrap(false);

        gridLayout3->addWidget(textLabel7, 0, 0, 1, 1);

        textLabel9 = new QLabel(groupBox4);
        textLabel9->setObjectName(QString::fromUtf8("textLabel9"));
        textLabel9->setWordWrap(false);

        gridLayout3->addWidget(textLabel9, 2, 0, 1, 1);


        gridLayout1->addLayout(gridLayout3, 2, 0, 1, 1);

        gridLayout4 = new QGridLayout();
        gridLayout4->setSpacing(6);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        m_Key_Object_Select_ComboBox = new QComboBox(groupBox4);
        m_Key_Object_Select_ComboBox->setObjectName(QString::fromUtf8("m_Key_Object_Select_ComboBox"));

        gridLayout4->addWidget(m_Key_Object_Select_ComboBox, 0, 2, 1, 1);

        m_Button_Object_Select_ComboBox = new QComboBox(groupBox4);
        m_Button_Object_Select_ComboBox->setObjectName(QString::fromUtf8("m_Button_Object_Select_ComboBox"));

        gridLayout4->addWidget(m_Button_Object_Select_ComboBox, 0, 1, 1, 1);

        textLabel10 = new QLabel(groupBox4);
        textLabel10->setObjectName(QString::fromUtf8("textLabel10"));
        textLabel10->setWordWrap(false);

        gridLayout4->addWidget(textLabel10, 0, 0, 1, 1);


        gridLayout1->addLayout(gridLayout4, 4, 0, 1, 1);


        gridLayout->addWidget(groupBox4, 0, 0, 1, 1);


        retranslateUi(MouseHandlerDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), MouseHandlerDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), MouseHandlerDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(MouseHandlerDialogBase);
    } // setupUi

    void retranslateUi(QDialog *MouseHandlerDialogBase)
    {
        MouseHandlerDialogBase->setWindowTitle(QCoreApplication::translate("MouseHandlerDialogBase", "User interaction settings"));
        buttonOk->setWindowTitle(QString());
        buttonOk->setText(QCoreApplication::translate("MouseHandlerDialogBase", "&OK"));
        buttonCancel->setText(QCoreApplication::translate("MouseHandlerDialogBase", "&Cancel"));
        groupBox4->setTitle(QCoreApplication::translate("MouseHandlerDialogBase", "Choose mouse key shortcuts"));
        textLabel5->setText(QCoreApplication::translate("MouseHandlerDialogBase", "View zoom"));
        textLabel2->setText(QCoreApplication::translate("MouseHandlerDialogBase", "Key"));
        textLabel3->setText(QCoreApplication::translate("MouseHandlerDialogBase", "Interaction"));
        textLabel6->setText(QCoreApplication::translate("MouseHandlerDialogBase", "View rotate"));
        textLabel1->setText(QCoreApplication::translate("MouseHandlerDialogBase", "Button"));
        textLabel4->setText(QCoreApplication::translate("MouseHandlerDialogBase", "View translate"));
        textLabel8->setText(QCoreApplication::translate("MouseHandlerDialogBase", "Object zoom"));
        textLabel7->setText(QCoreApplication::translate("MouseHandlerDialogBase", "Object translate"));
        textLabel9->setText(QCoreApplication::translate("MouseHandlerDialogBase", "Object rotate"));
        textLabel10->setText(QCoreApplication::translate("MouseHandlerDialogBase", "Sub-object selection"));
        Q_UNUSED(MouseHandlerDialogBase);
    } // retranslateUi

};

namespace Ui {
    class MouseHandlerDialogBase: public Ui_MouseHandlerDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class MouseHandlerDialogBase : public QDialog, public Ui::MouseHandlerDialogBase
{
    Q_OBJECT

public:
    MouseHandlerDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {}) : QDialog(parent, fl) { setupUi(this); if(name) setObjectName(name); setModal(modal); }
    ~MouseHandlerDialogBase() {}

protected slots:
    virtual void languageChange() {}

};

#endif // MOUSEHANDLERDIALOGBASE_H
