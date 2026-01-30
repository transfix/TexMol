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
#include <cstdlib>
#include <cstring>
#include <Nurbs/NURBS.h>
#include <Nurbs/NURBSLoader.h>
#include <Nurbs/Patch.h>
#include <Nurbs/TrimmingCurve.h>

using namespace std;
using namespace CCV_NURBS;

NURBSLoader::NURBSLoader()
{
}

NURBSLoader::~NURBSLoader()
{
}

// comments start with whitespace followed by #
bool NURBSLoader::isCommentOrWhitespace(const char* line)
{
	int i;
	int len = strlen(line);
	if (!len)
	{
		return true;
	}
	if (line[0] == '#')
	{
		return true;
	}
	for (i=0; i<len; i++)
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
		{
			break;
		}
	}
	if (i == len)
	{
		return true;
	}
	return false;
}

bool NURBSLoader::getNextLine(char* line, FILE* fp)
{
	memset(line, '\0', sizeof(line));	// Clear out the buffer in case of short lines
	while (true)
	{
		if (fgets(line, 1024, fp) == 0)
		{
			return false;
		}
		if (isCommentOrWhitespace(line))
		{
			continue;
		}
		break;
	}
	return true;
}

// Warning: this uses string tokenizer, which will overwrite the input line.
bool NURBSLoader::readFloats(char* line, int num, float* array)
{
	if (!line || !array)
	{
		return false;
	}
	char seps[10];
	char* token;
	strcpy(seps, " ,\t\n\r");
	token = strtok(line, seps);
	int count = 0;
	while (token != NULL)
	{
		if (count < num)
		{
			array[count] = atof(token);
		}
		count++;
		token = strtok(NULL, seps);
	}
	if (count == num)
	{
		return true;
	}
	return false;
}

NURBS* NURBSLoader::load(const char* fileName)
{
	if (!fileName)
	{
		return 0;
	}
	FILE* fp = fopen(fileName, "r");
	if (!fp)
	{
		return 0;
	}
	// make a simple reader for now
	char line[1025];
	if (!getNextLine(line, fp))
	{
		return 0;
	}
	NURBS* nurbs = new NURBS();
	while (true)
	{
		if (strstr(line, "New patch"))
		{
			Patch* patch = new Patch();
			if (readPatch(fp, patch))
			{
				nurbs->m_Patches.push_back(patch);
			}
			else
			{
				delete patch;
			}
		}
		if (!getNextLine(line, fp))
		{
			break;
		}
	}
	if (nurbs->m_Patches.size() == 0)
	{
		delete nurbs;
		return 0;
	}
	return nurbs;
}

bool NURBSLoader::readTrimmingCurve(FILE* fp, TrimmingCurve* trimmingCurve)
{
	if (!fp || !trimmingCurve)
	{
		return false;
	}
	char line[1025];
	if (!getNextLine(line, fp))
	{
		return false;
	}
	if (sscanf(line, " %d %d %d ", &(trimmingCurve->m_UOrder), &(trimmingCurve->m_NumberOfUKnots), &(trimmingCurve->m_UStride)) != 3)
	{
		return false;
	}
	if (!getNextLine(line, fp))
	{
		return false;
	}
	trimmingCurve->m_UKnots = new float[trimmingCurve->m_NumberOfUKnots];
	if (!readFloats(line, trimmingCurve->m_NumberOfUKnots, trimmingCurve->m_UKnots))
	{
		return false;
	}
	if (!getNextLine(line, fp))
	{
		return false;
	}
	int numDims = 3;
	{
		int type;
		if (!sscanf(line, " %d ", &type))
		{
			return false;
		}
		if (type == 3)
		{
			numDims = 3;
			trimmingCurve->m_Type = GLU_MAP1_TRIM_2;
		}
		else
		{
			numDims = 4;
			trimmingCurve->m_Type = GLU_MAP1_TRIM_3;
		}
	}
	int numControlPoints = (trimmingCurve->m_NumberOfUKnots-trimmingCurve->m_UOrder);
	if (numControlPoints < 1)
	{
		return false;
	}
	trimmingCurve->m_ControlArray = new float[numControlPoints*numDims];
	for (int i=0; i<numControlPoints; i++)
	{
		if (!getNextLine(line, fp))
		{
			return false;
		}
		if (!readFloats(line, numDims, &(trimmingCurve->m_ControlArray[i*numDims])))
		{
			return false;
		}
	}
	return true;
}

bool NURBSLoader::readPatch(FILE* fp, Patch* patch)
{
	char line[1025];
	if (!getNextLine(line, fp))
	{
		return false;
	}
	int numTrimmingCurves = 0;
	if (!sscanf(line, "%d\n", &numTrimmingCurves))
	{
		return false;
	}
	if (numTrimmingCurves < 0)
	{
		return false;
	}
	if (!getNextLine(line, fp))
	{
		return false;
	}
	if (sscanf(line, " %d %d %d ", &(patch->m_UOrder), &(patch->m_NumberOfUKnots), &(patch->m_UStride)) != 3)
	{
		return false;
	}
	if (!getNextLine(line, fp))
	{
		return false;
	}
	if (sscanf(line, " %d %d %d ", &(patch->m_VOrder), &(patch->m_NumberOfVKnots), &(patch->m_VStride)) != 3)
	{
		return false;
	}
	if (!getNextLine(line, fp))
	{
		return false;
	}
	patch->m_UKnots = new float[patch->m_NumberOfUKnots];
	if (!readFloats(line, patch->m_NumberOfUKnots, patch->m_UKnots))
	{
		return false;
	}
	if (!getNextLine(line, fp))
	{
		return false;
	}
	patch->m_VKnots = new float[patch->m_NumberOfVKnots];
	if (!readFloats(line, patch->m_NumberOfVKnots, patch->m_VKnots))
	{
		return false;
	}
	if (!getNextLine(line, fp))
	{
		return false;
	}
	int numDims = 3;
	{
		int type;
		if (!sscanf(line, " %d ", &type))
		{
			return false;
		}
		if (type == 3)
		{
			numDims = 3;
			patch->m_Type = GL_MAP2_VERTEX_3;
		}
		else
		{
			numDims = 4;
			patch->m_Type = GL_MAP2_VERTEX_4;
		}
	}
	int numControlPoints = (patch->m_NumberOfUKnots-patch->m_UOrder)*(patch->m_NumberOfVKnots-patch->m_VOrder);
	if (numControlPoints < 1)
	{
		return false;
	}
	for (int i=0; i<numControlPoints; i++)
	{
		if (!getNextLine(line, fp))
		{
			return false;
		}
		if (!readFloats(line, numDims, &(patch->m_ControlArray[i*numDims])))
		{
			return false;
		}
	}
	for (int i=0; i<numTrimmingCurves; i++)
	{
		TrimmingCurve* trimmingCurve = new TrimmingCurve();
		if (readTrimmingCurve(fp, trimmingCurve))
		{
			patch->m_TrimmingCurves.push_back(trimmingCurve);
		}
		else
		{
			delete trimmingCurve;
		}
	}
	if (patch->m_TrimmingCurves.size() != numTrimmingCurves)
	{
		return false;
	}
	return true;
}

bool NURBSLoader::save(NURBS* nurbs, const char* fileName)
{
	if (!nurbs || !fileName)
	{
		return false;
	}
	return true;
}
