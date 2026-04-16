// ImposterSphereRenderer.cpp: implementation of the ImposterSphereRenderer class.

#include <GL/glew.h>
#include <cstdlib>
#include <ObjectRenderer/Sphere/FunctionImposterSphereRenderer.h>
#include <ObjectRenderer/Sphere/ImposterSphereRenderer.h>
#include <ObjectRenderer/Sphere/PlainImposterSphereRenderer.h>
#include <ObjectRenderer/TransformationParameters.h>
#include <GL/glew.h>

using namespace std;
using namespace ObjectRendererLibrary;

ImposterSphereRenderer::ImposterSphereRenderer()
{
	m_Vertices = 0;
	m_Colors = 0;
	m_VertexBuffer = 0;
	m_ColorBuffer = 0;
	m_FunctionImposterSphereRenderer = 0;
	m_PlainImposterSphereRenderer = 0;
}

ImposterSphereRenderer::~ImposterSphereRenderer()
{
	deleteData();
	delete m_FunctionImposterSphereRenderer;
	m_FunctionImposterSphereRenderer = 0;
	delete m_PlainImposterSphereRenderer;
	m_PlainImposterSphereRenderer = 0;
}

void ImposterSphereRenderer::deleteData()
{
	if(m_Vertices)
	{
		delete []m_Vertices;
		m_Vertices = 0;
	}

	if(m_Colors)
	{
		delete []m_Colors;
		m_Colors = 0;
	}

	m_SphereData.clear();
	m_NumberOfSpheres = 0;

	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_ColorBuffer);
}

bool ImposterSphereRenderer::initRenderer()
{
	// Extension availability handled by GLEW (initialized in RenderAreaWidgetBase)

	m_FunctionImposterSphereRenderer = new FunctionImposterSphereRenderer();
	m_PlainImposterSphereRenderer = new PlainImposterSphereRenderer();

	if(!m_FunctionImposterSphereRenderer->initialize() || !m_PlainImposterSphereRenderer->initialize())
	{
		delete m_FunctionImposterSphereRenderer;
		delete m_PlainImposterSphereRenderer;
		return false;
	}

	return true;
}

void ImposterSphereRenderer::clearObjects()
{
	m_Dirty = true;
	deleteData();
}

bool ImposterSphereRenderer::fillUpBuffers()
{

	if(!m_Vertices || !m_Colors)
	{
		return false;
	}

	if(m_SphereData.size() == 0)
	{
		return false;    // nothing to add to buffers!
	}

	if((m_SphereData.size() / 7)*7 !=  m_SphereData.size())
	{
		return false;    // something wrong in data!
	}

	for(int i=0; i<m_SphereData.size()/7; i++)
	{
		m_Vertices[i*16+ 0] = m_SphereData[i*7+0]; //x
		m_Vertices[i*16+ 1] = m_SphereData[i*7+1]; //y
		m_Vertices[i*16+ 2] = m_SphereData[i*7+2]; //z
		m_Vertices[i*16+ 3] = 0;                   //index of vertex in quad
		m_Vertices[i*16+ 4] = m_SphereData[i*7+0];
		m_Vertices[i*16+ 5] = m_SphereData[i*7+1];
		m_Vertices[i*16+ 6] = m_SphereData[i*7+2];
		m_Vertices[i*16+ 7] = 1;
		m_Vertices[i*16+ 8] = m_SphereData[i*7+0];
		m_Vertices[i*16+ 9] = m_SphereData[i*7+1];
		m_Vertices[i*16+10] = m_SphereData[i*7+2];
		m_Vertices[i*16+11] = 2;
		m_Vertices[i*16+12] = m_SphereData[i*7+0];
		m_Vertices[i*16+13] = m_SphereData[i*7+1];
		m_Vertices[i*16+14] = m_SphereData[i*7+2];
		m_Vertices[i*16+15] = 3;
		m_Colors[i*16+ 0] = m_SphereData[i*7+4]; // red
		m_Colors[i*16+ 1] = m_SphereData[i*7+5]; // green
		m_Colors[i*16+ 2] = m_SphereData[i*7+6]; // blue
		m_Colors[i*16+ 3] = m_SphereData[i*7+3]; // radius
		m_Colors[i*16+ 4] = m_SphereData[i*7+4];
		m_Colors[i*16+ 5] = m_SphereData[i*7+5];
		m_Colors[i*16+ 6] = m_SphereData[i*7+6];
		m_Colors[i*16+ 7] = m_SphereData[i*7+3];
		m_Colors[i*16+ 8] = m_SphereData[i*7+4];
		m_Colors[i*16+ 9] = m_SphereData[i*7+5];
		m_Colors[i*16+10] = m_SphereData[i*7+6];
		m_Colors[i*16+11] = m_SphereData[i*7+3];
		m_Colors[i*16+12] = m_SphereData[i*7+4];
		m_Colors[i*16+13] = m_SphereData[i*7+5];
		m_Colors[i*16+14] = m_SphereData[i*7+6];
		m_Colors[i*16+15] = m_SphereData[i*7+3];
	}

	return true;
}

bool ImposterSphereRenderer::prepareBuffers()
{
	if(m_SphereData.size() == 0)
	{
		return false;    // nothing to add to buffers!
	}

	int s = m_SphereData.size();

	if((m_SphereData.size() / 7)*7 !=  m_SphereData.size())
	{
		return false;    // something wrong in data!
	}

	// createBuffers();
	if(m_Vertices)
	{
		delete []m_Vertices;
		m_Vertices = 0;
	}

	if(m_Colors)
	{
		delete []m_Colors;
		m_Colors = 0;
	}

	m_Vertices = new float[m_NumberOfSpheres*16];
	m_Colors = new float[m_NumberOfSpheres*16];

	if(!fillUpBuffers())
	{
		return false;
	}

	m_SphereData.clear(); // dont need this anymore
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfSpheres*16*sizeof(float), m_Vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &m_ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfSpheres*16*sizeof(float), m_Colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//if( m_Vertices) { delete []m_Vertices; m_Vertices = 0; }
	//if( m_Colors ) { delete []m_Colors; m_Colors = 0; }
	m_Dirty = false;
	return true;
}

// client states must be set already. All data must be set up!
void ImposterSphereRenderer::renderOnce()
{
	/*	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glVertexPointer(4, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
		glColorPointer(4, GL_FLOAT, 0, 0);

		glDrawArrays(GL_QUADS, 0, m_NumberOfSpheres*4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);*/
	glColor4f(1,0,0,1);
	glBegin(GL_QUADS);
	glVertex4f(0,0,0,0);
	glVertex4f(1,1,0,1);
	glVertex4f(2,0,0,2);
	glVertex4f(3,0,1,3);
	glEnd();
}

void ImposterSphereRenderer::render(TransformationParameters* transformationParameters)
{
	if(m_Dirty)
	{
		if(!prepareBuffers())
		{
			return;
		}
	}

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);

	if(m_RenderFunctionOnSurface)
	{
		if(!m_FunctionImposterSphereRenderer || !m_FunctionImposterSphereRenderer->isInitialized())
		{
			return;
		}

		m_FunctionImposterSphereRenderer->bindProgramAndParams();
	}
	else
	{
		if(!m_PlainImposterSphereRenderer || !m_PlainImposterSphereRenderer->isInitialized())
		{
			return;
		}

		m_PlainImposterSphereRenderer->bindProgramAndParams();
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_COLOR_SUM);
	glSecondaryColor3f(0.,0.,1.);
	glEnableClientState(GL_VERTEX_ARRAY);

	if(! transformationParameters->useRandomColors())
	{
		glEnableClientState(GL_COLOR_ARRAY);
	}

	srand(255); // needed to call rand to set colors for SS, CHAIN and PROTEINS

	if(transformationParameters && transformationParameters->hasTransformations())
	{
		for(int i=0; i<transformationParameters->getNumberOfTransformations(); i++)
		{
			GLfloat trans[16];

			if(!transformationParameters->getTransformations(trans, i))
			{
				continue;
			}

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(trans);

			if(transformationParameters->useRandomColors())
			{
				float blue = (float)(rand() / (RAND_MAX + 1.0f));
				float red = (float)(rand() / (RAND_MAX + 1.0f));
				float green = (float)(rand() / (RAND_MAX + 1.0f));
				glColor4f(red, blue, green, transformationParameters->getAverageRadius());
			}

			renderOnce();
			glPopMatrix();
		}
	}
	else
	{
		renderOnce();
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	if(! transformationParameters->useRandomColors())
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

	(m_RenderFunctionOnSurface) ? m_FunctionImposterSphereRenderer->unbindProgramAndParams() : m_PlainImposterSphereRenderer->unbindProgramAndParams();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_SUM);
	glPopAttrib();
}
