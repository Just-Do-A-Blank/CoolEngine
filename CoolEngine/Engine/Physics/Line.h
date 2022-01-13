#pragma once

class Line
{
public:
	XMFLOAT2 m_point1;
	XMFLOAT2 m_point2;

	Line()
	{
		m_point1 = XMFLOAT2(0, 0);
		m_point2 = XMFLOAT2(0, 0);
	}

	Line(XMFLOAT2 p1, XMFLOAT2 p2)
	{
		m_point1 = p1;
		m_point2 = p2;
	}
};