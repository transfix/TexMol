#ifndef QUADGEN_H
#define QUADGEN_H

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
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_quadGen
{
public:
    Q3GroupBox *fileGB;
    QLabel *ligandFileLabel;
    QLabel *receptorFileLabel;
    QPushButton *receptorFileButton;
    QPushButton *ligadFileButton;
    QLineEdit *RF_TB;
    QLineEdit *LF_TB;
    QRadioButton *fineRB;
    QPushButton *submit_Button;
    QRadioButton *coarseRB;

    void setupUi(QDialog *quadGen)
    {
        if (quadGen->objectName().isEmpty())
            quadGen->setObjectName(QString::fromUtf8("quadGen"));
        quadGen->resize(652, 249);
        fileGB = new Q3GroupBox(quadGen);
        fileGB->setObjectName(QString::fromUtf8("fileGB"));
        fileGB->setGeometry(QRect(10, 70, 630, 100));
        fileGB->setFrameShape(Q3GroupBox::GroupBoxPanel);
        fileGB->setFrameShadow(Q3GroupBox::Sunken);
        ligandFileLabel = new QLabel(fileGB);
        ligandFileLabel->setObjectName(QString::fromUtf8("ligandFileLabel"));
        ligandFileLabel->setGeometry(QRect(10, 60, 150, 20));
        ligandFileLabel->setWordWrap(false);
        receptorFileLabel = new QLabel(fileGB);
        receptorFileLabel->setObjectName(QString::fromUtf8("receptorFileLabel"));
        receptorFileLabel->setGeometry(QRect(10, 20, 150, 20));
        receptorFileLabel->setWordWrap(false);
        receptorFileButton = new QPushButton(fileGB);
        receptorFileButton->setObjectName(QString::fromUtf8("receptorFileButton"));
        receptorFileButton->setGeometry(QRect(510, 20, 100, 24));
        ligadFileButton = new QPushButton(fileGB);
        ligadFileButton->setObjectName(QString::fromUtf8("ligadFileButton"));
        ligadFileButton->setGeometry(QRect(510, 60, 100, 24));
        RF_TB = new QLineEdit(fileGB);
        RF_TB->setObjectName(QString::fromUtf8("RF_TB"));
        RF_TB->setGeometry(QRect(170, 20, 330, 22));
        LF_TB = new QLineEdit(fileGB);
        LF_TB->setObjectName(QString::fromUtf8("LF_TB"));
        LF_TB->setGeometry(QRect(170, 60, 330, 22));
        fineRB = new QRadioButton(quadGen);
        fineRB->setObjectName(QString::fromUtf8("fineRB"));
        fineRB->setGeometry(QRect(330, 30, 240, 20));
        QFont font;
        font.setPointSize(11);
        fineRB->setFont(font);
        fineRB->setChecked(true);
        submit_Button = new QPushButton(quadGen);
        submit_Button->setObjectName(QString::fromUtf8("submit_Button"));
        submit_Button->setGeometry(QRect(10, 190, 630, 40));
        coarseRB = new QRadioButton(quadGen);
        coarseRB->setObjectName(QString::fromUtf8("coarseRB"));
        coarseRB->setGeometry(QRect(30, 30, 220, 20));
        coarseRB->setFont(font);

        retranslateUi(quadGen);
        QObject::connect(ligadFileButton, SIGNAL(clicked()), quadGen, SLOT(lfButtonSlot()));
        QObject::connect(receptorFileButton, SIGNAL(clicked()), quadGen, SLOT(rfButtonSlot()));
        QObject::connect(submit_Button, SIGNAL(clicked()), quadGen, SLOT(genButtonSlot()));
        QObject::connect(fineRB, SIGNAL(clicked()), quadGen, SLOT(fineSlot()));
        QObject::connect(coarseRB, SIGNAL(clicked()), quadGen, SLOT(coarseSlot()));

        QMetaObject::connectSlotsByName(quadGen);
    } // setupUi

    void retranslateUi(QDialog *quadGen)
    {
        quadGen->setWindowTitle(QApplication::translate("quadGen", "Generate Quad Files from pdbs", 0, QApplication::UnicodeUTF8));
        fileGB->setTitle(QApplication::translate("quadGen", "Files", 0, QApplication::UnicodeUTF8));
        ligandFileLabel->setText(QApplication::translate("quadGen", "Ligand File (pdb)", 0, QApplication::UnicodeUTF8));
        receptorFileLabel->setText(QApplication::translate("quadGen", "Receptor File (pdb)", 0, QApplication::UnicodeUTF8));
        receptorFileButton->setText(QApplication::translate("quadGen", "Browse", 0, QApplication::UnicodeUTF8));
        ligadFileButton->setText(QApplication::translate("quadGen", "Browse", 0, QApplication::UnicodeUTF8));
        fineRB->setText(QApplication::translate("quadGen", "Fine Sampling", 0, QApplication::UnicodeUTF8));
        submit_Button->setText(QApplication::translate("quadGen", "Submit Job", 0, QApplication::UnicodeUTF8));
        coarseRB->setText(QApplication::translate("quadGen", "Coarse Sampling", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(quadGen);
    } // retranslateUi

};

namespace Ui {
    class quadGen: public Ui_quadGen {};
} // namespace Ui

QT_END_NAMESPACE

class quadGen : public QDialog, public Ui::quadGen
{
    Q_OBJECT

public:
    quadGen(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~quadGen();

public slots:
    virtual void rfButtonSlot();
    virtual void lfButtonSlot();
    virtual void genButtonSlot();
    virtual void fineSlot();
    virtual void coarseSlot();

protected slots:
    virtual void languageChange();

};

#endif // QUADGEN_H
