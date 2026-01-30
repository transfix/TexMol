#if !defined(AFX_EXTENTS_H__541859B1_96FC_453D_8524_1D6789C4F864__INCLUDED_)
#define AFX_EXTENTS_H__541859B1_96FC_453D_8524_1D6789C4F864__INCLUDED_

#include <UsefulMath/Vector.h>

using namespace CCVOpenGLMath;

class Extents
{
	public:
		Extents();
		Extents(
			double xMin, double xMax,
			double yMin, double yMax,
			double zMin, double zMax
		);
		virtual ~Extents();
		void setExtents(
			double xMin, double xMax,
			double yMin, double yMax,
			double zMin, double zMax
		);
		Vector getOrigin() const;
		void setOrigin(Vector vector, const Extents& boundaryExtents);
		void move(const Vector& vector);
		bool withinCube(const Vector& vector) const;
		void clampTo(const Extents& boundaryExtents);
		double getXMin() const;
		double getYMin() const;
		double getZMin() const;
		double getXMax() const;
		double getYMax() const;
		double getZMax() const;
		void setXMin(double xMin);
		void setYMin(double yMin);
		void setZMin(double zMin);
		void setXMax(double xMax);
		void setYMax(double yMax);
		void setZMax(double zMax);

	protected:
		double m_XMin, m_XMax;
		double m_YMin, m_YMax;
		double m_ZMin, m_ZMax;
};

#endif
