#include "Collision.h"

bool Collision::BoxCollision(Box box1, Box box2)
{
	return (box1.m_x + box1.m_width > box2.m_x && box1.m_x < box2.m_x + box2.m_width && box1.m_y + box1.m_height > box2.m_y && box1.m_y < box2.m_y + box2.m_height);
}

bool Collision::LineBoxCollision(Line line, Box box)
{
	return (box.m_x + box.m_width > line.m_point1.m_x && box.m_x < line.m_point1.m_x&& box.m_y + box.m_height > line.m_point1.m_y && box.m_y < line.m_point1.m_y);
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