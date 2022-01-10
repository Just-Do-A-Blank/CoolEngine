#include "Collision.h"

bool Collision::BoxCollision(Box box1, Box box2)
{
	if ( box1.m_x + box1.m_width > box2.m_x && box1.m_x < box2.m_x + box2.m_width && box1.m_y + box1.m_height > box2.m_y && box1.m_y < box2.m_y + box2.m_height )
	{
		return true;
	}

	return false;
}

bool Collision::CircleCollision(Circle circle1, Circle circle2)
{
	float m_distanceBetweenCircles = sqrt((circle2.m_x - circle1.m_x) * (circle2.m_x - circle1.m_x) + (circle2.m_y - circle1.m_y) * (circle2.m_y - circle1.m_y));
	if (m_distanceBetweenCircles <= circle1.m_radius + circle2.m_radius)
	{
		return true;
	}
	return false;
}