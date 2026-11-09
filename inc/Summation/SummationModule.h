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
#ifndef CCV_SUMMATION_MODULE_H
#define CCV_SUMMATION_MODULE_H

#include <Utility/utility.h>
#include <Summation/Kernel.h>

namespace CCVSummationModule
{
	class Kernel;
	class OutputGrid;
	class SummationModule
	{
		public:
			SummationModule(double* centers, double* radii, double* weights, int numberOfCenters, Kernel* kernel);
			virtual ~SummationModule();
			virtual bool sum() = 0;
		protected:
			double* m_Centers;
			double* m_Radii;
			double* m_Weights;
			int m_NumberOfCenters;
			Kernel* m_Kernel;
	};
};

#endif
