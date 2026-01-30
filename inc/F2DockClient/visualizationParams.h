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
#ifndef VISPAR
#define VISPAR

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

enum {UOFB_SURF_VIS, SMOOTH_SURF_VIS, POTENTIAL_VIS};
enum {COULOMBIC_POT, GB_POT, PB_POT};

class VisualizationParams
{
	private:
	int receptorVisType;
	int ligandVisType;
	int receptorPotType;
	int ligandPotType;

	bool highlightInterface;

	bool multipleLigand;

	public:

	VisualizationParams();
	~VisualizationParams();
	
	bool init();
	bool save(string fileName);
	bool load(string fileName);

	void setReceptorDisplayType(int r) {receptorVisType = r;}
	void setLigandDisplayType(int l) {ligandVisType = l;}
	void setReceptorPotentialType(int r) {receptorPotType = r;}
	void setLigandPotentialType(int l) {ligandPotType = l;}

	void setDisplayType(int d){receptorVisType = d; ligandVisType = d;}
	void setPotentialType(int d){receptorPotType = d; ligandPotType = d;}

	void setHighlightInterface(bool hi) {highlightInterface = hi;}
	void setMultipleLigand(bool ml) {multipleLigand = ml;}

	int getReceptorDisplayType(void) {return receptorVisType;}
	int getLigandDisplayType(void) {return ligandVisType;}
	int getReceptorPotentialType(void) {return receptorPotType;}
	int getLigandPotentialType(void) {return ligandPotType;}
	bool isHighlightInterface(void) {return highlightInterface;}
	bool isMultipleLigand(void) {return multipleLigand;}
};



#endif
