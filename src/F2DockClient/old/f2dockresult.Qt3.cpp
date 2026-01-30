#include "../../inc/F2DockClient/f2dockresultQt3.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a f2DockResult as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
f2DockResult::f2DockResult(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
f2DockResult::~f2DockResult()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void f2DockResult::languageChange()
{
    retranslateUi(this);
}

void f2DockResult::detailsSlot()
{
    qWarning("f2DockResult::detailsSlot(): Not implemented yet");
}

void f2DockResult::doneSlot()
{
    qWarning("f2DockResult::doneSlot(): Not implemented yet");
}

