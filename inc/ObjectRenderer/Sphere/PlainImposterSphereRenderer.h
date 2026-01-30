#ifndef CVC_TEXMOL_PLAIN_IMPOSTER_SPHERE_RENDERER_H
#define CVC_TEXMOL_PLAIN_IMPOSTER_SPHERE_RENDERER_H

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__APPLE__)
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

#include <ObjectRenderer/OpenGLShadingLanguageRenderer.h>

namespace ObjectRendererLibrary
{
	class Texture;

	class PlainImposterSphereRenderer : public OpenGLShadingLanguageRenderer
	{
		public:
			PlainImposterSphereRenderer(MyExtensions* extensions);
			virtual ~PlainImposterSphereRenderer();
			bool bindProgramAndParams();
			bool unbindProgramAndParams();
			virtual bool initialize();
			virtual const char* getVertexProgram();
			virtual const char* getFragmentProgram();

		protected:
			GLint m_MainColorParam;
			GLint m_NormalMapParam;
			GLint m_DepthMapParam;
			Texture* m_TextureNormalMap;
			Texture* m_TextureDepthMap;
			static const char* vertexShader;
			static const char* fragmentShader;
	};
};

#endif
