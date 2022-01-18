#pragma once

class Box
{
public:
	int m_x, m_y, m_width, m_height;

	Box()
	{
		m_x = 0;
		m_y = 0;
		m_width = 1;
		m_height = 1;
	}

	Box(int x, int y, int width, int height)
	{
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;
	}

	/// <summary>
	/// Returns X coord of the middle of the box
	/// </summary>
	/// <returns></returns>
	int GetCentreX()
	{
		return m_x + m_width / 2;
	}

	/// <summary>
	/// Returns Y coord of the middle of the box
	/// </summary>
	/// <returns></returns>
	int GetCentreY()
	{
		return m_y + m_height / 2;
	}

	inline Box operator+(Box box)
	{
		return Box(m_x + box.m_x, m_y + box.m_y, m_width + box.m_width, m_height + box.m_height);
	}

	inline Box operator-(Box box)
	{
		return Box(m_x - box.m_x, m_y - box.m_y, m_width - box.m_width, m_height - box.m_height);
	}

	inline Box operator*(Box box)
	{
		return Box(m_x * box.m_x, m_y * box.m_y, m_width * box.m_width, m_height * box.m_height);
	}

	inline Box operator/(Box box)
	{
		return Box(m_x / box.m_x, m_y / box.m_y, m_width / box.m_width, m_height / box.m_height);
	}

	inline void operator+=(Box box)
	{
		m_x += box.m_x;
		m_y += box.m_y;
		m_width += box.m_width;
		m_height += box.m_height;
	}

	inline void operator-=(Box box)
	{
		m_x -= box.m_x;
		m_y -= box.m_y;
		m_width -= box.m_width;
		m_height -= box.m_height;
	}
};