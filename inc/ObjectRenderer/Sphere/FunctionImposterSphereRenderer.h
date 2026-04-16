#ifndef CVC_TEXMOL_FUNCTION_IMPOSTER_SPHERE_RENDERER_H
#define CVC_TEXMOL_FUNCTION_IMPOSTER_SPHERE_RENDERER_H

#include <ObjectRenderer/OpenGLShadingLanguageRenderer.h>

namespace ObjectRendererLibrary
{
	class FunctionImposterSphereRenderer  : public OpenGLShadingLanguageRenderer
	{
		public:
				FunctionImposterSphereRenderer();
			bool bindProgramAndParams();
			bool bindMatrices();
			bool unbindProgramAndParams();
			virtual ~FunctionImposterSphereRenderer();
			virtual const char* getVertexProgram();
			virtual const char* getFragmentProgram();

		protected:
			static const char* vertexShader;
			static const char* fragmentShader;
	};
};

#endif
