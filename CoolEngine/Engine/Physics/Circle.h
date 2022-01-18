#pragma once
#include "Engine/Physics/Shape.h"

/// <summary>
/// Circle class that contains its position and the radius. Used for Circle collisions.
/// </summary>
class Circle : public Shape
{
public:
	/// <summary>
	/// Default constructor for circle.
	/// </summary>
	Circle()
	{
		m_transform = nullptr;
		m_radius = 0;
	};

	/// <summary>
	/// Constructor for circle.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="radius"></param>
	Circle(Transform* t, float radius)
	{
		m_transform = t;
		m_radius = radius;
	}

	~Circle()
	{
		m_transform = nullptr;
	}

	float m_radius;

	bool Collide(Shape* shape)
	{
		return shape->Collide(this);
	}

	bool Collide(Circle* circle)
	{
		return Collision::CircleCollision(circle, this);
	}

	bool Collide(Box* box)
	{
		return Collision::CircleBoxCollision(this, box);
	}

	bool CollideResponse(Shape* shape)
	{
		return shape->CollideResponse(this);
	}

	bool CollideResponse(Circle* circle)
	{
		return Collision::CircleCollisionAndResponse(circle, this);
	}

	bool CollideResponse(Box* box)
	{
		return Collision::CircleBoxCollisionAndResponse(this, box);
	}

	/*Circle operator + (const Circle& c2)
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
	}*/
};