#if !defined(AFX_VIEW_H__B3C3BBFF_964D_4AE0_AB50_AC1D94E2B32D__INCLUDED_)
#define AFX_VIEW_H__B3C3BBFF_964D_4AE0_AB50_AC1D94E2B32D__INCLUDED_

#include <UsefulMath/Vector.h>
#include <UsefulMath/Quaternion.h>
#include <UsefulMath/Matrix.h>

namespace CCVOpenGLMath
{
	class Ray;
};

struct Camera
{
	CCVOpenGLMath::Vector m_Origin;
	CCVOpenGLMath::Vector m_LookAt;
	CCVOpenGLMath::Vector m_UImage;
	CCVOpenGLMath::Vector m_VImage;
	double m_RealWidth;
	double m_RealHeight;
};

class View
{
	public:
		View(float windowsize);
		virtual ~View();
		virtual View* clone() const = 0;
		virtual void SetView() = 0;
		virtual int SetTiledViewON(int _tileResolX, int _nTilesX);
		virtual bool SetNextTile( int *i, int *j );
		virtual void SetTiledViewOFF();
		virtual CCVOpenGLMath::Ray GetPickRay(int x, int y) const = 0;
		virtual CCVOpenGLMath::Vector GetScreenPoint(const CCVOpenGLMath::Vector& p) const = 0;
		virtual void SetOrientation(const View& view);
		virtual void setTarget(const CCVOpenGLMath::Vector& target);
		virtual void resizeWindow(int w, int h);
		virtual void startDrag(int x, int y);
		virtual void pan(int xNew, int yNew);
		virtual void rotate(int xNew, int yNew);
		virtual void mouseWorldAxisRotate(int xNew, int yNew);
		virtual void mouseTrackBallRotate(int xNew, int yNew);
		virtual void zoom(int xNew, int yNew);
		virtual void defaultTransformation(int xNew, int yNew);
		virtual CCVOpenGLMath::Vector getPan(int xDiff, int yDiff);
		virtual CCVOpenGLMath::Matrix getRotation(float xDiff, float yDiff);
		virtual CCVOpenGLMath::Matrix getRotationTC(float xDiff, float yDiff);
		virtual float getScale(int xDiff, int yDiff);
		virtual void protected_pan(float dx, float dy);
		virtual void protected_rotate(float dx, float dy);
		virtual void rotateWorldAxis(float dx, float dy);
		virtual void rotateTrackBall(float x1, float y1, float x2, float y2);
		virtual void zoom(float dz);
		virtual float GetWindowSize();
		virtual void SetWindowSize(float size);
		virtual CCVOpenGLMath::Quaternion getOrientation() const;
		virtual void setOrientation(const CCVOpenGLMath::Quaternion& orientation);
		virtual CCVOpenGLMath::Vector getTarget() const;
		virtual int InvertY(int y) const;
		virtual void updateCamera() = 0;
		virtual CCVOpenGLMath::Ray* getPrimaryRay(int xScreenPos, int yScreenPos) = 0;
		void saveCurrentViewport();
		void resetSavedViewport();

		int m_Width, m_Height;
	protected:
		virtual void savePosition(int x, int y);
		int m_xOld, m_yOld;
		bool m_bDragStarted;
		CCVOpenGLMath::Vector m_Target;
                CCVOpenGLMath::Vector m_TranslatedTarget;
		CCVOpenGLMath::Quaternion m_Orientation;
		float m_WindowSize;
		Camera m_Camera;
		bool m_hasSavedViewport;
		int m_viewport[4];
		int m_tileResolX;
		int m_tileResolY;
		int m_nTilesX;
		int m_nTilesY;
		int m_curTileX;
		int m_curTileY;
		bool m_flagTiledViewMode;
};

#endif
