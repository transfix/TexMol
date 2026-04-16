#ifndef F2DOCKADV_H
#define F2DOCKADV_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_F2DockAdv
{
public:
    QPushButton *f2dAdvButton;
    QPushButton *f2dAdvResetButton;
    Q3GroupBox *skinGB;
    QCheckBox *SLS_CB;
    QLineEdit *PR_TB;
    QCheckBox *CWR_CB;
    QCheckBox *CWL_CB;
    QLineEdit *CWR_TB;
    QLabel *curvatureWRLabel_2;
    QLabel *curvatureWLLabel_2_2;
    QLabel *CWradLabel;
    QLabel *pseudoRadLabel;
    QLabel *skinLayerLabel;
    Q3GroupBox *fftGB;
    QCheckBox *NBF_CB;
    QCheckBox *SF_CB;
    QLabel *spfftLabel;
    QLabel *nbfftLabel;
    Q3GroupBox *rotGB;
    QCheckBox *RRI_CB;
    QCheckBox *RV_CB;
    QLineEdit *NPR_TB;
    QLabel *randRotLabel;
    QLabel *rotVolLabel;
    QLabel *peakLabel;
    Q3GroupBox *clashGB;
    QCheckBox *CF_CB;
    QLineEdit *CT_TB;
    QLineEdit *CW_TB;
    QLabel *clWeightLabel;
    QLabel *clCutoffLabel_2;
    QLabel *clApplyLabel_2;
    Q3GroupBox *miscGB;
    QCheckBox *MSC_CB;
    QLabel *miscFilterLabel;
    Q3GroupBox *vdwGB;
    QLabel *vdwCutoffLabel;
    QLabel *vdwGridLabel;
    QCheckBox *VDW_CB;
    QLineEdit *VDWC_TB;
    QLineEdit *VDWG_TB;
    QCheckBox *SVDW_CB;
    QLabel *vdwSurfLabel;
    QLabel *vdwApplyLabel;
    Q3GroupBox *hbondGB;
    QLineEdit *HBW_TB;
    QLineEdit *HBC_TB;
    QLabel *hbondLabel;
    QLabel *hbondCutoffLabel;

    void setupUi(QDialog *F2DockAdv)
    {
        if (F2DockAdv->objectName().isEmpty())
            F2DockAdv->setObjectName(QString::fromUtf8("F2DockAdv"));
        F2DockAdv->resize(859, 577);
        f2dAdvButton = new QPushButton(F2DockAdv);
        f2dAdvButton->setObjectName(QString::fromUtf8("f2dAdvButton"));
        f2dAdvButton->setGeometry(QRect(450, 511, 381, 40));
        QFont font;
        font.setPointSize(12);
        f2dAdvButton->setFont(font);
        f2dAdvResetButton = new QPushButton(F2DockAdv);
        f2dAdvResetButton->setObjectName(QString::fromUtf8("f2dAdvResetButton"));
        f2dAdvResetButton->setGeometry(QRect(450, 461, 381, 40));
        skinGB = new Q3GroupBox(F2DockAdv);
        skinGB->setObjectName(QString::fromUtf8("skinGB"));
        skinGB->setGeometry(QRect(20, 10, 390, 180));
        skinGB->setContentsMargins(0, 0, 0, 0);
        SLS_CB = new QCheckBox(skinGB);
        SLS_CB->setObjectName(QString::fromUtf8("SLS_CB"));
        SLS_CB->setGeometry(QRect(290, 20, 16, 20));
        PR_TB = new QLineEdit(skinGB);
        PR_TB->setObjectName(QString::fromUtf8("PR_TB"));
        PR_TB->setGeometry(QRect(290, 50, 70, 22));
        PR_TB->setAlignment(Qt::AlignRight);
        CWR_CB = new QCheckBox(skinGB);
        CWR_CB->setObjectName(QString::fromUtf8("CWR_CB"));
        CWR_CB->setGeometry(QRect(290, 80, 16, 20));
        CWR_CB->setChecked(true);
        CWL_CB = new QCheckBox(skinGB);
        CWL_CB->setObjectName(QString::fromUtf8("CWL_CB"));
        CWL_CB->setGeometry(QRect(290, 110, 16, 20));
        CWR_TB = new QLineEdit(skinGB);
        CWR_TB->setObjectName(QString::fromUtf8("CWR_TB"));
        CWR_TB->setGeometry(QRect(290, 140, 70, 22));
        CWR_TB->setAlignment(Qt::AlignRight);
        curvatureWRLabel_2 = new QLabel(skinGB);
        curvatureWRLabel_2->setObjectName(QString::fromUtf8("curvatureWRLabel_2"));
        curvatureWRLabel_2->setGeometry(QRect(20, 80, 260, 20));
        curvatureWRLabel_2->setWordWrap(false);
        curvatureWLLabel_2_2 = new QLabel(skinGB);
        curvatureWLLabel_2_2->setObjectName(QString::fromUtf8("curvatureWLLabel_2_2"));
        curvatureWLLabel_2_2->setGeometry(QRect(17, 110, 260, 20));
        curvatureWLLabel_2_2->setWordWrap(false);
        CWradLabel = new QLabel(skinGB);
        CWradLabel->setObjectName(QString::fromUtf8("CWradLabel"));
        CWradLabel->setGeometry(QRect(17, 140, 260, 20));
        CWradLabel->setWordWrap(false);
        pseudoRadLabel = new QLabel(skinGB);
        pseudoRadLabel->setObjectName(QString::fromUtf8("pseudoRadLabel"));
        pseudoRadLabel->setGeometry(QRect(17, 50, 250, 20));
        pseudoRadLabel->setWordWrap(false);
        skinLayerLabel = new QLabel(skinGB);
        skinLayerLabel->setObjectName(QString::fromUtf8("skinLayerLabel"));
        skinLayerLabel->setGeometry(QRect(17, 20, 250, 20));
        skinLayerLabel->setWordWrap(false);
        fftGB = new Q3GroupBox(F2DockAdv);
        fftGB->setObjectName(QString::fromUtf8("fftGB"));
        fftGB->setGeometry(QRect(20, 210, 390, 90));
        fftGB->setContentsMargins(0, 0, 0, 0);
        NBF_CB = new QCheckBox(fftGB);
        NBF_CB->setObjectName(QString::fromUtf8("NBF_CB"));
        NBF_CB->setGeometry(QRect(290, 20, 16, 20));
        NBF_CB->setChecked(true);
        SF_CB = new QCheckBox(fftGB);
        SF_CB->setObjectName(QString::fromUtf8("SF_CB"));
        SF_CB->setGeometry(QRect(290, 50, 16, 20));
        SF_CB->setChecked(true);
        spfftLabel = new QLabel(fftGB);
        spfftLabel->setObjectName(QString::fromUtf8("spfftLabel"));
        spfftLabel->setGeometry(QRect(13, 50, 240, 20));
        spfftLabel->setWordWrap(false);
        nbfftLabel = new QLabel(fftGB);
        nbfftLabel->setObjectName(QString::fromUtf8("nbfftLabel"));
        nbfftLabel->setGeometry(QRect(13, 20, 220, 20));
        nbfftLabel->setWordWrap(false);
        rotGB = new Q3GroupBox(F2DockAdv);
        rotGB->setObjectName(QString::fromUtf8("rotGB"));
        rotGB->setGeometry(QRect(20, 320, 390, 120));
        rotGB->setContentsMargins(0, 0, 0, 0);
        RRI_CB = new QCheckBox(rotGB);
        RRI_CB->setObjectName(QString::fromUtf8("RRI_CB"));
        RRI_CB->setGeometry(QRect(290, 20, 16, 20));
        RV_CB = new QCheckBox(rotGB);
        RV_CB->setObjectName(QString::fromUtf8("RV_CB"));
        RV_CB->setGeometry(QRect(290, 50, 16, 20));
        RV_CB->setChecked(true);
        NPR_TB = new QLineEdit(rotGB);
        NPR_TB->setObjectName(QString::fromUtf8("NPR_TB"));
        NPR_TB->setGeometry(QRect(290, 78, 70, 22));
        NPR_TB->setAlignment(Qt::AlignRight);
        randRotLabel = new QLabel(rotGB);
        randRotLabel->setObjectName(QString::fromUtf8("randRotLabel"));
        randRotLabel->setGeometry(QRect(13, 20, 260, 20));
        randRotLabel->setWordWrap(false);
        rotVolLabel = new QLabel(rotGB);
        rotVolLabel->setObjectName(QString::fromUtf8("rotVolLabel"));
        rotVolLabel->setGeometry(QRect(13, 50, 250, 20));
        rotVolLabel->setWordWrap(false);
        peakLabel = new QLabel(rotGB);
        peakLabel->setObjectName(QString::fromUtf8("peakLabel"));
        peakLabel->setGeometry(QRect(13, 80, 250, 20));
        peakLabel->setWordWrap(false);
        clashGB = new Q3GroupBox(F2DockAdv);
        clashGB->setObjectName(QString::fromUtf8("clashGB"));
        clashGB->setGeometry(QRect(450, 180, 390, 130));
        clashGB->setFrameShape(Q3GroupBox::GroupBoxPanel);
        clashGB->setFrameShadow(Q3GroupBox::Sunken);
        clashGB->setContentsMargins(0, 0, 0, 0);
        CF_CB = new QCheckBox(clashGB);
        CF_CB->setObjectName(QString::fromUtf8("CF_CB"));
        CF_CB->setGeometry(QRect(290, 20, 16, 20));
        CT_TB = new QLineEdit(clashGB);
        CT_TB->setObjectName(QString::fromUtf8("CT_TB"));
        CT_TB->setGeometry(QRect(290, 50, 70, 22));
        CT_TB->setAlignment(Qt::AlignRight);
        CW_TB = new QLineEdit(clashGB);
        CW_TB->setObjectName(QString::fromUtf8("CW_TB"));
        CW_TB->setGeometry(QRect(290, 80, 70, 22));
        CW_TB->setAlignment(Qt::AlignRight);
        clWeightLabel = new QLabel(clashGB);
        clWeightLabel->setObjectName(QString::fromUtf8("clWeightLabel"));
        clWeightLabel->setGeometry(QRect(17, 80, 210, 20));
        clWeightLabel->setWordWrap(false);
        clCutoffLabel_2 = new QLabel(clashGB);
        clCutoffLabel_2->setObjectName(QString::fromUtf8("clCutoffLabel_2"));
        clCutoffLabel_2->setGeometry(QRect(17, 50, 250, 20));
        clCutoffLabel_2->setWordWrap(false);
        clApplyLabel_2 = new QLabel(clashGB);
        clApplyLabel_2->setObjectName(QString::fromUtf8("clApplyLabel_2"));
        clApplyLabel_2->setGeometry(QRect(17, 20, 230, 20));
        clApplyLabel_2->setWordWrap(false);
        miscGB = new Q3GroupBox(F2DockAdv);
        miscGB->setObjectName(QString::fromUtf8("miscGB"));
        miscGB->setGeometry(QRect(450, 330, 390, 110));
        miscGB->setContentsMargins(0, 0, 0, 0);
        MSC_CB = new QCheckBox(miscGB);
        MSC_CB->setObjectName(QString::fromUtf8("MSC_CB"));
        MSC_CB->setGeometry(QRect(290, 20, 16, 20));
        MSC_CB->setChecked(true);
        miscFilterLabel = new QLabel(miscGB);
        miscFilterLabel->setObjectName(QString::fromUtf8("miscFilterLabel"));
        miscFilterLabel->setGeometry(QRect(17, 20, 250, 20));
        miscFilterLabel->setWordWrap(false);
        vdwGB = new Q3GroupBox(F2DockAdv);
        vdwGB->setObjectName(QString::fromUtf8("vdwGB"));
        vdwGB->setGeometry(QRect(450, 10, 390, 150));
        vdwGB->setContentsMargins(0, 0, 0, 0);
        vdwCutoffLabel = new QLabel(vdwGB);
        vdwCutoffLabel->setObjectName(QString::fromUtf8("vdwCutoffLabel"));
        vdwCutoffLabel->setGeometry(QRect(17, 50, 124, 20));
        vdwCutoffLabel->setWordWrap(false);
        vdwGridLabel = new QLabel(vdwGB);
        vdwGridLabel->setObjectName(QString::fromUtf8("vdwGridLabel"));
        vdwGridLabel->setGeometry(QRect(17, 80, 210, 20));
        vdwGridLabel->setWordWrap(false);
        VDW_CB = new QCheckBox(vdwGB);
        VDW_CB->setObjectName(QString::fromUtf8("VDW_CB"));
        VDW_CB->setGeometry(QRect(290, 20, 16, 20));
        VDWC_TB = new QLineEdit(vdwGB);
        VDWC_TB->setObjectName(QString::fromUtf8("VDWC_TB"));
        VDWC_TB->setGeometry(QRect(290, 50, 70, 22));
        VDWC_TB->setAlignment(Qt::AlignRight);
        VDWG_TB = new QLineEdit(vdwGB);
        VDWG_TB->setObjectName(QString::fromUtf8("VDWG_TB"));
        VDWG_TB->setGeometry(QRect(290, 80, 70, 22));
        VDWG_TB->setAlignment(Qt::AlignRight);
        SVDW_CB = new QCheckBox(vdwGB);
        SVDW_CB->setObjectName(QString::fromUtf8("SVDW_CB"));
        SVDW_CB->setGeometry(QRect(290, 110, 16, 20));
        vdwSurfLabel = new QLabel(vdwGB);
        vdwSurfLabel->setObjectName(QString::fromUtf8("vdwSurfLabel"));
        vdwSurfLabel->setGeometry(QRect(17, 110, 260, 20));
        vdwSurfLabel->setWordWrap(false);
        vdwApplyLabel = new QLabel(vdwGB);
        vdwApplyLabel->setObjectName(QString::fromUtf8("vdwApplyLabel"));
        vdwApplyLabel->setGeometry(QRect(17, 20, 240, 20));
        vdwApplyLabel->setWordWrap(false);
        hbondGB = new Q3GroupBox(F2DockAdv);
        hbondGB->setObjectName(QString::fromUtf8("hbondGB"));
        hbondGB->setGeometry(QRect(20, 460, 390, 90));
        hbondGB->setContentsMargins(0, 0, 0, 0);
        HBW_TB = new QLineEdit(hbondGB);
        HBW_TB->setObjectName(QString::fromUtf8("HBW_TB"));
        HBW_TB->setGeometry(QRect(290, 20, 70, 22));
        HBW_TB->setAlignment(Qt::AlignRight);
        HBC_TB = new QLineEdit(hbondGB);
        HBC_TB->setObjectName(QString::fromUtf8("HBC_TB"));
        HBC_TB->setGeometry(QRect(290, 50, 70, 22));
        HBC_TB->setAlignment(Qt::AlignRight);
        hbondLabel = new QLabel(hbondGB);
        hbondLabel->setObjectName(QString::fromUtf8("hbondLabel"));
        hbondLabel->setGeometry(QRect(17, 20, 250, 20));
        hbondLabel->setWordWrap(false);
        hbondCutoffLabel = new QLabel(hbondGB);
        hbondCutoffLabel->setObjectName(QString::fromUtf8("hbondCutoffLabel"));
        hbondCutoffLabel->setGeometry(QRect(17, 50, 250, 20));
        hbondCutoffLabel->setWordWrap(false);
        QWidget::setTabOrder(SLS_CB, PR_TB);
        QWidget::setTabOrder(PR_TB, CWR_CB);
        QWidget::setTabOrder(CWR_CB, CWL_CB);
        QWidget::setTabOrder(CWL_CB, CWR_TB);
        QWidget::setTabOrder(CWR_TB, NBF_CB);
        QWidget::setTabOrder(NBF_CB, SF_CB);
        QWidget::setTabOrder(SF_CB, RRI_CB);
        QWidget::setTabOrder(RRI_CB, RV_CB);
        QWidget::setTabOrder(RV_CB, NPR_TB);
        QWidget::setTabOrder(NPR_TB, HBW_TB);
        QWidget::setTabOrder(HBW_TB, HBC_TB);
        QWidget::setTabOrder(HBC_TB, VDW_CB);
        QWidget::setTabOrder(VDW_CB, VDWC_TB);
        QWidget::setTabOrder(VDWC_TB, VDWG_TB);
        QWidget::setTabOrder(VDWG_TB, SVDW_CB);
        QWidget::setTabOrder(SVDW_CB, CF_CB);
        QWidget::setTabOrder(CF_CB, CT_TB);
        QWidget::setTabOrder(CT_TB, CW_TB);
        QWidget::setTabOrder(CW_TB, MSC_CB);
        QWidget::setTabOrder(MSC_CB, f2dAdvResetButton);
        QWidget::setTabOrder(f2dAdvResetButton, f2dAdvButton);

        retranslateUi(F2DockAdv);
        QObject::connect(f2dAdvButton, SIGNAL(clicked()), F2DockAdv, SLOT(saveSlot()));
        QObject::connect(f2dAdvResetButton, SIGNAL(clicked()), F2DockAdv, SLOT(defaultSlot()));

        QMetaObject::connectSlotsByName(F2DockAdv);
    } // setupUi

    void retranslateUi(QDialog *F2DockAdv)
    {
        F2DockAdv->setWindowTitle(QApplication::translate("F2DockAdv", "Advanced Options", 0, QApplication::UnicodeUTF8));
        f2dAdvButton->setText(QApplication::translate("F2DockAdv", "Save", 0, QApplication::UnicodeUTF8));
        f2dAdvResetButton->setText(QApplication::translate("F2DockAdv", "Reset to default", 0, QApplication::UnicodeUTF8));
        skinGB->setTitle(QApplication::translate("F2DockAdv", "Skin", 0, QApplication::UnicodeUTF8));
        SLS_CB->setText(QString());
        PR_TB->setText(QApplication::translate("F2DockAdv", "1.1", 0, QApplication::UnicodeUTF8));
        CWR_CB->setText(QString());
        CWL_CB->setText(QString());
        CWR_TB->setText(QApplication::translate("F2DockAdv", "4.5", 0, QApplication::UnicodeUTF8));
        curvatureWRLabel_2->setText(QApplication::translate("F2DockAdv", "Curvature Weighted Receptor skin", 0, QApplication::UnicodeUTF8));
        curvatureWLLabel_2_2->setText(QApplication::translate("F2DockAdv", "Curvature Weighted Ligand skin", 0, QApplication::UnicodeUTF8));
        CWradLabel->setText(QApplication::translate("F2DockAdv", "Curvature Weighting Radius", 0, QApplication::UnicodeUTF8));
        pseudoRadLabel->setText(QApplication::translate("F2DockAdv", "PseudoAtom Radius", 0, QApplication::UnicodeUTF8));
        skinLayerLabel->setText(QApplication::translate("F2DockAdv", "Single Layered Ligand Skin", 0, QApplication::UnicodeUTF8));
        fftGB->setTitle(QApplication::translate("F2DockAdv", "FFT", 0, QApplication::UnicodeUTF8));
        NBF_CB->setText(QString());
        SF_CB->setText(QString());
        spfftLabel->setText(QApplication::translate("F2DockAdv", "Use Sparse FFT", 0, QApplication::UnicodeUTF8));
        nbfftLabel->setText(QApplication::translate("F2DockAdv", "Use Narrow Band", 0, QApplication::UnicodeUTF8));
        rotGB->setTitle(QApplication::translate("F2DockAdv", "Rotation", 0, QApplication::UnicodeUTF8));
        RRI_CB->setText(QString());
        RV_CB->setText(QString());
        NPR_TB->setText(QApplication::translate("F2DockAdv", "1", 0, QApplication::UnicodeUTF8));
        randRotLabel->setText(QApplication::translate("F2DockAdv", "Apply Random Rotation Initially", 0, QApplication::UnicodeUTF8));
        rotVolLabel->setText(QApplication::translate("F2DockAdv", "Rotate Volume", 0, QApplication::UnicodeUTF8));
        peakLabel->setText(QApplication::translate("F2DockAdv", "number of Peaks per Rotation", 0, QApplication::UnicodeUTF8));
        clashGB->setTitle(QApplication::translate("F2DockAdv", "Clash Filtering", 0, QApplication::UnicodeUTF8));
        CF_CB->setText(QString());
        CT_TB->setText(QApplication::translate("F2DockAdv", "6", 0, QApplication::UnicodeUTF8));
        CW_TB->setText(QApplication::translate("F2DockAdv", "0", 0, QApplication::UnicodeUTF8));
        clWeightLabel->setText(QApplication::translate("F2DockAdv", "Clash Weight", 0, QApplication::UnicodeUTF8));
        clCutoffLabel_2->setText(QApplication::translate("F2DockAdv", "Clash Tolerance", 0, QApplication::UnicodeUTF8));
        clApplyLabel_2->setText(QApplication::translate("F2DockAdv", "Apply clash Filter", 0, QApplication::UnicodeUTF8));
        miscGB->setTitle(QApplication::translate("F2DockAdv", "Misc", 0, QApplication::UnicodeUTF8));
        MSC_CB->setText(QString());
        miscFilterLabel->setText(QApplication::translate("F2DockAdv", "Apply Miscellaneous Filter", 0, QApplication::UnicodeUTF8));
        vdwGB->setTitle(QApplication::translate("F2DockAdv", "van der Waals Filtering", 0, QApplication::UnicodeUTF8));
        vdwCutoffLabel->setText(QApplication::translate("F2DockAdv", "Cutoff", 0, QApplication::UnicodeUTF8));
        vdwGridLabel->setText(QApplication::translate("F2DockAdv", "Grid Size", 0, QApplication::UnicodeUTF8));
        VDW_CB->setText(QString());
        VDWC_TB->setText(QApplication::translate("F2DockAdv", "100000", 0, QApplication::UnicodeUTF8));
        VDWG_TB->setText(QApplication::translate("F2DockAdv", "512", 0, QApplication::UnicodeUTF8));
        SVDW_CB->setText(QString());
        vdwSurfLabel->setText(QApplication::translate("F2DockAdv", "Use surface based van der Waals", 0, QApplication::UnicodeUTF8));
        vdwApplyLabel->setText(QApplication::translate("F2DockAdv", "Apply van der Waals Filter", 0, QApplication::UnicodeUTF8));
        hbondGB->setTitle(QApplication::translate("F2DockAdv", "Hydrogen Bond", 0, QApplication::UnicodeUTF8));
        HBW_TB->setText(QApplication::translate("F2DockAdv", "0", 0, QApplication::UnicodeUTF8));
        HBC_TB->setText(QApplication::translate("F2DockAdv", "2.0", 0, QApplication::UnicodeUTF8));
        hbondLabel->setText(QApplication::translate("F2DockAdv", "Hbond Weight", 0, QApplication::UnicodeUTF8));
        hbondCutoffLabel->setText(QApplication::translate("F2DockAdv", "Hbond Distance Cutoff", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(F2DockAdv);
    } // retranslateUi

};

namespace Ui {
    class F2DockAdv: public Ui_F2DockAdv {};
} // namespace Ui

QT_END_NAMESPACE

class F2DockAdv : public QDialog, public Ui::F2DockAdv
{
    Q_OBJECT

public:
    F2DockAdv(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~F2DockAdv();

public slots:
    virtual void saveSlot();
    virtual void defaultSlot();

protected slots:
    virtual void languageChange();

};

#endif // F2DOCKADV_H
