#include "Collision.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"

bool Collision::BoxCollision(Box* box1, Box* box2)
{
	XMFLOAT2 halfSize1 = { box1->m_transform->GetScale().x, box1->m_transform->GetScale().y };
	XMFLOAT2 halfSize2 = { box2->m_transform->GetScale().x, box2->m_transform->GetScale().y };
	XMFLOAT2 middle1 = { box1->m_transform->GetPosition().x, box1->m_transform->GetPosition().y };
	XMFLOAT2 middle2 = { box2->m_transform->GetPosition().x, box2->m_transform->GetPosition().y };

	return (middle1.x + halfSize1.x > middle2.x - halfSize2.x && middle1.x - halfSize1.x < middle2.x + halfSize2.x && middle1.y + halfSize1.y > middle2.y - halfSize2.y && middle1.y - halfSize1.y < middle2.y + halfSize2.y);
}

//bool Collision::LineBoxCollision(Line line, Box box)
//{
//	return (box.m_x + box.m_width > line.m_point1.x && box.m_x < line.m_point1.x && box.m_y + box.m_height > line.m_point1.y && box.m_y < line.m_point1.y);
//}

bool Collision::LineBoxCollision(Line line, Box* box)
{
	XMFLOAT2 halfSize = { box->m_transform->GetScale().x, box->m_transform->GetScale().y };
	XMFLOAT2 middle = { box->m_transform->GetPosition().x, box->m_transform->GetPosition().y };

	return (middle.x + halfSize.x > line.m_point1.x && middle.x - halfSize.x < line.m_point1.x && middle.y + halfSize.y > line.m_point1.y && middle.y - halfSize.y < line.m_point1.y);
}

bool Collision::CircleCollision(Circle* circle1, Circle* circle2)
{
	XMFLOAT2 pos1 = { circle1->m_transform->GetPosition().x, circle1->m_transform->GetPosition().y };
	XMFLOAT2 pos2 = { circle2->m_transform->GetPosition().x, circle2->m_transform->GetPosition().y };

	float m_distanceBetweenCircles = sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y));
	if (m_distanceBetweenCircles <= circle1->m_radius + circle2->m_radius)
	{
		return true;
	}
	return false;
}

//bool Collision::CircleBoxCollision(Circle circle, Box box)
//{
//	float distanceX = abs(circle.m_x - box.m_x);
//	float distanceY = abs(circle.m_y - box.m_y);
//
//	if (distanceX > (box.m_width / 2) + circle.m_radius || distanceY > (box.m_height / 2) + circle.m_radius)
//	{
//		return false;
//	}
//	else if (distanceX <= box.m_width / 2 || distanceY <= box.m_height / 2)
//	{
//		return true;
//	}
//
//	// Pythagoras
//	float cornerDistance = (distanceX - box.m_width / 2) * (distanceX - box.m_width / 2) + (distanceY - box.m_height / 2) * (distanceY - box.m_height / 2);
//	cornerDistance = sqrt(cornerDistance);
//
//	return (cornerDistance <= circle.m_radius);
//}

bool Collision::CircleBoxCollision(Circle* circle, Box* box)
{
	XMFLOAT2 halfSize = { box->m_transform->GetScale().x, box->m_transform->GetScale().y };
	XMFLOAT2 middle = { box->m_transform->GetPosition().x, box->m_transform->GetPosition().y };

	float distanceX = abs(circle->m_transform->GetPosition().x - middle.x);
	float distanceY = abs(circle->m_transform->GetPosition().y - middle.y);

	if (distanceX > halfSize.x + circle->m_radius || distanceY > halfSize.y + circle->m_radius)
	{
		return false;
	}
	else if (distanceX <= halfSize.x || distanceY <= halfSize.y)
	{
		return true;
	}

	// Pythagoras
	float cornerDistance = (distanceX - halfSize.x) * (distanceX - halfSize.x) + (distanceY - halfSize.y) * (distanceY - halfSize.y);
	cornerDistance = sqrt(cornerDistance);

	if (cornerDistance <= circle->m_radius)
	{
		return true;
	}

	return false;
}

bool Collision::BoxCollisionAndResponse(Box* player, Box* object)
{
	XMFLOAT2 halfSizeP = { player->m_transform->GetScale().x, player->m_transform->GetScale().y };
	XMFLOAT2 halfSizeO = { object->m_transform->GetScale().x, object->m_transform->GetScale().y };
	XMFLOAT2 middleP = { player->m_transform->GetPosition().x, player->m_transform->GetPosition().y };
	XMFLOAT2 middleO = { object->m_transform->GetPosition().x, object->m_transform->GetPosition().y };

	if (middleP.x + halfSizeP.x > middleO.x - halfSizeO.x && middleP.x - halfSizeP.x < middleO.x + halfSizeO.x && middleP.y + halfSizeP.y > middleO.y - halfSizeO.y && middleP.y - halfSizeP.y < middleO.y + halfSizeO.y)
	{
		// Collision

		XMFLOAT2 vertexToPlayer = XMFLOAT2(middleP.x - middleO.x, middleP.y - middleO.y);
		// Left to right - positive
		// Bottom to top - positive
		if (vertexToPlayer.y > 0 && vertexToPlayer.x < vertexToPlayer.y && vertexToPlayer.x > vertexToPlayer.y * -1)
		{
			// Up side
			XMFLOAT3 pos = { middleP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
			player->m_transform->SetPosition(pos);
		}
		else if (vertexToPlayer.y < 0 && vertexToPlayer.x > vertexToPlayer.y && vertexToPlayer.x < vertexToPlayer.y * -1)
		{
			// Down side
			XMFLOAT3 pos = { middleP.x, middleO.y - halfSizeO.y - halfSizeP.y, player->m_transform->GetPosition().z };
			player->m_transform->SetPosition(pos);
		}
		else if (vertexToPlayer.x > 0 && vertexToPlayer.y < vertexToPlayer.x && vertexToPlayer.y > vertexToPlayer.x * -1)
		{
			// Right side
			XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleP.y, player->m_transform->GetPosition().z };
			player->m_transform->SetPosition(pos);
		}
		else if (vertexToPlayer.x < 0 && vertexToPlayer.y > vertexToPlayer.x && vertexToPlayer.y < vertexToPlayer.x * -1)
		{
			// Left side
			XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, middleP.y, player->m_transform->GetPosition().z };
			player->m_transform->SetPosition(pos);
		}
		else if (vertexToPlayer.x == vertexToPlayer.y)
		{
			if (vertexToPlayer.x > 0)
			{
				// Top right corner
				XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
			else if(vertexToPlayer.x < 0)
			{
				// Bottom left corner
				XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, middleO.y - halfSizeO.y - halfSizeP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
		}
		else if (vertexToPlayer.x == vertexToPlayer.y * -1)
		{
			if (vertexToPlayer.x > 0)
			{
				// Bottom right corner
				XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleO.y - halfSizeO.y - halfSizeP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
			else if (vertexToPlayer.x < 0)
			{
				// Top left corner
				XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
		}
		else
		{
			// Default to top side
			// Should only happen if player is somehow at exact position of object
			XMFLOAT3 pos = { middleP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
			player->m_transform->SetPosition(pos);
		}

		return true;
	}

	return false;
}

bool Collision::CircleBoxCollisionAndResponse(Circle* circle, Box* box)
{
	XMFLOAT2 halfSize = { box->m_transform->GetScale().x, box->m_transform->GetScale().y };
	XMFLOAT2 middle = { box->m_transform->GetPosition().x, box->m_transform->GetPosition().y };

	float distanceX = abs(circle->m_transform->GetPosition().x - middle.x);
	float distanceY = abs(circle->m_transform->GetPosition().y - middle.y);

	if (distanceX > halfSize.x + circle->m_radius || distanceY > halfSize.y + circle->m_radius)
	{
		return false;
	}
	else if (distanceX <= halfSize.x || distanceY <= halfSize.y)
	{
		return true;
	}

	// Pythagoras
	float cornerDistance = (distanceX - halfSize.x) * (distanceX - halfSize.x) + (distanceY - halfSize.y) * (distanceY - halfSize.y);
	cornerDistance = sqrt(cornerDistance);

	if (cornerDistance <= circle->m_radius)
	{
		// To do - this!!!!!
		// If you need this urgently for some reason yell at me

		return true;
	}

	return false;
}

bool Collision::CircleCollisionAndResponse(Circle* circle1, Circle* circle2)
{
	XMFLOAT2 pos1 = { circle1->m_transform->GetPosition().x, circle1->m_transform->GetPosition().y };
	XMFLOAT2 pos2 = { circle2->m_transform->GetPosition().x, circle2->m_transform->GetPosition().y };

	float m_distanceBetweenCircles = sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y));
	if (m_distanceBetweenCircles <= circle1->m_radius + circle2->m_radius)
	{
		XMFLOAT2 vectorToPlayer = XMFLOAT2( pos1.x - pos2.x, pos1.y - pos2.y );
		float penetration = circle1->m_radius + circle2->m_radius - m_distanceBetweenCircles;

		XMFLOAT3 pos = { pos1.x + vectorToPlayer.x / m_distanceBetweenCircles * penetration, pos1.y + vectorToPlayer.y / m_distanceBetweenCircles * penetration, circle1->m_transform->GetPosition().z };
		circle1->m_transform->SetPosition(pos);

		return true;
	}

	return false;
}

void Collision::Update(unordered_map<string, GameObject*> gameObjectMap)
{
	for (unordered_map<string, GameObject*>::iterator it1 = gameObjectMap.begin(); it1 != gameObjectMap.end(); ++it1)
	{
		for (unordered_map<string, GameObject*>::iterator it2 = gameObjectMap.begin(); it2 != gameObjectMap.end(); ++it2)
		{
			if (it1 != it2)
			{
				// Whether to just collisde or collide with response
				if (it1->second->IsCollidable() && it2->second->IsCollidable())
				{
					// To Do - Find a way to do something with hasCollided
					bool hasCollided = it1->second->GetShape()->CollideResponse(it2->second->GetShape());
				}
				else if (it1->second->IsTrigger() && it2->second->IsTrigger() || it1->second->IsCollidable() && it2->second->IsTrigger() || it2->second->IsCollidable() && it1->second->IsTrigger())
				{
					bool hasCollided = it1->second->GetShape()->Collide(it2->second->GetShape());
				}
			}
		}
	}
}