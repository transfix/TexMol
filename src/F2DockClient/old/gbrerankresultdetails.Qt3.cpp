#include "../../inc/F2DockClient/gbrerankresultdetails.Qt3.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a gbrerankresultdetails as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
gbrerankresultdetails::gbrerankresultdetails(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
gbrerankresultdetails::~gbrerankresultdetails()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void gbrerankresultdetails::languageChange()
{
    retranslateUi(this);
}

void gbrerankresultdetails::doneSlot()
{
    qWarning("gbrerankresultdetails::doneSlot(): Not implemented yet");
}

void gbrerankresultdetails::viewSlot()
{
    qWarning("gbrerankresultdetails::viewSlot(): Not implemented yet");
}

