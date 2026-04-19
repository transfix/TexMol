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
// This class extends the dialog created by QT Designer so we can implement functions

#include <TexMol/Dialogs/SurfaceDialog.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <iostream>
#include <string>
#ifdef MSA
#include <MolSurfAPI/molsurf.h>
#endif

using std::cout;
using std::string;

SurfaceDialog::SurfaceDialog(QWidget* parent,const char* name, bool modal, Qt::WindowFlags fl)
	: SurfaceDialogBase(parent,name,modal,fl)
{
	m_Parent = parent;
	setupSlot("Adaptive grid");
}

SurfaceDialog::~SurfaceDialog()
{
}

// Return a Geometry pointer
// Should take a const argument, but the trail of non-const naughtiness goes deep
Geometry* SurfaceDialog::getSurface(PDBParser::GroupOfAtoms* molecule)
{
	// Get variables in order
	QString algorithm	= inputAlgorithm->currentText();
	int size 		= inputSize->value();
	int isovalue		= inputIsovalue->value();
	double blobbiness	= inputBlobbiness->text().toDouble();
	bool radiusType		= (bool)inputRadiusType->currentIndex();
	int level		= inputLevel->value();
	int iterations		= inputIterations->value();
	double weight		= inputWeight->text().toDouble();
	bool optimizeRadii	= inputOptimizeRadii->isChecked();


	cout << "Get Surface Member Function No Longer In Use/Supported." << endl;

}


//Geometry* getSurfaceExtern(QString algorithm, int size, int isovalue,double blobbiness,
//			   bool radiusType, int level, int iterations, double weight, 
//			   bool optimizeRadii, PDBParser::GroupOfAtoms* molecule) {  
Geometry* getSurfaceExtern(GetSurfaceData d) {

  QString algorithm	= d.algorithm;
  int size 		= d.size;
  int isovalue		= d.isovalue;
  double blobbiness	= d.blobbiness;
  bool radiusType	= d.radiusType;
  int level		= d.level;
  int iterations	= d.iterations;
  double weight		= d.weight;
  bool optimizeRadii	= d.optimizeRadii;
  PDBParser::GroupOfAtoms* molecule = d.molecule;
	// Now call the appropriate function to get our geometry!
	Geometry* geo = 0;


/*Q3Err:C
	if(algorithm == "Adaptive grid")
*/
	if(algorithm.compare(QString("Adaptive grid")) == 0)
	{
		#ifdef MSA
		geo = surfaceUsingAdaptiveGrid(molecule, size);
		#else
		fprintf( stderr, "MolSurfAPI was not enabled in this build\n");
		#endif
	}
/*Q3Err:C
	else if(algorithm == "HLS")
*/
	else if(algorithm.compare(QString("HLS")) == 0)
	{
		#ifdef MSA
		//size, iterations, weight
		geo = surfaceUsingHLS(molecule, size, iterations, weight);
		#else
		fprintf( stderr, "MolSurfAPI was not enabled in this build\n");
		#endif
	}
/*Q3Err:C
	else if(algorithm == "Gaussian blur")
*/
	else if(algorithm.compare(QString("Gaussian blur")) == 0)
	{
		#ifdef MSA
		//all except iterations and weight
		geo = surfaceUsingGaussianBlur(molecule, isovalue, size, blobbiness, (int)radiusType, level, optimizeRadii);	
		#else
		fprintf( stderr, "MolSurfAPI was not enabled in this build\n");
		#endif

	}
/*Q3Err:C
	else if(algorithm == "LBIE")
*/
	else if(algorithm.compare(QString("LBIE")) == 0)
	{
		//isovalue
		int meshtype = 0; // For now we aren't exposing this functionality
		//geo = surfaceusingLBIE(molecule, isovalue, meshtype);
	}
	else
	{
		cout << "ERROR";
		exit(-1);
	}


	/* // debugging output
	cout << "Results: " << endl;
	cout << geo->m_bExtentsReady << " " << geo->m_bQuadFlatNormalsReady << " " 
	     << geo->m_bQuadSmoothNormalsReady << " " <<  geo->m_bTriFlatNormalsReady << " "
	     << geo->m_bTriSmoothNormalsReady << " " << geo->m_RenderLines << " " 
	     << geo->m_RenderPoints << " " << geo->m_RenderQuads << " "
	     << geo->m_RenderTriangles << " " << geo->m_RenderWireframe << " " 
	     << geo->m_TriangleDerivativesValid << " " << geo->m_Use2DTriangleTexture << " " 
	     << geo->m_UseLineColors << " " << geo->m_UsePointColors << " "
	     << geo->m_UseTriangleColors << " " << geo->m_UseWireframeColors << endl;

	cout << geo->m_Tris << " " << geo->m_TriVertTexCoords << " "
	     << geo->m_TriVerts << " " << geo->m_TriVertNormals << " " 
	     << geo->m_TriVertColorsTransparent << " " << geo->m_TriMeanCurv << " " 
	     << geo->m_TriGaussianCurv << " " << geo->m_TriFlatVerts << " "
	     << geo->m_TriFlatTexCoords << " " << geo->m_TriFlatNormals << " "
	     << endl;
	*/

	return geo;
}

// Gray out the inputs the selected algorithm does not use
void SurfaceDialog::setupSlot(const QString& algorithm)
{
	if(algorithm == "Adaptive grid")
	{
		//size only
		inputSize->setEnabled(true);
		inputIsovalue->setEnabled(false);
		inputBlobbiness->setEnabled(false);
		inputRadiusType->setEnabled(false);
		inputLevel->setEnabled(false);
		inputIterations->setEnabled(false);
		inputWeight->setEnabled(false);
		inputOptimizeRadii->setEnabled(false);
	}
	else if(algorithm == "HLS")
	{
		//size, iterations, weight
		inputSize->setEnabled(true);
		inputIsovalue->setEnabled(false);
		inputBlobbiness->setEnabled(false);
		inputRadiusType->setEnabled(false);
		inputLevel->setEnabled(false);
		inputIterations->setEnabled(true);
		inputWeight->setEnabled(true);
		inputOptimizeRadii->setEnabled(false);
	}
	else if(algorithm == "Gaussian blur")
	{
		//all except iterations and weight
		inputSize->setEnabled(true);
		inputIsovalue->setEnabled(true);
		inputBlobbiness->setEnabled(true);
		inputRadiusType->setEnabled(true);
		inputLevel->setEnabled(true);
		inputIterations->setEnabled(false);
		inputWeight->setEnabled(false);
		inputOptimizeRadii->setEnabled(true);
	}
	else if(algorithm == "LBIE")
	{
		//isovalue, mesh type=0
		inputSize->setEnabled(true);
		inputIsovalue->setEnabled(true);
		inputBlobbiness->setEnabled(false);
		inputRadiusType->setEnabled(false);
		inputLevel->setEnabled(false);
		inputIterations->setEnabled(false);
		inputWeight->setEnabled(false);
		inputOptimizeRadii->setEnabled(false);
	}
	else
	{
		cout << "ERROR";
		exit(-1);
	}
}
