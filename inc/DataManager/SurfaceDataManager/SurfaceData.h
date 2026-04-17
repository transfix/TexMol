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
// SurfaceData.h: interface for the SurfaceData class.

#ifndef __SURFACEDATA_H__
#define __SURFACEDATA_H__

#include <QString>
#include <qcolor.h>
#include <vector>

#include <DataManager/AbstractData.h>
#include <UsefulMath/Ray.h>
#include <OpenGL_Viewer/Plane.h>

using std::vector;

class SurfaceRenderingProperties;
class DataManager;
class Geometry;
class ContourGeom;
class QImage;

namespace CCVTexMolGeometryRenderer
{
	class GeometryRenderer;
}

class SurfaceData : public AbstractData
{
		Q_OBJECT

	public:
		SurfaceData(DataManager* dataManager);
		virtual ~SurfaceData();

		void setGeometry(Geometry* geometry);
		void initNewGeometry();
		virtual QWidget* getPropertiesWidget();
		SurfaceRenderingProperties* m_SurfaceRenderingProperties;
		virtual void setVisibilityInPropertiesWidget();
		virtual QString getSelection(CCVOpenGLMath::Ray targetVector);

		bool read(QStringList fileNames);
		bool save(const char* filename);

		float* getSurfaceColors();
		const char* getRenderingTypeName();
		float* getWireframeColors();
		int getWireframeThickness();
		void setWireFrameMode(bool  render);
		void setSurfaceMode(bool  render);

		double getVolumeN();
		double getArea();
		double getArea(float* point, double radius);
		bool getAreas(double isovalue, double* functionAtVertices, double* areaBelow, double* areaAbove);
		Plane getClipPlane();

		static bool supportsFileNames(QStringList fileNames);
		virtual bool getMinMax(float* min, float* max);
		void render();
		void setCurvatureVectors(QStringList curvFileNames);
		Geometry* getGeometry();
		Geometry* getGeometry(int index);
		bool associateNormalsToGeometry(int geometryIndex);
		bool computeCurvatures(int geometryIndex);
		bool computeCurvatures();

		bool parseAnimationCommand(QStringList commands, int curCommand);
		Geometry* createDuplicates();
		float getAlpha() const
		{
			return m_SurfaceAlpha;
		}
		void setAlpha(float alpha);

	public slots:
		void clipValueChangedSlot(int value);
		void wireframeColorSlot();
		void surfaceColorSlot();
		void lineColorSlot();
		void pointColorSlot();

		void enableScript();
		void setVertexColors(float* colors, int numberOfColors); // numberOfValues for safety
		void setVertexColor(float r, float g, float b, int index);
		void setRadiusCutOffForRendering(float radius);
		void renderCurvatures(bool render);
		void frameValueChangedSlot(int value);
		void replicateSlot();

		void renderLinesSlot();
		void renderPointsSlot();
		void renderTrianglesSlot();
		void renderWireframesSlot();

		void surfaceTexturePushButtonSlot();
		bool readSurfaceTexture();

		void wireframeUniqueColorRadioButtonSlot();
		void wireframeDefaultColorRadioButtonSlot();

		void lineUniqueColorRadioButtonSlot();
		void lineDefaultColorRadioButtonSlot();
		void pointUniqueColorRadioButtonSlot();
		void pointDefaultColorRadioButtonSlot();

		void wireframeThicknessChangedSlot(int thickness);
		void pointThicknessChangedSlot(int thickness);
		void lineThicknessChangedSlot(int thickness);
		void renderingTypeSlot(int renderingType);

		void transparencySliderSlot(int value);

	protected:
		QColor getUserSelectedWireframeColor();
		void setUserSelectedWireframeColor(const QColor& color);
		QColor getUserSelectedSurfaceColor();
		void setUserSelectedSurfaceColor(const QColor& color);
		QColor getUserSelectedLineColor();
		QColor getUserSelectedPointColor();

		bool convertContourGeomToGeometry(ContourGeom* cGeom, Geometry* geometry, bool colorPresent);
		bool setVectorGeometry(Geometry* geometry, int numberOfVectors, double* vectors, double length, int curGeomIndex);
		bool setRadiiOfCurvature(double* HandK, int curGeomIndex);
		double distVtxToPoint(int vertex, float* point);
		void deleteCurvatures();
		void deleteData();
		void renderOnce();

		//double m_min[3], m_max[3];
		vector<double> m_min;
		vector<double> m_max;

		vector<Geometry*> m_Geometry;
		vector<Geometry*> m_NormalVectors;
		vector<Geometry*> m_K1CurvatureVectors;
		vector<Geometry*> m_K2CurvatureVectors;
		vector<double*> m_RadiiOfCurvature;

		double m_ClippingPlane;
		float m_WireframeColors[3];
		float m_SurfaceColors[3];
		float m_SurfaceAlpha;
		bool m_RenderCurvatures;
		CCVTexMolGeometryRenderer::GeometryRenderer* m_GeometryRenderer;

		bool hasTextureImage;
		QImage m_opengl_qimage;
};

#endif
