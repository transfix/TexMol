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
// VolumeRenderingProperties.cpp: implementation of the VolumeRenderingProperties class.

#include "ui_VolumeRenderingSetBox.h"
#include <DataManager/VolumeDataManager/VolumeRenderingProperties.h>
#include <QGridLayout>
#include <QFrame>

VolumeRenderingProperties::VolumeRenderingProperties(QWidget* parent, const char* name, Qt::WFlags fl)
	:QWidget(parent, fl)
{
	m_Layout = new QGridLayout(this);

	m_ColorTable = new CVCColorTable::ColorTable(this);
  	m_ColorTable->interactiveUpdates(true);
        m_ColorTable->opacityCubed(true);
	m_Layout->addWidget(m_ColorTable,0,0);

	m_RenderProperties = new QFrame(this);
	m_RenderProperties->setSizePolicy( QSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed, 0, 0));
	m_RenderProperties->setMinimumSize(240,190);
	m_RenderProperties->setMaximumSize(240,190);
 	m_RenderProperties->setFrameShape(QFrame::Panel);
        m_RenderProperties->setFrameShadow(QFrame::Sunken);

	_ui = new Ui::VolumeRenderingSetBox();
	_ui->setupUi(m_RenderProperties);

	m_Layout->addWidget(m_RenderProperties,0,1);
}

VolumeRenderingProperties::~VolumeRenderingProperties()
{
}

