#ifndef F2DOCKRESULTDETAILS_H
#define F2DOCKRESULTDETAILS_H

#include <qvariant.h>


#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3Table>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_f2dockresultdetails
{
public:
    Q3Table *f2dResultsTable;
    QPushButton *view_Button;
    QPushButton *done_Button;
    QCheckBox *surfaceBox;

    void setupUi(QDialog *f2dockresultdetails)
    {
        if (f2dockresultdetails->objectName().isEmpty())
            f2dockresultdetails->setObjectName(QString::fromUtf8("f2dockresultdetails"));
        f2dockresultdetails->resize(1152, 650);
        f2dResultsTable = new Q3Table(f2dockresultdetails);
        f2dResultsTable->setObjectName(QString::fromUtf8("f2dResultsTable"));
        f2dResultsTable->setGeometry(QRect(10, 10, 1130, 580));
        f2dResultsTable->setNumRows(0);
        f2dResultsTable->setNumCols(11);
        f2dResultsTable->setReadOnly(false);
        f2dResultsTable->setSorting(true);
        f2dResultsTable->setSelectionMode(Q3Table::MultiRow);
        view_Button = new QPushButton(f2dockresultdetails);
        view_Button->setObjectName(QString::fromUtf8("view_Button"));
        view_Button->setGeometry(QRect(10, 600, 490, 41));
        done_Button = new QPushButton(f2dockresultdetails);
        done_Button->setObjectName(QString::fromUtf8("done_Button"));
        done_Button->setGeometry(QRect(510, 600, 490, 41));
        surfaceBox = new QCheckBox(f2dockresultdetails);
        surfaceBox->setObjectName(QString::fromUtf8("surfaceBox"));
        surfaceBox->setGeometry(QRect(1010, 600, 130, 41));

        retranslateUi(f2dockresultdetails);
        QObject::connect(done_Button, SIGNAL(clicked()), f2dockresultdetails, SLOT(doneSlot()));
        QObject::connect(view_Button, SIGNAL(clicked()), f2dockresultdetails, SLOT(viewSlot()));
        QObject::connect(f2dResultsTable, SIGNAL(clicked(int,int,int,QPoint)), f2dockresultdetails, SLOT(selectRowSlot()));
        QObject::connect(surfaceBox, SIGNAL(toggled(bool)), f2dockresultdetails, SLOT(surfaceSlot(bool)));

        QMetaObject::connectSlotsByName(f2dockresultdetails);
    } // setupUi

    void retranslateUi(QDialog *f2dockresultdetails)
    {
        f2dockresultdetails->setWindowTitle(QApplication::translate("f2dockresultdetails", "Result Details", 0, QApplication::UnicodeUTF8));
        view_Button->setText(QApplication::translate("f2dockresultdetails", "Save Complex", 0, QApplication::UnicodeUTF8));
        done_Button->setText(QApplication::translate("f2dockresultdetails", "Done", 0, QApplication::UnicodeUTF8));
        surfaceBox->setText(QApplication::translate("f2dockresultdetails", "Render Surface", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(f2dockresultdetails);
    } // retranslateUi

};

namespace Ui {
    class f2dockresultdetails: public Ui_f2dockresultdetails {};
} // namespace Ui

QT_END_NAMESPACE

class f2dockresultdetails : public QDialog, public Ui::f2dockresultdetails
{
    Q_OBJECT

public:
    f2dockresultdetails(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~f2dockresultdetails();

public slots:
    virtual void doneSlot();
    virtual void viewSlot();
    virtual void selectRowSlot();
    virtual void surfaceSlot(bool surface);

protected slots:
    virtual void languageChange();

};

#endif // F2DOCKRESULTDETAILS_H
