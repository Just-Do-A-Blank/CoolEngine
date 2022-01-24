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

bool Collision::CircleBoxCollision(Circle* circle, Box* box)
{
	XMFLOAT2 halfSize = { box->m_transform->GetScale().x, box->m_transform->GetScale().y };
	XMFLOAT2 middle = { box->m_transform->GetPosition().x, box->m_transform->GetPosition().y };

	float x = max(middle.x - halfSize.x, min(circle->m_transform->GetPosition().x, middle.x + halfSize.x));
	float y = max(middle.y - halfSize.y, min(circle->m_transform->GetPosition().y, middle.y + halfSize.y));

	XMFLOAT2 distance = { x - circle->m_transform->GetPosition().x , y - circle->m_transform->GetPosition().y };
	float distMagnitude = sqrt(distance.x * distance.x + distance.y * distance.y);

	if (distMagnitude < circle->m_radius)
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
		// Left to right - positive
		// Bottom to top - positive

		// Based on stackoverflow.com/questions/46172953/aabb-collision-resolution-slipping-sides
		XMFLOAT2 penetration = { 0, 0 };
		XMFLOAT2 vertexToPlayerCentre = XMFLOAT2(middleP.x - middleO.x, middleP.y - middleO.y);
		XMFLOAT2 minDistance = XMFLOAT2(halfSizeO.x + halfSizeP.x, halfSizeO.y + halfSizeP.y);
		if (abs(vertexToPlayerCentre.x) < minDistance.x && abs(vertexToPlayerCentre.y) < minDistance.y)
		{
			penetration.x = ( vertexToPlayerCentre.x > 0 ? minDistance.x - vertexToPlayerCentre.x : -minDistance.x - vertexToPlayerCentre.x );
			penetration.y = ( vertexToPlayerCentre.y > 0 ? minDistance.y - vertexToPlayerCentre.y : -minDistance.y - vertexToPlayerCentre.y );
		}

		if (abs(penetration.x) < abs(penetration.y))
		{
			if (penetration.x < 0)
			{
				// Left side
				XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, middleP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
			else
			{
				// Right side
				XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
		}
		else if (abs(penetration.x) > abs(penetration.y))
		{
			if (penetration.y < 0)
			{
				// Bottom side
				XMFLOAT3 pos = { middleP.x, middleO.y - halfSizeO.y - halfSizeP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
			else
			{
				// Top side
				XMFLOAT3 pos = { middleP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
				player->m_transform->SetPosition(pos);
			}
		}
		else
		{
			if (penetration.x > 0)
			{
				if (penetration.y > 0)
				{
					// Top right corner
					XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
					player->m_transform->SetPosition(pos);
				}
				else
				{
					// Bottom right corner
					XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleO.y - halfSizeO.y - halfSizeP.y, player->m_transform->GetPosition().z };
					player->m_transform->SetPosition(pos);
				}
			}
			else if(penetration.x < 0)
			{
				if (penetration.y > 0)
				{
					// Top left corner
					XMFLOAT3 pos = { middleO.x - halfSizeO.x - halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
					player->m_transform->SetPosition(pos);
				}
				else
				{
					// Top right corner
					XMFLOAT3 pos = { middleO.x + halfSizeO.x + halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_transform->GetPosition().z };
					player->m_transform->SetPosition(pos);
				}
			}
		}

		return true;
	}

	return false;
}

bool Collision::CircleBoxCollisionAndResponse(Circle* circle, Box* box)
{
	XMFLOAT2 halfSize = { box->m_transform->GetScale().x, box->m_transform->GetScale().y };
	XMFLOAT2 middle = { box->m_transform->GetPosition().x, box->m_transform->GetPosition().y };

	float x = max(middle.x - halfSize.x, min(circle->m_transform->GetPosition().x, middle.x + halfSize.x));
	float y = max(middle.y - halfSize.y, min(circle->m_transform->GetPosition().y, middle.y + halfSize.y));

	XMFLOAT2 distance = { x - circle->m_transform->GetPosition().x , y - circle->m_transform->GetPosition().y };
	float distMagnitude = sqrt(distance.x * distance.x + distance.y * distance.y);

	if (distMagnitude < circle->m_radius)
	{
		XMFLOAT2 norm = { distance.x / distMagnitude, distance.y / distMagnitude };
		XMFLOAT2 penetration = { norm.x * circle->m_radius, norm.y * circle->m_radius };
		penetration = { penetration.x - distance.x, penetration.y - distance.y };

		XMFLOAT2 vertexToPlayer = XMFLOAT2(circle->m_transform->GetPosition().x - middle.x, circle->m_transform->GetPosition().y - middle.y);
		// Left to right - positive
		// Bottom to top - positive
		if (vertexToPlayer.y > 0 && vertexToPlayer.x < vertexToPlayer.y && vertexToPlayer.x > vertexToPlayer.y * -1 || vertexToPlayer.y < 0 && vertexToPlayer.x > vertexToPlayer.y && vertexToPlayer.x < vertexToPlayer.y * -1)
		{
			// Up/Down side
			XMFLOAT3 pos = { circle->m_transform->GetPosition().x, circle->m_transform->GetPosition().y - penetration.y, circle->m_transform->GetPosition().z };
			circle->m_transform->SetPosition(pos);
		}
		else if (vertexToPlayer.x > 0 && vertexToPlayer.y < vertexToPlayer.x && vertexToPlayer.y > vertexToPlayer.x * -1 || vertexToPlayer.x < 0 && vertexToPlayer.y > vertexToPlayer.x && vertexToPlayer.y < vertexToPlayer.x * -1)
		{
			// Left/Right side
			//XMFLOAT3 pos = { middle.x + halfSize.x + circle->m_radius, circle->m_transform->GetPosition().y, circle->m_transform->GetPosition().z };
			XMFLOAT3 pos = { circle->m_transform->GetPosition().x - penetration.x, circle->m_transform->GetPosition().y, circle->m_transform->GetPosition().z };
			circle->m_transform->SetPosition(pos);
		}
		else
		{
			XMFLOAT3 pos = { circle->m_transform->GetPosition().x - penetration.x, circle->m_transform->GetPosition().y - penetration.y, circle->m_transform->GetPosition().z };
			circle->m_transform->SetPosition(pos);
		}

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
		// To Do - test it!

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
				if (it1->second->GetShape() == nullptr || it2->second->GetShape() == nullptr)
				{
					continue;
				}

				// Whether to just collisde or collide with response
				if (it1->second->GetShape()->IsCollidable() && it2->second->GetShape()->IsCollidable())
				{
					// To Do - Find a way to do something with hasCollided
					bool hasCollided = it1->second->GetShape()->CollideResponse(it2->second->GetShape());
				}
				else if (it1->second->GetShape()->IsTrigger() && it2->second->GetShape()->IsTrigger() || it1->second->GetShape()->IsCollidable() && it2->second->GetShape()->IsTrigger() || it2->second->GetShape()->IsCollidable() && it1->second->GetShape()->IsTrigger())
				{
					bool hasCollided = it1->second->GetShape()->Collide(it2->second->GetShape());
				}
			}
		}
	}
}