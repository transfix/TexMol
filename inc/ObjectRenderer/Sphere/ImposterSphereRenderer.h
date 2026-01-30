// ImposterSphereRenderer.h: interface for the ImposterSphereRenderer class.

#ifndef CCV_IMPOSTER_SPHERE_RENDERER_H
#define CCV_IMPOSTER_SPHERE_RENDERER_H

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

#include <ObjectRenderer/Sphere/SphereRendererBase.h>

class MyExtensions;

namespace ObjectRendererLibrary
{
	class FunctionImposterSphereRenderer;
	class PlainImposterSphereRenderer;
	class TransformationParameters;

	class ImposterSphereRenderer : public SphereRendererBase
	{
		public:
			ImposterSphereRenderer();
			virtual ~ImposterSphereRenderer();

			virtual bool initRenderer();
			virtual void clearObjects();
			virtual void render(TransformationParameters* transformationParameters);

		protected:
			void renderOnce();
			void deleteData();
			bool prepareBuffers();
			bool fillUpBuffers();
			float* m_Vertices;
			float* m_Colors;
			GLuint m_VertexBuffer;
			GLuint m_ColorBuffer;
			PlainImposterSphereRenderer* m_PlainImposterSphereRenderer;
			FunctionImposterSphereRenderer* m_FunctionImposterSphereRenderer;
			MyExtensions* m_Extensions; // manage extensions in this neat way!
	};
};

#endif
