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
// VolumeRenderingProperties.h: interface for the VolumeRenderingProperties class.

#if !defined(AFX_VOLUMERENDERINGPROPERTIES_H__54A1E83C_6EE6_4B70_B1EA_9279B9A2CDED__INCLUDED_)
#define AFX_VOLUMERENDERINGPROPERTIES_H__54A1E83C_6EE6_4B70_B1EA_9279B9A2CDED__INCLUDED_
#include <qapplication.h>
#include <QGridLayout>
#include <QFrame>
#include <ColorTable2/ColorTable.h>

namespace Ui
{
  class VolumeRenderingSetBox;
}

class VolumeRenderingProperties : public QWidget
{
	public:
		VolumeRenderingProperties(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = {});
		virtual ~VolumeRenderingProperties();

		CVCColorTable::ColorTable* m_ColorTable;
		Ui::VolumeRenderingSetBox* _ui;

        private:
		QGridLayout *m_Layout;
		QFrame *m_RenderProperties;
	
};

#endif // !defined(AFX_VOLUMERENDERINGPROPERTIES_H__54A1E83C_6EE6_4B70_B1EA_9279B9A2CDED__INCLUDED_)
