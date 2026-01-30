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
#ifndef CCV_TEXMOL_NURBS_TRIMMING_CURVES_H
#define CCV_TEXMOL_NURBS_TRIMMING_CURVES_H

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__APPLE__)
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

namespace CCV_NURBS
{
	class TrimmingCurve
	{
		public:
			TrimmingCurve();
			virtual ~TrimmingCurve();
			TrimmingCurve(const TrimmingCurve& copy);
			TrimmingCurve& operator=(const TrimmingCurve& copy);
			TrimmingCurve& set(const TrimmingCurve& copy);
			bool render();
			GLUnurbsObj* m_NurbCurve;
			GLint m_NumberOfUKnots;
			GLfloat* m_UKnots;
			GLint m_UStride;
			GLfloat* m_ControlArray;
			GLint m_UOrder;
			GLenum m_Type;

		protected:
			void deleteData();
	};
};

#endif
