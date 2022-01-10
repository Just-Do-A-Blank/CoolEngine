#include "Collision.h"

bool Collision::BoxCollision(Box box1, Box box2)
{
	if ( box1.m_x + box1.m_width > box2.m_x && box1.m_x < box2.m_x + box2.m_width && box1.m_y + box1.m_height > box2.m_y && box1.m_y < box2.m_y + box2.m_height )
	{
		return true;
	}

	return false;
}