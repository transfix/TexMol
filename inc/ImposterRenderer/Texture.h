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
#if !defined(AFX_TEXTURE_H__BC94627F_0E50_4516_A138_B7E672BEDE4E__INCLUDED_)
#define AFX_TEXTURE_H__BC94627F_0E50_4516_A138_B7E672BEDE4E__INCLUDED_

namespace CCVImposterRenderer
{
	class Texture
	{
		public:
			Texture();
			virtual ~Texture();
			void loadPGM(const char* name);
			void loadPPM(const char* name);
			void writePGM(const char* name, int w, int h, int max, void *img, int ncomp, int compidx);
			void writePPM(const char* name, int w, int h, int max, void *img, int ncomp);
			bool loadFunction(unsigned char* m_Data, int w, int h, int d);
			void calculateSphereOpacityAndNormalMap(int size);
			void calculateSphereOpacityAndLightMap(int size);
			void calculateElectronDensityBlurMap(int size, float blobby, float denScale);
			void calculateNormalMap(unsigned int w, unsigned int h);
			bool calculateHelixOpacityMap(unsigned int w, unsigned int h);
			bool calculateHelixOpacityMap2(unsigned int w, unsigned int h, int nspiral);
			bool calculateHelixSilhouetteMap(unsigned int w, unsigned int h, int nspiral, int filterw, int filterh);
			void calculateCylinderDepthMap(unsigned int w);
			void calculateCylinderNormalMap(unsigned int w);
			void calculateShadingLookupTable(int w, int h, double shininess);
			void calculateDepthMap(int w, int h);
			void test3DTexture(int w, int h, int d);
			unsigned int getTextureID() const;
			void bindTexture(int dim) const;

		protected:
			bool init3DTexture();

		private:
			unsigned int m_TextureID;
			int m_Texture3dPresent;
	};
};

#endif
