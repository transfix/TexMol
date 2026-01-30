#if !defined(AFX_PERSPECTIVEVIEW_H__7F523854_7177_40BC_8ACC_0E48DC611DC6__INCLUDED_)
#define AFX_PERSPECTIVEVIEW_H__7F523854_7177_40BC_8ACC_0E48DC611DC6__INCLUDED_

#include <OpenGL_Viewer/View.h>

namespace CCVOpenGLMath
{
	class Ray;
};

class PerspectiveView : public View
{
	public:
		PerspectiveView(float windowsize);
		virtual ~PerspectiveView();
		virtual View* clone() const;
		virtual void SetView();
		virtual CCVOpenGLMath::Ray GetPickRay(int x, int y)const;
		virtual CCVOpenGLMath::Vector GetScreenPoint(const CCVOpenGLMath::Vector& p) const;
		virtual void defaultTransformation(int xNew, int yNew);
		void setFieldOfView(float angle);
		float getFieldOfView();
		virtual void updateCamera();
		virtual CCVOpenGLMath::Ray* getPrimaryRay(int xScreenPos, int yScreenPos);
		static const float defaultFieldOfView;

	protected:
		float m_FieldOfView;
};

#endif
