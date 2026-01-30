#include "../../inc/F2DockClient/f2dock.Qt3.h"

#include <qvariant.h>
/*
 *  Constructs a F2Dock as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
F2Dock::F2Dock(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
F2Dock::~F2Dock()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void F2Dock::languageChange()
{
    retranslateUi(this);
}

void F2Dock::selectReceptorFileSlot()
{
    qWarning("F2Dock::selectReceptorFileSlot(): Not implemented yet");
}

void F2Dock::selectLigandFileSlot()
{
    qWarning("F2Dock::selectLigandFileSlot(): Not implemented yet");
}

void F2Dock::selectRMSDFileSlot()
{
    qWarning("F2Dock::selectRMSDFileSlot(): Not implemented yet");
}

void F2Dock::selectOutputFileSlot()
{
    qWarning("F2Dock::selectOutputFileSlot(): Not implemented yet");
}

void F2Dock::advencedOptionsSlot()
{
    qWarning("F2Dock::advencedOptionsSlot(): Not implemented yet");
}

void F2Dock::dockSlot()
{
    qWarning("F2Dock::dockSlot(): Not implemented yet");
}

void F2Dock::defaultSlot()
{
    qWarning("F2Dock::defaultSlot(): Not implemented yet");
}

void F2Dock::uuSlot()
{
    qWarning("F2Dock::uuSlot(): Not implemented yet");
}

void F2Dock::bbSlot()
{
    qWarning("F2Dock::bbSlot(): Not implemented yet");
}

void F2Dock::selectReceptorPDBSlot()
{
    qWarning("F2Dock::selectReceptorPDBSlot(): Not implemented yet");
}

