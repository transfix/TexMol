/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef CCV_FOURIER_BASED_TO_SCATTERED_POINTS_SUMMATION_MODULE_H
#define CCV_FOURIER_BASED_TO_SCATTERED_POINTS_SUMMATION_MODULE_H

#include <Utility/utility.h>
#include <Summation/FourierBasedSummationModule.h>

namespace CCVSummationModule
{
	class FourierBasedToScatteredPointsSummationModule
	 : public SummationModule
	{
		public:
			FourierBasedToScatteredPointsSummationModule(double* centers, double* radii, double* weights, int numberOfCenters, Kernel* kernel, OutputGrid* outputGrid);
			virtual ~FourierBasedToScatteredPointsSummationModule();
	};
};

#endif
