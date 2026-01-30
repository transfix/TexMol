#if !defined(AFX_GRID_H__966FA4B1_8684_46C0_928C_7ACF0DEFEBE4__INCLUDED_)
#define AFX_GRID_H__966FA4B1_8684_46C0_928C_7ACF0DEFEBE4__INCLUDED_

#include <UsefulMath/Quaternion.h>
#include <UsefulMath/Vector.h>
#include <Geometry/Geometry.h>

class Grid
{
	public:
		Grid();
		virtual ~Grid();

		void InitOptions();
		CCVOpenGLMath::Vector Intersect(const CCVOpenGLMath::Ray& ray) const;
		void SetGridSize(float size);
		void SetGridSpacing(float spacing);
		void SetMajorSpacing(unsigned int majorSpacing);
		void SetShowAxis(bool axis);
		void SetMajorGridColor(float r, float g, float b);
		void SetMinorGridColor(float r, float g, float b);
		void SetXAxisColor(float r, float g, float b);
		void SetYAxisColor(float r, float g, float b);
		void SetZAxisColor(float r, float g, float b);
		float GetGridSize() const;
		float GetGridSpacing() const;
		unsigned int GetMajorSpacing() const;
		bool GetShowAxis() const;
		const float* GetMajorGridColor() const;
		const float* GetMinorGridColor() const;
		const float* GetXAxisColor() const;
		const float* GetYAxisColor() const;
		const float* GetZAxisColor() const;
		Geometry* GetMajorLines();
		Geometry* GetMinorLines();
		void setGridPerspective();
		void setGridTop();
		void setGridBottom();
		void setGridRight();
		void setGridLeft();
		void setGridFront();
		void setGridBack();
		static const float defaultGridSpacing;
		static const float defaultGridExtent;
		static const unsigned int defaultMajorSpacing;
		static const bool defaultbShowAxis;
		static const float defaultMajorGridColor[3];
		static const float defaultMinorGridColor[3];
		static const float defaultXAxisColor[3];
		static const float defaultYAxisColor[3];
		static const float defaultZAxisColor[3];

	protected:
		void PrepareGeometry();
		unsigned int PrepareMajorLines();
		void PrepareMinorLines(unsigned int numberofmajorlines);
		void TransformGrid();
		float m_GridExtent;
		float m_GridSpacing;
		unsigned int m_MajorSpacing;
		bool m_bShowAxis;
		bool m_bOptionChanged;
		float m_MajorGridColor[3];
		float m_MinorGridColor[3];
		float m_XAxisColor[3];
		float m_YAxisColor[3];
		float m_ZAxisColor[3];
		Geometry m_MinorLines;
		Geometry m_MajorLines;
		CCVOpenGLMath::Quaternion m_Orientation;
		CCVOpenGLMath::Vector m_Origin;

	private:
		inline unsigned int MakeHorizontalLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines);
		inline unsigned int MakeVerticalLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines);
		inline unsigned int MakeHorizontalAxisLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines);
		inline unsigned int MakeVerticalAxisLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines);
		inline unsigned int MakeDepthAxisLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines);
};

inline unsigned int Grid::MakeHorizontalLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines)
{
	unsigned int v1 = nextvertex;
	nextvertex++;
	unsigned int v2 = nextvertex;
	nextvertex++;
	lines->m_LineVerts[v1*3+0] = low;
	lines->m_LineVerts[v1*3+1] = position;
	lines->m_LineVerts[v1*3+2] = 0.0f;
	lines->m_LineVerts[v2*3+0] = high;
	lines->m_LineVerts[v2*3+1] = position;
	lines->m_LineVerts[v2*3+2] = 0.0f;
	lines->m_Lines[linenumber*2+0] = v1;
	lines->m_Lines[linenumber*2+1] = v2;
	return nextvertex;
}

inline unsigned int Grid::MakeVerticalLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines)
{
	unsigned int v1 = nextvertex;
	nextvertex++;
	unsigned int v2 = nextvertex;
	nextvertex++;
	lines->m_LineVerts[v1*3+0] = position;
	lines->m_LineVerts[v1*3+1] = low;
	lines->m_LineVerts[v1*3+2] = 0.0f;
	lines->m_LineVerts[v2*3+0] = position;
	lines->m_LineVerts[v2*3+1] = high;
	lines->m_LineVerts[v2*3+2] = 0.0f;
	lines->m_Lines[linenumber*2+0] = v1;
	lines->m_Lines[linenumber*2+1] = v2;
	return nextvertex;
}

inline unsigned int Grid::MakeHorizontalAxisLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines)
{
	unsigned int v1 = nextvertex;
	nextvertex++;
	unsigned int v2 = nextvertex;
	nextvertex++;
	lines->m_LineVerts[v1*3+0] = low;
	lines->m_LineVerts[v1*3+1] = position;
	lines->m_LineVerts[v1*3+2] = 0.0f;
	lines->m_LineVerts[v2*3+0] = high;
	lines->m_LineVerts[v2*3+1] = position;
	lines->m_LineVerts[v2*3+2] = 0.0f;
	lines->m_LineColors[v1*3+0] = m_XAxisColor[0];
	lines->m_LineColors[v1*3+1] = m_XAxisColor[1];
	lines->m_LineColors[v1*3+2] = m_XAxisColor[2];
	lines->m_LineColors[v2*3+0] = m_XAxisColor[0];
	lines->m_LineColors[v2*3+1] = m_XAxisColor[1];
	lines->m_LineColors[v2*3+2] = m_XAxisColor[2];
	lines->m_Lines[linenumber*2+0] = v1;
	lines->m_Lines[linenumber*2+1] = v2;
	return nextvertex;
}

inline unsigned int Grid::MakeVerticalAxisLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines)
{
	unsigned int v1 = nextvertex;
	nextvertex++;
	unsigned int v2 = nextvertex;
	nextvertex++;
	lines->m_LineVerts[v1*3+0] = position;
	lines->m_LineVerts[v1*3+1] = low;
	lines->m_LineVerts[v1*3+2] = 0.0f;
	lines->m_LineVerts[v2*3+0] = position;
	lines->m_LineVerts[v2*3+1] = high;
	lines->m_LineVerts[v2*3+2] = 0.0f;
	lines->m_LineColors[v1*3+0] = m_YAxisColor[0];
	lines->m_LineColors[v1*3+1] = m_YAxisColor[1];
	lines->m_LineColors[v1*3+2] = m_YAxisColor[2];
	lines->m_LineColors[v2*3+0] = m_YAxisColor[0];
	lines->m_LineColors[v2*3+1] = m_YAxisColor[1];
	lines->m_LineColors[v2*3+2] = m_YAxisColor[2];
	lines->m_Lines[linenumber*2+0] = v1;
	lines->m_Lines[linenumber*2+1] = v2;
	return nextvertex;
}

inline unsigned int Grid::MakeDepthAxisLine(unsigned int linenumber, unsigned int nextvertex, float position, float low, float high, Geometry* lines)
{
	unsigned int v1 = nextvertex;
	nextvertex++;
	unsigned int v2 = nextvertex;
	nextvertex++;
	lines->m_LineVerts[v1*3+0] = 0.0f;
	lines->m_LineVerts[v1*3+1] = 0.0f;
	lines->m_LineVerts[v1*3+2] = low;
	lines->m_LineVerts[v2*3+0] = 0.0f;
	lines->m_LineVerts[v2*3+1] = 0.0f;
	lines->m_LineVerts[v2*3+2] = high;
	lines->m_LineColors[v1*3+0] = m_ZAxisColor[0];
	lines->m_LineColors[v1*3+1] = m_ZAxisColor[1];
	lines->m_LineColors[v1*3+2] = m_ZAxisColor[2];
	lines->m_LineColors[v2*3+0] = m_ZAxisColor[0];
	lines->m_LineColors[v2*3+1] = m_ZAxisColor[1];
	lines->m_LineColors[v2*3+2] = m_ZAxisColor[2];
	lines->m_Lines[linenumber*2+0] = v1;
	lines->m_Lines[linenumber*2+1] = v2;
	return nextvertex;
}

#endif
