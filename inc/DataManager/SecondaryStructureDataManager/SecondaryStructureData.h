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
// SecondaryStructureData.h: interface for the SecondaryStructureData class.

#ifndef __SECONDARYSTRUCTUREDATA_H__
#define __SECONDARYSTRUCTUREDATA_H__

#include <QString>
#include <qcolor.h>
#include <vector>

#include <DataManager/AbstractData.h>
#include <UsefulMath/Ray.h>
#include <OpenGL_Viewer/Plane.h>
#include <boost/shared_ptr.hpp>

using std::vector;

class SecondaryStructureRenderingProperties;
class DataManager;
class Geometry;
class ContourGeom;
class Skel;

namespace CCVTexMolGeometryRenderer
{
	class GeometryRenderer;
}

class SecondaryStructureData : public AbstractData
{
		Q_OBJECT

	public:
		SecondaryStructureData(DataManager* dataManager, Geometry* inputGeom);
		virtual ~SecondaryStructureData();

		virtual QWidget* getPropertiesWidget();
		SecondaryStructureRenderingProperties* m_SecondaryStructureRenderingProperties;
		virtual void setVisibilityInPropertiesWidget();
		virtual QString getSelection(CCVOpenGLMath::Ray targetVector);
		virtual bool read(QStringList fileNames);
		bool save(const char* filename);

		float* getSecondaryStructureColors();
		void setSecondaryStructureMode(bool  render);

		Plane getClipPlane();

		static bool supportsFileNames(QStringList fileNames);
		virtual bool getMinMax(float* min, float* max);
		void render();

		bool parseAnimationCommand(QStringList commands, int curCommand);

		Geometry* getSkeletonGeometry();
		float getAlpha() const
		{
			return m_SecondaryStructureAlpha;
		}
	public slots:
		void secondaryStructureColorSlot();
		void lineColorSlot();

		void enableScript();
		void renderLinesSlot();
		void renderTrianglesSlot();
		void secondaryStructureColorTypeSlot();
		void lineColorTypeSlot();
		void lineThicknessChangedSlot(int thickness);

		void transparencySliderSlot(int value);
		void alphaSpinBoxSlot(int value);
		void betaSpinBoxSlot(int value);
		void renderAlphaCheckBoxSlot();
		void renderBetaCheckBoxSlot();
		void alphaHistogramChangedSlot(float minValue, float maxValue);
		void betaHistogramChangedSlot(float minValue, float maxValue);
	protected:
		/* poll skel to rebuild the geometry model */
		void rebuildGeometry(bool alphaHistogramChanged, bool betaHistogramChanged);

		QColor getUserSelectedSecondaryStructureColor();
		void setUserSelectedSecondaryStructureColor(const QColor& color);
		QColor getUserSelectedLineColor();

		void renderOnce();

		vector<double> m_min;
		vector<double> m_max;

		Geometry* m_HelixGeometry;
		Geometry* m_SheetGeometry;

		double m_ClippingPlane;
		float m_SecondaryStructureColors[3];
		float m_SecondaryStructureAlpha;
		bool m_RenderCurvatures;
		CCVTexMolGeometryRenderer::GeometryRenderer* m_GeometryRenderer;

		Skel* m_Skeleton;
		bool m_SpinBoxesInitialized;
};

#endif
