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
#if !defined(AFX_ABSTRACTDATA_H__AC40E7D2_6499_4265_B23B_97DF82E2B171__INCLUDED_)
#define AFX_ABSTRACTDATA_H__AC40E7D2_6499_4265_B23B_97DF82E2B171__INCLUDED_

#include <qobject.h>
#include <QString>
#include <qstringlist.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QMouseEvent>

#include <UsefulMath/Ray.h>
#include <UsefulMath/Matrix.h>
#include <vector>
#include <DataManager/BoundingBox.h>

#include <TexMol/compat.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

class DataManager;
class Rover3DWidget;
class Axis3DHandler;
class View;
class ObjectBoundingBoxOctree;

const double OBJECT_PADDING = 1.0;

class AbstractData  : public QObject
{
		Q_OBJECT

	public:
		AbstractData(DataManager* dataManager);
		virtual ~AbstractData();
		bool readTransformations(const char* filename);
		CCVOpenGLMath::Matrix getTransformation();
		double distanceSqFromObject();
		static void transformPoint(float x, float y, float z, GLdouble* tr_x, GLdouble* tr_y, GLdouble* tr_z);
		virtual bool getDataMinMax(float* min, float* max);
		virtual bool getMinMax(float* min, float* max) = 0;
		virtual bool isEnabled(int rendererId);
		virtual bool mouseDoubleClick3DEvent(View*, QMouseEvent* e);
		virtual bool mouseMove3DEvent(View*, QMouseEvent* e);
		virtual bool mousePress3DEvent(View*, QMouseEvent* e);
		virtual bool mouseRelease3DEvent(View*, QMouseEvent* e);
		virtual bool parseAnimationCommand(QStringList commands, int curCommand) = 0;
		virtual bool read(QStringList fileNames) = 0;
		virtual bool save(const char* filename) = 0; // save myself!
		virtual QString getFileName();
		virtual QString getSelection(CCVOpenGLMath::Ray targetVector)=0;
		virtual QWidget* getPropertiesWidget() = 0;
		virtual void addPan(CCVOpenGLMath::Vector v);
		virtual void addRotation(CCVOpenGLMath::Matrix r);
		virtual void addRotation(CCVOpenGLMath::Matrix r, CCVOpenGLMath::Vector c);
		virtual void addRotation(int axis, float angle);
		virtual void addScale(float scale);
		virtual void addTransformation(CCVOpenGLMath::Matrix m, float scale); // scale is not required here in general, added only for imposter rendering
		virtual void appendReplicationMatrix(CCVOpenGLMath::Matrix* matrix); // this will own the matrices.
		virtual void associateDataManager(DataManager* dataManager);
		virtual void clearReplicationMatrices();
		virtual void setFileName(QString fileName);
		virtual void setTransformation(CCVOpenGLMath::Matrix m);
		virtual void setVisibilityInPropertiesWidget() = 0;
		virtual void useOctree(bool use);
		void beginRender();
		void endRender(bool renderDataBoundingBox);
		int m_Frame;
		int m_UniqueDataSetId;

	public slots:
		void renderEnabledSlot(bool renderMe);
		bool update();
		void roverExploringSlot();
		void roverReleasedSlot();

	protected:
		bool updateMinMax(double* min, double* max, CCVOpenGLMath::Vector* vCorner, CCVOpenGLMath::Matrix* m);
		virtual void buildOctree();
		bool getMinMaxWithTransformations(double* min, double* max);
		void updateVisibleTransforms();
		QString m_FileName;
		bool renderEnabled[100]; // somehow limits renderers to this number ! kindof stupid, replace with list
		DataManager* m_DataManager;
		Rover3DWidget* m_Rover3DWidget;
		Axis3DHandler* m_XAxisHandler;
		Axis3DHandler* m_YAxisHandler;
		Axis3DHandler* m_ZAxisHandler;

		GLint originalMmatrixMode;
		//   The transformation List contains tranlation, rotation and scale
		//   1. Transaltion is represented as
		//      1  < xtrans > < ytrans > < ztrans >
		//   2. Rotation:
		//      About x axis:    2 < angle >
		//      About y axis:    3 < angle >
		//      About z axis:    4 < angle >
		//   3. Scale:
		//      5 <scale>
		CCVOpenGLMath::Matrix m_Transformation; // precompute everytime the above list changes to speed up rendering.
		std::vector<CCVOpenGLMath::Matrix*> m_ReplicationMatrices;
		bool m_UseOctree;
		ObjectBoundingBoxOctree* m_ObjectBoundingBoxOctree;
		bool* m_ReplicationVisible;
		double m_ProjectedAreaCutOff;
};

#endif
