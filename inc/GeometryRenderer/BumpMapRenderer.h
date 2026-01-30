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
// BumpMapRenderer.h: interface for the BumpMapRenderer class.

#ifndef CCV_TEXMOL_GEOMETRY_RENDERER_BUMP_MAP_RENDERER_H
#define CCV_TEXMOL_GEOMETRY_RENDERER_BUMP_MAP_RENDERER_H

class Geometry;
class MyExtensions;

#ifdef CG
#include <Cg/cgGL.h>
#endif

namespace CCVTexMolGeometryRenderer
{
	class BumpMapRenderer
	{
		public:
			BumpMapRenderer();
			virtual ~BumpMapRenderer();

			bool initialize();
			bool render(Geometry* geometry, float* surfaceColors);

		protected:
			MyExtensions* m_Extensions;
			bool m_Initialized;

			#ifdef CG
			CGcontext m_Context;
			CGprofile m_FragmentProfile;
			CGprogram m_FragmentProgram;
			#endif
	};
};

#endif
