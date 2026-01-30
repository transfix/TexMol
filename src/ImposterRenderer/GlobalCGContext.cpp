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
#include <ImposterRenderer/GlobalCGContext.h>
#include <stdio.h>

using CCVImposterRenderer::GlobalCGContext;

#ifdef CG
GlobalCGContext* GlobalCGContext::ms_GlobalCGContext = 0;
#endif

GlobalCGContext::GlobalCGContext()
{
	#ifdef CG
	m_ShaderContext = cgCreateContext();
	cgSetErrorCallback(cgErrorCallback);
	#endif
}

GlobalCGContext::~GlobalCGContext()
{
	#ifdef CG
	cgDestroyContext(m_ShaderContext);
	#endif
}

#ifdef CG
CGcontext GlobalCGContext::getCGContext()
{
	if (ms_GlobalCGContext == 0)
	{
		initGlobalPointer();
		return ms_GlobalCGContext->m_ShaderContext;
	}
	else
	{
		return ms_GlobalCGContext->m_ShaderContext;
	}
}

void GlobalCGContext::initGlobalPointer()
{
	ms_GlobalCGContext = new GlobalCGContext;
}

void GlobalCGContext::cgErrorCallback(void)
{
	const char* error = cgGetErrorString(cgGetError());
	printf("Error: %s", error);
}
#endif
