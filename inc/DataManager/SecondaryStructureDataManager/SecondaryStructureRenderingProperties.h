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
// SecondaryStructureRenderingProperties.h: interface for the SecondaryStructureRenderingProperties class.

#ifndef __SECONDARYSTRUCTURERENDERINGPROPERTIES_H__
#define __SECONDARYSTRUCTURERENDERINGPROPERTIES_H__

#include <Histogram/histogram.h>

namespace Ui
{
	class SecondaryStructureRenderingPropertiesBase;
}

class SecondaryStructureRenderingProperties : public QWidget 
{
	public:
		SecondaryStructureRenderingProperties(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
		virtual ~SecondaryStructureRenderingProperties();
		void addNewRenderingTypeInOrder(const char* name, int index);

		Ui::SecondaryStructureRenderingPropertiesBase* _ui;
		Histogram* m_AlphaHistogram;
		Histogram* m_BetaHistogram;
};

#endif
