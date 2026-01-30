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
#include <PatchComplex/ConcavePatch.h>
#include <PatchComplex/ConvexPatch.h>
#include <PatchComplex/PatchComplex.h>
#include <PatchComplex/ToroidalPatch.h>
#include <PDBParser/GroupOfAtoms.h>
#include <UsefulMath/Vector.h>

using namespace SurfacePatchComplex;
using namespace PDBParser;

PatchComplex::PatchComplex()
{
	m_Molecule = 0;
	m_Initialized = false;
}

PatchComplex::PatchComplex(GroupOfAtoms* groupOfAtoms)
{
	m_Molecule = groupOfAtoms;
	m_Initialized = false;
}

PatchComplex::~PatchComplex()
{
	deleteData();
}

void PatchComplex::deleteData()
{
	for (int i=0; i<m_ConvexPatches.size(); i++)
	{
		delete m_ConvexPatches[i];
	}
	m_ConvexPatches.clear();
}

void PatchComplex::setMolecule(GroupOfAtoms* groupOfAtoms)
{
	deleteData();
	m_Molecule = groupOfAtoms;
	m_Initialized = false;
}

bool PatchComplex::createComplex()
{
	deleteData();
	if (!m_Molecule)
	{
		return false;
	}
	m_Initialized = false;
	// make grid, insert atoms, keep intersection information.
	m_Initialized = true;
	return true;
}

int PatchComplex::getNumberOfConvexPatches()
{
	if (!m_Initialized)
	{
		return 0;
	}
	return m_ConvexPatches.size();
}

int PatchComplex::getNumberOfConcavePatches()
{
	if (!m_Initialized)
	{
		return 0;
	}
	return m_ConcavePatches.size();
}

int PatchComplex::getNumberOfToroidalPatches()
{
	if (!m_Initialized)
	{
		return 0;
	}
	return m_ToroidalPatches.size();
}
