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
#ifndef __TRANSPARENTVOLUMERENDERER_HPP__
#define __TRANSPARENTVOLUMERENDERER_HPP__

#include <DataManager/VolumeDataManager/VolumeData.h>
#include <DataManager/VolumeDataManager/VolumeRendererExtn.h>

class TransparentVolumeRenderer
{
	public:
		TransparentVolumeRenderer() {}
		~TransparentVolumeRenderer() {}
		static void render(VolumeData* volumeData, VolumeRendererExtn* volumeRendererExtn);
		static void enableContourRendererGLState(VolumeData* volumeData, VolumeRendererExtn* volumeRendererExtn);
		static void enableVolumeRendererGLState(VolumeData* volumeData, VolumeRendererExtn* volumeRendererExtn);

	private:
		static Geometry* m_ContourGeometry;
		static Geometry* m_VolumeGeometry;
};

#endif
