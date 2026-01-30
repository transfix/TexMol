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
#include <VolumeLibrary/Polygon.h>
#include <VolumeLibrary/UnshadedBase.h>

using namespace OpenGLVolumeRendering;

UnshadedBase::UnshadedBase()
{
}

UnshadedBase::~UnshadedBase()
{
}

// Initializes the renderer.
// Should be called again if the renderer is moved to a different openGL context.
// If this returns false, do not try to use it to do volumeRendering
bool UnshadedBase::initRenderer()
{
	return RendererBase::initRenderer();
}
