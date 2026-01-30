#if !defined(AFX_PLANE_H__04F216E9_22AB_4373_9CF5_754D66B0F950__INCLUDED_)
#define AFX_PLANE_H__04F216E9_22AB_4373_9CF5_754D66B0F950__INCLUDED_

class Plane
{
	public:
		Plane();
		Plane(double a, double b, double c, double d);
		virtual ~Plane();
		double signedDistance(double x, double y, double z) const;
		void normalizeNormal();
		inline double& operator[](unsigned int index);
		inline const double& operator[](unsigned int index) const;
		inline double& a();
		inline double& b();
		inline double& c();
		inline double& d();
		inline const double& a() const;
		inline const double& b() const;
		inline const double& c() const;
		inline const double& d() const;
		double m_A, m_B, m_C, m_D;
};

inline double& Plane::operator[](unsigned int index)
{
	if (index == 0)
	{
		return m_A;
	}
	else if (index == 1)
	{
		return m_B;
	}
	else if (index == 2)
	{
		return m_C;
	}
	else
	{
		return m_D;
	}
}

inline const double& Plane::operator[](unsigned int index) const
{
	if (index == 0)
	{
		return m_A;
	}
	else if (index == 1)
	{
		return m_B;
	}
	else if (index == 2)
	{
		return m_C;
	}
	else
	{
		return m_D;
	}
}

inline double& Plane::a()
{
	return m_A;
}

inline double& Plane::b()
{
	return m_B;
}

inline double& Plane::c()
{
	return m_C;
}

inline double& Plane::d()
{
	return m_D;
}

inline const double& Plane::a() const
{
	return m_A;
}

inline const double& Plane::b() const
{
	return m_B;
}

inline const double& Plane::c() const
{
	return m_C;
}

inline const double& Plane::d() const
{
	return m_D;
}

#endif
