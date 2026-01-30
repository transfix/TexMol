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
#include <ObjectRenderer/OpenGLShadingLanguageRenderer.h>
#include <OpenGL_Viewer/MyExtensions.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <stdio.h>

using namespace ObjectRendererLibrary;

OpenGLShadingLanguageRenderer::OpenGLShadingLanguageRenderer()
{
	m_Program = 0;
	m_VertexShader = 0;
	m_FragmentShader = 0;
	m_Extensions = 0;
	m_Initialized = false;
}

OpenGLShadingLanguageRenderer::~OpenGLShadingLanguageRenderer()
{
	if (m_VertexShader)
	{
		m_Extensions->glDetachShader(m_Program, m_VertexShader);
		m_Extensions->glDeleteShader(m_VertexShader);
		m_VertexShader = 0;
	}
	if (m_FragmentShader)
	{
		m_Extensions->glDetachShader(m_Program, m_FragmentShader);
		m_Extensions->glDeleteShader(m_FragmentShader);
		m_FragmentShader = 0;
	}
	if (m_Program)
	{
		m_Extensions->glDeleteProgram(m_Program);
		m_Program = 0;
	}
	delete m_Extensions;
	OpenGL_Viewer::CHECK_GL_ERROR();
}

bool OpenGLShadingLanguageRenderer::createShaders()
{
	m_VertexShader = m_Extensions->glCreateShader(GL_VERTEX_SHADER);
	m_FragmentShader = m_Extensions->glCreateShader(GL_FRAGMENT_SHADER);
	if (OpenGL_Viewer::CHECK_GL_ERROR())
	{
		m_VertexShader = 0;
		m_FragmentShader = 0;
		return false;
	}
	const char* vp = getVertexProgram();
	const char* fp = getFragmentProgram();
	if (!vp || !fp)
	{
		return false;
	}
	m_Extensions->glShaderSource(m_VertexShader, 1, (const char**)&vp, NULL);
	m_Extensions->glShaderSource(m_FragmentShader, 1, (const char**)&fp, NULL);
	if (OpenGL_Viewer::CHECK_GL_ERROR())
	{
		m_VertexShader = 0;
		m_FragmentShader = 0;
		return false;
	}
	return true;
}

bool OpenGLShadingLanguageRenderer::shaderCompiled(GLuint shader)
{
	int status = GL_FALSE;
	m_Extensions->glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	return (status == GL_TRUE)? true : false;
}

bool OpenGLShadingLanguageRenderer::compile()
{
	if (!m_Extensions)
	{
		return false;
	}
	m_Extensions->glCompileShader(m_VertexShader);
	if (OpenGL_Viewer::CHECK_GL_ERROR())
	{
		printf("Error compiling vertex program\n");
		return false;
	}
	if (!shaderCompiled(m_VertexShader))
	{
		getShaderError("After checking if it compiled", m_VertexShader);
		return false;
	}
	m_Extensions->glCompileShader(m_FragmentShader);
	if (!shaderCompiled(m_FragmentShader))
	{
		return false;
	}
	return true;
}

bool OpenGLShadingLanguageRenderer::link()
{
	if (!m_Extensions)
	{
		return false;
	}
	m_Extensions->glLinkProgram(m_Program);
	getProgramError("After linking program");
	int status = GL_FALSE;
	m_Extensions->glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
	return (status == GL_TRUE)? true : false;
}

int OpenGLShadingLanguageRenderer::getNumberOfAttachedShaders(GLuint program)
{
	int numberOfAttachedShaders = 0;
	m_Extensions->glGetProgramiv(program, GL_ATTACHED_SHADERS, &numberOfAttachedShaders);
	return numberOfAttachedShaders;
}

bool OpenGLShadingLanguageRenderer::attachAndlink()
{
	if (!m_Extensions)
	{
		return false;
	}
	if (getNumberOfAttachedShaders(m_Program) != 0)
	{
		return false;
	}
	m_Extensions->glAttachShader(m_Program, m_VertexShader);
	if (OpenGL_Viewer::CHECK_GL_ERROR())
	{
		return false;
	}
	getShaderError("After attaching v s", m_VertexShader);
	m_Extensions->glAttachShader(m_Program, m_FragmentShader);
	if (OpenGL_Viewer::CHECK_GL_ERROR())
	{
		return false;
	}
	getShaderError("After attaching f s", m_VertexShader);
	if (!link())
	{
		return false;
	}
	return true;
}

bool OpenGLShadingLanguageRenderer::isInitialized()
{
	return m_Initialized;
}

bool OpenGLShadingLanguageRenderer::initialize()
{
	m_Initialized = false;
	if (!m_Extensions)
	{
		return false;
	}
	// create shaders
	if (!createShaders())
	{
		getShaderError("After creating shaders", m_VertexShader);
		return false;
	}
	// check the source and make sure it is correct
	{
		int length = 0;
		m_Extensions->glGetShaderiv(m_VertexShader, GL_SHADER_SOURCE_LENGTH, &length);
		char* buffer = new char[length+1];
		m_Extensions->glGetShaderSource(m_VertexShader, length+1, &length, buffer);
		printf("Vertex program is %s\n", buffer);
	}
	{
		int length = 0;
		m_Extensions->glGetShaderiv(m_FragmentShader, GL_SHADER_SOURCE_LENGTH, &length);
		char* buffer = new char[length+1];
		m_Extensions->glGetShaderSource(m_FragmentShader, length+1, &length, buffer);
		printf("Fragment program is %s\n", buffer);
	}
	// compile
	if (!compile())
	{
		getShaderError("After trying to compile", m_VertexShader);
		return false;
	}
	// create program
	if ((m_Program = m_Extensions->glCreateProgram()) == 0)
	{
		getShaderError("After creating program", m_VertexShader);
		return false;
	}
	// attach and link
	if (!attachAndlink())
	{
		getShaderError("After trying to attach and link", m_VertexShader);
		return false;
	}
	// check one last time for all public functions!
	if (OpenGL_Viewer::CHECK_GL_ERROR())
	{
		return false;
	}
	m_Initialized = true;
	return true;
}

void OpenGLShadingLanguageRenderer::getShaderError(const char* message, GLuint obj)
{
	int infologLength = 0;
	m_Extensions->glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (message)
	{
		printf("%s\n", message);
	}
	if (infologLength > 0)
	{
		char* infoLog = new char[infologLength];
		int charsWritten  = 0;
		m_Extensions->glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}
	if (message)
	{
		printf("Done printing error\n");
	}
}

void OpenGLShadingLanguageRenderer::getProgramError(const char* message)
{
	int infologLength = 0;
	m_Extensions->glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infologLength);
	if (message)
	{
		printf("%s\n", message);
	}
	if (infologLength > 0)
	{
		char* infoLog = new char[infologLength];
		int charsWritten  = 0;
		m_Extensions->glGetProgramInfoLog(m_Program, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}
	if (message)
	{
		printf("Done printing error\n");
	}
}
