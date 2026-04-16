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
#ifndef CCV_TEXMOL_OPENGL_SHADING_LANGUAGE_RENDERER_H
#define CCV_TEXMOL_OPENGL_SHADING_LANGUAGE_RENDERER_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

namespace ObjectRendererLibrary
{
	class OpenGLShadingLanguageRenderer
	{
		public:
			OpenGLShadingLanguageRenderer();
			virtual ~OpenGLShadingLanguageRenderer();
			virtual bool initialize();
			virtual bool isInitialized();
			virtual void getShaderError(const char* message, GLuint obj);
			virtual void getProgramError(const char* message);

		protected:
			virtual bool compile();
			virtual bool link();
			virtual bool attachAndlink();
			virtual bool createShaders();
			virtual const char* getVertexProgram() = 0;
			virtual const char* getFragmentProgram() = 0;
			virtual bool shaderCompiled(GLuint shader);
			virtual int getNumberOfAttachedShaders(GLuint program);
			GLuint m_Program;
			GLuint m_VertexShader;
			GLuint m_FragmentShader;
			bool m_Initialized;
	};
};

#endif
