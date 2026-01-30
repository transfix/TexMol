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
#include <Utility/utility.h>
#include <DataManager/CriticalPointExtractor.h>

using namespace CVCUtility;

// Construction/Destruction
CriticalPointExtractor::CriticalPointExtractor()
{
}

CriticalPointExtractor::~CriticalPointExtractor()
{
}

bool CriticalPointExtractor::findCriticalPoints(int XDIM,int YDIM,int ZDIM,float* dataset, std::vector<double>* criticalPoints, float tlow)
{
	int u,number;
	float tmp;
	number = 0;
	for (int k=0; k<ZDIM; k++)
	{
		for (int j=0; j<YDIM; j++)
		{
			for (int i=0; i<XDIM; i++)
			{
				tmp = dataset[((k)*XDIM*YDIM + (j)*XDIM + (i))];
				if (tmp > tlow)
				{
					u = 0;
					for (int z=maximum(k-1,0); z<=minimum(k+1,ZDIM-1); z++)
					{
						for (int y=maximum(j-1,0); y<=minimum(j+1,YDIM-1); y++)
						{
							for (int x=maximum(i-1,0); x<=minimum(i+1,XDIM-1); x++)
							{
								if (tmp < dataset[((k)*XDIM*YDIM + (j)*XDIM + (i))])
								{
									u = 1;
								}
							}
							if (u == 0)
							{
								number++;
								criticalPoints->push_back(i);
								criticalPoints->push_back(j);
								criticalPoints->push_back(k);
								criticalPoints->push_back(tmp);
							}
						}
					}
				}
			}
		}
	}
	printf("number of critical points: %d \n",number);
	return true;
}
