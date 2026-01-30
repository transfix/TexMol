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
#include <Nurbs/NURBS.h>
#include <Nurbs/Patch.h>

using namespace CCV_NURBS;

NURBS::NURBS()
{
}

NURBS::~NURBS()
{
	deleteData();
}

void NURBS::deleteData()
{
	for (int i=0; i<m_Patches.size(); i++)
	{
		delete m_Patches[i];
	}
	m_Patches.clear();
}

NURBS::NURBS(const NURBS& copy)
{
	set(copy);
}

NURBS& NURBS::operator=(const NURBS& copy)
{
	return set(copy);
}

NURBS& NURBS::set(const NURBS& copy)
{
	if (this!=&copy)
	{
		deleteData();
		int i;
		for (i=0; i<copy.m_Patches.size(); i++)
		{
			Patch* patch = new Patch(*(copy.m_Patches[i]));
			m_Patches.push_back(patch);
		}
	}
	return *this;
}

bool NURBS::render()
{
	for (int i=0; i<m_Patches.size(); i++)
	{
		m_Patches[i]->render();
	}
	return true;
}
