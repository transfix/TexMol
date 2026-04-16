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
// SecondaryStructureRenderingProperties.cpp: implementation of the SecondaryStructureRenderingProperties class.

#include "ui_SecondaryStructureRenderingPropertiesBase.h"
#include <DataManager/SecondaryStructureDataManager/SecondaryStructureRenderingProperties.h>
#include "qcombobox.h"

SecondaryStructureRenderingProperties::SecondaryStructureRenderingProperties(QWidget* parent, const char* name, Qt::WindowFlags fl)
	: QWidget(parent, fl)
{
	_ui = new Ui::SecondaryStructureRenderingPropertiesBase();
	_ui->setupUi(this);

	QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(_ui->m_AlphaHistogram->sizePolicy().hasHeightForWidth());

	m_AlphaHistogram = new Histogram(_ui->m_AlphaHistogram);
        sizePolicy3.setHeightForWidth(m_AlphaHistogram->sizePolicy().hasHeightForWidth());
	m_AlphaHistogram->setSizePolicy(sizePolicy3);
	m_AlphaHistogram->resize(_ui->m_AlphaHistogram->frameSize());
	
	m_BetaHistogram = new Histogram(_ui->m_BetaHistogram);
	sizePolicy3.setHeightForWidth(m_BetaHistogram->sizePolicy().hasHeightForWidth());
	m_BetaHistogram->setSizePolicy(sizePolicy3);
	m_BetaHistogram->resize(_ui->m_BetaHistogram->frameSize());
}

SecondaryStructureRenderingProperties::~SecondaryStructureRenderingProperties()
{
}

void SecondaryStructureRenderingProperties::addNewRenderingTypeInOrder(const char* name, int index)
{
	//m_SecondaryStructureRenderingTypesComboBox->insertItem( name, index );
}
