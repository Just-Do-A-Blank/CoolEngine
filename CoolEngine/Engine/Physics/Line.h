#pragma once
#include "Vector2.h"

class Line
{
public:
	Vector2 m_point1;
	Vector2 m_point2;

	Line()
	{
		m_point1 = Vector2();
		m_point2 = Vector2();
	}

	Line(Vector2 p1, Vector2 p2)
	{
		m_point1 = p1;
		m_point2 = p2;
	}
};