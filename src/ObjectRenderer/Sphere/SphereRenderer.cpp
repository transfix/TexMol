// SphereRenderer.cpp: implementation of the SphereRenderer class.

#include <ObjectRenderer/Sphere/ImposterSphereRenderer.h>
#include <ObjectRenderer/Sphere/OpenGLSphereRenderer.h>
#include <ObjectRenderer/Sphere/SphereRenderer.h>

using namespace ObjectRendererLibrary;

SphereRenderer::SphereRenderer()
{
	m_SphereRenderer = 0;
}

SphereRenderer::~SphereRenderer()
{
	delete m_SphereRenderer;
	m_SphereRenderer = 0;
}

bool SphereRenderer::initRenderer()
{
	if(!initSphereRenderer())
	{
		return false;
	}

	return true;
}

bool SphereRenderer::initSphereRenderer()
{
	m_SphereRenderer = new ImposterSphereRenderer;

	if(m_SphereRenderer->initRenderer())
	{
		m_Initialized = true;
		return true;
	}

	delete m_SphereRenderer;
	m_SphereRenderer = 0;
	m_SphereRenderer = new OpenGLSphereRenderer;

	if(m_SphereRenderer->initRenderer())
	{
		m_Initialized = true;
		return true;
	}

	delete m_SphereRenderer;
	m_SphereRenderer = 0;
	m_Initialized = false;
	return false;
}

void SphereRenderer::clearObjects()
{
	if(m_SphereRenderer)
	{
		m_SphereRenderer->clearObjects();
	}
}

unsigned int SphereRenderer::getNumberOfObjects()
{
	if(m_SphereRenderer)
	{
		return m_SphereRenderer->getNumberOfObjects();
	}

	return 0;
}

bool SphereRenderer::addSphere(float x, float y, float z, float r, float red, float green, float blue)
{
	if(m_SphereRenderer)
	{
		return m_SphereRenderer->addSphere(x,y,z,r,red,green,blue);
	}

	return false;
}

void SphereRenderer::render(TransformationParameters* transformationParameters)
{
	if(m_SphereRenderer)
	{
		m_SphereRenderer->render(transformationParameters);
	}
}
