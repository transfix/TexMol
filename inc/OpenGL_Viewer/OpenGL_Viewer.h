#if !defined(AFX_OPENGL_VIEWER_H__7E40AD09_007F_441D_98A3_1C7FCBF742E6__INCLUDED_)
#define AFX_OPENGL_VIEWER_H__7E40AD09_007F_441D_98A3_1C7FCBF742E6__INCLUDED_

#include <OpenGL_Viewer/Plane.h>

class Grid;
class View;
class GeometryScene;
class ViewFrustrum;

const int ORTHOGRAPHIC_VIEW = 0;
const int PERSPECTIVE_VIEW = 1;

// arand: added typecast below to eliminate warnings
#define CHECK_GL_ERROR() printOpenGLError((char *)__FILE__, __LINE__)

class OpenGL_Viewer
{
	public:
		OpenGL_Viewer();
		virtual ~OpenGL_Viewer();
		enum Views { Perspective, Top, Bottom, Right, Left, Front, Back };
		bool initParams();
		bool initView();
		bool enableFirstLight();
		void resizeGL(int w, int h);
		void setFieldOfView(double fieldOfView);
		void resetFieldOfView();
		void setGridSpacing(double gridspacing);
		void resetGridSpacing();
		void setGridExtent(const double gridExtent);
		void resetGridExtent();
		void setMajorSpacing(unsigned int majorSpacing);
		void resetMajorSpacing();
		void setViewDirection(Views view);
		void setViewDirectionPerspective();
		void setViewDirectionTop();
		void setViewDirectionBottom();
		void setViewDirectionRight();
		void setViewDirectionLeft();
		void setViewDirectionFront();
		void setViewDirectionBack();
		void resetViewDirection();
		double FieldOfView() const;
		double GridSpacing() const;
		double GridExtent() const;
		unsigned int MajorSpacing() const;
		bool bShowAxis() const;
		bool bShowGrid() const;
		bool m_bShowGrid;
		void setView(const OpenGL_Viewer* openGL_Viewer);
		void renderMe();
		void protectedSetBackground(float r, float g, float b);
		void getBackgroundColor(float* r, float* g, float* b);
		void initializeGL();
		void setViewType(int viewType);
		void dontShare();
		void shareWith(OpenGL_Viewer* openGL_Viewer);
		View* getView();
		void zoom(float zoomValue);
		void setView(View* view);
		View* m_View;
		ViewFrustrum* m_ViewFrustrum;
		// used to obtain the view plane.
		static void concatMatrices(double* result, float* m1, float* m2);
		static Plane getViewPlane();
		static Plane getClipPlane(double* min, double* max, double clippingPlane);
		static bool printOpenGLError();
		static int printOpenGLError(char* file, int line);

	protected:
		void prepareTransformation();
		bool initExtensions();
		Grid* m_Grid;
		Views m_ViewDirection;
		int m_ViewType;
		float m_BackgroundColor[3];
};

#endif
