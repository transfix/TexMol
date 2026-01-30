#ifndef F2DOCKRESULT_H
#define F2DOCKRESULT_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_f2DockResult
{
public:
    Q3GroupBox *JobDescGB;
    QLineEdit *SC_TB;
    QLineEdit *CC_TB;
    QLineEdit *EE_TB;
    QLineEdit *SS_TB;
    QLineEdit *RS_TB;
    QLineEdit *GS_TB;
    QLineEdit *OU_TB;
    QLineEdit *RM_TB;
    QLineEdit *LG_TB;
    QLineEdit *RC_TB;
    QLineEdit *DM_TB;
    QLabel *textLabel2_2;
    QLabel *textLabel2;
    QLabel *textLabel2_5;
    QLabel *textLabel2_3;
    QLabel *textLabel2_4;
    QLabel *rotationLabel;
    QLabel *gridSpaceLabel;
    QLabel *textLabel3;
    QLabel *textLabel3_2;
    QLabel *textLabel3_2_2;
    QLabel *textLabel3_2_2_2;
    QLabel *textLabel3_2_2_2_2;
    QPushButton *DR_Button;
    QPushButton *Q_Button;
    Q3GroupBox *groupBox10;
    QLabel *textLabel6;
    Q3GroupBox *minRMSD_2;
    QLabel *textLabel5_3;
    QLabel *textLabel5_2_2_2;
    QLabel *textLabel5_2_3;
    QLineEdit *ts_TB_2;
    QLineEdit *rmsd_TB_2;
    QLineEdit *rank_TB_2;
    Q3GroupBox *Peaks;
    QLabel *textLabel4_2_2_2_2;
    QLabel *textLabel4_2_2_2;
    QLabel *textLabel4;
    QLabel *textLabel4_2_2;
    QLabel *textLabel4_2_2_2_2_2;
    QLabel *textLabel4_2;
    QLineEdit *totPeak_TB;
    QLineEdit *rangePeak_TB_1;
    QLineEdit *rangePeak_TB_2;
    QLineEdit *rangePeak_TB_3;
    QLineEdit *rangePeak_TB_4;
    QLineEdit *rangePeak_TB_5;
    QLineEdit *time_TB;
    Q3GroupBox *minRMSD;
    QLabel *textLabel5;
    QLabel *textLabel5_2_2;
    QLabel *textLabel5_2;
    QLineEdit *rank_TB;
    QLineEdit *rmsd_TB;
    QLineEdit *ts_TB;

    void setupUi(QDialog *f2DockResult)
    {
        if (f2DockResult->objectName().isEmpty())
            f2DockResult->setObjectName(QString::fromUtf8("f2DockResult"));
        f2DockResult->resize(1104, 561);
        JobDescGB = new Q3GroupBox(f2DockResult);
        JobDescGB->setObjectName(QString::fromUtf8("JobDescGB"));
        JobDescGB->setGeometry(QRect(20, 10, 1070, 190));
        SC_TB = new QLineEdit(JobDescGB);
        SC_TB->setObjectName(QString::fromUtf8("SC_TB"));
        SC_TB->setGeometry(QRect(780, 60, 61, 21));
        SC_TB->setReadOnly(true);
        CC_TB = new QLineEdit(JobDescGB);
        CC_TB->setObjectName(QString::fromUtf8("CC_TB"));
        CC_TB->setGeometry(QRect(870, 60, 61, 21));
        CC_TB->setReadOnly(true);
        EE_TB = new QLineEdit(JobDescGB);
        EE_TB->setObjectName(QString::fromUtf8("EE_TB"));
        EE_TB->setGeometry(QRect(960, 60, 61, 21));
        EE_TB->setReadOnly(true);
        SS_TB = new QLineEdit(JobDescGB);
        SS_TB->setObjectName(QString::fromUtf8("SS_TB"));
        SS_TB->setGeometry(QRect(690, 60, 61, 21));
        SS_TB->setReadOnly(true);
        RS_TB = new QLineEdit(JobDescGB);
        RS_TB->setObjectName(QString::fromUtf8("RS_TB"));
        RS_TB->setGeometry(QRect(780, 120, 61, 21));
        RS_TB->setReadOnly(true);
        GS_TB = new QLineEdit(JobDescGB);
        GS_TB->setObjectName(QString::fromUtf8("GS_TB"));
        GS_TB->setGeometry(QRect(780, 150, 61, 21));
        GS_TB->setReadOnly(true);
        OU_TB = new QLineEdit(JobDescGB);
        OU_TB->setObjectName(QString::fromUtf8("OU_TB"));
        OU_TB->setGeometry(QRect(140, 150, 390, 22));
        OU_TB->setReadOnly(true);
        RM_TB = new QLineEdit(JobDescGB);
        RM_TB->setObjectName(QString::fromUtf8("RM_TB"));
        RM_TB->setGeometry(QRect(140, 120, 390, 22));
        RM_TB->setReadOnly(true);
        LG_TB = new QLineEdit(JobDescGB);
        LG_TB->setObjectName(QString::fromUtf8("LG_TB"));
        LG_TB->setGeometry(QRect(140, 90, 390, 22));
        LG_TB->setReadOnly(true);
        RC_TB = new QLineEdit(JobDescGB);
        RC_TB->setObjectName(QString::fromUtf8("RC_TB"));
        RC_TB->setGeometry(QRect(140, 60, 390, 22));
        RC_TB->setReadOnly(true);
        DM_TB = new QLineEdit(JobDescGB);
        DM_TB->setObjectName(QString::fromUtf8("DM_TB"));
        DM_TB->setGeometry(QRect(140, 30, 390, 22));
        DM_TB->setReadOnly(true);
        textLabel2_2 = new QLabel(JobDescGB);
        textLabel2_2->setObjectName(QString::fromUtf8("textLabel2_2"));
        textLabel2_2->setGeometry(QRect(20, 90, 100, 20));
        textLabel2_2->setWordWrap(false);
        textLabel2 = new QLabel(JobDescGB);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setGeometry(QRect(20, 60, 100, 20));
        textLabel2->setWordWrap(false);
        textLabel2_5 = new QLabel(JobDescGB);
        textLabel2_5->setObjectName(QString::fromUtf8("textLabel2_5"));
        textLabel2_5->setGeometry(QRect(20, 30, 110, 20));
        textLabel2_5->setWordWrap(false);
        textLabel2_3 = new QLabel(JobDescGB);
        textLabel2_3->setObjectName(QString::fromUtf8("textLabel2_3"));
        textLabel2_3->setGeometry(QRect(20, 120, 100, 20));
        textLabel2_3->setWordWrap(false);
        textLabel2_4 = new QLabel(JobDescGB);
        textLabel2_4->setObjectName(QString::fromUtf8("textLabel2_4"));
        textLabel2_4->setGeometry(QRect(20, 150, 90, 20));
        textLabel2_4->setWordWrap(false);
        rotationLabel = new QLabel(JobDescGB);
        rotationLabel->setObjectName(QString::fromUtf8("rotationLabel"));
        rotationLabel->setGeometry(QRect(590, 120, 170, 20));
        rotationLabel->setFrameShape(QFrame::NoFrame);
        rotationLabel->setWordWrap(false);
        gridSpaceLabel = new QLabel(JobDescGB);
        gridSpaceLabel->setObjectName(QString::fromUtf8("gridSpaceLabel"));
        gridSpaceLabel->setGeometry(QRect(590, 150, 180, 20));
        gridSpaceLabel->setWordWrap(false);
        textLabel3 = new QLabel(JobDescGB);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setGeometry(QRect(590, 60, 80, 20));
        textLabel3->setWordWrap(false);
        textLabel3_2 = new QLabel(JobDescGB);
        textLabel3_2->setObjectName(QString::fromUtf8("textLabel3_2"));
        textLabel3_2->setGeometry(QRect(680, 30, 80, 20));
        textLabel3_2->setAlignment(Qt::AlignCenter);
        textLabel3_2->setWordWrap(false);
        textLabel3_2_2 = new QLabel(JobDescGB);
        textLabel3_2_2->setObjectName(QString::fromUtf8("textLabel3_2_2"));
        textLabel3_2_2->setGeometry(QRect(770, 30, 80, 20));
        textLabel3_2_2->setAlignment(Qt::AlignCenter);
        textLabel3_2_2->setWordWrap(false);
        textLabel3_2_2_2 = new QLabel(JobDescGB);
        textLabel3_2_2_2->setObjectName(QString::fromUtf8("textLabel3_2_2_2"));
        textLabel3_2_2_2->setGeometry(QRect(860, 30, 80, 20));
        textLabel3_2_2_2->setAlignment(Qt::AlignCenter);
        textLabel3_2_2_2->setWordWrap(false);
        textLabel3_2_2_2_2 = new QLabel(JobDescGB);
        textLabel3_2_2_2_2->setObjectName(QString::fromUtf8("textLabel3_2_2_2_2"));
        textLabel3_2_2_2_2->setGeometry(QRect(940, 30, 110, 20));
        textLabel3_2_2_2_2->setAlignment(Qt::AlignCenter);
        textLabel3_2_2_2_2->setWordWrap(false);
        DR_Button = new QPushButton(f2DockResult);
        DR_Button->setObjectName(QString::fromUtf8("DR_Button"));
        DR_Button->setGeometry(QRect(20, 500, 530, 40));
        Q_Button = new QPushButton(f2DockResult);
        Q_Button->setObjectName(QString::fromUtf8("Q_Button"));
        Q_Button->setGeometry(QRect(560, 500, 530, 40));
        groupBox10 = new Q3GroupBox(f2DockResult);
        groupBox10->setObjectName(QString::fromUtf8("groupBox10"));
        groupBox10->setGeometry(QRect(20, 210, 1070, 270));
        textLabel6 = new QLabel(groupBox10);
        textLabel6->setObjectName(QString::fromUtf8("textLabel6"));
        textLabel6->setGeometry(QRect(450, 50, 180, 20));
        textLabel6->setWordWrap(false);
        minRMSD_2 = new Q3GroupBox(groupBox10);
        minRMSD_2->setObjectName(QString::fromUtf8("minRMSD_2"));
        minRMSD_2->setGeometry(QRect(440, 180, 610, 70));
        textLabel5_3 = new QLabel(minRMSD_2);
        textLabel5_3->setObjectName(QString::fromUtf8("textLabel5_3"));
        textLabel5_3->setGeometry(QRect(20, 30, 60, 20));
        textLabel5_3->setWordWrap(false);
        textLabel5_2_2_2 = new QLabel(minRMSD_2);
        textLabel5_2_2_2->setObjectName(QString::fromUtf8("textLabel5_2_2_2"));
        textLabel5_2_2_2->setGeometry(QRect(430, 30, 60, 20));
        textLabel5_2_2_2->setWordWrap(false);
        textLabel5_2_3 = new QLabel(minRMSD_2);
        textLabel5_2_3->setObjectName(QString::fromUtf8("textLabel5_2_3"));
        textLabel5_2_3->setGeometry(QRect(230, 30, 60, 20));
        textLabel5_2_3->setWordWrap(false);
        ts_TB_2 = new QLineEdit(minRMSD_2);
        ts_TB_2->setObjectName(QString::fromUtf8("ts_TB_2"));
        ts_TB_2->setGeometry(QRect(490, 30, 100, 22));
        ts_TB_2->setReadOnly(true);
        rmsd_TB_2 = new QLineEdit(minRMSD_2);
        rmsd_TB_2->setObjectName(QString::fromUtf8("rmsd_TB_2"));
        rmsd_TB_2->setGeometry(QRect(290, 30, 80, 22));
        rmsd_TB_2->setReadOnly(true);
        rank_TB_2 = new QLineEdit(minRMSD_2);
        rank_TB_2->setObjectName(QString::fromUtf8("rank_TB_2"));
        rank_TB_2->setGeometry(QRect(80, 30, 80, 22));
        rank_TB_2->setReadOnly(true);
        Peaks = new Q3GroupBox(groupBox10);
        Peaks->setObjectName(QString::fromUtf8("Peaks"));
        Peaks->setGeometry(QRect(10, 30, 411, 220));
        textLabel4_2_2_2_2 = new QLabel(Peaks);
        textLabel4_2_2_2_2->setObjectName(QString::fromUtf8("textLabel4_2_2_2_2"));
        textLabel4_2_2_2_2->setGeometry(QRect(110, 150, 270, 20));
        textLabel4_2_2_2_2->setWordWrap(false);
        textLabel4_2_2_2 = new QLabel(Peaks);
        textLabel4_2_2_2->setObjectName(QString::fromUtf8("textLabel4_2_2_2"));
        textLabel4_2_2_2->setGeometry(QRect(110, 120, 270, 20));
        textLabel4_2_2_2->setWordWrap(false);
        textLabel4 = new QLabel(Peaks);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setGeometry(QRect(110, 30, 270, 20));
        textLabel4->setWordWrap(false);
        textLabel4_2_2 = new QLabel(Peaks);
        textLabel4_2_2->setObjectName(QString::fromUtf8("textLabel4_2_2"));
        textLabel4_2_2->setGeometry(QRect(110, 90, 270, 20));
        textLabel4_2_2->setWordWrap(false);
        textLabel4_2_2_2_2_2 = new QLabel(Peaks);
        textLabel4_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel4_2_2_2_2_2"));
        textLabel4_2_2_2_2_2->setGeometry(QRect(110, 180, 270, 20));
        textLabel4_2_2_2_2_2->setWordWrap(false);
        textLabel4_2 = new QLabel(Peaks);
        textLabel4_2->setObjectName(QString::fromUtf8("textLabel4_2"));
        textLabel4_2->setGeometry(QRect(110, 60, 270, 20));
        textLabel4_2->setWordWrap(false);
        totPeak_TB = new QLineEdit(Peaks);
        totPeak_TB->setObjectName(QString::fromUtf8("totPeak_TB"));
        totPeak_TB->setGeometry(QRect(30, 30, 60, 21));
        totPeak_TB->setAlignment(Qt::AlignRight);
        totPeak_TB->setReadOnly(true);
        rangePeak_TB_1 = new QLineEdit(Peaks);
        rangePeak_TB_1->setObjectName(QString::fromUtf8("rangePeak_TB_1"));
        rangePeak_TB_1->setGeometry(QRect(30, 60, 60, 21));
        rangePeak_TB_1->setAlignment(Qt::AlignRight);
        rangePeak_TB_1->setReadOnly(true);
        rangePeak_TB_2 = new QLineEdit(Peaks);
        rangePeak_TB_2->setObjectName(QString::fromUtf8("rangePeak_TB_2"));
        rangePeak_TB_2->setGeometry(QRect(30, 90, 60, 21));
        rangePeak_TB_2->setAlignment(Qt::AlignRight);
        rangePeak_TB_2->setReadOnly(true);
        rangePeak_TB_3 = new QLineEdit(Peaks);
        rangePeak_TB_3->setObjectName(QString::fromUtf8("rangePeak_TB_3"));
        rangePeak_TB_3->setGeometry(QRect(30, 120, 60, 21));
        rangePeak_TB_3->setAlignment(Qt::AlignRight);
        rangePeak_TB_3->setReadOnly(true);
        rangePeak_TB_4 = new QLineEdit(Peaks);
        rangePeak_TB_4->setObjectName(QString::fromUtf8("rangePeak_TB_4"));
        rangePeak_TB_4->setGeometry(QRect(30, 150, 60, 21));
        rangePeak_TB_4->setAlignment(Qt::AlignRight);
        rangePeak_TB_4->setReadOnly(true);
        rangePeak_TB_5 = new QLineEdit(Peaks);
        rangePeak_TB_5->setObjectName(QString::fromUtf8("rangePeak_TB_5"));
        rangePeak_TB_5->setGeometry(QRect(30, 180, 60, 21));
        rangePeak_TB_5->setAlignment(Qt::AlignRight);
        rangePeak_TB_5->setReadOnly(true);
        time_TB = new QLineEdit(groupBox10);
        time_TB->setObjectName(QString::fromUtf8("time_TB"));
        time_TB->setGeometry(QRect(640, 50, 110, 22));
        time_TB->setReadOnly(true);
        minRMSD = new Q3GroupBox(groupBox10);
        minRMSD->setObjectName(QString::fromUtf8("minRMSD"));
        minRMSD->setGeometry(QRect(440, 90, 610, 70));
        textLabel5 = new QLabel(minRMSD);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        textLabel5->setGeometry(QRect(20, 30, 60, 20));
        textLabel5->setWordWrap(false);
        textLabel5_2_2 = new QLabel(minRMSD);
        textLabel5_2_2->setObjectName(QString::fromUtf8("textLabel5_2_2"));
        textLabel5_2_2->setGeometry(QRect(430, 30, 60, 20));
        textLabel5_2_2->setWordWrap(false);
        textLabel5_2 = new QLabel(minRMSD);
        textLabel5_2->setObjectName(QString::fromUtf8("textLabel5_2"));
        textLabel5_2->setGeometry(QRect(230, 30, 60, 20));
        textLabel5_2->setWordWrap(false);
        rank_TB = new QLineEdit(minRMSD);
        rank_TB->setObjectName(QString::fromUtf8("rank_TB"));
        rank_TB->setGeometry(QRect(80, 30, 80, 22));
        rank_TB->setReadOnly(true);
        rmsd_TB = new QLineEdit(minRMSD);
        rmsd_TB->setObjectName(QString::fromUtf8("rmsd_TB"));
        rmsd_TB->setGeometry(QRect(290, 30, 80, 22));
        rmsd_TB->setReadOnly(true);
        ts_TB = new QLineEdit(minRMSD);
        ts_TB->setObjectName(QString::fromUtf8("ts_TB"));
        ts_TB->setGeometry(QRect(490, 30, 100, 22));
        ts_TB->setReadOnly(true);

        retranslateUi(f2DockResult);
        QObject::connect(DR_Button, SIGNAL(clicked()), f2DockResult, SLOT(detailsSlot()));
        QObject::connect(Q_Button, SIGNAL(clicked()), f2DockResult, SLOT(doneSlot()));

        QMetaObject::connectSlotsByName(f2DockResult);
    } // setupUi

    void retranslateUi(QDialog *f2DockResult)
    {
        f2DockResult->setWindowTitle(QApplication::translate("f2DockResult", "F2Dock Results", 0, QApplication::UnicodeUTF8));
        JobDescGB->setTitle(QApplication::translate("f2DockResult", "Docking Job Description", 0, QApplication::UnicodeUTF8));
        textLabel2_2->setText(QApplication::translate("f2DockResult", "Ligand", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("f2DockResult", "Receptor", 0, QApplication::UnicodeUTF8));
        textLabel2_5->setText(QApplication::translate("f2DockResult", "Docking Mode", 0, QApplication::UnicodeUTF8));
        textLabel2_3->setText(QApplication::translate("f2DockResult", "RMSD File", 0, QApplication::UnicodeUTF8));
        textLabel2_4->setText(QApplication::translate("f2DockResult", "Output", 0, QApplication::UnicodeUTF8));
        rotationLabel->setText(QApplication::translate("f2DockResult", "Rotation separation", 0, QApplication::UnicodeUTF8));
        gridSpaceLabel->setText(QApplication::translate("f2DockResult", "Grid Spacing", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("f2DockResult", "Weights", 0, QApplication::UnicodeUTF8));
        textLabel3_2->setText(QApplication::translate("f2DockResult", "Skin-Skin", 0, QApplication::UnicodeUTF8));
        textLabel3_2_2->setText(QApplication::translate("f2DockResult", "Skin-Core", 0, QApplication::UnicodeUTF8));
        textLabel3_2_2_2->setText(QApplication::translate("f2DockResult", "Core-Core", 0, QApplication::UnicodeUTF8));
        textLabel3_2_2_2_2->setText(QApplication::translate("f2DockResult", "Electrostatics", 0, QApplication::UnicodeUTF8));
        DR_Button->setText(QApplication::translate("f2DockResult", "View Detailed Results", 0, QApplication::UnicodeUTF8));
        Q_Button->setText(QApplication::translate("f2DockResult", "Done", 0, QApplication::UnicodeUTF8));
        groupBox10->setTitle(QApplication::translate("f2DockResult", "Summary Results", 0, QApplication::UnicodeUTF8));
        textLabel6->setText(QApplication::translate("f2DockResult", "Time to complete", 0, QApplication::UnicodeUTF8));
        minRMSD_2->setTitle(QApplication::translate("f2DockResult", "Peak Having Best RMSD", 0, QApplication::UnicodeUTF8));
        textLabel5_3->setText(QApplication::translate("f2DockResult", "Rank", 0, QApplication::UnicodeUTF8));
        textLabel5_2_2_2->setText(QApplication::translate("f2DockResult", "Score", 0, QApplication::UnicodeUTF8));
        textLabel5_2_3->setText(QApplication::translate("f2DockResult", "RMSD", 0, QApplication::UnicodeUTF8));
        Peaks->setTitle(QApplication::translate("f2DockResult", "Number of Peaks (<=5.0 RMSD)", 0, QApplication::UnicodeUTF8));
        textLabel4_2_2_2_2->setText(QApplication::translate("f2DockResult", "peaks in ranks [1, 1000]", 0, QApplication::UnicodeUTF8));
        textLabel4_2_2_2->setText(QApplication::translate("f2DockResult", "peaks in ranks [1, 100]", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("f2DockResult", "peaks in total", 0, QApplication::UnicodeUTF8));
        textLabel4_2_2->setText(QApplication::translate("f2DockResult", "peaks in ranks [1, 10]", 0, QApplication::UnicodeUTF8));
        textLabel4_2_2_2_2_2->setText(QApplication::translate("f2DockResult", "peaks in ranks [1, 10000]", 0, QApplication::UnicodeUTF8));
        textLabel4_2->setText(QApplication::translate("f2DockResult", "peaks in ranks [1, 1]", 0, QApplication::UnicodeUTF8));
        minRMSD->setTitle(QApplication::translate("f2DockResult", "Best Ranked Peak", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("f2DockResult", "Rank", 0, QApplication::UnicodeUTF8));
        textLabel5_2_2->setText(QApplication::translate("f2DockResult", "Score", 0, QApplication::UnicodeUTF8));
        textLabel5_2->setText(QApplication::translate("f2DockResult", "RMSD", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(f2DockResult);
    } // retranslateUi

};

namespace Ui {
    class f2DockResult: public Ui_f2DockResult {};
} // namespace Ui

QT_END_NAMESPACE

class f2DockResult : public QDialog, public Ui::f2DockResult
{
    Q_OBJECT

public:
    f2DockResult(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~f2DockResult();

public slots:
    virtual void detailsSlot();
    virtual void doneSlot();

protected slots:
    virtual void languageChange();

};

#endif // F2DOCKRESULT_H
