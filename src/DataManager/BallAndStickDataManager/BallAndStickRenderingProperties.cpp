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
// BallAndStickRenderingProperties.cpp: implementation of the BallAndStickRenderingProperties class.

#include "ui_BallAndStickRenderingPropertiesBase.h"
#include <DataManager/BallAndStickDataManager/BallAndStickRenderingProperties.h>
#include <qcombobox.h>

BallAndStickRenderingProperties::BallAndStickRenderingProperties(QWidget* parent, const char* name, Qt::WindowFlags fl)
	: QWidget(parent, fl)
{
	_ui = new Ui::BallAndStickRenderingProperties();
	_ui->setupUi(this);

	_ui->m_ComboBox_StructureLOD->addItem("Atomic - Space filling");
	// arand, 9-8-2011: restoring the ball and stick renderer
	//                  I think it should work again?
	_ui->m_ComboBox_StructureLOD->addItem("Atomic - Ball and Stick");
	_ui->m_ComboBox_StructureLOD->addItem("Atomic - Backbone");
	_ui->m_ComboBox_StructureLOD->addItem("Residues");
	_ui->m_ComboBox_StructureLOD->addItem("Secondary Structures");
	_ui->m_ComboBox_StructureLOD->addItem("Chains");
	_ui->m_ComboBox_StructureLOD->addItem("Proteins");
	_ui->m_ComboBox_StructureLOD->addItem("HBond - all");
	_ui->m_ComboBox_StructureLOD->addItem("HBond - internal");
	_ui->m_ComboBox_StructureLOD->addItem("HBond - external");
	//_ui->m_ComboBox_StructureLOD->addItem("Multiresolution Surface");
	// these should correspond to the set of const ints definining the structures.
	_ui->m_ComboBox_ColorLOD->addItem("Atoms");
	_ui->m_ComboBox_ColorLOD->addItem("Residues");
	_ui->m_ComboBox_ColorLOD->addItem("Secondary Structures");
	_ui->m_ComboBox_ColorLOD->addItem("Chains");
	_ui->m_ComboBox_ColorLOD->addItem("Proteins");
	_ui->m_ComboBox_ColorLOD->addItem("Function");
	_ui->m_ComboBox_ColorLOD->addItem("Boundary / Interior");
	_ui->m_ComboBox_ColorLOD->addItem("User defined color map");
	_ui->m_ComboBox_ColorLOD->addItem("User defined color");
}

BallAndStickRenderingProperties::~BallAndStickRenderingProperties()
{
}
