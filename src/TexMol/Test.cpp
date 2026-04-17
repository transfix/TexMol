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
#include <GOAFileTypes/GOALoader.h>
#include <math.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GroupOfAtoms.h>
#include <stdio.h>
#include <TexMol/Test.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Quaternion.h>
#include <UsefulMath/Vector.h>
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

bool makeACHRCoordinates()
{
	{
		FILE* fpReadA = fopen("../Dataset/2BG9/AInitial.txt", "r");
		if (!fpReadA)
		{
			return false;
		}
		FILE* fpWriteA = fopen("../Dataset/2BG9/ARotated.txt", "w");
		if (!fpWriteA)
		{
			return false;
		}
		for (int i=0; i<2991; i++)
		{
			float x, y, z;
			if (fscanf(fpReadA, "%f %f %f\n", &x, &y, &z) != 3)
			{
				return false;
			}
			x -= 48.096;
			y -= 80.851;
			z -= 118.124;
			CCVOpenGLMath::Vector axis_of_rotation = CCVOpenGLMath::Vector(0,0,1,0);
			double angle_of_rotation = 15.0*3.14159/180.0;
			float aw = cos(angle_of_rotation / 2.0);
			float ax = axis_of_rotation[0] * sin(angle_of_rotation / 2.0);
			float ay = axis_of_rotation[1] * sin(angle_of_rotation / 2.0);
			float az = axis_of_rotation[2] * sin(angle_of_rotation / 2.0);
			CCVOpenGLMath::Quaternion quaternion(aw, ax, ay, az);
			CCVOpenGLMath::Matrix rotation = quaternion.buildMatrix();
			CCVOpenGLMath::Vector center = CCVOpenGLMath::Vector(x, y, z, 1);
			CCVOpenGLMath::Vector newCenter = rotation*center;
			x = newCenter[0] + 48.096;
			y = newCenter[1] + 80.851;
			z = newCenter[2] + 118.124;
			fprintf(fpWriteA, "%8.3f%8.3f%8.3f\n", x, y, z);
		}
		fclose(fpReadA);
		fclose(fpWriteA);
	}
	{
		FILE* fpReadD = fopen("../Dataset/2BG9/DInitial.txt", "r");
		if (!fpReadD)
		{
			return false;
		}
		FILE* fpWriteD = fopen("../Dataset/2BG9/DRotated.txt", "w");
		if (!fpWriteD)
		{
			return false;
		}
		for (int i=0; i<2991; i++)
		{
			float x, y, z;
			if (fscanf(fpReadD, "%f %f %f\n", &x, &y, &z) != 3)
			{
				return false;
			}
			x -= 64.564;
			y -= 42.237;
			z -= 118.070;
			CCVOpenGLMath::Vector axis_of_rotation = CCVOpenGLMath::Vector(0,0,1,0);
			double angle_of_rotation = 15.0*3.14159/180.0;
			float aw = cos(angle_of_rotation / 2.0);
			float ax = axis_of_rotation[0] * sin(angle_of_rotation / 2.0);
			float ay = axis_of_rotation[1] * sin(angle_of_rotation / 2.0);
			float az = axis_of_rotation[2] * sin(angle_of_rotation / 2.0);
			CCVOpenGLMath::Quaternion quaternion(aw, ax, ay, az);
			CCVOpenGLMath::Matrix rotation = quaternion.buildMatrix();
			CCVOpenGLMath::Vector center = CCVOpenGLMath::Vector(x, y, z, 1);
			CCVOpenGLMath::Vector newCenter = rotation*center;
			x = newCenter[0] + 64.564;
			y = newCenter[1] + 42.237;
			z = newCenter[2] + 118.070;
			fprintf(fpWriteD, "%8.3f%8.3f%8.3f\n", x, y, z);
		}
		fclose(fpReadD);
		fclose(fpWriteD);
	}
	return true;
}

bool averagePositions()
{
	FILE* fp = fopen("../Dataset/junk.txt", "r");
	if (!fp)
	{
		return false;
	}
	FILE* fpw = fopen("../Dataset/nmj/achrpos.txt", "w");
	if (!fpw)
	{
		return false;
	}
	for (int i=0; i<61814/3; i++)
	{
		double x1, y1, z1, nx1, ny1, nz1;
		double x2, y2, z2, nx2, ny2, nz2;
		double x3, y3, z3, nx3, ny3, nz3;
		double x,  y,  z,  nx,  ny,  nz;
		fscanf(fp, "%lf %lf %lf %lf %lf %lf\n", &x1, &y1, &z1, &nx1, &ny1, &nz1);
		fscanf(fp, "%lf %lf %lf %lf %lf %lf\n", &x2, &y2, &z2, &nx2, &ny2, &nz2);
		fscanf(fp, "%lf %lf %lf %lf %lf %lf\n", &x3, &y3, &z3, &nx3, &ny3, &nz3);
		x = (x1 + x2 + x3)/3.0;
		y = (y1 + y2 + y3)/3.0;
		z = (z1 + z2 + z3)/3.0;
		nx = nx1 + nx2 + nx3;
		ny = ny1 + ny2 + ny3;
		nz = nz1 + nz2 + nz3;
		double denom = sqrt(nx*nx + ny*ny + nz*nz);
		nx /= denom;
		ny /= denom;
		nz /= denom;
		fprintf(fpw, "%lf %lf %lf %lf %lf %lf\n", x3, y3, z3, nx, ny, nz);
	}
	fclose(fp);
	fclose(fpw);
	return true;
}

void setZeros(PDBParser::GroupOfAtoms* copyGOA)
{
	for (int i=0; i<copyGOA->m_SubGroups.size(); i++)
	{
		setZeros(copyGOA->m_SubGroups[i]);
	}
	for (int i=0; i<copyGOA->m_Atoms.size(); i++)
	{
		copyGOA->m_Atoms[i]->m_Position[0] = copyGOA->m_Atoms[i]->m_Position[1] = copyGOA->m_Atoms[i]->m_Position[2] = 0;
	}
}

void testGOADeepCopy()
{
	PDBParser::GroupOfAtoms* molecule = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule = gLoader->loadFile("C:/a1.pdb");
		delete gLoader;
		if (!molecule)
		{
			return;
		}
	}
	PDBParser::GroupOfAtoms* copyGOA = new PDBParser::GroupOfAtoms(*molecule);
	setZeros(copyGOA);
	GOALoader* goaLoader = new GOALoader();
	goaLoader->saveFile("C:/a1_modified.pdb", copyGOA, 0, 0);
	goaLoader->saveFile("C:/a1_original.pdb", molecule, 0, 0);
	delete goaLoader;
}
bool TexMol::test()
{
	testGOADeepCopy();
	return true;
}
