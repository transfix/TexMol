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
#if !defined(AFX_GLOBALCGCONTEXT_H__37EBD53C_2758_4A18_9DD1_7FDF0DB7D705__INCLUDED_)
#define AFX_GLOBALCGCONTEXT_H__37EBD53C_2758_4A18_9DD1_7FDF0DB7D705__INCLUDED_

#include <glew/glew.h>
#ifdef CG
#include <Cg/cgGL.h>
#endif

namespace CCVImposterRenderer
{
	class GlobalCGContext
	{
		public:
			virtual ~GlobalCGContext();
			#ifdef CG
			static CGcontext getCGContext();
			#endif

		protected:
			GlobalCGContext();
			#ifdef CG
			CGcontext m_ShaderContext;
			static GlobalCGContext* ms_GlobalCGContext;
			static void cgErrorCallback(void);
			static void initGlobalPointer();
			#endif
	};
};

#endif
