#ifndef __SECONDARYSTRUCTUREDATA_H__
#define __SECONDARYSTRUCTUREDATA_H__

// SecondaryStructureData.h: interface for the SecondaryStructureData class.

#include <qstring.h>
#include <qcolor.h>
#include <vector>
#include "../AbstractData.h"
#include "Ray.h"
#include "Plane.h"
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

		const QColor& getUserSelectedSecondaryStructureColor();
		void setUserSelectedSecondaryStructureColor(const QColor& color);
		const QColor& getUserSelectedLineColor();

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
