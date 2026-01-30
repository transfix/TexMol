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
#if !defined(AFX_SPLATTINGRENDERER_H__021F88F2_7B41_455B_954A_AF9E6BB50C2C__INCLUDED_)
#define AFX_SPLATTINGRENDERER_H__021F88F2_7B41_455B_954A_AF9E6BB50C2C__INCLUDED_

class SimpleVolumeData;

namespace CCVImposterRenderer
{
	class ImposterRenderer;
}

namespace CCVSplattingRenderer
{
	class SplattingRenderer
	{
		public:
			SplattingRenderer(int octreeLeafSize);
			virtual ~SplattingRenderer();
			bool initRenderer();
			bool render(double* dColorMap, int colorMapSize);
			bool setOctreeLeafSize(int octreeLeafSize);
			bool uploadColorMap(double* colorMap, int size);

		protected:
			bool buildNewOctree();
			bool renderCube(double* dColorMap, int colorMapSize);
			int getColorMapIndex(int volumeIndex, int colorMapSize, double minVal, double maxVal);
			int m_OctreeLeafSize;
			SimpleVolumeData* m_CurrentSimpleVolumeData;
			CCVImposterRenderer::ImposterRenderer* m_ImposterRenderer;
	};
};

#endif
