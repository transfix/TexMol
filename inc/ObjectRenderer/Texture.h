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
#ifndef CCV_TEXMOL_OBJECT_RENDERER_TEXTURE_H
#define CCV_TEXMOL_OBJECT_RENDERER_TEXTURE_H

namespace ObjectRendererLibrary
{
	class Texture
	{
		public:
			Texture();
			virtual ~Texture();
			void loadPGM(const char* name);
			void loadPPM(const char* name);
			bool loadFunction(unsigned char* m_Data, int w, int h, int d);
			void calculateSphereOpacityAndNormalMap(int size);
			void calculateNormalMap(unsigned int w, unsigned int h);
			bool calculateHelixOpacityMap(unsigned int w, unsigned int h);
			void calculateCylinderDepthMap(unsigned int w);
			void calculateCylinderNormalMap(unsigned int w);
			void calculateShadingLookupTable(int w, int h, double shininess);
			void calculateDepthMap(int w, int h);
			unsigned int getTextureID() const;
			void bindTexture(int dim) const;

		protected:
			bool init3DTexture();
			void normalize(float* n);
			float clamp_0_to_1(float x);

		private:
			unsigned int m_TextureID;
			int m_Texture3dPresent;
	};
};

#endif
