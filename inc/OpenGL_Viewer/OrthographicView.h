#if !defined(AFX_ORTHOGRAPHICVIEW_H__4003C434_82A1_4E4E_9154_10785DB00338__INCLUDED_)
#define AFX_ORTHOGRAPHICVIEW_H__4003C434_82A1_4E4E_9154_10785DB00338__INCLUDED_

#include <OpenGL_Viewer/View.h>
#include <UsefulMath/Vector.h>
#include <UsefulMath/Quaternion.h>

namespace CCVOpenGLMath
{
	class Ray;
};

class OrthographicView : public View
{
	public:
		OrthographicView(float windowsize);
		virtual ~OrthographicView();
		virtual View* clone() const;
		static OrthographicView* Top(float windowsize);
		static OrthographicView* Right(float windowsize);
		static OrthographicView* Left(float windowsize);
		static OrthographicView* Bottom(float windowsize);
		static OrthographicView* Front(float windowsize);
		static OrthographicView* Back(float windowsize);
		virtual void SetView();
		virtual CCVOpenGLMath::Ray GetPickRay(int x, int y) const;
		virtual CCVOpenGLMath::Vector GetScreenPoint(const CCVOpenGLMath::Vector& p) const;
		virtual void defaultTransformation(int xNew, int yNew);
		virtual void updateCamera();
		virtual CCVOpenGLMath::Ray* getPrimaryRay(int xScreenPos, int yScreenPos);
};

#endif
