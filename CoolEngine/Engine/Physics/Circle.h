#pragma once

/// <summary>
/// Circle class that contains its position and the radius. Used for Circle collisions.
/// </summary>
class Circle
{
public:
	/// <summary>
	/// Default constructor for circle.
	/// </summary>
	Circle()
	{
		m_x = 0;
		m_y = 0;
		m_radius = 0;
	};


	/// <summary>
	/// Constructor for circle.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="radius"></param>
	Circle(float x, float y, float radius) : m_x(x), m_y(y), m_radius(radius) {};

	float m_x, m_y, m_radius;

	Circle operator + (const Circle& c2)
	{
		Circle output;
		output.m_x = m_x + c2.m_x;
		output.m_y = m_y + c2.m_y;
		output.m_radius = m_radius + c2.m_radius;
		return output;
	}

	Circle operator - (const Circle& c2)
	{
		Circle output;
		output.m_x = m_x - c2.m_x;
		output.m_y = m_y - c2.m_y;
		output.m_radius = m_radius - c2.m_radius;
		return output;
	}

	Circle operator / (const Circle& c2)
	{
		Circle output;
		output.m_x = m_x / c2.m_x;
		output.m_y = m_y / c2.m_y;
		output.m_radius = m_radius / c2.m_radius;
		return output;
	}

	Circle operator * (const Circle& c2)
	{
		Circle output;
		output.m_x = m_x * c2.m_x;
		output.m_y = m_y * c2.m_y;
		output.m_radius = m_radius / c2.m_radius;
		return output;
	}
};