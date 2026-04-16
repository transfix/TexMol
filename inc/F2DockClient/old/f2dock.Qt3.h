#ifndef F2DOCK_H
#define F2DOCK_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_F2Dock
{
public:
    QRadioButton *bbDockRB;
    QRadioButton *uuDockRB;
    QPushButton *dockButton;
    QPushButton *resetButton;
    QPushButton *avdButton;
    Q3GroupBox *otherGB;
    QLabel *elecWeightsLabel_4;
    QLineEdit *elecWeightTB;
    Q3GroupBox *searchGB;
    QLabel *numSolutionsLabel;
    QComboBox *rotationCombo;
    QLineEdit *gridSpaceTB;
    QLineEdit *numSolutionsTB;
    QLabel *rotationLabel;
    QLabel *gridSpaceLabel;
    Q3GroupBox *shapeGB;
    QLabel *ccWeightsLabel_3;
    QLabel *scWeightsLabel;
    QLabel *ssWeightsLabel;
    QLineEdit *ssWeightTB;
    QLineEdit *scWeightTB;
    QLineEdit *ccWeightTB;
    Q3GroupBox *fileGB;
    QLineEdit *OF_TB;
    QPushButton *outFileButton;
    QPushButton *rmsdFileButton;
    QPushButton *ligadFileButton;
    QLineEdit *RMSDF_TB;
    QLabel *outFileLabel;
    QLabel *rmsdFileLabel;
    QLabel *ligandFileLabel;
    QLineEdit *LF_TB;
    QPushButton *receptorFileButton;
    QLabel *receptorFileLabel;
    QLineEdit *RF_TB;
    QLineEdit *RPDB_TB;
    QPushButton *receptorPDBButton;
    QLabel *receptorPDBLabel;

    void setupUi(QDialog *F2Dock)
    {
        if (F2Dock->objectName().isEmpty())
            F2Dock->setObjectName(QString::fromUtf8("F2Dock"));
        F2Dock->setEnabled(true);
        F2Dock->resize(672, 671);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(13), static_cast<QSizePolicy::Policy>(13));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(F2Dock->sizePolicy().hasHeightForWidth());
        F2Dock->setSizePolicy(sizePolicy);
        F2Dock->setBaseSize(QSize(800, 500));
        F2Dock->setModal(true);
        bbDockRB = new QRadioButton(F2Dock);
        bbDockRB->setObjectName(QString::fromUtf8("bbDockRB"));
        bbDockRB->setGeometry(QRect(20, 20, 220, 20));
        QFont font;
        font.setPointSize(11);
        bbDockRB->setFont(font);
        uuDockRB = new QRadioButton(F2Dock);
        uuDockRB->setObjectName(QString::fromUtf8("uuDockRB"));
        uuDockRB->setGeometry(QRect(320, 20, 240, 20));
        uuDockRB->setFont(font);
        uuDockRB->setChecked(true);
        dockButton = new QPushButton(F2Dock);
        dockButton->setObjectName(QString::fromUtf8("dockButton"));
        dockButton->setGeometry(QRect(20, 610, 630, 40));
        resetButton = new QPushButton(F2Dock);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        resetButton->setGeometry(QRect(20, 560, 301, 40));
        avdButton = new QPushButton(F2Dock);
        avdButton->setObjectName(QString::fromUtf8("avdButton"));
        avdButton->setGeometry(QRect(340, 560, 310, 40));
        otherGB = new Q3GroupBox(F2Dock);
        otherGB->setObjectName(QString::fromUtf8("otherGB"));
        otherGB->setGeometry(QRect(20, 450, 630, 100));
        elecWeightsLabel_4 = new QLabel(otherGB);
        elecWeightsLabel_4->setObjectName(QString::fromUtf8("elecWeightsLabel_4"));
        elecWeightsLabel_4->setGeometry(QRect(17, 30, 180, 20));
        elecWeightsLabel_4->setWordWrap(false);
        elecWeightTB = new QLineEdit(otherGB);
        elecWeightTB->setObjectName(QString::fromUtf8("elecWeightTB"));
        elecWeightTB->setGeometry(QRect(210, 30, 80, 22));
        elecWeightTB->setAlignment(Qt::AlignRight);
        searchGB = new Q3GroupBox(F2Dock);
        searchGB->setObjectName(QString::fromUtf8("searchGB"));
        searchGB->setGeometry(QRect(350, 280, 300, 160));
        numSolutionsLabel = new QLabel(searchGB);
        numSolutionsLabel->setObjectName(QString::fromUtf8("numSolutionsLabel"));
        numSolutionsLabel->setGeometry(QRect(10, 110, 180, 20));
        numSolutionsLabel->setWordWrap(false);
        rotationCombo = new QComboBox(searchGB);
        rotationCombo->setObjectName(QString::fromUtf8("rotationCombo"));
        rotationCombo->setGeometry(QRect(230, 30, 50, 22));
        gridSpaceTB = new QLineEdit(searchGB);
        gridSpaceTB->setObjectName(QString::fromUtf8("gridSpaceTB"));
        gridSpaceTB->setGeometry(QRect(233, 70, 50, 22));
        gridSpaceTB->setAlignment(Qt::AlignRight);
        numSolutionsTB = new QLineEdit(searchGB);
        numSolutionsTB->setObjectName(QString::fromUtf8("numSolutionsTB"));
        numSolutionsTB->setGeometry(QRect(233, 110, 50, 22));
        numSolutionsTB->setAlignment(Qt::AlignRight);
        rotationLabel = new QLabel(searchGB);
        rotationLabel->setObjectName(QString::fromUtf8("rotationLabel"));
        rotationLabel->setGeometry(QRect(10, 30, 170, 20));
        rotationLabel->setFrameShape(QFrame::NoFrame);
        rotationLabel->setWordWrap(false);
        gridSpaceLabel = new QLabel(searchGB);
        gridSpaceLabel->setObjectName(QString::fromUtf8("gridSpaceLabel"));
        gridSpaceLabel->setGeometry(QRect(10, 70, 180, 20));
        gridSpaceLabel->setWordWrap(false);
        shapeGB = new Q3GroupBox(F2Dock);
        shapeGB->setObjectName(QString::fromUtf8("shapeGB"));
        shapeGB->setGeometry(QRect(20, 280, 310, 160));
        ccWeightsLabel_3 = new QLabel(shapeGB);
        ccWeightsLabel_3->setObjectName(QString::fromUtf8("ccWeightsLabel_3"));
        ccWeightsLabel_3->setGeometry(QRect(17, 110, 200, 20));
        ccWeightsLabel_3->setWordWrap(false);
        scWeightsLabel = new QLabel(shapeGB);
        scWeightsLabel->setObjectName(QString::fromUtf8("scWeightsLabel"));
        scWeightsLabel->setGeometry(QRect(17, 70, 200, 20));
        scWeightsLabel->setWordWrap(false);
        ssWeightsLabel = new QLabel(shapeGB);
        ssWeightsLabel->setObjectName(QString::fromUtf8("ssWeightsLabel"));
        ssWeightsLabel->setGeometry(QRect(17, 30, 210, 20));
        ssWeightsLabel->setWordWrap(false);
        ssWeightTB = new QLineEdit(shapeGB);
        ssWeightTB->setObjectName(QString::fromUtf8("ssWeightTB"));
        ssWeightTB->setGeometry(QRect(240, 30, 50, 22));
        ssWeightTB->setAlignment(Qt::AlignRight);
        scWeightTB = new QLineEdit(shapeGB);
        scWeightTB->setObjectName(QString::fromUtf8("scWeightTB"));
        scWeightTB->setGeometry(QRect(240, 70, 50, 22));
        scWeightTB->setAlignment(Qt::AlignRight);
        ccWeightTB = new QLineEdit(shapeGB);
        ccWeightTB->setObjectName(QString::fromUtf8("ccWeightTB"));
        ccWeightTB->setGeometry(QRect(240, 110, 50, 22));
        ccWeightTB->setAlignment(Qt::AlignRight);
        fileGB = new Q3GroupBox(F2Dock);
        fileGB->setObjectName(QString::fromUtf8("fileGB"));
        fileGB->setGeometry(QRect(20, 50, 630, 220));
        OF_TB = new QLineEdit(fileGB);
        OF_TB->setObjectName(QString::fromUtf8("OF_TB"));
        OF_TB->setGeometry(QRect(170, 180, 330, 22));
        outFileButton = new QPushButton(fileGB);
        outFileButton->setObjectName(QString::fromUtf8("outFileButton"));
        outFileButton->setGeometry(QRect(510, 180, 100, 24));
        rmsdFileButton = new QPushButton(fileGB);
        rmsdFileButton->setObjectName(QString::fromUtf8("rmsdFileButton"));
        rmsdFileButton->setGeometry(QRect(510, 140, 100, 24));
        ligadFileButton = new QPushButton(fileGB);
        ligadFileButton->setObjectName(QString::fromUtf8("ligadFileButton"));
        ligadFileButton->setGeometry(QRect(510, 100, 100, 24));
        RMSDF_TB = new QLineEdit(fileGB);
        RMSDF_TB->setObjectName(QString::fromUtf8("RMSDF_TB"));
        RMSDF_TB->setGeometry(QRect(170, 140, 330, 22));
        outFileLabel = new QLabel(fileGB);
        outFileLabel->setObjectName(QString::fromUtf8("outFileLabel"));
        outFileLabel->setGeometry(QRect(10, 180, 150, 20));
        outFileLabel->setWordWrap(false);
        rmsdFileLabel = new QLabel(fileGB);
        rmsdFileLabel->setObjectName(QString::fromUtf8("rmsdFileLabel"));
        rmsdFileLabel->setGeometry(QRect(10, 140, 150, 20));
        rmsdFileLabel->setWordWrap(false);
        ligandFileLabel = new QLabel(fileGB);
        ligandFileLabel->setObjectName(QString::fromUtf8("ligandFileLabel"));
        ligandFileLabel->setGeometry(QRect(10, 100, 150, 20));
        ligandFileLabel->setWordWrap(false);
        LF_TB = new QLineEdit(fileGB);
        LF_TB->setObjectName(QString::fromUtf8("LF_TB"));
        LF_TB->setGeometry(QRect(170, 100, 330, 22));
        receptorFileButton = new QPushButton(fileGB);
        receptorFileButton->setObjectName(QString::fromUtf8("receptorFileButton"));
        receptorFileButton->setGeometry(QRect(510, 20, 100, 24));
        receptorFileLabel = new QLabel(fileGB);
        receptorFileLabel->setObjectName(QString::fromUtf8("receptorFileLabel"));
        receptorFileLabel->setGeometry(QRect(10, 20, 150, 20));
        receptorFileLabel->setWordWrap(false);
        RF_TB = new QLineEdit(fileGB);
        RF_TB->setObjectName(QString::fromUtf8("RF_TB"));
        RF_TB->setGeometry(QRect(170, 20, 330, 22));
        RPDB_TB = new QLineEdit(fileGB);
        RPDB_TB->setObjectName(QString::fromUtf8("RPDB_TB"));
        RPDB_TB->setGeometry(QRect(170, 60, 330, 22));
        receptorPDBButton = new QPushButton(fileGB);
        receptorPDBButton->setObjectName(QString::fromUtf8("receptorPDBButton"));
        receptorPDBButton->setGeometry(QRect(510, 60, 100, 24));
        receptorPDBLabel = new QLabel(fileGB);
        receptorPDBLabel->setObjectName(QString::fromUtf8("receptorPDBLabel"));
        receptorPDBLabel->setGeometry(QRect(10, 60, 150, 20));
        receptorPDBLabel->setWordWrap(false);
        QWidget::setTabOrder(bbDockRB, uuDockRB);
        QWidget::setTabOrder(uuDockRB, receptorFileButton);
        QWidget::setTabOrder(receptorFileButton, ligadFileButton);
        QWidget::setTabOrder(ligadFileButton, rmsdFileButton);
        QWidget::setTabOrder(rmsdFileButton, outFileButton);
        QWidget::setTabOrder(outFileButton, rotationCombo);
        QWidget::setTabOrder(rotationCombo, gridSpaceTB);
        QWidget::setTabOrder(gridSpaceTB, numSolutionsTB);
        QWidget::setTabOrder(numSolutionsTB, ssWeightTB);
        QWidget::setTabOrder(ssWeightTB, scWeightTB);
        QWidget::setTabOrder(scWeightTB, ccWeightTB);
        QWidget::setTabOrder(ccWeightTB, elecWeightTB);
        QWidget::setTabOrder(elecWeightTB, avdButton);
        QWidget::setTabOrder(avdButton, resetButton);
        QWidget::setTabOrder(resetButton, dockButton);

        retranslateUi(F2Dock);
        QObject::connect(receptorFileButton, SIGNAL(clicked()), F2Dock, SLOT(selectReceptorFileSlot()));
        QObject::connect(ligadFileButton, SIGNAL(clicked()), F2Dock, SLOT(selectLigandFileSlot()));
        QObject::connect(avdButton, SIGNAL(clicked()), F2Dock, SLOT(advencedOptionsSlot()));
        QObject::connect(rmsdFileButton, SIGNAL(clicked()), F2Dock, SLOT(selectRMSDFileSlot()));
        QObject::connect(outFileButton, SIGNAL(clicked()), F2Dock, SLOT(selectOutputFileSlot()));
        QObject::connect(dockButton, SIGNAL(clicked()), F2Dock, SLOT(dockSlot()));
        QObject::connect(resetButton, SIGNAL(clicked()), F2Dock, SLOT(defaultSlot()));
        QObject::connect(bbDockRB, SIGNAL(clicked()), F2Dock, SLOT(bbSlot()));
        QObject::connect(uuDockRB, SIGNAL(clicked()), F2Dock, SLOT(uuSlot()));
        QObject::connect(receptorPDBButton, SIGNAL(clicked()), F2Dock, SLOT(selectReceptorPDBSlot()));

        rotationCombo->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(F2Dock);
    } // setupUi

    void retranslateUi(QDialog *F2Dock)
    {
        F2Dock->setWindowTitle(QApplication::translate("F2Dock", "F2Dock", 0, QApplication::UnicodeUTF8));
        bbDockRB->setText(QApplication::translate("F2Dock", "Bound-Bound Docking", 0, QApplication::UnicodeUTF8));
        uuDockRB->setText(QApplication::translate("F2Dock", "Unbound-Unbound Docking", 0, QApplication::UnicodeUTF8));
        dockButton->setText(QApplication::translate("F2Dock", "Dock", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("F2Dock", "Reset to Defaults", 0, QApplication::UnicodeUTF8));
        avdButton->setText(QApplication::translate("F2Dock", "Advanced Options", 0, QApplication::UnicodeUTF8));
        otherGB->setTitle(QApplication::translate("F2Dock", "Other parameters", 0, QApplication::UnicodeUTF8));
        elecWeightsLabel_4->setText(QApplication::translate("F2Dock", "Electrostatics Weight", 0, QApplication::UnicodeUTF8));
        elecWeightTB->setText(QApplication::translate("F2Dock", "0.86", 0, QApplication::UnicodeUTF8));
        searchGB->setTitle(QApplication::translate("F2Dock", "Search Parameters", 0, QApplication::UnicodeUTF8));
        numSolutionsLabel->setText(QApplication::translate("F2Dock", "number of Solutions", 0, QApplication::UnicodeUTF8));
        gridSpaceTB->setText(QApplication::translate("F2Dock", "1.2", 0, QApplication::UnicodeUTF8));
        numSolutionsTB->setText(QApplication::translate("F2Dock", "2000", 0, QApplication::UnicodeUTF8));
        rotationLabel->setText(QApplication::translate("F2Dock", "Rotation separation", 0, QApplication::UnicodeUTF8));
        gridSpaceLabel->setText(QApplication::translate("F2Dock", "Grid Spacing", 0, QApplication::UnicodeUTF8));
        shapeGB->setTitle(QApplication::translate("F2Dock", "Shape Complementarity Weights", 0, QApplication::UnicodeUTF8));
        ccWeightsLabel_3->setText(QApplication::translate("F2Dock", "Core-Core Overlap Weight", 0, QApplication::UnicodeUTF8));
        scWeightsLabel->setText(QApplication::translate("F2Dock", "Skin-Core Overlap Weight", 0, QApplication::UnicodeUTF8));
        ssWeightsLabel->setText(QApplication::translate("F2Dock", "Skin-Skin Overlap Weight", 0, QApplication::UnicodeUTF8));
        ssWeightTB->setText(QApplication::translate("F2Dock", "0.7", 0, QApplication::UnicodeUTF8));
        scWeightTB->setText(QApplication::translate("F2Dock", "17.0", 0, QApplication::UnicodeUTF8));
        ccWeightTB->setText(QApplication::translate("F2Dock", "-0.2", 0, QApplication::UnicodeUTF8));
        fileGB->setTitle(QApplication::translate("F2Dock", "Files", 0, QApplication::UnicodeUTF8));
        outFileButton->setText(QApplication::translate("F2Dock", "Browse", 0, QApplication::UnicodeUTF8));
        rmsdFileButton->setText(QApplication::translate("F2Dock", "Browse", 0, QApplication::UnicodeUTF8));
        ligadFileButton->setText(QApplication::translate("F2Dock", "Browse", 0, QApplication::UnicodeUTF8));
        outFileLabel->setText(QApplication::translate("F2Dock", "Output File", 0, QApplication::UnicodeUTF8));
        rmsdFileLabel->setText(QApplication::translate("F2Dock", "Atom RMSD File", 0, QApplication::UnicodeUTF8));
        ligandFileLabel->setText(QApplication::translate("F2Dock", "Ligand File (f2d)", 0, QApplication::UnicodeUTF8));
        receptorFileButton->setText(QApplication::translate("F2Dock", "Browse", 0, QApplication::UnicodeUTF8));
        receptorFileLabel->setText(QApplication::translate("F2Dock", "Receptor File (f2d)", 0, QApplication::UnicodeUTF8));
        receptorPDBButton->setText(QApplication::translate("F2Dock", "Browse", 0, QApplication::UnicodeUTF8));
        receptorPDBLabel->setText(QApplication::translate("F2Dock", "Receptor File (pdb)", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(F2Dock);
    } // retranslateUi

};

namespace Ui {
    class F2Dock: public Ui_F2Dock {};
} // namespace Ui

QT_END_NAMESPACE

class F2Dock : public QDialog, public Ui::F2Dock
{
    Q_OBJECT

public:
    F2Dock(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~F2Dock();

public slots:
    virtual void selectReceptorFileSlot();
    virtual void selectLigandFileSlot();
    virtual void selectRMSDFileSlot();
    virtual void selectOutputFileSlot();
    virtual void advencedOptionsSlot();
    virtual void dockSlot();
    virtual void defaultSlot();
    virtual void uuSlot();
    virtual void bbSlot();
    virtual void selectReceptorPDBSlot();

protected slots:
    virtual void languageChange();

};

#endif // F2DOCK_H
