// SphereRenderer.h: interface for the SphereRenderer class.

#ifndef CCV_SPHERE_RENDERER_H
#define CCV_SPHERE_RENDERER_H

#include <ObjectRenderer/ObjectRenderer.h>

namespace ObjectRendererLibrary
{
	class SphereRendererBase;
	class TransformationParameters;

	class SphereRenderer : public ObjectRenderer
	{
		public:
			SphereRenderer();
			virtual ~SphereRenderer();
			bool initRenderer();
			virtual void clearObjects();
			virtual unsigned int getNumberOfObjects();
			virtual void render(TransformationParameters* transformationParameters);
			virtual bool addSphere(float x, float y, float z, float r, float red, float green, float blue);

		protected:
			bool initSphereRenderer();
			SphereRendererBase* m_SphereRenderer;
	};
};

#endif
