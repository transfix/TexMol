// FunctionImposterSphereRenderer.cpp: implementation of the FunctionImposterSphereRenderer class.

#include <GL/glew.h>
#include <ObjectRenderer/Sphere/FunctionImposterSphereRenderer.h>

using namespace ObjectRendererLibrary;

FunctionImposterSphereRenderer::FunctionImposterSphereRenderer()
{
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
