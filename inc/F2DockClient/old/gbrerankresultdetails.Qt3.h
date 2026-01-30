#ifndef GBRERANKRESULTDETAILS_H
#define GBRERANKRESULTDETAILS_H

#include <qvariant.h>


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

class Ui_gbrerankresultdetails
{
public:
    QPushButton *done_Button;
    Q3Table *gbResultsTable;
    QPushButton *view_Button;

    void setupUi(QDialog *gbrerankresultdetails)
    {
        if (gbrerankresultdetails->objectName().isEmpty())
            gbrerankresultdetails->setObjectName(QString::fromUtf8("gbrerankresultdetails"));
        gbrerankresultdetails->resize(1152, 649);
        done_Button = new QPushButton(gbrerankresultdetails);
        done_Button->setObjectName(QString::fromUtf8("done_Button"));
        done_Button->setGeometry(QRect(580, 600, 561, 41));
        gbResultsTable = new Q3Table(gbrerankresultdetails);
        gbResultsTable->setObjectName(QString::fromUtf8("gbResultsTable"));
        gbResultsTable->setGeometry(QRect(10, 10, 1130, 580));
        gbResultsTable->setNumRows(0);
        gbResultsTable->setNumCols(11);
        gbResultsTable->setReadOnly(false);
        gbResultsTable->setSorting(true);
        gbResultsTable->setSelectionMode(Q3Table::MultiRow);
        view_Button = new QPushButton(gbrerankresultdetails);
        view_Button->setObjectName(QString::fromUtf8("view_Button"));
        view_Button->setGeometry(QRect(10, 600, 561, 41));

        retranslateUi(gbrerankresultdetails);
        QObject::connect(done_Button, SIGNAL(clicked()), gbrerankresultdetails, SLOT(doneSlot()));
        QObject::connect(view_Button, SIGNAL(clicked()), gbrerankresultdetails, SLOT(viewSlot()));

        QMetaObject::connectSlotsByName(gbrerankresultdetails);
    } // setupUi

    void retranslateUi(QDialog *gbrerankresultdetails)
    {
        gbrerankresultdetails->setWindowTitle(QApplication::translate("gbrerankresultdetails", "Result Details", 0, QApplication::UnicodeUTF8));
        done_Button->setText(QApplication::translate("gbrerankresultdetails", "Done", 0, QApplication::UnicodeUTF8));
        view_Button->setText(QApplication::translate("gbrerankresultdetails", "Save Complex", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(gbrerankresultdetails);
    } // retranslateUi

};

namespace Ui {
    class gbrerankresultdetails: public Ui_gbrerankresultdetails {};
} // namespace Ui

QT_END_NAMESPACE

class gbrerankresultdetails : public QDialog, public Ui::gbrerankresultdetails
{
    Q_OBJECT

public:
    gbrerankresultdetails(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~gbrerankresultdetails();

public slots:
    virtual void doneSlot();
    virtual void viewSlot();

protected slots:
    virtual void languageChange();

};

#endif // GBRERANKRESULTDETAILS_H
