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

BallAndStickRenderingProperties::BallAndStickRenderingProperties(QWidget* parent, const char* name, Qt::WFlags fl)
	: QWidget(parent, fl)
{
	_ui = new Ui::BallAndStickRenderingProperties();
	_ui->setupUi(this);

	_ui->m_ComboBox_StructureLOD->insertItem("Atomic - Space filling");
	// arand, 9-8-2011: restoring the ball and stick renderer
	//                  I think it should work again?
	_ui->m_ComboBox_StructureLOD->insertItem("Atomic - Ball and Stick");
	_ui->m_ComboBox_StructureLOD->insertItem("Atomic - Backbone");
	_ui->m_ComboBox_StructureLOD->insertItem("Residues");
	_ui->m_ComboBox_StructureLOD->insertItem("Secondary Structures");
	_ui->m_ComboBox_StructureLOD->insertItem("Chains");
	_ui->m_ComboBox_StructureLOD->insertItem("Proteins");
	_ui->m_ComboBox_StructureLOD->insertItem("HBond - all");
	_ui->m_ComboBox_StructureLOD->insertItem("HBond - internal");
	_ui->m_ComboBox_StructureLOD->insertItem("HBond - external");
	//_ui->m_ComboBox_StructureLOD->insertItem("Multiresolution Surface");
	// these should correspond to the set of const ints definining the structures.
	_ui->m_ComboBox_ColorLOD->insertItem("Atoms");
	_ui->m_ComboBox_ColorLOD->insertItem("Residues");
	_ui->m_ComboBox_ColorLOD->insertItem("Secondary Structures");
	_ui->m_ComboBox_ColorLOD->insertItem("Chains");
	_ui->m_ComboBox_ColorLOD->insertItem("Proteins");
	_ui->m_ComboBox_ColorLOD->insertItem("Function");
	_ui->m_ComboBox_ColorLOD->insertItem("Boundary / Interior");
	_ui->m_ComboBox_ColorLOD->insertItem("User defined color map");
	_ui->m_ComboBox_ColorLOD->insertItem("User defined color");
}

BallAndStickRenderingProperties::~BallAndStickRenderingProperties()
{
}
