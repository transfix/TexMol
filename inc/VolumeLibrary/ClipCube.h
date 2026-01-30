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
#if !defined(AFX_CLIPCUBE_H__01A55CB5_9394_4F10_81A4_444B72A44AF9__INCLUDED_)
#define AFX_CLIPCUBE_H__01A55CB5_9394_4F10_81A4_444B72A44AF9__INCLUDED_

#include <VolumeLibrary/Polygon.h>
#include <OpenGL_Viewer/Plane.h>

namespace OpenGLVolumeRendering
{
	class ClipCube
	{
		public:
			ClipCube();
			ClipCube(double ratioX, double ratioY, double ratioZ,
					 double minX=0, double minY=0, double minZ=0,
					 double maxX=1, double maxY=1, double maxZ=1);
			virtual ~ClipCube();
			bool setAspectRatio(double ratioX, double ratioY, double ratioZ);
			bool setTextureSubCube(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);
			bool clipPlane(Polygon& result, const Plane& plane) const;

		protected:
			double m_RatioX;
			double m_RatioY;
			double m_RatioZ;
			double m_TexCoords[8*3];
			unsigned char getCaseAndCalculateSignedDistances(double* distances, const Plane& plane) const;
			double getAlphaForEdge(double* distances, unsigned int edgeIndex) const;
			void interpVertCoords(double* vert, double alpha, unsigned int edgeIndex) const;
			void interpTexCoords(double* texCoord, double alpha, unsigned int edgeIndex) const;
	};
};

#endif
