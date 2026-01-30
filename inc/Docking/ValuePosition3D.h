#ifndef CCV_VALUE_POSITION_3D_H
#define CCV_VALUE_POSITION_3D_H

class ValuePosition3D
{
	public:
		ValuePosition3D();
		virtual ~ValuePosition3D();
		double m_Value;
		int m_RotationIndex;
		int m_FineRotationIndex;
		int m_ConformationIndex;
		double m_Translation[3];
};

#endif
