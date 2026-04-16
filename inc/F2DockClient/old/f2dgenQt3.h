#ifndef F2DGEN_H
#define F2DGEN_H

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

class Ui_f2dGen
{
public:
    Q3GroupBox *fileGB;
    QLabel *receptorFileLabel;
    QLabel *ligandFileLabel;
    QLineEdit *RF_TB;
    QLineEdit *LF_TB;
    QPushButton *receptorFileButton;
    QPushButton *ligadFileButton;
    QPushButton *genButton;

    void setupUi(QDialog *f2dGen)
    {
        if (f2dGen->objectName().isEmpty())
            f2dGen->setObjectName(QString::fromUtf8("f2dGen"));
        f2dGen->resize(670, 179);
        fileGB = new Q3GroupBox(f2dGen);
        fileGB->setObjectName(QString::fromUtf8("fileGB"));
        fileGB->setGeometry(QRect(20, 10, 630, 100));
        receptorFileLabel = new QLabel(fileGB);
        receptorFileLabel->setObjectName(QString::fromUtf8("receptorFileLabel"));
        receptorFileLabel->setGeometry(QRect(10, 20, 150, 20));
        receptorFileLabel->setWordWrap(false);
        ligandFileLabel = new QLabel(fileGB);
        ligandFileLabel->setObjectName(QString::fromUtf8("ligandFileLabel"));
        ligandFileLabel->setGeometry(QRect(10, 60, 150, 20));
        ligandFileLabel->setWordWrap(false);
        RF_TB = new QLineEdit(fileGB);
        RF_TB->setObjectName(QString::fromUtf8("RF_TB"));
        RF_TB->setGeometry(QRect(170, 20, 330, 22));
        LF_TB = new QLineEdit(fileGB);
        LF_TB->setObjectName(QString::fromUtf8("LF_TB"));
        LF_TB->setGeometry(QRect(170, 60, 330, 22));
        receptorFileButton = new QPushButton(fileGB);
        receptorFileButton->setObjectName(QString::fromUtf8("receptorFileButton"));
        receptorFileButton->setGeometry(QRect(510, 20, 100, 24));
        ligadFileButton = new QPushButton(fileGB);
        ligadFileButton->setObjectName(QString::fromUtf8("ligadFileButton"));
        ligadFileButton->setGeometry(QRect(510, 60, 100, 24));
        genButton = new QPushButton(f2dGen);
        genButton->setObjectName(QString::fromUtf8("genButton"));
        genButton->setGeometry(QRect(20, 120, 631, 41));

        retranslateUi(f2dGen);
        QObject::connect(genButton, SIGNAL(clicked()), f2dGen, SLOT(genButtonSlot()));
        QObject::connect(receptorFileButton, SIGNAL(clicked()), f2dGen, SLOT(rfButtonSlot()));
        QObject::connect(ligadFileButton, SIGNAL(clicked()), f2dGen, SLOT(lfButtonSlot()));

        QMetaObject::connectSlotsByName(f2dGen);
    } // setupUi

    void retranslateUi(QDialog *f2dGen)
    {
        f2dGen->setWindowTitle(QApplication::translate("f2dGen", "Generate f2d Files", 0, QApplication::UnicodeUTF8));
        fileGB->setTitle(QApplication::translate("f2dGen", "Files", 0, QApplication::UnicodeUTF8));
        receptorFileLabel->setText(QApplication::translate("f2dGen", "Receptor File (pdb)", 0, QApplication::UnicodeUTF8));
        ligandFileLabel->setText(QApplication::translate("f2dGen", "Ligand File (pdb)", 0, QApplication::UnicodeUTF8));
        receptorFileButton->setText(QApplication::translate("f2dGen", "Browse", 0, QApplication::UnicodeUTF8));
        ligadFileButton->setText(QApplication::translate("f2dGen", "Browse", 0, QApplication::UnicodeUTF8));
        genButton->setText(QApplication::translate("f2dGen", "Generate f2ds", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(f2dGen);
    } // retranslateUi

};

namespace Ui {
    class f2dGen: public Ui_f2dGen {};
} // namespace Ui

QT_END_NAMESPACE

class f2dGen : public QDialog, public Ui::f2dGen
{
    Q_OBJECT

public:
    f2dGen(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
    ~f2dGen();

public slots:
    virtual void rfButtonSlot();
    virtual void lfButtonSlot();
    virtual void genButtonSlot();

protected slots:
    virtual void languageChange();

};

#endif // F2DGEN_H
