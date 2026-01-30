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
// SurfaceRenderingProperties.h: interface for the SurfaceRenderingProperties class.

#if !defined(AFX_SURFACERENDERINGPROPERTIES_H__9AB7858F_4BEE_4FC6_A973_B5CE3995BE5F__INCLUDED_)
#define AFX_SURFACERENDERINGPROPERTIES_H__9AB7858F_4BEE_4FC6_A973_B5CE3995BE5F__INCLUDED_

namespace Ui
{
	class SurfaceRenderingPropertiesBase;
}

class SurfaceRenderingProperties : public QWidget 
{
	public:
		SurfaceRenderingProperties(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
		virtual ~SurfaceRenderingProperties();
		void addNewRenderingTypeInOrder(const char* name, int index);

		QFrame *m_BaseBox;
		QGridLayout *m_Layout;
		Ui::SurfaceRenderingPropertiesBase* _ui;
};

#endif // !defined(AFX_SURFACERENDERINGPROPERTIES_H__9AB7858F_4BEE_4FC6_A973_B5CE3995BE5F__INCLUDED_)
