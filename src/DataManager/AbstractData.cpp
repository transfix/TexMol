/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Utility/utility.h>
#include <DataManager/AbstractData.h>
#include <DataManager/DataManager.h>
#include <DataManager/ObjectBoundingBoxOctree.h>
#include <DataManager/Rover3DWidget.h>
#include <GeometryRenderer/GeometryRenderer.h>
#include <OpenGL_Viewer/ViewFrustrum.h>
#include <TexMol/RendererSet.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Quaternion.h>
//Added by qt3to4:
#include <QMouseEvent>

using CCVOpenGLMath::Matrix;

AbstractData::AbstractData(DataManager* dataManager)
{
	m_DataManager = dataManager;
	m_FileName = "";
	m_Rover3DWidget = new Rover3DWidget();
	m_XAxisHandler = new Axis3DHandler(m_Rover3DWidget, Axis3DHandler::XAxis);
	m_YAxisHandler = new Axis3DHandler(m_Rover3DWidget, Axis3DHandler::YAxis);
	m_ZAxisHandler = new Axis3DHandler(m_Rover3DWidget, Axis3DHandler::ZAxis);
	for (int i=0; i<10; i++)
	{
		renderEnabled[i] = false;
	}
	m_UniqueDataSetId = -1;
	m_Transformation.reset();
	m_Frame = 0;
	m_UseOctree = false;
	m_ObjectBoundingBoxOctree = 0;
	m_ReplicationVisible = 0;
	m_ProjectedAreaCutOff = 0;
}

AbstractData::~AbstractData()
{
	clearReplicationMatrices();
	delete m_ObjectBoundingBoxOctree;
	m_ObjectBoundingBoxOctree = 0;
	delete []m_ReplicationVisible;
	m_ReplicationVisible = 0;
	delete m_Rover3DWidget;
	m_Rover3DWidget = 0;
	delete m_XAxisHandler;
	m_XAxisHandler = 0;
	delete m_YAxisHandler;
	m_YAxisHandler = 0;
	delete m_ZAxisHandler;
	m_ZAxisHandler = 0;
}

void AbstractData::associateDataManager(DataManager* dataManager)
{
	m_DataManager = dataManager;
}

void AbstractData::setFileName(QString fileName)
{
	m_FileName = fileName;
}

QString AbstractData::getFileName()
{
	return m_FileName;
}

bool AbstractData::isEnabled(int rendererId)
{
	return renderEnabled[rendererId];
}

void AbstractData::renderEnabledSlot(bool renderMe)
{
	renderEnabled[m_DataManager->getCurrentRenderer()] = renderMe;
	m_DataManager->update();
}

bool AbstractData::update()
{
	if (m_DataManager)
	{
		return m_DataManager->update();
	}
	return false;
}

void AbstractData::beginRender()
{
	glGetIntegerv(GL_MATRIX_MODE, &originalMmatrixMode);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(m_Transformation.getMatrix());
}

void AbstractData::endRender(bool renderDataBoundingBox)
{
	if (renderDataBoundingBox)
	{
		float min[3], max[3];
		if (!getDataMinMax(min, max))
		{
			return;
		}
		float backgroundColor[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, backgroundColor);
		m_Rover3DWidget->setColor(1.0f-backgroundColor[0], 1.0f-backgroundColor[1], 1.0f-backgroundColor[2]);
		m_Rover3DWidget->setBoundary(min, max);
		Geometry* wireCubes = m_Rover3DWidget->getWireCubes();
		Geometry* axes = m_Rover3DWidget->getAxes();
		glPushAttrib(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		CCVTexMolGeometryRenderer::GeometryRenderer* geometryRenderer = new CCVTexMolGeometryRenderer::GeometryRenderer();
		geometryRenderer->render(wireCubes);
		geometryRenderer->render(axes);
		glPopAttrib();
		delete geometryRenderer;
	}
	glPopMatrix();
	glMatrixMode(originalMmatrixMode);
}

void AbstractData::setTransformation(CCVOpenGLMath::Matrix m)
{
	m_Transformation = m;
}

void AbstractData::addTransformation(CCVOpenGLMath::Matrix m, float scale)
{
	m_Transformation.preMultiplication(m);
}

Matrix AbstractData::getTransformation()
{
	return m_Transformation;
}

void AbstractData::addPan(CCVOpenGLMath::Vector v)
{
	m_Transformation.preMultiplication(Matrix::translation(v));
}

void AbstractData::addRotation(CCVOpenGLMath::Matrix r)
{
	m_Transformation.preMultiplication(r);
}

void AbstractData::addRotation(CCVOpenGLMath::Matrix r, CCVOpenGLMath::Vector center)
{
        m_Transformation.postMultiplication( CCVOpenGLMath::Matrix::translation( center ) );
	m_Transformation.postMultiplication(r);
        m_Transformation.postMultiplication( CCVOpenGLMath::Matrix::translation( -center ) );
}

void AbstractData::addRotation(int axis, float angle)
{
	if (axis == 0)
	{
		m_Transformation.preMultiplication(Matrix::rotationX(angle));
	}
	if (axis == 1)
	{
		m_Transformation.preMultiplication(Matrix::rotationY(angle));
	}
	if (axis == 2)
	{
		m_Transformation.preMultiplication(Matrix::rotationZ(angle));
	}
}

void AbstractData::addScale(float scale)
{
	m_Transformation.preMultiplication(Matrix::scale(scale, scale, scale));
}

bool AbstractData::getDataMinMax(float* min, float* max)
{
	if (!min || !max)
	{
		return false;
	}
	if (!getMinMax(min, max))
	{
		return false;
	}
	// add gap, especially useful for thin objects
	min[0] -= OBJECT_PADDING;
	max[0] += OBJECT_PADDING;
	min[1] -= OBJECT_PADDING;
	max[1] += OBJECT_PADDING;
	min[2] -= OBJECT_PADDING;
	max[2] += OBJECT_PADDING;
	return true;
}

bool AbstractData::mousePress3DEvent(View* view, QMouseEvent* e)
{
	if (m_XAxisHandler->mousePress3DEvent(view, e))
	{
		return true;
	}
	if (m_YAxisHandler->mousePress3DEvent(view, e))
	{
		return true;
	}
	if (m_ZAxisHandler->mousePress3DEvent(view, e))
	{
		return true;
	}
	return false;
}

bool AbstractData::mouseRelease3DEvent(View* view, QMouseEvent* e)
{
	if (m_XAxisHandler->mouseRelease3DEvent(view, e))
	{
		return true;
	}
	if (m_YAxisHandler->mouseRelease3DEvent(view, e))
	{
		return true;
	}
	if (m_ZAxisHandler->mouseRelease3DEvent(view, e))
	{
		return true;
	}
	return false;
}

bool AbstractData::mouseDoubleClick3DEvent(View* view, QMouseEvent* e)
{
	if (m_XAxisHandler->mouseDoubleClick3DEvent(view, e))
	{
		return true;
	}
	if (m_YAxisHandler->mouseDoubleClick3DEvent(view, e))
	{
		return true;
	}
	if (m_ZAxisHandler->mouseDoubleClick3DEvent(view, e))
	{
		return true;
	}
	return false;
}

bool AbstractData::mouseMove3DEvent(View* view, QMouseEvent* e)
{
	if (m_XAxisHandler->mouseMove3DEvent(view, e))
	{
		return true;
	}
	if (m_YAxisHandler->mouseMove3DEvent(view, e))
	{
		return true;
	}
	if (m_ZAxisHandler->mouseMove3DEvent(view, e))
	{
		return true;
	}
	return false;
}

void AbstractData::roverExploringSlot()
{
}

void AbstractData::roverReleasedSlot()
{
}

void AbstractData::appendReplicationMatrix(CCVOpenGLMath::Matrix* matrix)
{
	m_ReplicationMatrices.push_back(matrix);
}

void AbstractData::clearReplicationMatrices()
{
	{
		std::vector<CCVOpenGLMath::Matrix*>::iterator iter = m_ReplicationMatrices.begin(), end = m_ReplicationMatrices.end();
		while (iter++ != end)
		{
			delete *iter;
		}
		m_ReplicationMatrices.clear();
	}
}

bool AbstractData::readTransformations(const char* filename)
{
	if (!filename)
	{
		return false;
	}
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		return false;
	}
	int numTrans = 0;
	fscanf(fp, "%d\n", &numTrans);
	for (int i=0; i<numTrans; i++)
	{
		float c1x, c1y, c1z, n1x, n1y, n1z;
		float c2x, c2y, c2z, n2x, n2y, n2z;
		if (!fscanf(fp, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
					&c1x, &c1y, &c1z, &n1x, &n1y, &n1z,
					&c2x, &c2y, &c2z, &n2x, &n2y, &n2z))
		{
			break;
		}
		CCVOpenGLMath::Vector old_center(c1x, c1y, c1z, 1);
		CCVOpenGLMath::Vector old_normal(n1x, n1y, n1z, 0);
		CCVOpenGLMath::Vector new_center(c2x, c2y, c2z, 1);
		CCVOpenGLMath::Vector new_normal(n2x, n2y, n2z, 0);
		// shift center to new one.
		// rotate to align quaternions
		// for each mol, we need a predefined center and normal.
		// translate mol_center to input_center
		// now to rotate:
		//     cross product gives axis of rotation
		//     dot product gives the angle
		old_normal.normalize();
		new_normal.normalize();
		CCVOpenGLMath::Vector axis_of_rotation = old_normal.cross(new_normal);
		axis_of_rotation.normalize();
		double angle_of_rotation = acos(new_normal.dot(old_normal));
		float w = cos(angle_of_rotation / 2.0);
		float x = axis_of_rotation[0] * sin(angle_of_rotation / 2.0);
		float y = axis_of_rotation[1] * sin(angle_of_rotation / 2.0);
		float z = axis_of_rotation[2] * sin(angle_of_rotation / 2.0);
		CCVOpenGLMath::Quaternion quaternion(w, x, y, z);
		CCVOpenGLMath::Matrix rotation = quaternion.buildMatrix();
		CCVOpenGLMath::Matrix translation = CCVOpenGLMath::Matrix::translation(new_center - old_center);
		CCVOpenGLMath::Matrix trToOrigin = CCVOpenGLMath::Matrix::translation(-c1x, -c1y, -c1z);
		CCVOpenGLMath::Matrix trFromOrigin = CCVOpenGLMath::Matrix::translation(c1x, c1y, c1z);
		CCVOpenGLMath::Matrix trans = trToOrigin;
		trans = trans.preMultiplication(rotation);
		trans = trans.preMultiplication(trFromOrigin);
		trans = trans.preMultiplication(translation);
		CCVOpenGLMath::Matrix* transformation = new CCVOpenGLMath::Matrix(trans);
		appendReplicationMatrix(transformation);
	}
	fclose(fp);
	return true;
}

void AbstractData::transformPoint(float x, float y, float z, GLdouble* tr_x, GLdouble* tr_y, GLdouble* tr_z)
{
	GLdouble projMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluProject(x,
			   y,
			   z,
			   modelMatrix,
			   projMatrix,
			   viewport,
			   tr_x,
			   tr_y,
			   tr_z);
	return;
	GLfloat modelviewmatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewmatrix);
	float tr_w;
	*tr_x = modelviewmatrix[0]*x + modelviewmatrix[4]*y + modelviewmatrix[8] *z + modelviewmatrix[12];
	*tr_y = modelviewmatrix[1]*x + modelviewmatrix[5]*y + modelviewmatrix[9] *z + modelviewmatrix[13];
	*tr_z = modelviewmatrix[2]*x + modelviewmatrix[6]*y + modelviewmatrix[10]*z + modelviewmatrix[14];
	tr_w  = modelviewmatrix[3]*x + modelviewmatrix[7]*y + modelviewmatrix[11]*z + modelviewmatrix[15];
	(*tr_x) /= tr_w;
	(*tr_y) /= tr_w;
	(*tr_z) /= tr_w;
}

double dist_ad(GLdouble* p1, GLdouble* p2)
{
	return	(p1[0]-p2[0])*(p1[0]-p2[0]) +
			(p1[1]-p2[1])*(p1[1]-p2[1]);
}

double AbstractData::distanceSqFromObject()
{
	float min[3], max[3];
	getMinMax(min, max);
	GLdouble tr_pt1[3];
	transformPoint(min[0], min[1], min[2], &(tr_pt1[0]), &(tr_pt1[1]), &(tr_pt1[2]));
	GLdouble tr_pt2[3];
	transformPoint(max[0], max[1], max[2], &(tr_pt2[0]), &(tr_pt2[1]), &(tr_pt2[2]));
	GLdouble tr_pt3[3];
	transformPoint(max[0], min[1], min[2], &(tr_pt3[0]), &(tr_pt3[1]), &(tr_pt3[2]));
	GLdouble tr_pt4[3];
	transformPoint(min[0], max[1], max[2], &(tr_pt4[0]), &(tr_pt4[1]), &(tr_pt4[2]));
	double screen_distance_1 =	dist_ad(tr_pt1, tr_pt2);
	double screen_distance_2 =	dist_ad(tr_pt3, tr_pt4);
	printf("dist = %lf\n", sqrt(screen_distance_1) + sqrt(screen_distance_2));
	return sqrt(screen_distance_1) * sqrt(screen_distance_2);
}

bool AbstractData::updateMinMax(double* min, double* max, CCVOpenGLMath::Vector* vCorner, CCVOpenGLMath::Matrix* m)
{
	if (!min || !max || !vCorner || !m)
	{
		return false;
	}
	for (int i=0; i<8; i++)
	{
		CCVOpenGLMath::Vector trVec = (*m)*vCorner[i];
		trVec.normalize();
		if (min[0] > trVec[0])
		{
			min[0] = trVec[0];
		}
		if (min[1] > trVec[1])
		{
			min[1] = trVec[1];
		}
		if (min[2] > trVec[2])
		{
			min[2] = trVec[2];
		}
		if (max[0] < trVec[0])
		{
			max[0] = trVec[0];
		}
		if (max[1] < trVec[1])
		{
			max[1] = trVec[1];
		}
		if (max[2] < trVec[2])
		{
			max[2] = trVec[2];
		}
	}
	return true;
}

bool AbstractData::getMinMaxWithTransformations(double* min, double* max)
{
	if (!min || !max)
	{
		return false;
	}
	min[0] = min[1] = min[2] = 1e20; // change this to be more stable by assigning it to first value SKVINAY
	max[0] = max[1] = max[2] = -1e20;
	float fmin[3], fmax[3];
	if (!getMinMax(fmin, fmax))
	{
		return false;
	}
	double smin[3], smax[3];
	smin[0] = fmin[0];
	smin[1] = fmin[1];
	smin[2] = fmin[2];
	smax[0] = fmax[0];
	smax[1] = fmax[1];
	smax[2] = fmax[2];
	CCVOpenGLMath::Vector vCorner[8];
	CCVOpenGLMath::Vector::getCorners(smin, smax, vCorner);
	int n = m_ReplicationMatrices.size();
	for (int i=0; i<n; i++)
	{
		CCVOpenGLMath::Matrix* m = m_ReplicationMatrices[i];
		if (m)
		{
			updateMinMax(min, max, vCorner, m);
		}
	}
	return true;
}

void AbstractData::buildOctree()
{
	delete m_ObjectBoundingBoxOctree;
	m_ObjectBoundingBoxOctree = 0;
	if (m_ReplicationVisible)
	{
		delete []m_ReplicationVisible;
		m_ReplicationVisible = 0;
	}
	if (m_ReplicationMatrices.size() < 1)
	{
		return;
	}
	double min[3], max[3];
	min[0] = min[1] = min[2] = 0;
	max[0] = max[1] = max[2] = 1;
	int maxlevels = 3; // CHANGE!! SKVINAY
	getMinMaxWithTransformations(min, max);
	m_ObjectBoundingBoxOctree = new ObjectBoundingBoxOctree(min, max, maxlevels);
	m_ReplicationVisible = new bool[m_ReplicationMatrices.size()];
	double mid[3];
	float fmin[3], fmax[3];
	if (!getMinMax(fmin, fmax))
	{
		return;
	}
	mid[0] = (fmax[0] + fmin[0]) / 2.0;
	mid[1] = (fmax[1] + fmin[1]) / 2.0;
	mid[2] = (fmax[2] + fmin[2]) / 2.0;
	double radius = sqrt((mid[0]-fmin[0])*(mid[0]-fmin[0]) +
						 (mid[1]-fmin[1])*(mid[1]-fmin[1]) +
						 (mid[2]-fmin[2])*(mid[2]-fmin[2]));
	for (int i=0; i<m_ReplicationMatrices.size(); i++)
	{
		double center[3];
		CCVOpenGLMath::Vector v(mid[0], mid[1], mid[2], 1);
		v = (*(m_ReplicationMatrices[i]))*v;
		v.normalize();
		center[0] = v[0];
		center[1] = v[1];
		center[2] = v[2];
		m_ObjectBoundingBoxOctree->addObject(i, center, radius, 0);
	}
}

void AbstractData::useOctree(bool use)
{
	m_UseOctree = use;
	if (!m_ObjectBoundingBoxOctree)
	{
		buildOctree();
	}
}

void AbstractData::updateVisibleTransforms()
{
	if (!m_ObjectBoundingBoxOctree)
	{
		useOctree(true);
	}
	for (int i=0; i<m_ReplicationMatrices.size(); i++)
	{
		m_ReplicationVisible[i] = false;
	}
	RenderAreaWidget* renderer = m_DataManager->m_RendererSet->getCurrentRenderer();
	m_ObjectBoundingBoxOctree->findObjects(m_ReplicationVisible, m_ProjectedAreaCutOff, 0, renderer->m_OpenGL_Viewer.m_ViewFrustrum);
	int count = 0;
	for (int i=0; i<m_ReplicationMatrices.size(); i++)
	{
		if (m_ReplicationVisible[i])
		{
			count++;
		}
	}
	printf("Number visible = %d of %lu\n", count, m_ReplicationMatrices.size());
}
