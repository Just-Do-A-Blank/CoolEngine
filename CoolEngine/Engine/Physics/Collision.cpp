#include "Collision.h"

//bool Collision::BoxCollision(Box box1, Box box2)
//{
//	return (box1.m_x + box1.m_width > box2.m_x && box1.m_x < box2.m_x + box2.m_width && box1.m_y + box1.m_height > box2.m_y && box1.m_y < box2.m_y + box2.m_height);
//}

bool Collision::BoxCollision(Transform* t1, Transform* t2)
{
	XMFLOAT2 halfSize1 = { t1->GetScale().x, t1->GetScale().y };
	XMFLOAT2 halfSize2 = { t2->GetScale().x, t2->GetScale().y };
	XMFLOAT2 middle1 = { t1->GetPosition().x, t1->GetPosition().y };
	XMFLOAT2 middle2 = { t2->GetPosition().x, t2->GetPosition().y };

	return (middle1.x + halfSize1.x > middle2.x - halfSize2.x && middle1.x - halfSize1.x < middle2.x + halfSize2.x && middle1.y + halfSize1.y > middle2.y - halfSize2.y && middle1.y - halfSize1.y < middle2.y + halfSize2.y);
}

bool Collision::LineBoxCollision(Line line, Box box)
{
	return (box.m_x + box.m_width > line.m_point1.x && box.m_x < line.m_point1.x && box.m_y + box.m_height > line.m_point1.y && box.m_y < line.m_point1.y);
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

bool Collision::CircleBoxCollision(Circle circle, Box box)
{
	float distanceX = abs(circle.m_x - box.m_x);
	float distanceY = abs(circle.m_y - box.m_y);

	if (distanceX > (box.m_width / 2) + circle.m_radius || distanceY > (box.m_height / 2) + circle.m_radius)
	{
		return false;
	}
	else if (distanceX <= box.m_width / 2 || distanceY <= box.m_height / 2)
	{
		return true;
	}

	// Pythagoras
	float cornerDistance = (distanceX - box.m_width / 2) * (distanceX - box.m_width / 2) + (distanceY - box.m_height / 2) * (distanceY - box.m_height / 2);
	cornerDistance = sqrt(cornerDistance);

	return (cornerDistance <= circle.m_radius);
}

bool Collision::BoxCollisionAndResponse(Transform* player, Transform* object)
{
	XMFLOAT2 halfSizeP = { player->GetScale().x, player->GetScale().y };
	XMFLOAT2 halfSizeO = { object->GetScale().x, object->GetScale().y };
	XMFLOAT2 middleP = { player->GetPosition().x, player->GetPosition().y };
	XMFLOAT2 middleO = { object->GetPosition().x, object->GetPosition().y };

	if (middleP.x + halfSizeP.x > middleO.x - halfSizeO.x && middleP.x - halfSizeP.x < middleO.x + halfSizeO.x && middleP.y + halfSizeP.y > middleO.y - halfSizeO.y && middleP.y - halfSizeP.y < middleO.y + halfSizeO.y)
	{
		// Collision

		XMFLOAT2 vertexToPlayer = XMFLOAT2(middleP.x - middleO.x, middleP.y - middleO.y);
		// Left to right - positive
		// Bottom to top - positive
		if (vertexToPlayer.y > 0 && vertexToPlayer.x < vertexToPlayer.y && vertexToPlayer.x > vertexToPlayer.y * -1)
		{
			// Up side
			XMFLOAT3 pos = { player->GetPosition().x, middleO.y + halfSizeO.y + halfSizeP.y, player->GetPosition().z };
			player->SetPosition(pos);
		}
		else if (vertexToPlayer.y < 0 && vertexToPlayer.x > vertexToPlayer.y && vertexToPlayer.x < vertexToPlayer.y * -1)
		{
			// Down side
			XMFLOAT3 pos = { player->GetPosition().x, middleO.y - halfSizeO.y - halfSizeP.y, player->GetPosition().z };
			player->SetPosition(pos);
		}
		else if (vertexToPlayer.x > 0 && vertexToPlayer.y < vertexToPlayer.x && vertexToPlayer.y > vertexToPlayer.x * -1)
		{
			// Right side
			XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, player->GetPosition().y, player->GetPosition().z };
			player->SetPosition(pos);
		}
		else if (vertexToPlayer.x < 0 && vertexToPlayer.y > vertexToPlayer.x && vertexToPlayer.y < vertexToPlayer.x * -1)
		{
			// Left side
			XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, player->GetPosition().y, player->GetPosition().z };
			player->SetPosition(pos);
		}
		else if (vertexToPlayer.x == vertexToPlayer.y)
		{
			if (vertexToPlayer.x > 0)
			{
				// Top right corner
				XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->GetPosition().z };
				player->SetPosition(pos);
			}
			else if(vertexToPlayer.x < 0)
			{
				// Bottom left corner
				XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, middleO.y - halfSizeO.y - halfSizeP.y, player->GetPosition().z };
				player->SetPosition(pos);
			}
		}
		else if (vertexToPlayer.x == vertexToPlayer.y * -1)
		{
			if (vertexToPlayer.x > 0)
			{
				// Bottom right corner
				XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleO.y - halfSizeO.y - halfSizeP.y, player->GetPosition().z };
				player->SetPosition(pos);
			}
			else if (vertexToPlayer.x < 0)
			{
				// Top left corner
				XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->GetPosition().z };
				player->SetPosition(pos);
			}
		}
		else
		{
			// Default to top side
			// Should only happen if player is somehow at exact position of object
			XMFLOAT3 pos = { player->GetPosition().x, middleO.y + halfSizeO.y + halfSizeP.y, player->GetPosition().z };
			player->SetPosition(pos);
		}

		return true;
	}

	return false;
}