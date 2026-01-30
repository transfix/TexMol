#ifndef GBRERANKRESULT_H
#define GBRERANKRESULT_H

#include <qvariant.h>


#include <Qt3Support/Q3ButtonGroup>
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

class Ui_gbRerankResult
{
public:
    QGridLayout *gridLayout;
    Q3ButtonGroup *JobDescriptionGB;
    QGridLayout *gridLayout1;
    QLabel *receptorPQRLabel;
    QLabel *ligandPQRLabel;
    QLabel *receptorQUADLabel;
    QLabel *ligandQUADLabel;
    QLabel *f2dockOuputLabel;
    QLabel *gbrerankLabel;
    QLineEdit *receptorPQR_TB;
    QLineEdit *ligandPQR_TB;
    QLineEdit *receptorQUAD_TB;
    QLineEdit *ligandQUAD_TB;
    QLineEdit *f2dockOutput_TB;
    QLineEdit *gbRerankOutput_TB;
    QLabel *weightsLabel;
    QLabel *errorLabel;
    QLabel *gpolLabel;
    QLabel *gnonpolLabel;
    QLabel *f2dockScoreLabel;
    QLabel *epsilonBRLabel;
    QLabel *numSolLabel;
    QLineEdit *gpol_TB;
    QLineEdit *gnonpol_TB;
    QLineEdit *f2dockScore_TB;
    QLineEdit *epsilonBR_TB;
    QLineEdit *epsilonGpol_TB;
    QLineEdit *numSol_TB;
    QLabel *epsilonGpolLabel;
    Q3GroupBox *resultsSummaryBox;
    QGridLayout *gridLayout2;
    Q3GroupBox *peaksGB;
    QGridLayout *gridLayout3;
    QLabel *totalPeaksLabel;
    QLabel *range1PeaksLabel;
    QLabel *range2PeaksLabel;
    QLabel *range3PeaksLabel;
    QLabel *range4PeaksLabel;
    QLabel *range5PeaksLabel;
    QLineEdit *totalPeaks_TB;
    QLineEdit *range1Peaks_TB;
    QLineEdit *range2Peaks_TB;
    QLineEdit *range3Peaks_TB;
    QLineEdit *range4Peaks_TB;
    QLineEdit *range5Peaks_TB;
    QLabel *timelabel;
    QLineEdit *time_TB;
    Q3GroupBox *bestRMSDGB;
    QGridLayout *gridLayout4;
    QLabel *peakRankLabel2;
    QLabel *peakRMSDLabel2;
    QLineEdit *rank2_TB;
    QLineEdit *rmsd2_TB;
    QLineEdit *score2_TB;
    QLabel *score2Label;
    Q3GroupBox *bestRankGB;
    QGridLayout *gridLayout5;
    QLabel *peakRankLabel1;
    QLabel *peakRMSDLabel1;
    QLineEdit *rank1_TB;
    QLineEdit *rmsd1_TB;
    QLabel *score1Label;
    QLineEdit *score1_TB;
    QPushButton *detailedResultsButton;
    QPushButton *doneButton;

    void setupUi(QDialog *gbRerankResult)
    {
        if (gbRerankResult->objectName().isEmpty())
            gbRerankResult->setObjectName(QString::fromUtf8("gbRerankResult"));
        gbRerankResult->resize(1022, 693);
        gridLayout = new QGridLayout(gbRerankResult);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        JobDescriptionGB = new Q3ButtonGroup(gbRerankResult);
        JobDescriptionGB->setObjectName(QString::fromUtf8("JobDescriptionGB"));
        JobDescriptionGB->setColumnLayout(0, Qt::Vertical);
        JobDescriptionGB->layout()->setSpacing(6);
        JobDescriptionGB->layout()->setMargin(11);
        gridLayout1 = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(JobDescriptionGB->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        receptorPQRLabel = new QLabel(JobDescriptionGB);
        receptorPQRLabel->setObjectName(QString::fromUtf8("receptorPQRLabel"));
        receptorPQRLabel->setWordWrap(false);

        gridLayout1->addWidget(receptorPQRLabel, 0, 0, 1, 1);

        ligandPQRLabel = new QLabel(JobDescriptionGB);
        ligandPQRLabel->setObjectName(QString::fromUtf8("ligandPQRLabel"));
        ligandPQRLabel->setWordWrap(false);

        gridLayout1->addWidget(ligandPQRLabel, 1, 0, 1, 1);

        receptorQUADLabel = new QLabel(JobDescriptionGB);
        receptorQUADLabel->setObjectName(QString::fromUtf8("receptorQUADLabel"));
        receptorQUADLabel->setWordWrap(false);

        gridLayout1->addWidget(receptorQUADLabel, 2, 0, 1, 1);

        ligandQUADLabel = new QLabel(JobDescriptionGB);
        ligandQUADLabel->setObjectName(QString::fromUtf8("ligandQUADLabel"));
        ligandQUADLabel->setWordWrap(false);

        gridLayout1->addWidget(ligandQUADLabel, 3, 0, 2, 1);

        f2dockOuputLabel = new QLabel(JobDescriptionGB);
        f2dockOuputLabel->setObjectName(QString::fromUtf8("f2dockOuputLabel"));
        f2dockOuputLabel->setWordWrap(false);

        gridLayout1->addWidget(f2dockOuputLabel, 5, 0, 2, 1);

        gbrerankLabel = new QLabel(JobDescriptionGB);
        gbrerankLabel->setObjectName(QString::fromUtf8("gbrerankLabel"));
        gbrerankLabel->setWordWrap(false);

        gridLayout1->addWidget(gbrerankLabel, 7, 0, 1, 1);

        receptorPQR_TB = new QLineEdit(JobDescriptionGB);
        receptorPQR_TB->setObjectName(QString::fromUtf8("receptorPQR_TB"));

        gridLayout1->addWidget(receptorPQR_TB, 0, 1, 1, 1);

        ligandPQR_TB = new QLineEdit(JobDescriptionGB);
        ligandPQR_TB->setObjectName(QString::fromUtf8("ligandPQR_TB"));

        gridLayout1->addWidget(ligandPQR_TB, 1, 1, 1, 1);

        receptorQUAD_TB = new QLineEdit(JobDescriptionGB);
        receptorQUAD_TB->setObjectName(QString::fromUtf8("receptorQUAD_TB"));

        gridLayout1->addWidget(receptorQUAD_TB, 2, 1, 1, 1);

        ligandQUAD_TB = new QLineEdit(JobDescriptionGB);
        ligandQUAD_TB->setObjectName(QString::fromUtf8("ligandQUAD_TB"));

        gridLayout1->addWidget(ligandQUAD_TB, 3, 1, 2, 1);

        f2dockOutput_TB = new QLineEdit(JobDescriptionGB);
        f2dockOutput_TB->setObjectName(QString::fromUtf8("f2dockOutput_TB"));

        gridLayout1->addWidget(f2dockOutput_TB, 5, 1, 2, 1);

        gbRerankOutput_TB = new QLineEdit(JobDescriptionGB);
        gbRerankOutput_TB->setObjectName(QString::fromUtf8("gbRerankOutput_TB"));

        gridLayout1->addWidget(gbRerankOutput_TB, 7, 1, 1, 1);

        weightsLabel = new QLabel(JobDescriptionGB);
        weightsLabel->setObjectName(QString::fromUtf8("weightsLabel"));
        weightsLabel->setWordWrap(false);

        gridLayout1->addWidget(weightsLabel, 1, 2, 1, 1);

        errorLabel = new QLabel(JobDescriptionGB);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setWordWrap(false);

        gridLayout1->addWidget(errorLabel, 3, 2, 2, 1);

        gpolLabel = new QLabel(JobDescriptionGB);
        gpolLabel->setObjectName(QString::fromUtf8("gpolLabel"));
        gpolLabel->setWordWrap(false);

        gridLayout1->addWidget(gpolLabel, 0, 3, 1, 1);

        gnonpolLabel = new QLabel(JobDescriptionGB);
        gnonpolLabel->setObjectName(QString::fromUtf8("gnonpolLabel"));
        gnonpolLabel->setWordWrap(false);

        gridLayout1->addWidget(gnonpolLabel, 0, 4, 1, 1);

        f2dockScoreLabel = new QLabel(JobDescriptionGB);
        f2dockScoreLabel->setObjectName(QString::fromUtf8("f2dockScoreLabel"));
        f2dockScoreLabel->setWordWrap(false);

        gridLayout1->addWidget(f2dockScoreLabel, 0, 5, 1, 1);

        epsilonBRLabel = new QLabel(JobDescriptionGB);
        epsilonBRLabel->setObjectName(QString::fromUtf8("epsilonBRLabel"));
        epsilonBRLabel->setWordWrap(false);

        gridLayout1->addWidget(epsilonBRLabel, 2, 3, 2, 1);

        numSolLabel = new QLabel(JobDescriptionGB);
        numSolLabel->setObjectName(QString::fromUtf8("numSolLabel"));
        numSolLabel->setWordWrap(false);

        gridLayout1->addWidget(numSolLabel, 7, 2, 1, 2);

        gpol_TB = new QLineEdit(JobDescriptionGB);
        gpol_TB->setObjectName(QString::fromUtf8("gpol_TB"));

        gridLayout1->addWidget(gpol_TB, 1, 3, 1, 1);

        gnonpol_TB = new QLineEdit(JobDescriptionGB);
        gnonpol_TB->setObjectName(QString::fromUtf8("gnonpol_TB"));

        gridLayout1->addWidget(gnonpol_TB, 1, 4, 1, 1);

        f2dockScore_TB = new QLineEdit(JobDescriptionGB);
        f2dockScore_TB->setObjectName(QString::fromUtf8("f2dockScore_TB"));

        gridLayout1->addWidget(f2dockScore_TB, 1, 5, 1, 1);

        epsilonBR_TB = new QLineEdit(JobDescriptionGB);
        epsilonBR_TB->setObjectName(QString::fromUtf8("epsilonBR_TB"));

        gridLayout1->addWidget(epsilonBR_TB, 4, 3, 2, 1);

        epsilonGpol_TB = new QLineEdit(JobDescriptionGB);
        epsilonGpol_TB->setObjectName(QString::fromUtf8("epsilonGpol_TB"));

        gridLayout1->addWidget(epsilonGpol_TB, 4, 4, 2, 1);

        numSol_TB = new QLineEdit(JobDescriptionGB);
        numSol_TB->setObjectName(QString::fromUtf8("numSol_TB"));

        gridLayout1->addWidget(numSol_TB, 6, 4, 2, 1);

        epsilonGpolLabel = new QLabel(JobDescriptionGB);
        epsilonGpolLabel->setObjectName(QString::fromUtf8("epsilonGpolLabel"));
        epsilonGpolLabel->setWordWrap(false);

        gridLayout1->addWidget(epsilonGpolLabel, 2, 4, 2, 2);


        gridLayout->addWidget(JobDescriptionGB, 0, 0, 1, 2);

        resultsSummaryBox = new Q3GroupBox(gbRerankResult);
        resultsSummaryBox->setObjectName(QString::fromUtf8("resultsSummaryBox"));
        resultsSummaryBox->setColumnLayout(0, Qt::Vertical);
        resultsSummaryBox->layout()->setSpacing(6);
        resultsSummaryBox->layout()->setMargin(11);
        gridLayout2 = new QGridLayout();
        QBoxLayout *boxlayout1 = qobject_cast<QBoxLayout *>(resultsSummaryBox->layout());
        if (boxlayout1)
            boxlayout1->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        peaksGB = new Q3GroupBox(resultsSummaryBox);
        peaksGB->setObjectName(QString::fromUtf8("peaksGB"));
        peaksGB->setColumnLayout(0, Qt::Vertical);
        peaksGB->layout()->setSpacing(6);
        peaksGB->layout()->setMargin(11);
        gridLayout3 = new QGridLayout();
        QBoxLayout *boxlayout2 = qobject_cast<QBoxLayout *>(peaksGB->layout());
        if (boxlayout2)
            boxlayout2->addLayout(gridLayout3);
        gridLayout3->setAlignment(Qt::AlignTop);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        totalPeaksLabel = new QLabel(peaksGB);
        totalPeaksLabel->setObjectName(QString::fromUtf8("totalPeaksLabel"));
        totalPeaksLabel->setWordWrap(false);

        gridLayout3->addWidget(totalPeaksLabel, 0, 1, 1, 1);

        range1PeaksLabel = new QLabel(peaksGB);
        range1PeaksLabel->setObjectName(QString::fromUtf8("range1PeaksLabel"));
        range1PeaksLabel->setWordWrap(false);

        gridLayout3->addWidget(range1PeaksLabel, 1, 1, 1, 1);

        range2PeaksLabel = new QLabel(peaksGB);
        range2PeaksLabel->setObjectName(QString::fromUtf8("range2PeaksLabel"));
        range2PeaksLabel->setWordWrap(false);

        gridLayout3->addWidget(range2PeaksLabel, 2, 1, 1, 1);

        range3PeaksLabel = new QLabel(peaksGB);
        range3PeaksLabel->setObjectName(QString::fromUtf8("range3PeaksLabel"));
        range3PeaksLabel->setWordWrap(false);

        gridLayout3->addWidget(range3PeaksLabel, 3, 1, 1, 1);

        range4PeaksLabel = new QLabel(peaksGB);
        range4PeaksLabel->setObjectName(QString::fromUtf8("range4PeaksLabel"));
        range4PeaksLabel->setWordWrap(false);

        gridLayout3->addWidget(range4PeaksLabel, 4, 1, 1, 1);

        range5PeaksLabel = new QLabel(peaksGB);
        range5PeaksLabel->setObjectName(QString::fromUtf8("range5PeaksLabel"));
        range5PeaksLabel->setWordWrap(false);

        gridLayout3->addWidget(range5PeaksLabel, 5, 1, 1, 1);

        totalPeaks_TB = new QLineEdit(peaksGB);
        totalPeaks_TB->setObjectName(QString::fromUtf8("totalPeaks_TB"));

        gridLayout3->addWidget(totalPeaks_TB, 0, 0, 1, 1);

        range1Peaks_TB = new QLineEdit(peaksGB);
        range1Peaks_TB->setObjectName(QString::fromUtf8("range1Peaks_TB"));

        gridLayout3->addWidget(range1Peaks_TB, 1, 0, 1, 1);

        range2Peaks_TB = new QLineEdit(peaksGB);
        range2Peaks_TB->setObjectName(QString::fromUtf8("range2Peaks_TB"));

        gridLayout3->addWidget(range2Peaks_TB, 2, 0, 1, 1);

        range3Peaks_TB = new QLineEdit(peaksGB);
        range3Peaks_TB->setObjectName(QString::fromUtf8("range3Peaks_TB"));

        gridLayout3->addWidget(range3Peaks_TB, 3, 0, 1, 1);

        range4Peaks_TB = new QLineEdit(peaksGB);
        range4Peaks_TB->setObjectName(QString::fromUtf8("range4Peaks_TB"));

        gridLayout3->addWidget(range4Peaks_TB, 4, 0, 1, 1);

        range5Peaks_TB = new QLineEdit(peaksGB);
        range5Peaks_TB->setObjectName(QString::fromUtf8("range5Peaks_TB"));

        gridLayout3->addWidget(range5Peaks_TB, 5, 0, 1, 1);


        gridLayout2->addWidget(peaksGB, 0, 0, 3, 1);

        timelabel = new QLabel(resultsSummaryBox);
        timelabel->setObjectName(QString::fromUtf8("timelabel"));
        timelabel->setWordWrap(false);

        gridLayout2->addWidget(timelabel, 0, 1, 1, 1);

        time_TB = new QLineEdit(resultsSummaryBox);
        time_TB->setObjectName(QString::fromUtf8("time_TB"));

        gridLayout2->addWidget(time_TB, 0, 2, 1, 1);

        bestRMSDGB = new Q3GroupBox(resultsSummaryBox);
        bestRMSDGB->setObjectName(QString::fromUtf8("bestRMSDGB"));
        bestRMSDGB->setColumnLayout(0, Qt::Vertical);
        bestRMSDGB->layout()->setSpacing(6);
        bestRMSDGB->layout()->setMargin(11);
        gridLayout4 = new QGridLayout();
        QBoxLayout *boxlayout3 = qobject_cast<QBoxLayout *>(bestRMSDGB->layout());
        if (boxlayout3)
            boxlayout3->addLayout(gridLayout4);
        gridLayout4->setAlignment(Qt::AlignTop);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        peakRankLabel2 = new QLabel(bestRMSDGB);
        peakRankLabel2->setObjectName(QString::fromUtf8("peakRankLabel2"));
        peakRankLabel2->setWordWrap(false);

        gridLayout4->addWidget(peakRankLabel2, 0, 0, 1, 1);

        peakRMSDLabel2 = new QLabel(bestRMSDGB);
        peakRMSDLabel2->setObjectName(QString::fromUtf8("peakRMSDLabel2"));
        peakRMSDLabel2->setWordWrap(false);

        gridLayout4->addWidget(peakRMSDLabel2, 0, 2, 1, 1);

        rank2_TB = new QLineEdit(bestRMSDGB);
        rank2_TB->setObjectName(QString::fromUtf8("rank2_TB"));

        gridLayout4->addWidget(rank2_TB, 0, 1, 1, 1);

        rmsd2_TB = new QLineEdit(bestRMSDGB);
        rmsd2_TB->setObjectName(QString::fromUtf8("rmsd2_TB"));

        gridLayout4->addWidget(rmsd2_TB, 0, 3, 1, 1);

        score2_TB = new QLineEdit(bestRMSDGB);
        score2_TB->setObjectName(QString::fromUtf8("score2_TB"));

        gridLayout4->addWidget(score2_TB, 0, 5, 1, 1);

        score2Label = new QLabel(bestRMSDGB);
        score2Label->setObjectName(QString::fromUtf8("score2Label"));
        score2Label->setWordWrap(false);

        gridLayout4->addWidget(score2Label, 0, 4, 1, 1);


        gridLayout2->addWidget(bestRMSDGB, 2, 1, 1, 2);

        bestRankGB = new Q3GroupBox(resultsSummaryBox);
        bestRankGB->setObjectName(QString::fromUtf8("bestRankGB"));
        bestRankGB->setColumnLayout(0, Qt::Vertical);
        bestRankGB->layout()->setSpacing(6);
        bestRankGB->layout()->setMargin(11);
        gridLayout5 = new QGridLayout();
        QBoxLayout *boxlayout4 = qobject_cast<QBoxLayout *>(bestRankGB->layout());
        if (boxlayout4)
            boxlayout4->addLayout(gridLayout5);
        gridLayout5->setAlignment(Qt::AlignTop);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        peakRankLabel1 = new QLabel(bestRankGB);
        peakRankLabel1->setObjectName(QString::fromUtf8("peakRankLabel1"));
        peakRankLabel1->setWordWrap(false);

        gridLayout5->addWidget(peakRankLabel1, 0, 0, 1, 1);

        peakRMSDLabel1 = new QLabel(bestRankGB);
        peakRMSDLabel1->setObjectName(QString::fromUtf8("peakRMSDLabel1"));
        peakRMSDLabel1->setWordWrap(false);

        gridLayout5->addWidget(peakRMSDLabel1, 0, 2, 1, 1);

        rank1_TB = new QLineEdit(bestRankGB);
        rank1_TB->setObjectName(QString::fromUtf8("rank1_TB"));

        gridLayout5->addWidget(rank1_TB, 0, 1, 1, 1);

        rmsd1_TB = new QLineEdit(bestRankGB);
        rmsd1_TB->setObjectName(QString::fromUtf8("rmsd1_TB"));

        gridLayout5->addWidget(rmsd1_TB, 0, 3, 1, 1);

        score1Label = new QLabel(bestRankGB);
        score1Label->setObjectName(QString::fromUtf8("score1Label"));
        score1Label->setWordWrap(false);

        gridLayout5->addWidget(score1Label, 0, 4, 1, 1);

        score1_TB = new QLineEdit(bestRankGB);
        score1_TB->setObjectName(QString::fromUtf8("score1_TB"));

        gridLayout5->addWidget(score1_TB, 0, 5, 1, 1);


        gridLayout2->addWidget(bestRankGB, 1, 1, 1, 2);


        gridLayout->addWidget(resultsSummaryBox, 1, 0, 1, 2);

        detailedResultsButton = new QPushButton(gbRerankResult);
        detailedResultsButton->setObjectName(QString::fromUtf8("detailedResultsButton"));

        gridLayout->addWidget(detailedResultsButton, 2, 0, 1, 1);

        doneButton = new QPushButton(gbRerankResult);
        doneButton->setObjectName(QString::fromUtf8("doneButton"));

        gridLayout->addWidget(doneButton, 2, 1, 1, 1);


        retranslateUi(gbRerankResult);
        QObject::connect(detailedResultsButton, SIGNAL(clicked()), gbRerankResult, SLOT(detailedsSlot()));
        QObject::connect(doneButton, SIGNAL(clicked()), gbRerankResult, SLOT(doneSlot()));

        QMetaObject::connectSlotsByName(gbRerankResult);
    } // setupUi

    void retranslateUi(QDialog *gbRerankResult)
    {
        gbRerankResult->setWindowTitle(QApplication::translate("gbRerankResult", "GBRerank Results", 0, QApplication::UnicodeUTF8));
        JobDescriptionGB->setTitle(QApplication::translate("gbRerankResult", "Reranking Job Description", 0, QApplication::UnicodeUTF8));
        receptorPQRLabel->setText(QApplication::translate("gbRerankResult", "Receptor PQR", 0, QApplication::UnicodeUTF8));
        ligandPQRLabel->setText(QApplication::translate("gbRerankResult", "Ligand PQR", 0, QApplication::UnicodeUTF8));
        receptorQUADLabel->setText(QApplication::translate("gbRerankResult", "Receptor 	QUAD", 0, QApplication::UnicodeUTF8));
        ligandQUADLabel->setText(QApplication::translate("gbRerankResult", "Ligand QUAD", 0, QApplication::UnicodeUTF8));
        f2dockOuputLabel->setText(QApplication::translate("gbRerankResult", "F2Dock Output", 0, QApplication::UnicodeUTF8));
        gbrerankLabel->setText(QApplication::translate("gbRerankResult", "GBRerank Output", 0, QApplication::UnicodeUTF8));
        weightsLabel->setText(QApplication::translate("gbRerankResult", "Weights", 0, QApplication::UnicodeUTF8));
        errorLabel->setText(QApplication::translate("gbRerankResult", "Error", 0, QApplication::UnicodeUTF8));
        gpolLabel->setText(QApplication::translate("gbRerankResult", "G(pol)", 0, QApplication::UnicodeUTF8));
        gnonpolLabel->setText(QApplication::translate("gbRerankResult", "G(nonpol)", 0, QApplication::UnicodeUTF8));
        f2dockScoreLabel->setText(QApplication::translate("gbRerankResult", "F2DockScore", 0, QApplication::UnicodeUTF8));
        epsilonBRLabel->setText(QApplication::translate("gbRerankResult", "Epsilon(BR)", 0, QApplication::UnicodeUTF8));
        numSolLabel->setText(QApplication::translate("gbRerankResult", "Number of Solutions", 0, QApplication::UnicodeUTF8));
        epsilonGpolLabel->setText(QApplication::translate("gbRerankResult", "Epsilon(Gpol)", 0, QApplication::UnicodeUTF8));
        resultsSummaryBox->setTitle(QApplication::translate("gbRerankResult", "Summary Results", 0, QApplication::UnicodeUTF8));
        peaksGB->setTitle(QApplication::translate("gbRerankResult", "Number of Peaks (<= 5.0 RMSD)", 0, QApplication::UnicodeUTF8));
        totalPeaksLabel->setText(QApplication::translate("gbRerankResult", "peaks in total", 0, QApplication::UnicodeUTF8));
        range1PeaksLabel->setText(QApplication::translate("gbRerankResult", "peaks in range [1, 1]", 0, QApplication::UnicodeUTF8));
        range2PeaksLabel->setText(QApplication::translate("gbRerankResult", "peaks in range [1, 10]", 0, QApplication::UnicodeUTF8));
        range3PeaksLabel->setText(QApplication::translate("gbRerankResult", "peaks in range [1, 100]", 0, QApplication::UnicodeUTF8));
        range4PeaksLabel->setText(QApplication::translate("gbRerankResult", "peaks in range [1, 1000]", 0, QApplication::UnicodeUTF8));
        range5PeaksLabel->setText(QApplication::translate("gbRerankResult", "peaks in range [1, 10000]", 0, QApplication::UnicodeUTF8));
        timelabel->setText(QApplication::translate("gbRerankResult", "Time to complete", 0, QApplication::UnicodeUTF8));
        bestRMSDGB->setTitle(QApplication::translate("gbRerankResult", "Peak with best RMSD", 0, QApplication::UnicodeUTF8));
        peakRankLabel2->setText(QApplication::translate("gbRerankResult", "Rank", 0, QApplication::UnicodeUTF8));
        peakRMSDLabel2->setText(QApplication::translate("gbRerankResult", "RMSD", 0, QApplication::UnicodeUTF8));
        score2Label->setText(QApplication::translate("gbRerankResult", "Score", 0, QApplication::UnicodeUTF8));
        bestRankGB->setTitle(QApplication::translate("gbRerankResult", "Best ranked peak", 0, QApplication::UnicodeUTF8));
        peakRankLabel1->setText(QApplication::translate("gbRerankResult", "Rank", 0, QApplication::UnicodeUTF8));
        peakRMSDLabel1->setText(QApplication::translate("gbRerankResult", "RMSD", 0, QApplication::UnicodeUTF8));
        score1Label->setText(QApplication::translate("gbRerankResult", "Score", 0, QApplication::UnicodeUTF8));
        detailedResultsButton->setText(QApplication::translate("gbRerankResult", "View Detailed Results", 0, QApplication::UnicodeUTF8));
        doneButton->setText(QApplication::translate("gbRerankResult", "Done", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(gbRerankResult);
    } // retranslateUi

};

namespace Ui {
    class gbRerankResult: public Ui_gbRerankResult {};
} // namespace Ui

QT_END_NAMESPACE

class gbRerankResult : public QDialog, public Ui::gbRerankResult
{
    Q_OBJECT

public:
    gbRerankResult(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~gbRerankResult();

public slots:
    virtual void detailedsSlot();
    virtual void doneSlot();

protected slots:
    virtual void languageChange();

};

#endif // GBRERANKRESULT_H
