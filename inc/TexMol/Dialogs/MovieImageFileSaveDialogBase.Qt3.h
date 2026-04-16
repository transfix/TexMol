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
#ifndef MOVIEIMAGEFILESAVEDIALOGBASE_H
#define MOVIEIMAGEFILESAVEDIALOGBASE_H

#include <qvariant.h>


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
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_MovieImageFileSaveDialogBase
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonHelp;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QHBoxLayout *hboxLayout1;
    QLineEdit *m_AnimationFileNameLineEdit1;
    QPushButton *m_AnimationFileNamePushButton;
    QHBoxLayout *hboxLayout2;
    QLineEdit *m_BaseImageFileNameLineEdit;
    QPushButton *m_BaseImageFileNamePushButton;
    QHBoxLayout *hboxLayout3;
    QComboBox *m_ImageFileFormatsComboBox;
    QLabel *m_ImageFileFormatsTextLabel;

    void setupUi(QDialog *MovieImageFileSaveDialogBase)
    {
        if (MovieImageFileSaveDialogBase->objectName().isEmpty())
            MovieImageFileSaveDialogBase->setObjectName(QString::fromUtf8("MovieImageFileSaveDialogBase"));
        MovieImageFileSaveDialogBase->resize(511, 152);
        MovieImageFileSaveDialogBase->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(MovieImageFileSaveDialogBase);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        buttonHelp = new QPushButton(MovieImageFileSaveDialogBase);
        buttonHelp->setObjectName(QString::fromUtf8("buttonHelp"));
        buttonHelp->setAutoDefault(true);

        hboxLayout->addWidget(buttonHelp);

        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(MovieImageFileSaveDialogBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(MovieImageFileSaveDialogBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_AnimationFileNameLineEdit1 = new QLineEdit(MovieImageFileSaveDialogBase);
        m_AnimationFileNameLineEdit1->setObjectName(QString::fromUtf8("m_AnimationFileNameLineEdit1"));

        hboxLayout1->addWidget(m_AnimationFileNameLineEdit1);

        m_AnimationFileNamePushButton = new QPushButton(MovieImageFileSaveDialogBase);
        m_AnimationFileNamePushButton->setObjectName(QString::fromUtf8("m_AnimationFileNamePushButton"));

        hboxLayout1->addWidget(m_AnimationFileNamePushButton);


        gridLayout->addLayout(hboxLayout1, 0, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_BaseImageFileNameLineEdit = new QLineEdit(MovieImageFileSaveDialogBase);
        m_BaseImageFileNameLineEdit->setObjectName(QString::fromUtf8("m_BaseImageFileNameLineEdit"));

        hboxLayout2->addWidget(m_BaseImageFileNameLineEdit);

        m_BaseImageFileNamePushButton = new QPushButton(MovieImageFileSaveDialogBase);
        m_BaseImageFileNamePushButton->setObjectName(QString::fromUtf8("m_BaseImageFileNamePushButton"));

        hboxLayout2->addWidget(m_BaseImageFileNamePushButton);


        gridLayout->addLayout(hboxLayout2, 1, 0, 1, 1);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        m_ImageFileFormatsComboBox = new QComboBox(MovieImageFileSaveDialogBase);
        m_ImageFileFormatsComboBox->setObjectName(QString::fromUtf8("m_ImageFileFormatsComboBox"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_ImageFileFormatsComboBox->sizePolicy().hasHeightForWidth());
        m_ImageFileFormatsComboBox->setSizePolicy(sizePolicy);

        hboxLayout3->addWidget(m_ImageFileFormatsComboBox);

        m_ImageFileFormatsTextLabel = new QLabel(MovieImageFileSaveDialogBase);
        m_ImageFileFormatsTextLabel->setObjectName(QString::fromUtf8("m_ImageFileFormatsTextLabel"));
        m_ImageFileFormatsTextLabel->setWordWrap(false);

        hboxLayout3->addWidget(m_ImageFileFormatsTextLabel);


        gridLayout->addLayout(hboxLayout3, 2, 0, 1, 1);


        retranslateUi(MovieImageFileSaveDialogBase);
        QObject::connect(buttonOk, SIGNAL(clicked()), MovieImageFileSaveDialogBase, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), MovieImageFileSaveDialogBase, SLOT(reject()));
        QObject::connect(m_BaseImageFileNamePushButton, SIGNAL(clicked()), MovieImageFileSaveDialogBase, SLOT(baseImageFileSlot()));
        QObject::connect(m_AnimationFileNamePushButton, SIGNAL(clicked()), MovieImageFileSaveDialogBase, SLOT(animationFileSlot()));

        QMetaObject::connectSlotsByName(MovieImageFileSaveDialogBase);
    } // setupUi

    void retranslateUi(QDialog *MovieImageFileSaveDialogBase)
    {
        MovieImageFileSaveDialogBase->setWindowTitle(QApplication::translate("MovieImageFileSaveDialogBase", "Movie images information", 0, QApplication::UnicodeUTF8));
        buttonHelp->setText(QApplication::translate("MovieImageFileSaveDialogBase", "&Help", 0, QApplication::UnicodeUTF8));
        buttonHelp->setShortcut(QApplication::translate("MovieImageFileSaveDialogBase", "F1", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("MovieImageFileSaveDialogBase", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
        buttonCancel->setText(QApplication::translate("MovieImageFileSaveDialogBase", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        m_AnimationFileNamePushButton->setText(QApplication::translate("MovieImageFileSaveDialogBase", "Animation file", 0, QApplication::UnicodeUTF8));
        m_BaseImageFileNamePushButton->setText(QApplication::translate("MovieImageFileSaveDialogBase", "Base image file", 0, QApplication::UnicodeUTF8));
        m_ImageFileFormatsTextLabel->setText(QApplication::translate("MovieImageFileSaveDialogBase", "Image formats", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MovieImageFileSaveDialogBase);
    } // retranslateUi

};

namespace Ui {
    class MovieImageFileSaveDialogBase: public Ui_MovieImageFileSaveDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

class MovieImageFileSaveDialogBase : public QDialog, public Ui::MovieImageFileSaveDialogBase
{
    Q_OBJECT

public:
    MovieImageFileSaveDialogBase(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~MovieImageFileSaveDialogBase();

public slots:
    virtual void baseImageFileSlot();
    virtual void animationFileSlot();

protected slots:
    virtual void languageChange();

};

#endif // MOVIEIMAGEFILESAVEDIALOGBASE_H
