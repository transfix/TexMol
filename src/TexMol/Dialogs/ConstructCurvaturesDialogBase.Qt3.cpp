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
#include "../../../inc/TexMol/Dialogs/ConstructCurvaturesDialogBase.Qt3.h"

#include <qvariant.h>
/*
 *  Constructs a ConstructCurvaturesDialogBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ConstructCurvaturesDialogBase::ConstructCurvaturesDialogBase(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
ConstructCurvaturesDialogBase::~ConstructCurvaturesDialogBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConstructCurvaturesDialogBase::languageChange()
{
    retranslateUi(this);
}

void ConstructCurvaturesDialogBase::inputPDBFileSlot()
{
    qWarning("ConstructCurvaturesDialogBase::inputPDBFileSlot(): Not implemented yet");
}

void ConstructCurvaturesDialogBase::inputIsocontourFileSlot()
{
    qWarning("ConstructCurvaturesDialogBase::inputIsocontourFileSlot(): Not implemented yet");
}

void ConstructCurvaturesDialogBase::outputMeanCurvatureFileSlot()
{
    qWarning("ConstructCurvaturesDialogBase::outputMeanCurvatureFileSlot(): Not implemented yet");
}

void ConstructCurvaturesDialogBase::outputGaussianCurvatureFileSlot()
{
    qWarning("ConstructCurvaturesDialogBase::outputGaussianCurvatureFileSlot(): Not implemented yet");
}

void ConstructCurvaturesDialogBase::isovalueRadioButtonSelectedSlot(bool)
{
    qWarning("ConstructCurvaturesDialogBase::isovalueRadioButtonSelectedSlot(bool): Not implemented yet");
}

void ConstructCurvaturesDialogBase::isocontourRadioButtonSelectedSlot(bool)
{
    qWarning("ConstructCurvaturesDialogBase::isocontourRadioButtonSelectedSlot(bool): Not implemented yet");
}

void ConstructCurvaturesDialogBase::outputVolumeFileNameSlot()
{
    qWarning("ConstructCurvaturesDialogBase::outputVolumeFileNameSlot(): Not implemented yet");
}

void ConstructCurvaturesDialogBase::outputCurvatureFileSlot()
{
    qWarning("ConstructCurvaturesDialogBase::outputCurvatureFileSlot(): Not implemented yet");
}

