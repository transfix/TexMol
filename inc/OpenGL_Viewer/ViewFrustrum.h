#ifndef CCV_TEXMOL_OPENGL_VIEWER_VIEW_FRUSTRUM_H
#define CCV_TEXMOL_OPENGL_VIEWER_VIEW_FRUSTRUM_H

#include <OpenGL_Viewer/Plane.h>
#include <UsefulMath/Vector.h>

class ViewFrustrum
{
	public:
		ViewFrustrum();
		virtual ~ViewFrustrum();
		enum POSITION {INSIDE, OUTSIDE, INTERSECTS};
		POSITION intersects(double* center, double radius);
		POSITION intersects(double* min, double* max);
		void getMinMaxArea(double* min, double* max, double* minArea, double* maxArea);
		void update();
		void print();

	protected:
		bool sideOfPlane(Plane plane, CCVOpenGLMath::Vector point);
		Plane m_Planes[6]; // left, right, top, bottom, near, far
		double m_MVP_Matrix[16];
};

#endif
