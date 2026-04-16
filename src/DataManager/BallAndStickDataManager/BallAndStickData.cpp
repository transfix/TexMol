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
// BallAndStickData.cpp: implementation of the BallAndStickData class.

#include <QFileDialog>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qslider.h>

#include <QColor>
#include <QColorDialog>

#include "ui_BallAndStickRenderingPropertiesBase.h"
#include <ImposterRenderer/ImposterRenderer.h>
#include <DataManager/BallAndStickDataManager/ColorMapInterface.h>
#include <DataManager/VolumeDataManager/VolumeData.h>
#include <DataManager/BallAndStickDataManager/BallAndStickData.h>
#include <DataManager/BallAndStickDataManager/BallAndStickRenderingProperties.h>
#include <PDBParser/SecondaryStructureData.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/Atom.h>
#include <DataManager/DataManager.h>
#include <PDBParser/ResidueHeader.h>
// AnglesDialog.Qt3.h removed — needs Qt6 port
#include <ObjectRenderer/TransformationParameters.h>
#include <ObjectRenderer/Sphere/SphereRenderer.h>
#include <DataManager/BallAndStickDataManager/ColorLegend.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <PDBParser/GOAColor.h>
#include <PDBParser/CollectionData.h>
#include <GOAFileTypes/GOALoader.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <OpenGL_Viewer/Extents.h>
#include <DataManager/Rover3DWidget.h>
#include <DataManager/Correlation.h>
#include <VolumeFileTypes/VolumeLoader.h>
#include <stdio.h>
#include <math.h>
#include <Blurmaps/BlurMapsDataManager.h>
#ifdef MS
#include <MultiresolutionSurface/MultiresolutionSurface.h>
#endif
#include <TexMol/RendererSet.h>

BallAndStickData::BallAndStickData(DataManager* dataManager)
	: AbstractData(dataManager)
{
	m_ObjectLod = LOD_ATOM;
	m_ColorLod = COLOR_ATOM;
	m_FunctionAssociated = false;
	m_UseHarwareBuffer = true;
	m_FileName = "";
	m_ScaleValue = 1.0;
	m_min[0] = m_min[1] = m_min[2] = 0;
	m_max[0] = m_max[1] = m_max[2] = -1;
	m_ClippingPlane = 0;
	m_BackboneAtomFromPreviousResidue = 0;
	m_ImposterRenderer = new CCVImposterRenderer::ImposterRenderer;
	m_ImposterRenderer->initRenderer();
	m_SphereRenderer = 0;
	//m_SphereRenderer = new ObjectRendererLibrary::SphereRenderer;
	//if( !m_SphereRenderer->initRenderer() ) delete m_SphereRenderer;
	m_SubCubeIsovalue = 1.0f;
	m_OuterCubeIsovalue = 1.0f;
	m_SubCubeBlobbiness = 1.0f;
	m_OuterCubeBlobbiness = 1.0f;
	m_CorrelationModule = 0;
	#ifdef MS
	m_MultiresolutionSurface = 0;
	#endif

	colorLODsave = -1;
	functionAssociateSave = -1;

	if(dataManager)
	{
		m_BallAndStickRenderingProperties = new BallAndStickRenderingProperties(dataManager->m_ParentPropertiesWidget);
		m_BallAndStickRenderingProperties->hide();
		m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setEnabled(false);

		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_CheckBox_Render), SIGNAL(toggled(bool)), this, SLOT(renderEnabledSlot(bool)));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_ComboBox_ColorLOD), SIGNAL(activated(int)), this, SLOT(colorLODSlot(int)));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_ComboBox_StructureLOD), SIGNAL(activated(int)), this, SLOT(structureLODSlot(int)));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_PushButton_PlotAngles), SIGNAL(clicked()), this, SLOT(plotAnglesSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_PushButton_ShowLegend), SIGNAL(clicked()), this, SLOT(displayColorLegendSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_PushButton_AssociateFunction), SIGNAL(clicked()), this, SLOT(selectFunctionSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_PushButton_ColorMap), SIGNAL(clicked()), this, SLOT(selectColorMapSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_CheckBox_SolventEnlargedRadius), SIGNAL(toggled(bool)), this, SLOT(solventRadiusSlot(bool)));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_Slider_Clip), SIGNAL(valueChanged(int)), this, SLOT(clipValueChangedSlot(int)));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_Slider_Frame), SIGNAL(valueChanged(int)), this, SLOT(frameValueChangedSlot(int)));
		// for multires surfaces
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeIso_2), SIGNAL(returnPressed()), this, SLOT(subCubeIsoChangedSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeIso_2), SIGNAL(returnPressed()), this, SLOT(outerCubeIsoChangedSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeB_2), SIGNAL(returnPressed()), this, SLOT(subCubeBChangedSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeB_2), SIGNAL(returnPressed()), this, SLOT(outerCubeBChangedSlot()));
		connect((const class QObject*)(m_Rover3DWidget), SIGNAL(RoverExploring()), this, SLOT(roverExploringSlot()));
		connect((const class QObject*)(m_Rover3DWidget), SIGNAL(RoverReleased()), this, SLOT(roverReleasedSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_PushButton_ColorMapInterface), SIGNAL(clicked()), this, SLOT(colorMapInterfaceSlot()));
		connect((const class QObject*)(m_BallAndStickRenderingProperties->_ui->m_PushButton_Replicate), SIGNAL(clicked()), this, SLOT(replicateSlot()));
	}

	{
		m_ColorMapInterface = new ColorMapInterface(25);
		m_ColorMapInterface->hide();
		connect((const class QObject*)(m_ColorMapInterface), SIGNAL(updateRenderer()), this, SLOT(resetRenderer()));
	}
}

BallAndStickData::~BallAndStickData()
{
	{
		std::vector<PDBParser::GroupOfAtoms*>::iterator iter = m_GroupOfAtoms.begin(), end = m_GroupOfAtoms.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_GroupOfAtoms.clear();
	}
	delete m_ImposterRenderer;
	m_ImposterRenderer = 0;
	delete m_CorrelationModule;
	m_CorrelationModule = 0;
	#ifdef MS
	delete m_MultiresolutionSurface;
	m_MultiresolutionSurface = 0;
	#endif
}

int BallAndStickData::colorLOD()
{
	return m_ColorLod;
}

int BallAndStickData::objectLOD()
{
	return m_ObjectLod;
}

PDBParser::GroupOfAtoms* BallAndStickData::getData()
{
	if(m_Frame >= m_GroupOfAtoms.size())
	{
		return 0;
	}

	return m_GroupOfAtoms[m_Frame];
}

QWidget* BallAndStickData::getPropertiesWidget()
{
	return m_BallAndStickRenderingProperties;
}

void BallAndStickData::renderEnabledSlot(bool render)
{
	AbstractData::renderEnabledSlot(render);
	prepareRenderer();
	m_DataManager->update();
}

void BallAndStickData::setVisibilityInPropertiesWidget()
{
	if(m_BallAndStickRenderingProperties)
	{
		int currentRenderer = m_DataManager->getCurrentRenderer();
		bool enabled = renderEnabled[ currentRenderer ]; 
		m_BallAndStickRenderingProperties->_ui->m_CheckBox_Render->setChecked( enabled );
	}
}


void BallAndStickData::structureLODSlot(int index)
{
	// here we also need to do something about the combo box items
	m_ObjectLod = index;
	prepareRenderer();
	m_DataManager->update();
}

void BallAndStickData::colorLODSlot(int index)
{

  m_ColorLod = index;

  PDBParser::GroupOfAtoms * m_Molecule = m_GroupOfAtoms[0];

  PDBParser::GOAColor* goaColor = new PDBParser::GOAColor();
  if (m_ColorLod== COLOR_ATOM)
    goaColor->ApplyColorsByGOALevel(m_Molecule,PDBParser::GroupOfAtoms::ATOM);
  
  if (m_ColorLod== COLOR_RESIDUE)
    goaColor->ApplyColorsByGOALevel(m_Molecule,PDBParser::GroupOfAtoms::RESIDUE);
  
  if (m_ColorLod== COLOR_SS)
    goaColor->ApplyColorsByGOALevel(m_Molecule,PDBParser::GroupOfAtoms::SECONDARY_STRUCTURE);
  
  if (m_ColorLod== COLOR_CHAIN)
    goaColor->ApplyColorsByGOALevel(m_Molecule,PDBParser::GroupOfAtoms::CHAIN);
  
  if (m_ColorLod== COLOR_PROTEIN)
    goaColor->ApplyColorsByGOALevel(m_Molecule,PDBParser::GroupOfAtoms::PROTEIN);

  if (m_ColorLod== COLOR_USER_DEFINED)
    m_ColorMapInterface->colorGOA();

  if (m_ColorLod== COLOR_USER_COLOR) {
    // get a color and apply it
    QColor prevColor(255,255,255);
    QColor newColor = QColorDialog::getColor(prevColor);
    goaColor->ApplyColorToGOA(newColor.redF(),newColor.greenF(),newColor.blueF(),1.0,m_Molecule);
  }


  delete goaColor;


  prepareRenderer();
  m_DataManager->update();
}

// AnglesDialog is Qt3-only; stub until ported
#if 0
void recursivePlotAngles(AnglesDialog* anglesDialog, PDBParser::GroupOfAtoms* molecule)
{
	int i;

	for(i=0; i<molecule->m_SubGroups.size(); i++)
	{
		recursivePlotAngles(anglesDialog, molecule->m_SubGroups.at(i));
	}
}

bool BallAndStickData::plotAnglesSlot()
{
	if(m_Frame >= m_GroupOfAtoms.size())
	{
		return false;
	}

	if(!(m_GroupOfAtoms[m_Frame]))
	{
		return false;
	}

	AnglesDialog* anglesDialog = new AnglesDialog();
	recursivePlotAngles(anglesDialog, m_GroupOfAtoms[m_Frame]);
	anglesDialog->show();
	return true;
}
#else
bool BallAndStickData::plotAnglesSlot()
{
	return false; // AnglesDialog not yet ported to Qt6
}
#endif

bool BallAndStickData::getAtomColorRadius(PDBParser::Atom* atom, GLfloat* r, GLfloat* g, GLfloat* b, GLfloat* radius, int colorLOD)
{
	if(!atom || !r || !g || !b || !radius)
	{
		return false;
	}

	*r = *g = *b = 0.0f;
	*radius = 1.0f;

	if(colorLOD == COLOR_ATOM || colorLOD == COLOR_USER_DEFINED || colorLOD == COLOR_USER_COLOR)
	{
		float a;
		atom->getColor(r, g, b, &a);
	}
	else if(colorLOD == COLOR_BOUNDARY)
	{
		if(atom->isBoundaryAtom())
		{
			*r = 1.0f;
		}
		else
		{
			*g = 1.0f;
		}
	}
	else if(colorLOD == COLOR_USER_DEFINED)
	{
		*r = ((unsigned char)((atom->m_UserDefinedColor & 0xff000000) >> 24))/256.0;
		*g = ((unsigned char)((atom->m_UserDefinedColor & 0xff0000) >> 16))/256.0;
		*b = ((unsigned char)((atom->m_UserDefinedColor & 0xff00) >> 8))/256.0;
	}
	else
	{
		GLfloat colors[4];
		glGetFloatv(GL_CURRENT_COLOR, colors);
		*r = colors[0];
		*g = colors[1];
		*b = colors[2];
	}

	*radius = atom->getRadius();
	return true;
}

bool BallAndStickData::addBondAndAtom(PDBParser::Atom* atom1, PDBParser::Atom* atom2, int colorLOD)
{
	if(!atom2)
	{
		return false;    // atom 1 can be null!
	}

	GLfloat r1, g1, b1, radius1;
	GLfloat r2, g2, b2, radius2;

	if(atom1)
	{
		if(!getAtomColorRadius(atom1, &r1, &g1, &b1, &radius1, colorLOD))
		{
			return false;
		}

		if(!getAtomColorRadius(atom2, &r2, &g2, &b2, &radius2, colorLOD))
		{
			return false;
		}

		GLfloat radius = radius1;

		if(radius > radius2)
		{
			radius = radius2;
		}

		if(radius <= 0)
		{
			return false;
		}

		m_ImposterRenderer->m_StickRenderer->addStick(
			atom1->m_Position[0],
			atom1->m_Position[1],
			atom1->m_Position[2],
			atom2->m_Position[0],
			atom2->m_Position[1],
			atom2->m_Position[2],
			radius/5.0*m_ScaleValue,
			r1,
			g1,
			b1,
			r2,
			g2,
			b2);

	}
	m_ImposterRenderer->m_BallRenderer->addBall(
		atom2->m_Position[0],
		atom2->m_Position[1],
		atom2->m_Position[2],
		radius2/4.0 * m_ScaleValue,
		r2,
		g2,
		b2);
	return true;
}

bool BallAndStickData::renderBackboneInResidue(PDBParser::GroupOfAtoms* residue, int colorLOD)
{
	if(!residue)
	{
		return false;
	}

	if(residue->getResidueType() == PDBParser::GroupOfAtoms::AMINO_ACID_RESIDUE)
	{
		// declare atoms
		PDBParser::Atom* nAtom = 0, *caAtom = 0, *cAtom = 0;

		// getAtoms
		if((nAtom =  residue->getAtom(" N  ")) == 0)
		{
			m_BackboneAtomFromPreviousResidue = NULL;
			return false;
		}

		if((caAtom = residue->getAtom(" CA ")) == 0)
		{
			m_BackboneAtomFromPreviousResidue = NULL;
			return false;
		}

		if((cAtom =  residue->getAtom(" C  ")) == 0)
		{
			m_BackboneAtomFromPreviousResidue = NULL;
			return false;
		}

		if(!addBondAndAtom(m_BackboneAtomFromPreviousResidue, nAtom, colorLOD))
		{
			m_BackboneAtomFromPreviousResidue = NULL;
			return false;
		}

		if(!addBondAndAtom(nAtom, caAtom, colorLOD))
		{
			m_BackboneAtomFromPreviousResidue = NULL;
			return false;
		}

		if(!addBondAndAtom(caAtom, cAtom, colorLOD))
		{
			m_BackboneAtomFromPreviousResidue = NULL;
			return false;
		}

		m_BackboneAtomFromPreviousResidue = cAtom;
	}
	else if(residue->getResidueType() == PDBParser::GroupOfAtoms::RNA_RESIDUE)
	{
	}
	else
	{
		return false;
	}

	return true;
}

void BallAndStickData::prepareHydrogenBondRender(PDBParser::GroupOfAtoms* molecule, int objectLOD)
{
	float radius = 0.2;
	float col_donor[3] = {0,0,1};
	float col_accep[3] = {1,0,0};

	for(int i = 0; i < molecule->m_SubGroups.size(); i++ ) {
	std::vector<PDBParser::HBOND> *hbond = molecule->m_SubGroups[i]->getHBond();
	fprintf( stderr, "n hbond: %d\n", hbond->size() );
	for( std::vector<PDBParser::HBOND>::iterator it = hbond->begin();  it < hbond->end(); it++ ) {
		float *p_donor = it->d->getPosition();
		float *p_accep = it->a->getPosition();

		if( objectLOD == LOD_HBOND_ALL ) {
			m_ImposterRenderer->m_StickRenderer->addStick(p_donor[0], p_donor[1], p_donor[2],
								      p_accep[0], p_accep[1], p_accep[2],
								      radius,
								      col_donor[0], col_donor[1], col_donor[2],
								      col_accep[0], col_accep[1], col_accep[2]);
		}
		else if(objectLOD == LOD_HBOND_INT) {
			if( it->internal )
 				m_ImposterRenderer->m_StickRenderer->addStick(p_donor[0], p_donor[1], p_donor[2],
								      p_accep[0], p_accep[1], p_accep[2],
								      radius,
								      col_donor[0], col_donor[1], col_donor[2],
								      col_accep[0], col_accep[1], col_accep[2]);
		}
		else if( !it->internal ) {
			m_ImposterRenderer->m_StickRenderer->addStick(p_donor[0], p_donor[1], p_donor[2],
								      p_accep[0], p_accep[1], p_accep[2],
								      radius,
								      col_donor[0], col_donor[1], col_donor[2],
								      col_accep[0], col_accep[1], col_accep[2]);
		}
	}
	}
}

void BallAndStickData::recursivePrepareRender(PDBParser::GroupOfAtoms* molecule, int objectLOD, int colorLOD)
{
//    printf("print molecule -> resId= %d\n", molecule->resID);
	if(!molecule)
	{
		return;
	}

	// HACK : to render a general type, or atoms attached at the protein level
	if(molecule->type == PDBParser::PROTEIN_TYPE || molecule->type == PDBParser::GENERAL_TYPE)
		//if (molecule->type == GENERAL_TYPE)
		// no longer rendering the HETATMS.
		// Should we put them into a separate SS, etc etc, or do they actually have something given ?
		// Or should we neglect them ??
	{
		// If the current object is an entire protein, use the default colors from the atom color array
		int i;
		for(i=0; i<molecule->m_Atoms.size(); i++)
		{
			PDBParser::Atom* atom = molecule->m_Atoms.at(i);
			GLfloat r = 0.0, g = 0.0, b = 0.0, a = 0.0;
			atom->getColor(&r, &g, &b, &a);
			double radius = getCorrectRadius(atom);
			m_ImposterRenderer->m_BallRenderer->addBall(atom->m_Position[0],
					atom->m_Position[1],
					atom->m_Position[2],
					radius * m_ScaleValue,
					r,
					g,
					b);
		}
	}

	// If the proper level of detail is set, render the group of atoms in multiresolution form but
	// not the component subgroups
	if(molecule->type == PDBParser::RESIDUE_TYPE)
	{	

		// For a residue, render either the component atoms or the multires approximation
		if(colorLOD == COLOR_RESIDUE)
		{
			glColor4f((float)molecule->multi_r, (float)molecule->multi_g, (float)molecule->multi_b, 0.4f);
		}

		if( (objectLOD == LOD_ATOM) || (objectLOD == LOD_HBOND_ALL) || (objectLOD == LOD_HBOND_INT) || (objectLOD == LOD_HBOND_EXT) )
		{
			int i;

			bool render = true;

			if( !(objectLOD == LOD_ATOM) )
			{
				render = false;

				for(i=0; i<molecule->m_Atoms.size(); i++)
				{
					PDBParser::Atom* atom = molecule->m_Atoms.at(i);

					if((atom->m_donorOrAcceptor) && ( (objectLOD == LOD_HBOND_ALL) || (objectLOD == LOD_HBOND_INT && atom->m_internalHbond>1) || (objectLOD == LOD_HBOND_EXT && (atom->m_internalHbond%2==1)) ) ) 
						render = true;
				}
			}
   
			if(render)
			for(i=0; i<molecule->m_Atoms.size(); i++)
			{
				GLfloat r, g, b, a;
				r = g = b = 0.0f;
				a = 1.0;
				PDBParser::Atom* atom = molecule->m_Atoms.at(i);
				/*
				if(!(objectLOD == LOD_ATOM))
				{
					if( !(atom->m_donorOrAcceptor))
						continue;
					else if ((objectLOD == LOD_HBOND_INT && (atom->m_internalHbond<2)) || (objectLOD == LOD_HBOND_EXT && (atom->m_internalHbond%2==0)) ) 
						continue;
				}*/

				if(colorLOD == COLOR_ATOM || colorLOD == COLOR_USER_DEFINED || colorLOD == COLOR_USER_COLOR) 
				{
					atom->getColor(&r, &g, &b, &a);
				}
				else if(colorLOD == COLOR_BOUNDARY)
				{
					if(atom->isBoundaryAtom())
					{
						r = 1.0f;
					}
					else
					{
						g = 1.0f;
					}
				}
				else if(colorLOD == COLOR_USER_DEFINED)
				{
					r = ((unsigned char)((atom->m_UserDefinedColor & 0xff000000) >> 24))/256.0;
					g = ((unsigned char)((atom->m_UserDefinedColor & 0xff0000) >> 16))/256.0;
					b = ((unsigned char)((atom->m_UserDefinedColor & 0xff00) >> 8))/256.0;
				}
				else
				{
					GLfloat colors[4];
					glGetFloatv(GL_CURRENT_COLOR, colors);
					r = colors[0];
					g = colors[1];
					b = colors[2];
				}

				double x, y, z, rad;
				x = atom->m_Position[0];
				y = atom->m_Position[1];
				z = atom->m_Position[2];
				rad = getCorrectRadius(atom);

				if( (objectLOD == LOD_HBOND_ALL) || (objectLOD == LOD_HBOND_INT) || (objectLOD == LOD_HBOND_EXT) )
				{
					rad /= 2.0;
				}

				m_ImposterRenderer->m_BallRenderer->addBall(x, y, z, rad * m_ScaleValue, r, g, b);

				if(m_SphereRenderer)
				{
					m_SphereRenderer->addSphere(x, y, z, rad*m_ScaleValue, r, g, b);
				}
			}
		}

		if(objectLOD == LOD_BACKBONE)
		{
			renderBackboneInResidue(molecule, colorLOD);
		}
		else if(objectLOD == LOD_SS)
		{
			// we come in only for SS_NULL , objectLOD is LOD_SS. Other SS should return before this point
			if(molecule->resID == -1)
			{
				return;    // no backbone for non standard residues
			}

			int i;

			for(i=0; i<molecule->m_Atoms.size(); i++)
			{
				GLfloat r, g, b, a;
				r = g = b = 0.0f;
				a = 1.0f;
				PDBParser::Atom* atom = molecule->m_Atoms.at(i);

				if(colorLOD == COLOR_ATOM)
				{
					atom->getColor(&r, &g, &b, &a);
				}
				else
				{
					GLfloat colors[4];
					glGetFloatv(GL_CURRENT_COLOR, colors);
					r = colors[0];
					g = colors[1];
					b = colors[2];
				}

				double x, y, z, rad;
				x = atom->m_Position[0];
				y = atom->m_Position[1];
				z = atom->m_Position[2];
				rad = getCorrectRadius(atom) / 3.0;

				if(i == 1)
				{
					return;    // render only the CA atoms!
				}

				m_ImposterRenderer->m_BallRenderer->addBall(x, y, z, rad * m_ScaleValue, r, g, b);
			}
		}
		else if(objectLOD == LOD_RESIDUE)
		{
			int i;
			int count = molecule->m_MultiresAtoms.size();

			for(i=0; i<count; i++)
			{
				GLfloat r, g, b;

				if(colorLOD == COLOR_RESIDUE || COLOR_FUNCTION)
				{
					r = molecule->multi_r;
					g = molecule->multi_g;
					b = molecule->multi_b;
				}
				else
				{
					GLfloat colors[4];
					glGetFloatv(GL_CURRENT_COLOR, colors);
					r = colors[0];
					g = colors[1];
					b = colors[2];
				}

				m_ImposterRenderer->m_BallRenderer->addBall(molecule->m_MultiresAtoms[i]->m_Position[0],
						molecule->m_MultiresAtoms[i]->m_Position[1],
						molecule->m_MultiresAtoms[i]->m_Position[2],
						molecule->m_MultiresAtoms[i]->getRadius() * m_ScaleValue,
						r,
						g,
						b);
			}

			return;
		}
	}
	else if(molecule->type == PDBParser::SS_TYPE)
	{
		if(colorLOD == COLOR_SS)
		{
			glColor4f((float)molecule->multi_r, (float)molecule->multi_g, (float)molecule->multi_b, 0.4f);
		}

		if(objectLOD == LOD_SS)
		{
			// if its a helix, draw a helix, return
			// if its a sheet, draw a sheet, return
			// else just draw the atoms ? but bounding sphere is too big
			// so dont return, draw atoms when we reach residue level
			if((molecule->resID == PDBParser::SS_HELIX))
			{
				if(molecule->m_SecondaryStructureData == 0)
				{
					return;
				}

				float r, g, b;

				if(colorLOD == COLOR_CHAIN || colorLOD == COLOR_PROTEIN)
				{
					GLfloat colors[4];
					glGetFloatv(GL_CURRENT_COLOR, colors);
					r = colors[0];
					g = colors[1];
					b = colors[2];
				}
				else
				{
					//r = (float)molecule->multi_r;
					//g = (float)molecule->multi_g;
					//b = (float)molecule->multi_b;
					srand((int)(rand()*1200));
					r = rand() / (RAND_MAX + 1.0f) / 2.0 + 0.5;
					g = rand() / (RAND_MAX + 1.0f) / 2.0 + 0.5;
					b = rand() / (RAND_MAX + 1.0f) / 2.0 + 0.5;
				}

				//float x1 = molecule->m_SecondaryStructureData->m_CylinderEndPoint1[0], y1 = molecule->m_SecondaryStructureData->m_CylinderEndPoint1[1], z1 = molecule->m_SecondaryStructureData->m_CylinderEndPoint1[2];
				//float x2 = molecule->m_SecondaryStructureData->m_CylinderEndPoint2[0], y2 = molecule->m_SecondaryStructureData->m_CylinderEndPoint2[1], z2 = molecule->m_SecondaryStructureData->m_CylinderEndPoint2[2];
				//float radius = molecule->m_SecondaryStructureData->m_CylinderRadius;
				//m_ImposterRenderer->m_HelixRenderer->add( x1, y1, z1, x2, y2, z2, radius, r, g, b );

				int i;
				float xPrev = 0;
				float yPrev = 0;
				float zPrev = 0;
				bool gotPrevPoint = false;

				for(i=0; i<molecule->m_SecondaryStructureData->m_HelixGeometry.size(); i+= 7)
				{
					float x1, y1, z1;

					if(gotPrevPoint)
					{
						x1 = xPrev;
						y1 = yPrev;
						z1 = zPrev;
					}
					else
					{
						x1		= molecule->m_SecondaryStructureData->m_HelixGeometry.at(i+0);
						y1		= molecule->m_SecondaryStructureData->m_HelixGeometry.at(i+1);
						z1		= molecule->m_SecondaryStructureData->m_HelixGeometry.at(i+2);
					}

					xPrev		= molecule->m_SecondaryStructureData->m_HelixGeometry.at(i+3);
					yPrev		= molecule->m_SecondaryStructureData->m_HelixGeometry.at(i+4);
					zPrev		= molecule->m_SecondaryStructureData->m_HelixGeometry.at(i+5);
					float radius	= molecule->m_SecondaryStructureData->m_HelixGeometry.at(i+6);
					radius = 1.0;
					m_ImposterRenderer->m_HelixRenderer->addHelix(x1, y1, z1, xPrev, yPrev, zPrev, radius * m_ScaleValue, r, g, b, r, g, b);
					gotPrevPoint = true;
				}

				return;
			}
			// parsing sheets is tough as pdb is all messed up
			else if(molecule->resID == PDBParser::SS_SHEET)
			{

				if(molecule->m_SecondaryStructureData == 0)
				{
					return;
				}

				GLfloat colors[4];
				glGetFloatv(GL_CURRENT_COLOR, colors);
				float r = colors[0];
				float g = colors[1];
				float b = colors[2];
				int i;

				for(i=0; i<molecule->m_SecondaryStructureData->m_ResSequenceNumberPairList.size()/2; i++)
				{
				//	printf("Strand: %d to %d\n", molecule->m_SecondaryStructureData->m_ResSequenceNumberPairList[i*2],
				//		   molecule->m_SecondaryStructureData->m_ResSequenceNumberPairList[i*2+1]);
					// for each strand, add the cylinder!

					float x1 = molecule->m_SecondaryStructureData->m_StrandGeometry[i*7+0];
					float y1 = molecule->m_SecondaryStructureData->m_StrandGeometry[i*7+1];
					float z1 = molecule->m_SecondaryStructureData->m_StrandGeometry[i*7+2];
					float x2 = molecule->m_SecondaryStructureData->m_StrandGeometry[i*7+3];
					float y2 = molecule->m_SecondaryStructureData->m_StrandGeometry[i*7+4];
					float z2 = molecule->m_SecondaryStructureData->m_StrandGeometry[i*7+5];
					float radius = molecule->m_SecondaryStructureData->m_StrandGeometry[i*7+6];
					m_ImposterRenderer->m_HollowCylinderRenderer->add(x1, y1, z1, x2, y2, z2, radius, r, g, b);
				}

				return;
			}
			else
			{
				// need to render turns !
				// then return. If we dont return,
				//    we got a SS which was NULL. So we render the backbone there!
				//    Hence, from a least squares fit, we fixed the end point of cylinders to
				//		be points on the backbone, which is kind of silly, but looks better
			}

		}
	}
	else if(molecule->type == PDBParser::CHAIN_TYPE)
	{
	  //printf("Begin chain %c\n", (char)molecule->resID);
		m_BackboneAtomFromPreviousResidue = 0; // chains are disconnected

		if(colorLOD == COLOR_CHAIN)
		{
			glColor4f((float)molecule->multi_r, (float)molecule->multi_g, (float)molecule->multi_b, 0.4f);
		}

		// delete thi
		if(objectLOD == LOD_CHAIN)
		{
			// debug
			int i;
			int count = molecule->m_MultiresAtoms.size();


			for(i=0; i<count; i++)
			{
				GLfloat r, g, b;

				if(colorLOD == COLOR_CHAIN)
				{
					r = molecule->multi_r;
					g = molecule->multi_g;
					b = molecule->multi_b;
				}
				else
				{
					GLfloat colors[4];
					glGetFloatv(GL_CURRENT_COLOR, colors);
					r = colors[0];
					g = colors[1];
					b = colors[2];
				}

				m_ImposterRenderer->m_BallRenderer->addBall(molecule->m_MultiresAtoms[i]->m_Position[0],
						molecule->m_MultiresAtoms[i]->m_Position[1],
						molecule->m_MultiresAtoms[i]->m_Position[2],
						molecule->m_MultiresAtoms[i]->getRadius() * m_ScaleValue,
						r,
						g,
						b);
			}

			return;
		}
	}
	else if(molecule->type == PDBParser::PROTEIN_TYPE)
	{
		if(colorLOD == LOD_PROTEIN)
		{
			glColor4f((float)molecule->multi_r, (float)molecule->multi_g, (float)molecule->multi_b, 0.4f);
		}

		if(objectLOD == LOD_PROTEIN)
		{
			// Render the molecule level of detail
			int i;

			for(i=0; i<molecule->m_MultiresAtoms.size(); i++)
			{
				GLfloat colors[4];
				glGetFloatv(GL_CURRENT_COLOR, colors);
				float r = colors[0];
				float g = colors[1];
				float b = colors[2];
				r = molecule->multi_r;
				g = molecule->multi_g;
				b = molecule->multi_b;
				m_ImposterRenderer->m_BallRenderer->addBall(molecule->m_MultiresAtoms[i]->m_Position[0],
						molecule->m_MultiresAtoms[i]->m_Position[1],
						molecule->m_MultiresAtoms[i]->m_Position[2],
						molecule->m_MultiresAtoms[i]->getRadius() * m_ScaleValue,
						r,
						g,
						b);
			}

			// Drawing should stop at this level of detail, so return
			return;
		}
	}

	// If control has reached this point, iterate through the subgroup array and draw all subgroups
	if(molecule->type == PDBParser::COLLECTION_TYPE)
	{
		PDBParser::GroupOfAtoms* currSubGroup = molecule->m_SubGroups.at(0);   // render only the first model

		if(currSubGroup)
		{
			recursivePrepareRender(currSubGroup, objectLOD, colorLOD);
		}
	}
	else
	{
		int i;
		int n = (int) molecule->m_SubGroups.size();
		//fprintf( stderr, "sub groups: %d\n", molecule->m_SubGroups.size() );
		for(i=0; i < n; i++)
		{
			PDBParser::GroupOfAtoms* currSubGroup = molecule->m_SubGroups.at(i);

			if(currSubGroup)
			{
				recursivePrepareRender(currSubGroup, objectLOD, colorLOD);
			}
		}
	}
}


// ugly global hacks, for the sake of deadline, need to remove these
bool BallAndStickData_recursivePrepareBackbone_foundNewChain = true;
double BallAndStickData_recursivePrepareBackbone_prev_x;
double BallAndStickData_recursivePrepareBackbone_prev_y;
double BallAndStickData_recursivePrepareBackbone_prev_z;
// We render the backbone in the chain model and when we do SS_NULL
// Draws the backbone of a molecule as a chain.  Each bond in the backbone is drawn
// as a cylinder, and the atomic spheres are not drawn.
void BallAndStickData::recursivePrepareBackbone(PDBParser::GroupOfAtoms* molecule, int color, int objectLOD)
{
	if((objectLOD == LOD_SS) && (molecule->type == PDBParser::SS_TYPE))
	{
		// removing the sheets as parsing sheets was tough
		if((molecule->resID == PDBParser::SS_HELIX) || (molecule->resID == PDBParser::SS_SHEET))
			//if( molecule->resID == PDBParser::SS_HELIX )
		{
			BallAndStickData_recursivePrepareBackbone_foundNewChain = true;
			return;
		}
	}

	int i, n;

	if(((objectLOD == LOD_SS) || (objectLOD == LOD_CHAIN)) && (molecule->type == PDBParser::CHAIN_TYPE))
	{
		// setting chain color. So all SS_NULL under that chain appear in that color
		double r, g, b;
		srand(color*100000);
		r = rand() / (RAND_MAX + 1.0f);
		g = rand() / (RAND_MAX + 1.0f);
		b = rand() / (RAND_MAX + 1.0f);
		glColor4f(r,g,b,0.6f);
		// reset global var foundNewChain to true
		BallAndStickData_recursivePrepareBackbone_foundNewChain = true;
	}

	if(((objectLOD == LOD_SS) || (objectLOD == LOD_CHAIN)) && (molecule->type == PDBParser::RESIDUE_TYPE))
	{
		if(molecule->resID == -1)
		{
			return;    // no backbone for non standard types !
		}

		int m = molecule->m_Atoms.size();

		if(m < 3)
		{
			return;    // need atleast 3 for sanity
		}

		// if first time in new chain,
		//		start new chain.
		//		draw 2 cylinders
		// else, you should have stored the prev point to connect to!
		//		draw 3 cylinders
		// store new end point
		// reset global variable foundNewChain to false
		double r, g, b;	// these should be set by chain
		GLfloat colors[4];
		glGetFloatv(GL_CURRENT_COLOR, colors);
		r = colors[0];
		g = colors[1];
		b = colors[2];

		for(i=0; i<1; i++)
		{
			double x, y, z, rad;
			PDBParser::Atom* atom = molecule->m_Atoms.at(i);
			x = atom->m_Position[0];
			y = atom->m_Position[1];
			z = atom->m_Position[2];
			rad = getCorrectRadius(atom);

			if(BallAndStickData_recursivePrepareBackbone_foundNewChain)
			{
				BallAndStickData_recursivePrepareBackbone_foundNewChain = false;
			}
			else
			{
				if((x-BallAndStickData_recursivePrepareBackbone_prev_x)*(x-BallAndStickData_recursivePrepareBackbone_prev_x)+
						(y-BallAndStickData_recursivePrepareBackbone_prev_y)*(y-BallAndStickData_recursivePrepareBackbone_prev_y)+
						(z-BallAndStickData_recursivePrepareBackbone_prev_z)*(z-BallAndStickData_recursivePrepareBackbone_prev_z)  > 100.0)
				{
				  //printf("whats wrong ?");
				}

				else
				{
					m_ImposterRenderer->m_StickRenderer->addStick(BallAndStickData_recursivePrepareBackbone_prev_x,
							BallAndStickData_recursivePrepareBackbone_prev_y,
							BallAndStickData_recursivePrepareBackbone_prev_z,
							x,
							y,
							z,
							0.3f*m_ScaleValue,
							r,
							g,
							b,
							r,
							g,
							b);
				}
			}

			BallAndStickData_recursivePrepareBackbone_prev_x = x;
			BallAndStickData_recursivePrepareBackbone_prev_y = y;
			BallAndStickData_recursivePrepareBackbone_prev_z = z;
		}
	}

	// If control has reached this point, iterate through the subgroup array and draw all subgroups
	if(molecule->type == PDBParser::COLLECTION_TYPE)
	{
		PDBParser::GroupOfAtoms* currSubGroup = molecule->m_SubGroups.at(0);   // render only the first model

		if(currSubGroup)
		{
			recursivePrepareBackbone(currSubGroup, 0, objectLOD);
		}
	}
	else
	{
		n = (int) molecule->m_SubGroups.size();

		for(i=0; i < n; i++)
		{
			PDBParser::GroupOfAtoms* currSubGroup = molecule->m_SubGroups.at(i);
			recursivePrepareBackbone(currSubGroup, i, objectLOD);
		}
	}
}

bool BallAndStickData::prepareRenderer()
{
	if(m_Frame >= m_GroupOfAtoms.size())
	{
		return false;
	}

	if(!(m_GroupOfAtoms[m_Frame]))
	{
		return false;
	}

	m_ImposterRenderer->clear();

	if(m_SphereRenderer)
	{
		m_SphereRenderer->clearObjects();
	}

	if( (objectLOD() == LOD_HBOND_ALL) || (objectLOD() == LOD_HBOND_INT) || (objectLOD() == LOD_HBOND_EXT) )
	{
		prepareHydrogenBondRender(m_GroupOfAtoms[m_Frame], objectLOD());
	}
//	else
	{
		recursivePrepareRender(m_GroupOfAtoms[m_Frame], objectLOD(), colorLOD());
		BallAndStickData_recursivePrepareBackbone_foundNewChain = true;
		recursivePrepareBackbone(m_GroupOfAtoms[m_Frame], 1, objectLOD());
	}

	// load the right program initially
	// CHA: save flags below to avoid initializing unnecessarily.
	if( (colorLODsave != m_ColorLod) || (functionAssociateSave != m_FunctionAssociated) )
	{
		colorLODsave = m_ColorLod;
		functionAssociateSave = m_FunctionAssociated;
		if(m_ColorLod == COLOR_FUNCTION && m_FunctionAssociated)
		{
			m_ImposterRenderer->initSubRenderers(true);
		}
		else
		{
			m_ImposterRenderer->initSubRenderers(false);
		}
	}

	return true;
}

void BallAndStickData::renderOnce()
{
	if(m_Frame >= m_GroupOfAtoms.size())
	{
		return;
	}

	if(!(m_GroupOfAtoms[m_Frame]))
	{
		return;
	}

	/*
	// CHA: after implementing rendering code for multi-resolution surface, we can enable this part again.
	// - we need to enable define in BallAndStickData.h and option in BallAndStickRenderingProperties.cpp
	if(m_ObjectLod == LOD_SURFACE)
	{
		// powei's display code
		return;
	}
	*/

	// terrible hack.
	// In imposter renderer, in ball renderer, we use the 4th component of color as the radius.
	// hence its not  easy to change the color each time we render.
	// If we are rendering a virus, and we want to render each protein separately, we need to
	// randomly set colors each time we transform because we dont have the entire data set.
	// hence we need to send in some 'average radius' into the 4th component of the randomly
	// generated protein color.
	// This hack is used only for molecules where we transform and render and use protein coloring
	float avgradius = 1.0f;

	if(m_ObjectLod == LOD_RESIDUE)
	{
		avgradius = 5.0f;
	}

	if(m_ObjectLod == LOD_SS)
	{
		avgradius = 0.5f;
	}

	if(m_ObjectLod == LOD_CHAIN)
	{
		avgradius = 0.5f;
	}

	bool randomizeColors = false;

	if(m_ColorLod == COLOR_PROTEIN)
	{
		randomizeColors = true;
	}

	double* rotations = 0;
	double* translations = 0;
	int numberOfTransformations = 0;
	{
		if(m_GroupOfAtoms[m_Frame]->m_CollectionData)
		{
			numberOfTransformations = m_GroupOfAtoms[m_Frame]->m_CollectionData->getNumberOfTransformations();

			if(numberOfTransformations > 0)
			{
				rotations    = new double[numberOfTransformations*9];
				translations = new double[numberOfTransformations*3];

				if(!m_GroupOfAtoms[m_Frame]->m_CollectionData->getRotations(rotations) ||
						!m_GroupOfAtoms[m_Frame]->m_CollectionData->getTranslations(translations))
				{
					numberOfTransformations = 0;
					delete []rotations;
					rotations = 0;
					delete []translations;
					translations = 0;
				}
			}
		}
	}
	{
		if(m_max[0] < m_min[0])
		{
			m_GroupOfAtoms[m_Frame]->getMinMax(&(m_min[0]), &(m_min[1]), &(m_min[2]), &(m_max[0]), &(m_max[1]), &(m_max[2]));
		}

		Plane plane = OpenGL_Viewer::getClipPlane(m_min, m_max, m_ClippingPlane);
		double plane0[] = { plane.a(), plane.b(), plane.c(), plane.d() };
		glClipPlane(GL_CLIP_PLANE0, plane0);
		glEnable(GL_CLIP_PLANE0);
	}
	m_ImposterRenderer->renderBuffer(
		m_UseHarwareBuffer,
		(m_ColorLod == COLOR_FUNCTION && m_FunctionAssociated),
		rotations,
		translations,
		numberOfTransformations,
		randomizeColors,
		avgradius
	);
	ObjectRendererLibrary::TransformationParameters* transformationParameters = new ObjectRendererLibrary::TransformationParameters(
		rotations,
		translations,
		numberOfTransformations,
		randomizeColors,
		avgradius
	);

	if(m_SphereRenderer)
	{
		m_SphereRenderer->render(transformationParameters);
	}

	delete transformationParameters;
	{
		glDisable(GL_CLIP_PLANE0);
	}
}

void BallAndStickData::render()
{
	if(m_GroupOfAtoms.size() <= m_Frame)
	{
		return;
	}

	if(!m_GroupOfAtoms[m_Frame])
	{
		return;
	}

//	int currentRenderId = m_DataManager->m_RendererSet->getCurrentRendererID();
//	m_ImposterRenderer = m_ImposterRendererList[ currentRenderId ];
//	m_DataManager->m_RendererSet->getCurrentRenderer()->makeCurrent();

	if(m_ReplicationMatrices.size() > 0)
	{
		updateVisibleTransforms();
		int i;

		for(i=0; i<m_ReplicationMatrices.size(); i++)
		{
			if(!m_ReplicationVisible[i])
			{
				continue;
			}

			CCVOpenGLMath::Matrix* transformation = m_ReplicationMatrices[i];

			if(!transformation)
			{
				continue;
			}

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(transformation->getMatrix());
			renderOnce();
			glPopMatrix();
		}
	}
	else
	{
		//double cutoffSq = 10; // need to make this a parameter! SKVINAY
		//if( distanceSqFromObject() < cutoffSq ) // need to make this a multires call
		renderOnce();
	}
}

void BallAndStickData::displayColorLegendSlot()
{
	m_ColorLegend = new ColorLegend();
	m_ColorLegend->setText();
	m_ColorLegend->setColors();
	m_ColorLegend->show();
}

void BallAndStickData::selectFunctionSlot()
{
	if(!m_BallAndStickRenderingProperties)
	{
		return;    // sanity check
	}

	QString m_Filter = "";
	m_Filter.append("Volume files (*.rawiv *.rawv);; Rawiv (*.rawiv);; RawV (*.rawv);; All files (*.*)");
	QString fileName = QFileDialog::getOpenFileName(nullptr, "Open", "../DataSet", m_Filter);
/*Q3Err:C
	if(fileName != 0)
*/
	if(!fileName.isNull())
	{
		m_BallAndStickRenderingProperties->_ui->m_LineEdit_FunctionEdit->setText(fileName);
		SimpleVolumeData* simpleVolumeData = 0;
		{
			VolumeLoader* vLoader = new VolumeLoader();
			simpleVolumeData = vLoader->loadFile(fileName.toLatin1().constData());
			delete vLoader;

			if(!simpleVolumeData)
			{
				return;
			}
		}

		if(simpleVolumeData->getNumberOfVariables() < 1)
		{
			delete simpleVolumeData;
			return;
		}

		m_ImposterRenderer->addFunction((unsigned char*)simpleVolumeData->getData(0),
										simpleVolumeData->getMinX(),
										simpleVolumeData->getMinY(),
										simpleVolumeData->getMinZ(),
										simpleVolumeData->getMaxX(),
										simpleVolumeData->getMaxY(),
										simpleVolumeData->getMaxZ(),
										simpleVolumeData->getWidth(),
										simpleVolumeData->getHeight(),
										simpleVolumeData->getDepth());
		m_FunctionAssociated = true;
	}
}

void BallAndStickData::selectColorMapSlot()
{
	if(!m_BallAndStickRenderingProperties)
	{
		return;    // sanity check
	}

	if(m_Frame >= m_GroupOfAtoms.size())
	{
		return;
	}

	if(!(m_GroupOfAtoms[m_Frame]))
	{
		return;
	}

	QString m_Filter = "";
	m_Filter.append("Color map files (*.txt *.cmap);; All files (*.*)");
	QStringList colorMapfileNames = QFileDialog::getOpenFileNames(nullptr, "Open", "../DataSet", m_Filter);

	if(colorMapfileNames.size() < 1)
	{
		return;
	}

	if(colorMapfileNames.size() != m_GroupOfAtoms.size())
	{
		return;
	}

	int i;

	for(i=0; i<colorMapfileNames.size(); i++)
	{
		if(!(m_GroupOfAtoms[i]) || ! colorMapfileNames[i].length())
		{
			continue;
		}

		PDBParser::GOAColor* goaColor = new PDBParser::GOAColor();
		goaColor->parseColormapFile((char*)(colorMapfileNames[i].toLatin1().constData()));
		goaColor->ApplyColormap(m_GroupOfAtoms[i]);
		delete goaColor;
		m_BallAndStickRenderingProperties->_ui->m_LineEdit_ColorMap->setText(colorMapfileNames[0]);
	}
}

QString BallAndStickData::getSelection(CCVOpenGLMath::Ray targetVector)
{
	if(m_Frame >= m_GroupOfAtoms.size())
	{
		return 0;
	}

	if(!(m_GroupOfAtoms[m_Frame]))
	{
		return 0;
	}

	// return [atom type, position radius, hydrophobicity], [residue type, number], [chain number]
	//        [protein name]
	PDBParser::Atom* selectedAtom = m_GroupOfAtoms[m_Frame]->getSelectedAtom(targetVector);

	if(selectedAtom)
	{
		QString selString = QString(selectedAtom->getName()) +
							QString::number(selectedAtom->getRadius()) +
							selectedAtom->getResidueName() +
							QString::number(selectedAtom->getResidueNum()) +
							QString::number(selectedAtom->getChain()) +
							m_FileName;
		//fprintf(stderr, "Got an atom %s\n", selString.toLatin1().constData());
		return selString;
	}

	return 0;
}

void BallAndStickData::solventRadiusSlot(bool useSolventRadii)
{
	prepareRenderer();
	m_DataManager->update();
}

// see if the solvent enlarged radius is enabled.
// Get correct entry from the table.
double BallAndStickData::getCorrectRadius(PDBParser::Atom* atom)
{
	double radius = 1.0;

	if(!atom)
	{
		radius =  1.0;
	}

	if(!m_BallAndStickRenderingProperties)
	{
		radius =  atom->getRadius();
	}

	if(m_BallAndStickRenderingProperties->_ui->m_CheckBox_SolventEnlargedRadius->isChecked())
	{
		radius =  atom->getRadius(PDBParser::GroupOfAtoms::ROUX_RADIUS);
	}
	else
	{
		radius =  atom->getRadius();
	}

	return radius;
}

bool BallAndStickData::read(QStringList fileNames)
{
	if(fileNames.size() < 1)
	{
		return false;
	}

	const char* fileName = (fileNames[0]).toLatin1().constData();

	{
		std::vector<PDBParser::GroupOfAtoms*>::iterator iter = m_GroupOfAtoms.begin(), end = m_GroupOfAtoms.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_GroupOfAtoms.clear();
	}

	int i;

	for(i=0; i<fileNames.size(); i++)
	{
		GOALoader* goaLoader = new GOALoader();
		m_GroupOfAtoms.push_back(goaLoader->loadFile(fileNames[i].toLatin1().constData()));

		if(!m_GroupOfAtoms[i])
		{
			int j;

			for(j=0; j<i-1; j++)
			{
				delete m_GroupOfAtoms[j];
				m_GroupOfAtoms.clear();
			}

			delete goaLoader;
			return false;
		}

		delete goaLoader;
	}

	if(m_BallAndStickRenderingProperties)
	{
		if(m_GroupOfAtoms.size() > 1)
		{
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setMinimum(0);
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setMaximum(m_GroupOfAtoms.size()-1);
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setTickInterval(1);
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setEnabled(true);
		}
		else
		{
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setMinimum(0);
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setMaximum(0);
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setTickInterval(1);
			m_BallAndStickRenderingProperties->_ui->m_Slider_Frame->setEnabled(false);
		}
	}

	m_Frame = 0;

	// BUG: interface crashes on certain PQR data
	//  arand: trying to fix this, 9-6-2011
	if( m_ColorMapInterface && m_GroupOfAtoms.size() > m_Frame ) m_ColorMapInterface->setGOAData( m_GroupOfAtoms[m_Frame] );
	if(m_BallAndStickRenderingProperties && m_Rover3DWidget)
	{
		fprintf( stderr, "%f\n", m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeB_2->text().toFloat() );
		fprintf( stderr, "%f\n", m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeB_2->text().toFloat() );
		fprintf( stderr, "%f\n", m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeIso_2->text().toFloat() );
		fprintf( stderr, "%f\n", m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeIso_2->text().toFloat());
		#ifdef MS
		m_MultiresolutionSurface = new MultiresolutionSurface(m_GroupOfAtoms[m_Frame],
				&(m_Rover3DWidget->getBoundary()), &(m_Rover3DWidget->getSubVolume()),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeB_2->text().toFloat(),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeB_2->text().toFloat(),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeIso_2->text().toFloat(),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeIso_2->text().toFloat());
		#endif
	}

	return true;
}

bool BallAndStickData::save(const char* filename)
{
	if(!filename)
	{
		return false;
	}

	if(m_Frame >= m_GroupOfAtoms.size())
	{
		return 0;
	}

	if(!(m_GroupOfAtoms[m_Frame]))
	{
		return 0;
	}

	GOALoader* goaLoader = new GOALoader();
	CCVOpenGLMath::Matrix m = getTransformation(); // arand: change to eliminate warning
	bool ret = goaLoader->saveFile(filename, m_GroupOfAtoms[m_Frame], PDBParser::ATOM_TYPE, &m);//&(getTransformation())); // arand change
	delete goaLoader;
	return ret;
}

void BallAndStickData::clipValueChangedSlot(int value)
{
	if(!m_BallAndStickRenderingProperties)
	{
		return;
	}

	int minVal = m_BallAndStickRenderingProperties->_ui->m_Slider_Clip->minimum();
	int maxVal = m_BallAndStickRenderingProperties->_ui->m_Slider_Clip->maximum();
	int clamped = (value>minVal ? value : minVal);
	clamped = (clamped<maxVal ? clamped: maxVal);
	m_ClippingPlane = (double)clamped/((double)(maxVal - minVal));

	if(m_DataManager)
	{
		m_DataManager->update();
	}
}

bool BallAndStickData::supportsFileNames(QStringList fileNames)
{
	if(fileNames.size() < 1)
	{
		return false;
	}

	int i;
	bool ret = true;

	for(i=0; i<fileNames.size(); i++)
	{
		GOALoader* goaLoader = new GOALoader();
		ret = ret && (fileNames[i].endsWith("pqr") ||
					  fileNames[i].endsWith("pdb") ||
					  fileNames[i].endsWith("pdb1") ||
					  fileNames[i].endsWith("pts")||
					  fileNames[i].endsWith("xyzr")||
					  fileNames[i].endsWith("xyz"));
		delete goaLoader;
	}

	return ret;
}

void BallAndStickData::frameValueChangedSlot(int value)
{
	if(m_GroupOfAtoms.size() < 1)
	{
		return;
	}

	m_Frame = value;

	if(m_Frame < 0)
	{
		m_Frame = 0;
	}

	if(m_Frame >= m_GroupOfAtoms.size())
	{
		m_Frame = m_GroupOfAtoms.size() - 1;
	}

	prepareRenderer();
	// if( m_ColorMapInterface && m_GroupOfAtoms.size() > m_Frame ) m_ColorMapInterface->setGOAData( m_GroupOfAtoms[m_Frame] );
	m_DataManager->update();
}

bool BallAndStickData::getMinMax(float* min, float* max)
{
	if(m_GroupOfAtoms.size() <= m_Frame)
	{
		return false;
	}

	if(!(m_GroupOfAtoms[m_Frame]) || !min || !max)
	{
		return false;
	}

	double dmin[3], dmax[3];
	m_GroupOfAtoms[m_Frame]->getMinMax(&(dmin[0]), &(dmin[1]), &(dmin[2]), &(dmax[0]), &(dmax[1]), &(dmax[2]));
	min[0] = dmin[0];
	min[1] = dmin[1];
	min[2] = dmin[2];
	max[0] = dmax[0];
	max[1] = dmax[1];
	max[2] = dmax[2];
	return true;
}

void BallAndStickData::colorMapInterfaceSlot()
{
	m_ColorMapInterface->show();
}

void BallAndStickData::resetRenderer()
{
	prepareRenderer();
	m_DataManager->update();
}

double BallAndStickData::correlate()
{
	if(m_GroupOfAtoms.size() <= m_Frame)
	{
		return false;
	}

	if(!(m_GroupOfAtoms[m_Frame]))
	{
		return false;
	}

	// find previous vol, if none, return 0
	SimpleVolumeData* sData = 0;
	{
		VolumeData* vData = m_DataManager->getScalarVolumeData();

		if(!vData)
		{
			return 0;
		}

		sData = vData->m_SimpleVolumeData;

		if(!sData)
		{
			return 0;
		}
	}

	if(!m_CorrelationModule)
	{
		m_CorrelationModule = new Correlation(sData, m_GroupOfAtoms[m_Frame]);
	}

	return m_CorrelationModule->correlate(getTransformation());
}

void BallAndStickData::setTransformation(CCVOpenGLMath::Matrix m)
{
	AbstractData::setTransformation(m);
}

void BallAndStickData::addTransformation(CCVOpenGLMath::Matrix m, float scale)
{
	AbstractData::addTransformation(m, scale);
	m_ScaleValue *= fabs(scale);
	prepareRenderer();
}

void BallAndStickData::addPan(CCVOpenGLMath::Vector v)
{
	AbstractData::addPan(v);

	if(!m_BallAndStickRenderingProperties)
	{
		return;
	}

	if(m_BallAndStickRenderingProperties->_ui->m_CheckBox_Correlate->isChecked())
	{
		m_BallAndStickRenderingProperties->_ui->m_LineEdit_Correlate->setText(QString::number(correlate()));
	}
}

void BallAndStickData::addRotation(CCVOpenGLMath::Matrix r)
{
	AbstractData::addRotation(r);
}

void BallAndStickData::addRotation(int axis, float angle)
{
	AbstractData::addRotation(axis, angle);
}

void BallAndStickData::addScale(float scale)
{
	AbstractData::addScale(scale);
	m_ScaleValue *= fabs(scale);
	prepareRenderer();
}

bool BallAndStickData::constructMultiresSurfaceObject()
{
	#ifdef MS
	if(m_MultiresolutionSurface)
	{
		return true;
	}

	if(m_BallAndStickRenderingProperties && m_Rover3DWidget && !m_MultiresolutionSurface)
	{
		m_MultiresolutionSurface = new MultiresolutionSurface(m_GroupOfAtoms[m_Frame],
				&(m_Rover3DWidget->getBoundary()), &(m_Rover3DWidget->getSubVolume()),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeB_2->text().toFloat(),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeB_2->text().toFloat(),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeIso_2->text().toFloat(),
				m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeIso_2->text().toFloat());
		return true;
	}
	#else
		fprintf( stderr, "Multi-resolution surface was not enabled in this build\n");
	#endif
	return false;
}

void BallAndStickData::subCubeBChangedSlot()
{
	if(!constructMultiresSurfaceObject())
	{
		return;
	}

	#ifdef MS
	m_MultiresolutionSurface->changeInnerB(m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeB_2->text().toFloat());
	#endif
}

void BallAndStickData::outerCubeBChangedSlot()
{
	if(!constructMultiresSurfaceObject())
	{
		return;
	}

	#ifdef MS
	m_MultiresolutionSurface->changeOuterB(m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeB_2->text().toFloat());
	#endif
}

void BallAndStickData::subCubeIsoChangedSlot()
{
	if(!constructMultiresSurfaceObject())
	{
		return;
	}

	#ifdef MS
	m_MultiresolutionSurface->changeInnerIsovalue(m_BallAndStickRenderingProperties->_ui->m_LineEdit_SubCubeIso_2->text().toFloat());
	#endif
}

void BallAndStickData::outerCubeIsoChangedSlot()
{
	if(!constructMultiresSurfaceObject())
	{
		return;
	}

	#ifdef MS
	m_MultiresolutionSurface->changeOuterIsovalue(m_BallAndStickRenderingProperties->_ui->m_LineEdit_OuterCubeIso_2->text().toFloat());
	#endif
}

void BallAndStickData::roverExploringSlot()
{
	if(!constructMultiresSurfaceObject())
	{
		return;
	}

	//m_MultiresolutionSurface->changeInnerExtent(&(m_Rover3DWidget->getSubVolume()));
	// powei recompute the surface
	if(m_DataManager)
	{
		m_DataManager->update();
	}
}

void BallAndStickData::roverReleasedSlot()
{
	if(!constructMultiresSurfaceObject())
	{
		return;
	}

	#ifdef MS
	m_MultiresolutionSurface->changeInnerExtent(&(m_Rover3DWidget->getSubVolume()));
	#endif

	// powei recompute the surface
	// "flatten" the GOA
	//std::vector<PDBParser::Atom *> m_AtomList;
	//BlurMapsDataManager::flattenGOA(m_GroupOfAtoms[m_Frame], m_AtomList, 0, 0, 0, 0, VDV_RADIUS, PDBParser::ATOM_TYPE  );
	if(m_DataManager)
	{
		m_DataManager->update();
	}
}

void BallAndStickData::replicateSlot()
{
	QString transFile = QFileDialog::getOpenFileName(nullptr, "Open", QString(), "Transformations (*.txt);;All files (*.*)");

	if(transFile.length() < 1)
	{
		return;
	}

	readTransformations(transFile.toLatin1().constData());
}

bool BallAndStickData::parseAnimationCommand(QStringList commands, int curCommand)
{
	return false;
}
