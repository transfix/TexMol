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
#include <Contour/contour.h>
#include <Contour/datasetreg3.h>

void getContourSpectrum(unsigned char* uchar_data, int type, int* dim, int array_size, float* isoval , float* area, float* min_vol, float* max_vol, float* gradient, float* span)
{
	int i;
	ConDataset* the_data;
	the_data = newDatasetReg(type, CONTOUR_REG_3D, 1, 1, dim,uchar_data);
	((Datareg3*)the_data->data->getData(0))->setSpan(span);
	Signature*       sig;
	sig=getSignatureFunctions(the_data, 0,0);

	for(i=0; i<array_size; i++)
	{
		isoval[i]=sig[0].fx[i];
		area[i]=sig[0].fy[i];
		min_vol[i]=sig[1].fy[i];
		max_vol[i]=sig[2].fy[i];
		gradient[i]=sig[3].fy[i];
	}

	delete the_data;
	delete sig;
}


void compute_Area_Volume(int array_size, float* isovalue, float* area, float* min_vol, float* max_vol, float* gradient, float isoval, float& return_area,float& return_minvol, float& return_maxvol, float& return_grad)
{
	int i;
	float ratio;
	return_area=return_minvol=return_maxvol=return_grad=0;

	for(i=0; i<array_size; i++)
	{
		if(isoval>=isovalue[i] && isoval<isovalue[i+1])
		{
			ratio = (isoval-isovalue[i]) / (isovalue[i+1]-isovalue[i]);
			return_area = area[i] + ratio*(area[i+1]-area[i]);
			return_minvol = min_vol[i] + ratio*(min_vol[i+1]-min_vol[i]);
			return_maxvol = max_vol[i] + ratio*(max_vol[i+1]-max_vol[i]);
			return_grad = gradient[i] + ratio*(gradient[i+1]-gradient[i]);
		}
	}
}
