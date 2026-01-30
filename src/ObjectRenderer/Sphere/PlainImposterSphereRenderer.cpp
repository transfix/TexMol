// PlainImposterSphereRenderer.cpp: implementation of the PlainImposterSphereRenderer class.

#include <ObjectRenderer/Sphere/PlainImposterSphereRenderer.h>
#include <ObjectRenderer/Texture.h>
#include <OpenGL_Viewer/MyExtensions.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <stdio.h>

using namespace ObjectRendererLibrary;

PlainImposterSphereRenderer::PlainImposterSphereRenderer(MyExtensions* extensions)
{
	m_Initialized = false;
	m_Extensions = extensions;
	m_Program = 0;
	m_MainColorParam = 0;
	m_NormalMapParam = 0;
	m_DepthMapParam = 0;
	m_TextureNormalMap = 0;
	m_TextureDepthMap = 0;
}

PlainImposterSphereRenderer::~PlainImposterSphereRenderer()
{
	delete m_TextureNormalMap;
	delete m_TextureDepthMap;
}

bool PlainImposterSphereRenderer::initialize()
{
	OpenGLShadingLanguageRenderer::initialize();
	// prepare the textures
	m_TextureNormalMap = new Texture();
	m_TextureNormalMap->calculateSphereOpacityAndNormalMap(512);
	m_TextureDepthMap = new Texture();
	m_TextureDepthMap->calculateDepthMap(512,512);
	return true;
}

bool PlainImposterSphereRenderer::bindProgramAndParams()
{
	if(!isInitialized())
	{
		return false;
	}

	m_Extensions->glUseProgram(m_Program);

	if(OpenGL_Viewer::CHECK_GL_ERROR())
	{
		return false;
	}

	// get parameter handles
	m_MainColorParam = m_Extensions->glGetUniformLocation(m_Program,"maincolor");
	m_NormalMapParam = m_Extensions->glGetUniformLocation(m_Program,"normalmap");
	m_DepthMapParam  = m_Extensions->glGetUniformLocation(m_Program,"depthmap");
	//cgGLSetTextureParameter(m_NormalMapParam, m_TextureNormalMap->getTextureID());
	//cgGLSetTextureParameter(m_DepthMapParam, m_TextureDepthMap->getTextureID());
	//cgGLEnableTextureParameter(m_NormalMapParam);
	//cgGLEnableTextureParameter(m_DepthMapParam);
	m_Extensions->glUniform4f(m_MainColorParam,1.0, 0.0, 0.0, 1.0); // red
	return true;
}

bool PlainImposterSphereRenderer::unbindProgramAndParams()
{
	m_Extensions->glUseProgram(0);   // 0 disables the shale and enables fixed pipeline rendering.
	return true;
}

const char* PlainImposterSphereRenderer::getVertexProgram()
{
	return vertexShader;
}

const char* PlainImposterSphereRenderer::getFragmentProgram()
{
	return fragmentShader;
}

const char* PlainImposterSphereRenderer::vertexShader =
{
	"void main()\n"
	"{\n"
	"       gl_Position = gl_Vertex;\n"
	"       //      float4 positions[4] = {float4(-1,-1,0, 0.1), float4(1,-1,0, 0.6), float4(1,1,0, 0.1), float4(-1,1,0, 0.1)};\n"
	"       vec4 positions[4] = {vec4(-1,-1,0,0), vec4(1,-1,0,0), vec4(1,1,0,0), vec4(-1,1,0,0)};\n"
	"       \n"
	"       // float3 offset = positions[IN.center.w].xyz;\n"
	"       vec4 offset = positions[gl_Position[3]];\n"
	"       \n"
	"       // IN.center.w = 1.0;\n"
	"       gl_Position[3] = 1.0;\n"
	"       \n"
	"       // float radius = IN.color.w;\n"
	"       //float radius = gl_Color[3];\n"
	"       \n"
	"       //float4 eye_space_offset = radius * float4(offset, 0.0);\n"
	"       //vec4 eye_space_offset = radius * offset;\n"
	"       vec4 eye_space_offset = offset;\n"
	"       \n"
	"       // float4 object_space_offset = mul(ModelViewInverse, eye_space_offset);\n"
	"       vec4 object_space_offset = gl_ModelViewMatrixInverse * eye_space_offset;\n"
	"       \n"
	"       //    IN.center.xyz = IN.center.xyz + object_space_offset.xyz;\n"
	"       gl_Position.xyz = gl_Position.xyz + object_space_offset.xyz;\n"
	"       \n"
	"       // OUT.HPOS = mul(ModelViewProj, IN.center);\n"
	"       gl_Position = gl_ModelViewProjectionMatrix * gl_Position;\n"
	"       \n"
	"}\n"
};

const char* PlainImposterSphereRenderer::fragmentShader =
{
	"void main()"
	"{"
	"    gl_FragColor = vec4(0.4,0.4,0.8,1.0);"
	"}"
};
