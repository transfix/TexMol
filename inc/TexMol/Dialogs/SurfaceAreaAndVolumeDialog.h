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
#ifndef SURFACE_AREA_AND_VOLUME_DIALOG_H
#define SURFACE_AREA_AND_VOLUME_DIALOG_H

#include <TexMol/Dialogs/SurfaceAreaAndVolumeDialogBase.h>

class SurfaceAreaAndVolumeDialog : public SurfaceAreaAndVolumeDialogBase
{
	public:
		SurfaceAreaAndVolumeDialog(double area, double volume, QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
		virtual ~SurfaceAreaAndVolumeDialog();

 	public slots:
		// no slots on this one

	protected:
		QWidget* m_Parent;
};

#endif

