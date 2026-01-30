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
#ifndef CCV_TRANSFORMATION_PARAMETERS_H
#define CCV_TRANSFORMATION_PARAMETERS_H

namespace ObjectRendererLibrary
{
	class TransformationParameters
	{
		public:
			TransformationParameters(double* rotations,
									 double* translations,
									 int numberOfTransformations,
									 bool randomizeColors,
									 float averageRadius // see if you can make do without this parameter
									);
			virtual ~TransformationParameters();
			void setAverageRadius(float averageRadius);
			float getAverageRadius();
			bool hasTransformations();
			int getNumberOfTransformations();
			bool getTransformations(float* trans, int transformationIndex);
			bool useRandomColors();

		protected:
			double* m_Rotations;
			double* m_Translations;
			int m_NumberOfTransformations;
			bool m_RandomizeColors;
			float m_AverageRadius;
	};
};

#endif
