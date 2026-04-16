// ImposterSphereRenderer.h: interface for the ImposterSphereRenderer class.

#ifndef CCV_IMPOSTER_SPHERE_RENDERER_H
#define CCV_IMPOSTER_SPHERE_RENDERER_H

#include <GL/glew.h>

#include <ObjectRenderer/Sphere/SphereRendererBase.h>

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
	};
};

#endif
