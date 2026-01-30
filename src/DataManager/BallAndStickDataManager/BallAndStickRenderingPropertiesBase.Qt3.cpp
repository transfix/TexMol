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
#include "../../../inc/DataManager/BallAndStickDataManager/BallAndStickRenderingPropertiesBase.Qt3.h"

#include <qvariant.h>
/*
 *  Constructs a BallAndStickRenderingPropertiesBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
BallAndStickRenderingPropertiesBase::BallAndStickRenderingPropertiesBase(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QWidget(parent, name, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
BallAndStickRenderingPropertiesBase::~BallAndStickRenderingPropertiesBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void BallAndStickRenderingPropertiesBase::languageChange()
{
    retranslateUi(this);
}

void BallAndStickRenderingPropertiesBase::redrawSlot()
{
    qWarning("BallAndStickRenderingPropertiesBase::redrawSlot(): Not implemented yet");
}

