#include <Docking/ValuePosition3D.h>
#include <stdio.h>

ValuePosition3D::ValuePosition3D()
{
	m_Value = -100000000.0;
	m_Translation[0] = m_Translation[1] = m_Translation[2] = 0.0;
	m_RotationIndex = m_FineRotationIndex = 0;
	m_ConformationIndex = -1;
}

ValuePosition3D::~ValuePosition3D()
{
}
