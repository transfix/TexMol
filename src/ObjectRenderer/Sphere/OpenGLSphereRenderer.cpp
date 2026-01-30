// OpenGLSphereRenderer.cpp: implementation of the OpenGLSphereRenderer class.

#include <math.h>
#include <ObjectRenderer/Sphere/OpenGLSphereRenderer.h>
#include <ObjectRenderer/TransformationParameters.h>
#include <stdio.h>
#include <stdlib.h>

#include <OpenGL_Viewer/MyExtensions.h>

using namespace ObjectRendererLibrary;

OpenGLSphereRenderer::OpenGLSphereRenderer()
{
	m_NumberOfLevels = NUMBER_OF_LEVELS;
	m_Dirty = true;
	m_Extensions = 0;
	m_Vertices = 0;
	m_Colors = 0;
	m_Normals = 0;
	m_NumberOfVertices = 0;
	m_SphereData.clear();
}

OpenGLSphereRenderer::~OpenGLSphereRenderer()
{
	deleteData();
	delete m_Extensions;
	m_Extensions = 0; // delete this last!
}

void OpenGLSphereRenderer::deleteData()
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

	if(m_Normals)
	{
		delete []m_Normals;
		m_Normals = 0;
	}

	m_SphereData.clear();

	if(m_Extensions)
	{
		m_Extensions->glDeleteBuffers(1, &m_VertexBuffer);
		m_Extensions->glDeleteBuffers(1, &m_NormalBuffer);
		m_Extensions->glDeleteBuffers(1, &m_ColorBuffer);
	}
}

bool OpenGLSphereRenderer::checkForVertexBuffers()
{
	delete m_Extensions;
	m_Extensions = 0;
	m_Extensions = new MyExtensions();

	if(!m_Extensions->initExtensions(
				"GL_VERSION_1_5"))  // if this version is defined, then I must get all the vertex buffer function calls !
	{
		delete m_Extensions;

		if(!m_Extensions->initExtensions(
					"GL_VERSION_1_1"))  // for drawarrays
		{
			return false; // sucky opengl.
		}
	}

	return true;
}

bool OpenGLSphereRenderer::initRenderer()
{
	m_Dirty = true;
	return checkForVertexBuffers();
}

void OpenGLSphereRenderer::clearObjects()
{
	m_Dirty = true;
	deleteData();
	m_NumberOfSpheres = 0;
}

void OpenGLSphereRenderer::setNumberOfLevels(unsigned int numberOfLevels)
{
	if(numberOfLevels >= 0)
	{
		m_NumberOfLevels = numberOfLevels;
	}

	m_Dirty = true;
}

// Draw a triangulation
void OpenGLSphereRenderer::GenerateCurve(float* v1, float* v2, float* v3, int s, float* sphereVertices, float* sphereNormals, int* curPos)
{
	if(s == 0)
	{
		sphereNormals[(*curPos)+0] = sphereVertices[(*curPos)+0] = v1[0];
		sphereNormals[(*curPos)+1] = sphereVertices[(*curPos)+1] = v1[1];
		sphereNormals[(*curPos)+2] = sphereVertices[(*curPos)+2] = v1[2];
		sphereNormals[(*curPos)+3] = sphereVertices[(*curPos)+3] = v2[0];
		sphereNormals[(*curPos)+4] = sphereVertices[(*curPos)+4] = v2[1];
		sphereNormals[(*curPos)+5] = sphereVertices[(*curPos)+5] = v2[2];
		sphereNormals[(*curPos)+6] = sphereVertices[(*curPos)+6] = v3[0];
		sphereNormals[(*curPos)+7] = sphereVertices[(*curPos)+7] = v3[1];
		sphereNormals[(*curPos)+8] = sphereVertices[(*curPos)+8] = v3[2];
		(*curPos) += 9;
		//glNormal3fv(v1); glVertex3fv(v1);
		//glNormal3fv(v2); glVertex3fv(v2);
		//glNormal3fv(v3); glVertex3fv(v3);
	}
	else
	{
		float v12[3], v23[3], v13[3], l;
		v12[0] = (v1[0] + v2[0]);
		v12[1] = (v1[1] + v2[1]);
		v12[2] = (v1[2] + v2[2]);
		l = (float) sqrt(v12[0]*v12[0] + v12[1]*v12[1] + v12[2]*v12[2]);
		v12[0] /= l;
		v12[1] /= l;
		v12[2] /= l;
		v23[0] = (v2[0] + v3[0]);
		v23[1] = (v2[1] + v3[1]);
		v23[2] = (v2[2] + v3[2]);
		l = (float) sqrt(v23[0]*v23[0] + v23[1]*v23[1] + v23[2]*v23[2]);
		v23[0] /= l;
		v23[1] /= l;
		v23[2] /= l;
		v13[0] = (v1[0] + v3[0]);
		v13[1] = (v1[1] + v3[1]);
		v13[2] = (v1[2] + v3[2]);
		l = (float) sqrt(v13[0]*v13[0] + v13[1]*v13[1] + v13[2]*v13[2]);
		v13[0] /= l;
		v13[1] /= l;
		v13[2] /= l;
		GenerateCurve(v1, v12, v13, s-1, sphereVertices, sphereNormals, curPos);
		GenerateCurve(v2, v23, v12, s-1, sphereVertices, sphereNormals, curPos);
		GenerateCurve(v3, v13, v23, s-1, sphereVertices, sphereNormals, curPos);
		GenerateCurve(v12, v23, v13, s-1, sphereVertices, sphereNormals, curPos);
	}
}

bool OpenGLSphereRenderer::getSphere(int level, float* sphereVertices, float* sphereNormals)
{
	if(level < 0 || (!sphereVertices) || (!sphereNormals))
	{
		return false;
	}

	int curPos = 0;

	for(int i=0; i<20; i++)
	{
		GenerateCurve(&vdata[faces[i][1]][0], &vdata[faces[i][0]][0], &vdata[faces[i][2]][0], level, sphereVertices, sphereNormals, &curPos);
	}

	return true;
}

int OpenGLSphereRenderer::getNumberOfVerticesPerSphere(int numLevels)
{
	int factor = 1;

	for(int i=0; i<numLevels; i++)
	{
		factor *= 4;
	}

	return factor*20*3; // the sphere subdivision code starts with 20 triangles and recurses 4 times.
}

bool OpenGLSphereRenderer::fillUpBuffers()
{
	if(!m_Vertices || !m_Colors || !m_Normals)
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

	// generate a single unit sphere
	int numVertPerSphere = getNumberOfVerticesPerSphere(m_NumberOfLevels);
	float* sphereVertices = new float[numVertPerSphere*3];
	float* sphereNormals = new float[numVertPerSphere*3];

	if(!getSphere(m_NumberOfLevels, sphereVertices, sphereNormals))
	{
		return false;
	}

	for(int i=0; i<m_SphereData.size()/7; i++)
	{
		for(int j=0; j<numVertPerSphere; j++)
		{
			m_Vertices[(i*numVertPerSphere+j)*3+0] = sphereVertices[j*3+0]*m_SphereData[i*7+3] + m_SphereData[i*7+0]; // mult by radius, add xcenter
			m_Vertices[(i*numVertPerSphere+j)*3+1] = sphereVertices[j*3+1]*m_SphereData[i*7+3] + m_SphereData[i*7+1]; // mult by radius, add ycenter
			m_Vertices[(i*numVertPerSphere+j)*3+2] = sphereVertices[j*3+2]*m_SphereData[i*7+3] + m_SphereData[i*7+2]; // mult by radius, add zcenter
			m_Colors[(i*numVertPerSphere+j)*3+0] = m_SphereData[i*7+4];
			m_Colors[(i*numVertPerSphere+j)*3+1] = m_SphereData[i*7+5];
			m_Colors[(i*numVertPerSphere+j)*3+2] = m_SphereData[i*7+6];
			m_Normals[(i*numVertPerSphere+j)*3+0] = sphereNormals[j*3+0];
			m_Normals[(i*numVertPerSphere+j)*3+1] = sphereNormals[j*3+1];
			m_Normals[(i*numVertPerSphere+j)*3+2] = sphereNormals[j*3+2];
		}
	}

	delete []sphereVertices;
	sphereVertices = 0;
	delete []sphereNormals;
	sphereNormals = 0;
	return true;
}

bool OpenGLSphereRenderer::prepareBuffers()
{
	if(m_SphereData.size() == 0)
	{
		return false;    // nothing to add to buffers!
	}

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

	if(m_Normals)
	{
		delete []m_Normals;
		m_Normals = 0;
	}

	m_NumberOfVertices = getNumberOfVerticesPerSphere(m_NumberOfLevels) * m_NumberOfSpheres;
	m_Vertices = new float[m_NumberOfVertices*3];
	m_Colors = new float[m_NumberOfVertices*3];
	m_Normals = new float[m_NumberOfVertices*3];

	if(!fillUpBuffers())
	{
		return false;
	}

	m_SphereData.clear(); // dont need this anymore

	if(m_Extensions)
	{
		m_Extensions->glGenBuffers(1, &m_VertexBuffer);
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		m_Extensions->glBufferData(GL_ARRAY_BUFFER, m_NumberOfVertices*3*sizeof(float), m_Vertices, GL_STATIC_DRAW);
		m_Extensions->glGenBuffers(1, &m_ColorBuffer);
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
		m_Extensions->glBufferData(GL_ARRAY_BUFFER, m_NumberOfVertices*3*sizeof(float), m_Colors, GL_STATIC_DRAW);
		m_Extensions->glGenBuffers(1, &m_NormalBuffer);
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
		m_Extensions->glBufferData(GL_ARRAY_BUFFER, m_NumberOfVertices*3*sizeof(float), m_Normals, GL_STATIC_DRAW);
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, 0);

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

		if(m_Normals)
		{
			delete []m_Normals;
			m_Normals = 0;
		}
	}
	else
	{
		// no need to prepare anything!
	}

	m_Dirty = false;
	return true;
}

// client states must be set already. All data must be set up!
void OpenGLSphereRenderer::renderOnce()
{
	if(m_Extensions)
	{
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
		glColorPointer(3, GL_FLOAT, 0, 0);
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
		glNormalPointer(GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, m_NumberOfVertices);
		m_Extensions->glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		glVertexPointer(3, GL_FLOAT, 0, m_Vertices);
		glNormalPointer(GL_FLOAT, 0, m_Normals);
		glColorPointer(3, GL_FLOAT, 0, m_Colors);
		glDrawArrays(GL_TRIANGLES, 0, m_NumberOfVertices);
	}
}

//   Keep NumberOfLevels levels of spheres. depending on object size, choose a level. Render using buffers if possible
void OpenGLSphereRenderer::render(TransformationParameters* transformationParameters)
{
	if(m_Dirty)
	{
		if(!prepareBuffers())
		{
			return;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

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
			renderOnce();
			glPopMatrix();
		}
	}
	else
	{
		renderOnce();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
