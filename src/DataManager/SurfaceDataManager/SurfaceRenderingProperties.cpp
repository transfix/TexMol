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
// SurfaceRenderingProperties.cpp: implementation of the SurfaceRenderingProperties class.

#include "ui_SurfaceRenderingPropertiesBase.h"

#include <DataManager/SurfaceDataManager/SurfaceRenderingProperties.h>
#include "qcombobox.h"

SurfaceRenderingProperties::SurfaceRenderingProperties(QWidget* parent, const char* name, Qt::WindowFlags fl)
	: QWidget(parent, fl)
{
	m_BaseBox = new QFrame(this);
	m_BaseBox->setFrameShape(QFrame::StyledPanel);
	m_Layout = new QGridLayout(this);
	m_Layout->addWidget(m_BaseBox);

	_ui = new Ui::SurfaceRenderingPropertiesBase();
	_ui->setupUi(m_BaseBox);
}

SurfaceRenderingProperties::~SurfaceRenderingProperties()
{
}

void SurfaceRenderingProperties::addNewRenderingTypeInOrder(const char* name, int index)
{
	_ui->m_ComboBox_SurfaceRenderingTypes->insertItem(index, QString(name));
}
