#ifndef GBRERANK_H
#define GBRERANK_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GBRerank
{
public:
    QGridLayout *gridLayout;
    Q3GroupBox *groupBox2;
    QGridLayout *gridLayout1;
    QLabel *receptorPQRLabel;
    QLabel *ligandPQRLabel;
    QLabel *receptorQUADLabel;
    QLabel *ligandQUADLabel;
    QLineEdit *receptorPQR_TB;
    QLineEdit *ligandPQR_TB;
    QLineEdit *receptorQUAD_TB;
    QLineEdit *ligandQUAD_TB;
    QPushButton *receptorPQRButton;
    QPushButton *ligandPQRButton;
    QPushButton *receptorQUADButton;
    QPushButton *ligandQUADButton;
    QLabel *f2dockOutputLabel;
    QLabel *gbrerankOutputLabel;
    QLineEdit *f2dockOutput_TB;
    QLineEdit *gbrerankOutput_TB;
    QPushButton *f2dockOutputButton;
    QPushButton *gbrerankOutputButton;
    Q3GroupBox *groupBox3;
    QGridLayout *gridLayout2;
    QLabel *gpolLabel;
    QLabel *gnonpolLabel;
    QLabel *f2dockScoreLabel;
    QLineEdit *gnonpol_TB;
    QLineEdit *f2dockScore_TB;
    QLineEdit *gpol_TB;
    Q3GroupBox *groupBox4;
    QGridLayout *gridLayout3;
    QLabel *epsilonBRLabel;
    QLineEdit *epsilonBR_TB;
    QLineEdit *epsilonGpol_TB;
    QLabel *epsilonGpolLabel;
    Q3GroupBox *groupBox5;
    QGridLayout *gridLayout4;
    QLabel *numSolLabel;
    QLineEdit *numSol_TB;
    QPushButton *resetButton;
    QPushButton *rerankButton;

    void setupUi(QDialog *GBRerank)
    {
        if (GBRerank->objectName().isEmpty())
            GBRerank->setObjectName(QString::fromUtf8("GBRerank"));
        GBRerank->resize(662, 374);
        gridLayout = new QGridLayout(GBRerank);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox2 = new Q3GroupBox(GBRerank);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        groupBox2->setColumnLayout(0, Qt::Vertical);
        groupBox2->layout()->setSpacing(6);
        groupBox2->layout()->setMargin(11);
        gridLayout1 = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(groupBox2->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        receptorPQRLabel = new QLabel(groupBox2);
        receptorPQRLabel->setObjectName(QString::fromUtf8("receptorPQRLabel"));
        receptorPQRLabel->setWordWrap(false);

        gridLayout1->addWidget(receptorPQRLabel, 0, 0, 1, 1);

        ligandPQRLabel = new QLabel(groupBox2);
        ligandPQRLabel->setObjectName(QString::fromUtf8("ligandPQRLabel"));
        ligandPQRLabel->setWordWrap(false);

        gridLayout1->addWidget(ligandPQRLabel, 1, 0, 1, 1);

        receptorQUADLabel = new QLabel(groupBox2);
        receptorQUADLabel->setObjectName(QString::fromUtf8("receptorQUADLabel"));
        receptorQUADLabel->setWordWrap(false);

        gridLayout1->addWidget(receptorQUADLabel, 2, 0, 1, 1);

        ligandQUADLabel = new QLabel(groupBox2);
        ligandQUADLabel->setObjectName(QString::fromUtf8("ligandQUADLabel"));
        ligandQUADLabel->setWordWrap(false);

        gridLayout1->addWidget(ligandQUADLabel, 3, 0, 1, 1);

        receptorPQR_TB = new QLineEdit(groupBox2);
        receptorPQR_TB->setObjectName(QString::fromUtf8("receptorPQR_TB"));

        gridLayout1->addWidget(receptorPQR_TB, 0, 1, 1, 2);

        ligandPQR_TB = new QLineEdit(groupBox2);
        ligandPQR_TB->setObjectName(QString::fromUtf8("ligandPQR_TB"));

        gridLayout1->addWidget(ligandPQR_TB, 1, 1, 1, 2);

        receptorQUAD_TB = new QLineEdit(groupBox2);
        receptorQUAD_TB->setObjectName(QString::fromUtf8("receptorQUAD_TB"));

        gridLayout1->addWidget(receptorQUAD_TB, 2, 1, 1, 2);

        ligandQUAD_TB = new QLineEdit(groupBox2);
        ligandQUAD_TB->setObjectName(QString::fromUtf8("ligandQUAD_TB"));

        gridLayout1->addWidget(ligandQUAD_TB, 3, 1, 1, 2);

        receptorPQRButton = new QPushButton(groupBox2);
        receptorPQRButton->setObjectName(QString::fromUtf8("receptorPQRButton"));

        gridLayout1->addWidget(receptorPQRButton, 0, 3, 1, 1);

        ligandPQRButton = new QPushButton(groupBox2);
        ligandPQRButton->setObjectName(QString::fromUtf8("ligandPQRButton"));

        gridLayout1->addWidget(ligandPQRButton, 1, 3, 1, 1);

        receptorQUADButton = new QPushButton(groupBox2);
        receptorQUADButton->setObjectName(QString::fromUtf8("receptorQUADButton"));

        gridLayout1->addWidget(receptorQUADButton, 2, 3, 1, 1);

        ligandQUADButton = new QPushButton(groupBox2);
        ligandQUADButton->setObjectName(QString::fromUtf8("ligandQUADButton"));

        gridLayout1->addWidget(ligandQUADButton, 3, 3, 1, 1);

        f2dockOutputLabel = new QLabel(groupBox2);
        f2dockOutputLabel->setObjectName(QString::fromUtf8("f2dockOutputLabel"));
        f2dockOutputLabel->setWordWrap(false);

        gridLayout1->addWidget(f2dockOutputLabel, 4, 0, 1, 1);

        gbrerankOutputLabel = new QLabel(groupBox2);
        gbrerankOutputLabel->setObjectName(QString::fromUtf8("gbrerankOutputLabel"));
        gbrerankOutputLabel->setWordWrap(false);

        gridLayout1->addWidget(gbrerankOutputLabel, 5, 0, 1, 2);

        f2dockOutput_TB = new QLineEdit(groupBox2);
        f2dockOutput_TB->setObjectName(QString::fromUtf8("f2dockOutput_TB"));

        gridLayout1->addWidget(f2dockOutput_TB, 4, 2, 1, 1);

        gbrerankOutput_TB = new QLineEdit(groupBox2);
        gbrerankOutput_TB->setObjectName(QString::fromUtf8("gbrerankOutput_TB"));

        gridLayout1->addWidget(gbrerankOutput_TB, 5, 2, 1, 1);

        f2dockOutputButton = new QPushButton(groupBox2);
        f2dockOutputButton->setObjectName(QString::fromUtf8("f2dockOutputButton"));

        gridLayout1->addWidget(f2dockOutputButton, 4, 3, 1, 1);

        gbrerankOutputButton = new QPushButton(groupBox2);
        gbrerankOutputButton->setObjectName(QString::fromUtf8("gbrerankOutputButton"));

        gridLayout1->addWidget(gbrerankOutputButton, 5, 3, 1, 1);


        gridLayout->addWidget(groupBox2, 0, 0, 1, 3);

        groupBox3 = new Q3GroupBox(GBRerank);
        groupBox3->setObjectName(QString::fromUtf8("groupBox3"));
        groupBox3->setColumnLayout(0, Qt::Vertical);
        groupBox3->layout()->setSpacing(6);
        groupBox3->layout()->setMargin(11);
        gridLayout2 = new QGridLayout();
        QBoxLayout *boxlayout1 = qobject_cast<QBoxLayout *>(groupBox3->layout());
        if (boxlayout1)
            boxlayout1->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        gpolLabel = new QLabel(groupBox3);
        gpolLabel->setObjectName(QString::fromUtf8("gpolLabel"));
        gpolLabel->setWordWrap(false);

        gridLayout2->addWidget(gpolLabel, 0, 0, 1, 1);

        gnonpolLabel = new QLabel(groupBox3);
        gnonpolLabel->setObjectName(QString::fromUtf8("gnonpolLabel"));
        gnonpolLabel->setWordWrap(false);

        gridLayout2->addWidget(gnonpolLabel, 1, 0, 1, 1);

        f2dockScoreLabel = new QLabel(groupBox3);
        f2dockScoreLabel->setObjectName(QString::fromUtf8("f2dockScoreLabel"));
        f2dockScoreLabel->setWordWrap(false);

        gridLayout2->addWidget(f2dockScoreLabel, 2, 0, 1, 1);

        gnonpol_TB = new QLineEdit(groupBox3);
        gnonpol_TB->setObjectName(QString::fromUtf8("gnonpol_TB"));
        gnonpol_TB->setAlignment(Qt::AlignRight);

        gridLayout2->addWidget(gnonpol_TB, 1, 1, 1, 1);

        f2dockScore_TB = new QLineEdit(groupBox3);
        f2dockScore_TB->setObjectName(QString::fromUtf8("f2dockScore_TB"));
        f2dockScore_TB->setAlignment(Qt::AlignRight);

        gridLayout2->addWidget(f2dockScore_TB, 2, 1, 1, 1);

        gpol_TB = new QLineEdit(groupBox3);
        gpol_TB->setObjectName(QString::fromUtf8("gpol_TB"));
        gpol_TB->setAlignment(Qt::AlignRight);

        gridLayout2->addWidget(gpol_TB, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox3, 1, 0, 1, 1);

        groupBox4 = new Q3GroupBox(GBRerank);
        groupBox4->setObjectName(QString::fromUtf8("groupBox4"));
        groupBox4->setColumnLayout(0, Qt::Vertical);
        groupBox4->layout()->setSpacing(6);
        groupBox4->layout()->setMargin(11);
        gridLayout3 = new QGridLayout();
        QBoxLayout *boxlayout2 = qobject_cast<QBoxLayout *>(groupBox4->layout());
        if (boxlayout2)
            boxlayout2->addLayout(gridLayout3);
        gridLayout3->setAlignment(Qt::AlignTop);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        epsilonBRLabel = new QLabel(groupBox4);
        epsilonBRLabel->setObjectName(QString::fromUtf8("epsilonBRLabel"));
        epsilonBRLabel->setWordWrap(false);

        gridLayout3->addWidget(epsilonBRLabel, 0, 0, 1, 1);

        epsilonBR_TB = new QLineEdit(groupBox4);
        epsilonBR_TB->setObjectName(QString::fromUtf8("epsilonBR_TB"));

        gridLayout3->addWidget(epsilonBR_TB, 0, 1, 1, 1);

        epsilonGpol_TB = new QLineEdit(groupBox4);
        epsilonGpol_TB->setObjectName(QString::fromUtf8("epsilonGpol_TB"));

        gridLayout3->addWidget(epsilonGpol_TB, 1, 1, 1, 1);

        epsilonGpolLabel = new QLabel(groupBox4);
        epsilonGpolLabel->setObjectName(QString::fromUtf8("epsilonGpolLabel"));
        epsilonGpolLabel->setWordWrap(false);

        gridLayout3->addWidget(epsilonGpolLabel, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox4, 1, 1, 1, 1);

        groupBox5 = new Q3GroupBox(GBRerank);
        groupBox5->setObjectName(QString::fromUtf8("groupBox5"));
        groupBox5->setColumnLayout(0, Qt::Vertical);
        groupBox5->layout()->setSpacing(6);
        groupBox5->layout()->setMargin(11);
        gridLayout4 = new QGridLayout();
        QBoxLayout *boxlayout3 = qobject_cast<QBoxLayout *>(groupBox5->layout());
        if (boxlayout3)
            boxlayout3->addLayout(gridLayout4);
        gridLayout4->setAlignment(Qt::AlignTop);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        numSolLabel = new QLabel(groupBox5);
        numSolLabel->setObjectName(QString::fromUtf8("numSolLabel"));
        numSolLabel->setWordWrap(false);

        gridLayout4->addWidget(numSolLabel, 0, 0, 1, 1);

        numSol_TB = new QLineEdit(groupBox5);
        numSol_TB->setObjectName(QString::fromUtf8("numSol_TB"));
        numSol_TB->setAlignment(Qt::AlignHCenter);

        gridLayout4->addWidget(numSol_TB, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox5, 1, 2, 1, 1);

        resetButton = new QPushButton(GBRerank);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        gridLayout->addWidget(resetButton, 2, 0, 1, 1);

        rerankButton = new QPushButton(GBRerank);
        rerankButton->setObjectName(QString::fromUtf8("rerankButton"));

        gridLayout->addWidget(rerankButton, 2, 1, 1, 1);


        retranslateUi(GBRerank);
        QObject::connect(receptorPQRButton, SIGNAL(clicked()), GBRerank, SLOT(selectReceptorPQRFileSlot()));
        QObject::connect(ligandPQRButton, SIGNAL(clicked()), GBRerank, SLOT(selectLigandPQRFileSlot()));
        QObject::connect(receptorQUADButton, SIGNAL(clicked()), GBRerank, SLOT(selectReceptorQUADFileSlot()));
        QObject::connect(ligandQUADButton, SIGNAL(clicked()), GBRerank, SLOT(selectLigandQUADFileSlot()));
        QObject::connect(f2dockOutputButton, SIGNAL(clicked()), GBRerank, SLOT(selectF2dockOutputFileSlot()));
        QObject::connect(gbrerankOutputButton, SIGNAL(clicked()), GBRerank, SLOT(selectGbrerankOutputFileSlot()));
        QObject::connect(resetButton, SIGNAL(clicked()), GBRerank, SLOT(resetSlot()));
        QObject::connect(rerankButton, SIGNAL(clicked()), GBRerank, SLOT(rerankSlot()));

        QMetaObject::connectSlotsByName(GBRerank);
    } // setupUi

    void retranslateUi(QDialog *GBRerank)
    {
        GBRerank->setWindowTitle(QApplication::translate("GBRerank", "GBRrerank Input", 0, QApplication::UnicodeUTF8));
        groupBox2->setTitle(QApplication::translate("GBRerank", "Files", 0, QApplication::UnicodeUTF8));
        receptorPQRLabel->setText(QApplication::translate("GBRerank", "Receptor PDB", 0, QApplication::UnicodeUTF8));
        ligandPQRLabel->setText(QApplication::translate("GBRerank", "Ligand PDB", 0, QApplication::UnicodeUTF8));
        receptorQUADLabel->setText(QApplication::translate("GBRerank", "Receptor QUAD", 0, QApplication::UnicodeUTF8));
        ligandQUADLabel->setText(QApplication::translate("GBRerank", "Ligand QUAD", 0, QApplication::UnicodeUTF8));
        receptorPQRButton->setText(QApplication::translate("GBRerank", "Browse", 0, QApplication::UnicodeUTF8));
        ligandPQRButton->setText(QApplication::translate("GBRerank", "Browse", 0, QApplication::UnicodeUTF8));
        receptorQUADButton->setText(QApplication::translate("GBRerank", "Browse", 0, QApplication::UnicodeUTF8));
        ligandQUADButton->setText(QApplication::translate("GBRerank", "Browse", 0, QApplication::UnicodeUTF8));
        f2dockOutputLabel->setText(QApplication::translate("GBRerank", "F2Dock Output", 0, QApplication::UnicodeUTF8));
        gbrerankOutputLabel->setText(QApplication::translate("GBRerank", "GBRerank Output", 0, QApplication::UnicodeUTF8));
        f2dockOutputButton->setText(QApplication::translate("GBRerank", "Browse", 0, QApplication::UnicodeUTF8));
        gbrerankOutputButton->setText(QApplication::translate("GBRerank", "Browse", 0, QApplication::UnicodeUTF8));
        groupBox3->setTitle(QApplication::translate("GBRerank", "Weights", 0, QApplication::UnicodeUTF8));
        gpolLabel->setText(QApplication::translate("GBRerank", "G(pol)", 0, QApplication::UnicodeUTF8));
        gnonpolLabel->setText(QApplication::translate("GBRerank", "G(nonpol)", 0, QApplication::UnicodeUTF8));
        f2dockScoreLabel->setText(QApplication::translate("GBRerank", "F2Dock Score", 0, QApplication::UnicodeUTF8));
        gnonpol_TB->setText(QApplication::translate("GBRerank", "0.005", 0, QApplication::UnicodeUTF8));
        f2dockScore_TB->setText(QApplication::translate("GBRerank", "0", 0, QApplication::UnicodeUTF8));
        gpol_TB->setText(QApplication::translate("GBRerank", "-1", 0, QApplication::UnicodeUTF8));
        groupBox4->setTitle(QApplication::translate("GBRerank", "Error", 0, QApplication::UnicodeUTF8));
        epsilonBRLabel->setText(QApplication::translate("GBRerank", "Epsilon(BR)", 0, QApplication::UnicodeUTF8));
        epsilonBR_TB->setText(QApplication::translate("GBRerank", "0.5", 0, QApplication::UnicodeUTF8));
        epsilonGpol_TB->setText(QApplication::translate("GBRerank", "0.7", 0, QApplication::UnicodeUTF8));
        epsilonGpolLabel->setText(QApplication::translate("GBRerank", "Epsilon(Gpol)", 0, QApplication::UnicodeUTF8));
        groupBox5->setTitle(QApplication::translate("GBRerank", "Other parameters", 0, QApplication::UnicodeUTF8));
        numSolLabel->setText(QApplication::translate("GBRerank", "Number of solutions", 0, QApplication::UnicodeUTF8));
        numSol_TB->setText(QApplication::translate("GBRerank", "2000", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("GBRerank", "Reset", 0, QApplication::UnicodeUTF8));
        rerankButton->setText(QApplication::translate("GBRerank", "Rerank", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(GBRerank);
    } // retranslateUi

};

namespace Ui {
    class GBRerank: public Ui_GBRerank {};
} // namespace Ui

QT_END_NAMESPACE

class GBRerank : public QDialog, public Ui::GBRerank
{
    Q_OBJECT

public:
    GBRerank(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~GBRerank();

public slots:
    virtual void selectReceptorPQRFileSlot();
    virtual void selectLigandPQRFileSlot();
    virtual void selectReceptorQUADFileSlot();
    virtual void selectLigandQUADFileSlot();
    virtual void selectF2dockOutputFileSlot();
    virtual void selectGbrerankOutputFileSlot();
    virtual void resetSlot();
    virtual void rerankSlot();

protected slots:
    virtual void languageChange();

};

#endif // GBRERANK_H
