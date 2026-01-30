// FunctionImposterSphereRenderer.cpp: implementation of the FunctionImposterSphereRenderer class.

#include <ObjectRenderer/Sphere/FunctionImposterSphereRenderer.h>
#include <OpenGL_Viewer/MyExtensions.h>

using namespace ObjectRendererLibrary;

FunctionImposterSphereRenderer::FunctionImposterSphereRenderer(MyExtensions* extensions)
{
	m_Extensions = extensions;
}

FunctionImposterSphereRenderer::~FunctionImposterSphereRenderer()
{
}

bool FunctionImposterSphereRenderer::bindProgramAndParams()
{
	return true;
}

bool FunctionImposterSphereRenderer::unbindProgramAndParams()
{
	return true;
}

bool FunctionImposterSphereRenderer::bindMatrices()
{
	return true;
}

const char* FunctionImposterSphereRenderer::getVertexProgram()
{
	return vertexShader;
}

const char* FunctionImposterSphereRenderer::getFragmentProgram()
{
	return fragmentShader;
}

const char* FunctionImposterSphereRenderer::vertexShader =
{
	"void main()"
	"{"
	"    gl_Position = ftransform();"
	"}"
};

const char* FunctionImposterSphereRenderer::fragmentShader =
{
	"void main()"
	"{"
	"    gl_FragColor = vec4(0.4,0.4,0.8,1.0);"
	"}"
};
