#include "../../inc/F2DockClient/f2dgenQt3.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a f2dGen as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
f2dGen::f2dGen(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
f2dGen::~f2dGen()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void f2dGen::languageChange()
{
    retranslateUi(this);
}

void f2dGen::rfButtonSlot()
{
    qWarning("f2dGen::rfButtonSlot(): Not implemented yet");
}

void f2dGen::lfButtonSlot()
{
    qWarning("f2dGen::lfButtonSlot(): Not implemented yet");
}

void f2dGen::genButtonSlot()
{
    qWarning("f2dGen::genButtonSlot(): Not implemented yet");
}

