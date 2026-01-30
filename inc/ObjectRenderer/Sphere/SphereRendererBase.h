// SphereRendererBase.h: interface for the SphereRendererBase class.

#ifndef CCV_SPHERE_RENDERER_BASE_H
#define CCV_SPHERE_RENDERER_BASE_H

#include <vector>

namespace ObjectRendererLibrary
{
	class TransformationParameters;

	class SphereRendererBase
	{
		public:
			SphereRendererBase();
			virtual ~SphereRendererBase();
			virtual bool initRenderer() = 0;
			virtual void clearObjects() = 0;
			virtual unsigned int getNumberOfObjects();
			virtual void render(TransformationParameters* transformationParameters) = 0;
			virtual bool addSphere(float x, float y, float z, float r, float red, float green, float blue);
			virtual void renderFunctionOnSurface(bool enable);

		protected:
			std::vector<float> m_SphereData; // store x,y, z, rad, r, g, b in a sequence.
			bool m_RenderFunctionOnSurface;
			unsigned int m_NumberOfSpheres;
			bool m_Dirty;
	};
};

#endif
