#include "../../inc/F2DockClient/f2dockadv.Qt3.h"

#include <qvariant.h>
/*
 *  Constructs a F2DockAdv as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
F2DockAdv::F2DockAdv(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
F2DockAdv::~F2DockAdv()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void F2DockAdv::languageChange()
{
    retranslateUi(this);
}

void F2DockAdv::saveSlot()
{
    qWarning("F2DockAdv::saveSlot(): Not implemented yet");
}

void F2DockAdv::defaultSlot()
{
    qWarning("F2DockAdv::defaultSlot(): Not implemented yet");
}

