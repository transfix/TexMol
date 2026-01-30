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
// VolumeRendererExtn.h: interface for the VolumeRendererExtn class.

#if !defined(AFX_VOLUMERENDEREREXTN_H__1A599981_8ACD_4F57_A712_081FCD0E5372__INCLUDED_)
#define AFX_VOLUMERENDEREREXTN_H__1A599981_8ACD_4F57_A712_081FCD0E5372__INCLUDED_

#include <DataManager/VolumeDataManager/VolumeData.h>
#include <VolumeLibrary/VolumeRenderer.h>

class VolumeRendererExtn : public VolumeRenderer
{
	public:
		VolumeRendererExtn();
		virtual ~VolumeRendererExtn();
		bool loadData(VolumeData* volumeData);
		bool renderVolume(double clipValue);
		float getLargestSide();
		float getExtents(double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ);
	protected:
		inline unsigned int upToPowerOfTwo(unsigned int value) const;
		inline double texCoordOfSample(double sample, int bufferWidth, int canvasWidth, double bufferMin, double bufferMax) const;
		void copyToUploadableBufferDensity(unsigned char* inputData, unsigned char* outputData, unsigned int
										   widthX, unsigned int widthY, unsigned int widthZ);

		float m_MinX, m_MinY, m_MinZ, m_MaxX, m_MaxY, m_MaxZ;
};

#endif // !defined(AFX_VOLUMERENDEREREXTN_H__1A599981_8ACD_4F57_A712_081FCD0E5372__INCLUDED_)
