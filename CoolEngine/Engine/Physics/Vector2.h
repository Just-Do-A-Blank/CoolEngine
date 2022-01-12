#pragma once

struct Vector2
{
	int m_x, m_y;
	Vector2()
	{
		m_x = 0;
		m_y = 0;
	}

	Vector2(int x, int y)
	{
		m_x = x;
		m_y = y;
	}
};