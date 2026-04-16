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
// BallAndStickData.h: interface for the BallAndStickData class.
#include <qobject.h>
#include <QString>
#include <vector>
#include <DataManager/AbstractData.h>
#include <UsefulMath/Ray.h>

#ifndef CCV_TEXMOL_BALL_AND_STICK_DATA_H
#define CCV_TEXMOL_BALL_AND_STICK_DATA_H

class DataManager;
namespace CCVImposterRenderer
{
	class ImposterRenderer;
}
class ColorLegend;
class Correlation;
#ifdef MS
class MultiresolutionSurface;
#endif

namespace ObjectRendererLibrary
{
	class SphereRenderer;
}

class ColorMapInterface;
class BallAndStickRenderingProperties;


const int LOD_ATOM = 0;
const int LOD_BALL_AND_STICK = 1;
const int LOD_BACKBONE = 2;
const int LOD_RESIDUE = 3;
const int LOD_SS = 4;
const int LOD_CHAIN = 5;
const int LOD_PROTEIN = 6;
//const int LOD_SURFACE = 7;
const int LOD_HBOND_ALL = 7;
const int LOD_HBOND_INT = 8;
const int LOD_HBOND_EXT = 9;
const int LOD_GENERAL = 10;

/*
const int LOD_BACKBONE = 1;
const int LOD_RESIDUE = 2;
const int LOD_SS = 3;
const int LOD_CHAIN = 4;
const int LOD_PROTEIN = 5;
const int LOD_GENERAL = 6;
*/

const int COLOR_ATOM = 0;
const int COLOR_RESIDUE = 1;
const int COLOR_SS = 2;
const int COLOR_CHAIN = 3;
const int COLOR_PROTEIN = 4;
const int COLOR_FUNCTION = 5;
const int COLOR_BOUNDARY = 6;
const int COLOR_USER_DEFINED = 7;
const int COLOR_USER_COLOR = 8;

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

using std::vector;

class BallAndStickData : public AbstractData
{
		Q_OBJECT

	public:
		BallAndStickData(DataManager* dataManager);
		virtual ~BallAndStickData();

		virtual int colorLOD();
		virtual int objectLOD();
		virtual PDBParser::GroupOfAtoms* getData();
		virtual QWidget* getPropertiesWidget();
		void render();

		virtual void setVisibilityInPropertiesWidget();
		virtual QString getSelection(CCVOpenGLMath::Ray targetVector);
		bool read(QStringList fileNames);
		bool save(const char* filename);
		bool setTransformation(QStringList qStringList);

		virtual void setTransformation(CCVOpenGLMath::Matrix m);
		virtual void addTransformation(CCVOpenGLMath::Matrix m, float scale);
		virtual void addPan(CCVOpenGLMath::Vector v);
		virtual void addRotation(CCVOpenGLMath::Matrix r);
		virtual void addRotation(int axis, float angle);
		virtual void addScale(float scale);

		static bool supportsFileNames(QStringList fileNames);
		virtual bool getMinMax(float* min, float* max);

		double correlate();

		bool parseAnimationCommand(QStringList commands, int curCommand);

	public slots:
		void renderEnabledSlot(bool render);
		void clipValueChangedSlot(int value);
		void frameValueChangedSlot(int value);
		void roverExploringSlot();
		void roverReleasedSlot();
		void replicateSlot();

	protected slots:
		void structureLODSlot(int index);
		void colorLODSlot(int index);
		bool plotAnglesSlot();
		void displayColorLegendSlot();
		void selectFunctionSlot();
		void selectColorMapSlot();
		void solventRadiusSlot(bool useSolventRadii);

		void subCubeIsoChangedSlot();
		void outerCubeIsoChangedSlot();
		void subCubeBChangedSlot();
		void outerCubeBChangedSlot();

		void colorMapInterfaceSlot();
		void resetRenderer();
		void renderOnce();

	protected:
		Correlation* m_CorrelationModule;

		int m_ObjectLod;
		int m_ColorLod;
		bool m_FunctionAssociated;
		bool m_UseHarwareBuffer;
		float m_ScaleValue; // needed for imposter rendering!

		double getCorrectRadius(PDBParser::Atom* atom);
		bool prepareRenderer();
		void recursivePrepareRender(PDBParser::GroupOfAtoms* molecule, int objectLOD, int colorLOD);
		void recursivePrepareBackbone(PDBParser::GroupOfAtoms* molecule, int color, int objectLOD);

		bool renderBackboneInResidue(PDBParser::GroupOfAtoms* residue, int colorLOD);
		bool getAtomColorRadius(PDBParser::Atom* atom, GLfloat* r, GLfloat* g, GLfloat* b, GLfloat* radius, int colorLOD);
		bool addBondAndAtom(PDBParser::Atom* atom1, PDBParser::Atom* atom2, int colorLOD);
		bool constructMultiresSurfaceObject();
		//CHA: render hydrogen bond
		void prepareHydrogenBondRender(PDBParser::GroupOfAtoms* molecule, int objectLOD);

		double m_ClippingPlane;
		PDBParser::Atom* m_BackboneAtomFromPreviousResidue;

		CCVImposterRenderer::ImposterRenderer* m_ImposterRenderer;
		vector<CCVImposterRenderer::ImposterRenderer*> m_ImposterRendererList;
		ObjectRendererLibrary::SphereRenderer* m_SphereRenderer;
		vector<PDBParser::GroupOfAtoms*> m_GroupOfAtoms;
		ColorLegend* m_ColorLegend;
		BallAndStickRenderingProperties* m_BallAndStickRenderingProperties;
		ColorMapInterface* m_ColorMapInterface;

		double m_min[3], m_max[3];

		// multires variables
		float m_SubCubeIsovalue;
		float m_OuterCubeIsovalue;
		float m_SubCubeBlobbiness;
		float m_OuterCubeBlobbiness;
		#ifdef MS
		MultiresolutionSurface* m_MultiresolutionSurface;
		#endif

		// CHA: save flags below to avoid initializing unnecessarily.
		int colorLODsave;
		int functionAssociateSave;
};

#endif // CCV_TEXMOL_BALL_AND_STICK_DATA_H
