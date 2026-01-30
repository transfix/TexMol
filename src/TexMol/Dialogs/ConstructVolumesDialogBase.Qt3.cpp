/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "../../../inc/TexMol/Dialogs/ConstructVolumesDialogBase.Qt3.h"

#include <qvariant.h>
/*
 *  Constructs a ConstructVolumesDialogBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ConstructVolumesDialogBase::ConstructVolumesDialogBase(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
ConstructVolumesDialogBase::~ConstructVolumesDialogBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConstructVolumesDialogBase::languageChange()
{
    retranslateUi(this);
}

void ConstructVolumesDialogBase::outputVolumeFileSlot()
{
    qWarning("ConstructVolumesDialogBase::outputVolumeFileSlot(): Not implemented yet");
}

void ConstructVolumesDialogBase::colorMapFileSlot()
{
    qWarning("ConstructVolumesDialogBase::colorMapFileSlot(): Not implemented yet");
}

void ConstructVolumesDialogBase::constructRawVSlot(bool)
{
    qWarning("ConstructVolumesDialogBase::constructRawVSlot(bool): Not implemented yet");
}

void ConstructVolumesDialogBase::functionTypeChangedSlot(int)
{
    qWarning("ConstructVolumesDialogBase::functionTypeChangedSlot(int): Not implemented yet");
}

void ConstructVolumesDialogBase::outputSurfaceFileSlot()
{
    qWarning("ConstructVolumesDialogBase::outputSurfaceFileSlot(): Not implemented yet");
}

