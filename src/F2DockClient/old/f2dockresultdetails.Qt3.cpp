#include "../../inc/F2DockClient/f2dockresultdetails.Qt3.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a f2dockresultdetails as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
f2dockresultdetails::f2dockresultdetails(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
f2dockresultdetails::~f2dockresultdetails()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void f2dockresultdetails::languageChange()
{
    retranslateUi(this);
}

void f2dockresultdetails::doneSlot()
{
    qWarning("f2dockresultdetails::doneSlot(): Not implemented yet");
}

void f2dockresultdetails::viewSlot()
{
    qWarning("f2dockresultdetails::viewSlot(): Not implemented yet");
}

void f2dockresultdetails::selectRowSlot()
{
    qWarning("f2dockresultdetails::selectRowSlot(): Not implemented yet");
}

void f2dockresultdetails::surfaceSlot(bool)
{
    qWarning("f2dockresultdetails::surfaceSlot(bool): Not implemented yet");
}

