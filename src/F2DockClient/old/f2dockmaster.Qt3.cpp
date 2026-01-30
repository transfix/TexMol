#include "../../inc/F2DockClient/f2dockmasterQt3.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a f2dockmaster as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
f2dockmaster::f2dockmaster(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
f2dockmaster::~f2dockmaster()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void f2dockmaster::languageChange()
{
    retranslateUi(this);
}

void f2dockmaster::dockSlot()
{
    qWarning("f2dockmaster::dockSlot(): Not implemented yet");
}

void f2dockmaster::f2dgenSlot()
{
    qWarning("f2dockmaster::f2dgenSlot(): Not implemented yet");
}

void f2dockmaster::rerankSlot()
{
    qWarning("f2dockmaster::rerankSlot(): Not implemented yet");
}

void f2dockmaster::quadgenSlot()
{
    qWarning("f2dockmaster::quadgenSlot(): Not implemented yet");
}

void f2dockmaster::jobdetSlot()
{
    qWarning("f2dockmaster::jobdetSlot(): Not implemented yet");
}

void f2dockmaster::resultSlot()
{
    qWarning("f2dockmaster::resultSlot(): Not implemented yet");
}

void f2dockmaster::selectSlot()
{
    qWarning("f2dockmaster::selectSlot(): Not implemented yet");
}

void f2dockmaster::saveSlot()
{
    qWarning("f2dockmaster::saveSlot(): Not implemented yet");
}

void f2dockmaster::loadSlot()
{
    qWarning("f2dockmaster::loadSlot(): Not implemented yet");
}

