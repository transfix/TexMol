#include "../../inc/F2DockClient/gbrerank.Qt3.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a GBRerank as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
GBRerank::GBRerank(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
GBRerank::~GBRerank()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GBRerank::languageChange()
{
    retranslateUi(this);
}

void GBRerank::selectReceptorPQRFileSlot()
{
    qWarning("GBRerank::selectReceptorPQRFileSlot(): Not implemented yet");
}

void GBRerank::selectLigandPQRFileSlot()
{
    qWarning("GBRerank::selectLigandPQRFileSlot(): Not implemented yet");
}

void GBRerank::selectReceptorQUADFileSlot()
{
    qWarning("GBRerank::selectReceptorQUADFileSlot(): Not implemented yet");
}

void GBRerank::selectLigandQUADFileSlot()
{
    qWarning("GBRerank::selectLigandQUADFileSlot(): Not implemented yet");
}

void GBRerank::selectF2dockOutputFileSlot()
{
    qWarning("GBRerank::selectF2dockOutputFileSlot(): Not implemented yet");
}

void GBRerank::selectGbrerankOutputFileSlot()
{
    qWarning("GBRerank::selectGbrerankOutputFileSlot(): Not implemented yet");
}

void GBRerank::resetSlot()
{
    qWarning("GBRerank::resetSlot(): Not implemented yet");
}

void GBRerank::rerankSlot()
{
    qWarning("GBRerank::rerankSlot(): Not implemented yet");
}

