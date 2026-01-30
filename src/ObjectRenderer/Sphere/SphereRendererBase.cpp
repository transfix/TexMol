// SphereRendererBase.cpp: implementation of the SphereRendererBase class.

#include <ObjectRenderer/Sphere/SphereRendererBase.h>

using namespace ObjectRendererLibrary;

SphereRendererBase::SphereRendererBase()
{
	m_NumberOfSpheres = 0;
	m_RenderFunctionOnSurface = false;
}

SphereRendererBase::~SphereRendererBase()
{
	m_NumberOfSpheres = 0;
}

unsigned int SphereRendererBase::getNumberOfObjects()
{
	return m_NumberOfSpheres;
}

bool SphereRendererBase::addSphere(float x, float y, float z, float r, float red, float green, float blue)
{
	m_SphereData.push_back(x);
	m_SphereData.push_back(y);
	m_SphereData.push_back(z);
	m_SphereData.push_back(r);
	m_SphereData.push_back(red);
	m_SphereData.push_back(green);
	m_SphereData.push_back(blue);
	m_Dirty = true;
	m_NumberOfSpheres++;
	return true;
}

void SphereRendererBase::renderFunctionOnSurface(bool enable)
{
	m_RenderFunctionOnSurface = enable;
}
