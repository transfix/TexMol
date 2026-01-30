#include "../../inc/F2DockClient/quadgenQt3.h"

#include <qvariant.h>
/*
 *  Constructs a quadGen as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
quadGen::quadGen(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
quadGen::~quadGen()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void quadGen::languageChange()
{
    retranslateUi(this);
}

void quadGen::rfButtonSlot()
{
    qWarning("quadGen::rfButtonSlot(): Not implemented yet");
}

void quadGen::lfButtonSlot()
{
    qWarning("quadGen::lfButtonSlot(): Not implemented yet");
}

void quadGen::genButtonSlot()
{
    qWarning("quadGen::genButtonSlot(): Not implemented yet");
}

void quadGen::fineSlot()
{
    qWarning("quadGen::fineSlot(): Not implemented yet");
}

void quadGen::coarseSlot()
{
    qWarning("quadGen::coarseSlot(): Not implemented yet");
}

