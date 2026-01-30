#ifndef F2DOCKMASTER_H
#define F2DOCKMASTER_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3Table>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_f2dockmaster
{
public:
    Q3GroupBox *submitGB;
    QPushButton *quadgenButton;
    QPushButton *f2dgenButton;
    QPushButton *dockButton;
    QPushButton *rerankButton;
    Q3GroupBox *currentGB;
    QPushButton *detailButton;
    QPushButton *resultButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    Q3Table *jobTable;

    void setupUi(QDialog *f2dockmaster)
    {
        if (f2dockmaster->objectName().isEmpty())
            f2dockmaster->setObjectName(QString::fromUtf8("f2dockmaster"));
        f2dockmaster->resize(840, 458);
        submitGB = new Q3GroupBox(f2dockmaster);
        submitGB->setObjectName(QString::fromUtf8("submitGB"));
        submitGB->setGeometry(QRect(20, 10, 800, 140));
        quadgenButton = new QPushButton(submitGB);
        quadgenButton->setObjectName(QString::fromUtf8("quadgenButton"));
        quadgenButton->setGeometry(QRect(419, 80, 360, 41));
        f2dgenButton = new QPushButton(submitGB);
        f2dgenButton->setObjectName(QString::fromUtf8("f2dgenButton"));
        f2dgenButton->setGeometry(QRect(419, 30, 360, 41));
        dockButton = new QPushButton(submitGB);
        dockButton->setObjectName(QString::fromUtf8("dockButton"));
        dockButton->setGeometry(QRect(19, 30, 360, 41));
        rerankButton = new QPushButton(submitGB);
        rerankButton->setObjectName(QString::fromUtf8("rerankButton"));
        rerankButton->setGeometry(QRect(19, 80, 360, 41));
        currentGB = new Q3GroupBox(f2dockmaster);
        currentGB->setObjectName(QString::fromUtf8("currentGB"));
        currentGB->setGeometry(QRect(20, 160, 800, 280));
        detailButton = new QPushButton(currentGB);
        detailButton->setObjectName(QString::fromUtf8("detailButton"));
        detailButton->setGeometry(QRect(630, 30, 150, 50));
        resultButton = new QPushButton(currentGB);
        resultButton->setObjectName(QString::fromUtf8("resultButton"));
        resultButton->setGeometry(QRect(630, 90, 150, 50));
        saveButton = new QPushButton(currentGB);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(630, 150, 151, 50));
        loadButton = new QPushButton(currentGB);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setGeometry(QRect(631, 210, 150, 51));
        jobTable = new Q3Table(currentGB);
        jobTable->setObjectName(QString::fromUtf8("jobTable"));
        jobTable->setGeometry(QRect(20, 30, 590, 230));
        jobTable->setNumRows(1);
        jobTable->setNumCols(3);
        jobTable->setReadOnly(true);
        jobTable->setSorting(false);
        jobTable->setSelectionMode(Q3Table::SingleRow);
        jobTable->setFocusStyle(Q3Table::SpreadSheet);

        retranslateUi(f2dockmaster);
        QObject::connect(detailButton, SIGNAL(clicked()), f2dockmaster, SLOT(jobdetSlot()));
        QObject::connect(dockButton, SIGNAL(clicked()), f2dockmaster, SLOT(dockSlot()));
        QObject::connect(f2dgenButton, SIGNAL(clicked()), f2dockmaster, SLOT(f2dgenSlot()));
        QObject::connect(quadgenButton, SIGNAL(clicked()), f2dockmaster, SLOT(quadgenSlot()));
        QObject::connect(rerankButton, SIGNAL(clicked()), f2dockmaster, SLOT(rerankSlot()));
        QObject::connect(resultButton, SIGNAL(clicked()), f2dockmaster, SLOT(resultSlot()));
        QObject::connect(jobTable, SIGNAL(selectionChanged()), f2dockmaster, SLOT(selectSlot()));
        QObject::connect(saveButton, SIGNAL(clicked()), f2dockmaster, SLOT(saveSlot()));
        QObject::connect(loadButton, SIGNAL(clicked()), f2dockmaster, SLOT(loadSlot()));

        QMetaObject::connectSlotsByName(f2dockmaster);
    } // setupUi

    void retranslateUi(QDialog *f2dockmaster)
    {
        f2dockmaster->setWindowTitle(QApplication::translate("f2dockmaster", "F2Dock Client", 0, QApplication::UnicodeUTF8));
        submitGB->setTitle(QApplication::translate("f2dockmaster", "Submit Jobs", 0, QApplication::UnicodeUTF8));
        quadgenButton->setText(QApplication::translate("f2dockmaster", "Submit New Quad File Generation Job", 0, QApplication::UnicodeUTF8));
        f2dgenButton->setText(QApplication::translate("f2dockmaster", "Submit New F2d File Generation Job", 0, QApplication::UnicodeUTF8));
        dockButton->setText(QApplication::translate("f2dockmaster", "Submit New Docking Job", 0, QApplication::UnicodeUTF8));
        rerankButton->setText(QApplication::translate("f2dockmaster", "Submit New Reranking Job", 0, QApplication::UnicodeUTF8));
        currentGB->setTitle(QApplication::translate("f2dockmaster", "Active Jobs", 0, QApplication::UnicodeUTF8));
        detailButton->setText(QApplication::translate("f2dockmaster", "View Job details", 0, QApplication::UnicodeUTF8));
        resultButton->setText(QApplication::translate("f2dockmaster", "View Results", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("f2dockmaster", "Save Job", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("f2dockmaster", "Load Job", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(f2dockmaster);
    } // retranslateUi

};

namespace Ui {
    class f2dockmaster: public Ui_f2dockmaster {};
} // namespace Ui

QT_END_NAMESPACE

class f2dockmaster : public QDialog, public Ui::f2dockmaster
{
    Q_OBJECT

public:
    f2dockmaster(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~f2dockmaster();

public slots:
    virtual void dockSlot();
    virtual void f2dgenSlot();
    virtual void rerankSlot();
    virtual void quadgenSlot();
    virtual void jobdetSlot();
    virtual void resultSlot();
    virtual void selectSlot();
    virtual void saveSlot();
    virtual void loadSlot();

protected slots:
    virtual void languageChange();

};

#endif // F2DOCKMASTER_H
