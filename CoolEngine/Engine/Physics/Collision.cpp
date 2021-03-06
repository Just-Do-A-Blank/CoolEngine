#include "Collision.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"

#include "Engine/GameObjects/CollidableGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/Events/CollisionEvents.h"

#include "Engine/Managers/DebugDrawManager.h"
#include "Engine/Managers/Events/BulletCreator.h"
#include "Engine/TileMap/TileMap/Tile.h"

bool Collision::BoxCollision(Box* box1, Box* box2)
{
	XMFLOAT2 halfSize1 = XMFLOAT2(box1->m_halfSize.x, box1->m_halfSize.y);
	XMFLOAT2 halfSize2 = XMFLOAT2(box2->m_halfSize.x, box2->m_halfSize.y);
	XMFLOAT2 middle1 = XMFLOAT2(box1->m_pgameObject->GetTransform()->GetWorldPosition().x, box1->m_pgameObject->GetTransform()->GetWorldPosition().y);
	XMFLOAT2 middle2 = XMFLOAT2(box2->m_pgameObject->GetTransform()->GetWorldPosition().x, box2->m_pgameObject->GetTransform()->GetWorldPosition().y);

	return (middle1.x + halfSize1.x > middle2.x - halfSize2.x && middle1.x - halfSize1.x < middle2.x + halfSize2.x && middle1.y + halfSize1.y > middle2.y - halfSize2.y && middle1.y - halfSize1.y < middle2.y + halfSize2.y);
}

bool Collision::CircleCollision(Circle* circle1, Circle* circle2)
{
	XMFLOAT2 pos1 = XMFLOAT2(circle1->m_pgameObject->GetTransform()->GetWorldPosition().x, circle1->m_pgameObject->GetTransform()->GetWorldPosition().y);
	XMFLOAT2 pos2 = XMFLOAT2(circle2->m_pgameObject->GetTransform()->GetWorldPosition().x, circle2->m_pgameObject->GetTransform()->GetWorldPosition().y);

	float m_distanceBetweenCircles = sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y));
	if (m_distanceBetweenCircles <= circle1->m_radius + circle2->m_radius)
	{
		return true;
	}
	return false;
}

bool Collision::CircleBoxCollision(Circle* circle, Box* box)
{
	XMFLOAT2 halfSize = XMFLOAT2(box->m_halfSize.x, box->m_halfSize.y);
	XMFLOAT2 middle = XMFLOAT2(box->m_pgameObject->GetTransform()->GetWorldPosition().x, box->m_pgameObject->GetTransform()->GetWorldPosition().y);

	float x = max(middle.x - halfSize.x, min(circle->m_pgameObject->GetTransform()->GetWorldPosition().x, middle.x + halfSize.x));
	float y = max(middle.y - halfSize.y, min(circle->m_pgameObject->GetTransform()->GetWorldPosition().y, middle.y + halfSize.y));

	XMFLOAT2 distance = XMFLOAT2(x - circle->m_pgameObject->GetTransform()->GetWorldPosition().x , y - circle->m_pgameObject->GetTransform()->GetWorldPosition().y);
	float distMagnitude = sqrt(distance.x * distance.x + distance.y * distance.y);

	if (distMagnitude < circle->m_radius)
	{
		return true;
	}

	return false;
}

bool Collision::BoxCollisionAndResponse(Box* player, Box* object)
{
	XMFLOAT2 halfSizeP = XMFLOAT2(player->m_halfSize.x, player->m_halfSize.y);
	XMFLOAT2 halfSizeO = XMFLOAT2(object->m_halfSize.x, object->m_halfSize.y);

	Transform* playerTransform = player->m_pgameObject->GetTransform();
	Transform* objectTransform = object->m_pgameObject->GetTransform();

	XMFLOAT2 middleP = XMFLOAT2(playerTransform->GetWorldPosition().x, playerTransform->GetWorldPosition().y);
	XMFLOAT2 middleO = XMFLOAT2(objectTransform->GetWorldPosition().x, objectTransform->GetWorldPosition().y);

	if (middleP.x + halfSizeP.x > middleO.x - halfSizeO.x && middleP.x - halfSizeP.x < middleO.x + halfSizeO.x && middleP.y + halfSizeP.y > middleO.y - halfSizeO.y && middleP.y - halfSizeP.y < middleO.y + halfSizeO.y)
	{
		// Left to right - positive
		// Bottom to top - positive

		// Based on stackoverflow.com/questions/46172953/aabb-collision-resolution-slipping-sides
		if (playerTransform->HasChildTransform(objectTransform) || objectTransform->HasChildTransform(playerTransform))
		{
			return false;
		}

		XMFLOAT2 penetration = XMFLOAT2(0, 0);
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
				XMFLOAT3 pos = XMFLOAT3(middleO.x - halfSizeO.x - halfSizeP.x, middleP.y, playerTransform->GetWorldPosition().z);
				player->m_pgameObject->GetTransform()->SetWorldPosition(pos);
			}
			else
			{
				// Right side
				XMFLOAT3 pos = XMFLOAT3(middleO.x + halfSizeO.x + halfSizeP.x, middleP.y, playerTransform->GetWorldPosition().z);
				playerTransform->SetWorldPosition(pos);
			}
		}
		else if (abs(penetration.x) > abs(penetration.y))
		{
			if (penetration.y < 0)
			{
				// Bottom side
				XMFLOAT3 pos = XMFLOAT3(middleP.x, middleO.y - halfSizeO.y - halfSizeP.y, playerTransform->GetWorldPosition().z);
				playerTransform->SetWorldPosition(pos);
			}
			else
			{
				// Top side
				XMFLOAT3 pos = XMFLOAT3(middleP.x, middleO.y + halfSizeO.y + halfSizeP.y, playerTransform->GetWorldPosition().z);
				playerTransform->SetWorldPosition(pos);
			}
		}
		else
		{
			if (penetration.x > 0)
			{
				if (penetration.y > 0)
				{
					// Top right corner
					XMFLOAT3 pos = XMFLOAT3(middleO.x + halfSizeO.x + halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, playerTransform->GetWorldPosition().z);
					playerTransform->SetWorldPosition(pos);
				}
				else
				{
					// Bottom right corner
					XMFLOAT3 pos = XMFLOAT3(middleO.x + halfSizeO.x + halfSizeP.x, middleO.y - halfSizeO.y - halfSizeP.y, playerTransform->GetWorldPosition().z);
					playerTransform->SetWorldPosition(pos);
				}
			}
			else if(penetration.x < 0)
			{
				if (penetration.y > 0)
				{
					// Top left corner
					XMFLOAT3 pos = XMFLOAT3(middleO.x - halfSizeO.x - halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_pgameObject->GetTransform()->GetWorldPosition().z);
					playerTransform->SetWorldPosition(pos);
				}
				else
				{
					// Top right corner
					XMFLOAT3 pos = XMFLOAT3(middleO.x + halfSizeO.x + halfSizeP.x, middleO.y + halfSizeO.y + halfSizeP.y, player->m_pgameObject->GetTransform()->GetWorldPosition().z);
					playerTransform->SetWorldPosition(pos);
				}
			}
		}

		return true;
	}

	return false;
}

bool Collision::CircleBoxCollisionAndResponse(Circle* circle, Box* box)
{
	XMFLOAT2 halfSize = XMFLOAT2(box->m_halfSize.x, box->m_halfSize.y);
	XMFLOAT2 middle = XMFLOAT2(box->m_pgameObject->GetTransform()->GetWorldPosition().x, box->m_pgameObject->GetTransform()->GetWorldPosition().y);

	float x = max(middle.x - halfSize.x, min(circle->m_pgameObject->GetTransform()->GetWorldPosition().x, middle.x + halfSize.x));
	float y = max(middle.y - halfSize.y, min(circle->m_pgameObject->GetTransform()->GetWorldPosition().y, middle.y + halfSize.y));

	XMFLOAT2 distance = XMFLOAT2(x - circle->m_pgameObject->GetTransform()->GetWorldPosition().x , y - circle->m_pgameObject->GetTransform()->GetWorldPosition().y);
	float distMagnitude = sqrt(distance.x * distance.x + distance.y * distance.y);

	if (distMagnitude < circle->m_radius)
	{
		// Left to right - positive
		// Bottom to top - positive

		XMFLOAT2 norm = XMFLOAT2(distance.x / distMagnitude, distance.y / distMagnitude);
		XMFLOAT2 penetration = XMFLOAT2(norm.x * circle->m_radius, norm.y * circle->m_radius);
		penetration = XMFLOAT2(penetration.x - distance.x, penetration.y - distance.y);

		XMFLOAT2 penetrationDepth = XMFLOAT2(0, 0);
		XMFLOAT2 minDistance = XMFLOAT2(halfSize.x + circle->m_radius, halfSize.y + circle->m_radius);
		if (abs(distance.x) < minDistance.x && abs(distance.y) < minDistance.y)
		{
			penetrationDepth.x = (distance.x > 0 ? minDistance.x - distance.x : -minDistance.x - distance.x);
			penetrationDepth.y = (distance.y > 0 ? minDistance.y - distance.y : -minDistance.y - distance.y);
		}

		if (abs(penetrationDepth.x) < abs(penetrationDepth.y))
		{
			XMFLOAT3 pos = XMFLOAT3(box->m_pgameObject->GetTransform()->GetWorldPosition().x + penetration.x, box->m_pgameObject->GetTransform()->GetWorldPosition().y + penetration.y, box->m_pgameObject->GetTransform()->GetWorldPosition().z);
			box->m_pgameObject->GetTransform()->SetWorldPosition(pos);
		}
		else if (abs(penetrationDepth.x) > abs(penetrationDepth.y))
		{
			XMFLOAT3 pos = XMFLOAT3(box->m_pgameObject->GetTransform()->GetWorldPosition().x + penetration.x, box->m_pgameObject->GetTransform()->GetWorldPosition().y + penetration.y, box->m_pgameObject->GetTransform()->GetWorldPosition().z);
			box->m_pgameObject->GetTransform()->SetWorldPosition(pos);
		}

		return true;
	}

	return false;
}

bool Collision::CircleCollisionAndResponse(Circle* circle1, Circle* circle2)
{
	XMFLOAT2 pos1 = XMFLOAT2(circle1->m_pgameObject->GetTransform()->GetWorldPosition().x, circle1->m_pgameObject->GetTransform()->GetWorldPosition().y);
	XMFLOAT2 pos2 = XMFLOAT2(circle2->m_pgameObject->GetTransform()->GetWorldPosition().x, circle2->m_pgameObject->GetTransform()->GetWorldPosition().y);

	float m_distanceBetweenCircles = sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y));
	if (m_distanceBetweenCircles <= circle1->m_radius + circle2->m_radius)
	{
		XMFLOAT2 vectorToPlayer = XMFLOAT2( pos1.x - pos2.x, pos1.y - pos2.y );
		float penetration = circle1->m_radius + circle2->m_radius - m_distanceBetweenCircles;

		XMFLOAT3 pos = XMFLOAT3(pos1.x + vectorToPlayer.x / m_distanceBetweenCircles * penetration, pos1.y + vectorToPlayer.y / m_distanceBetweenCircles * penetration, circle1->m_pgameObject->GetTransform()->GetWorldPosition().z);
		circle1->m_pgameObject->GetTransform()->SetWorldPosition(pos);

		return true;
	}

	return false;
}

void Collision::Update(vector<GameObject*> gameObjectMap)
{
	CollidableGameObject* pcollidable1 = nullptr;
	CollidableGameObject* pcollidable2 = nullptr;

	for (int it1 = 0; it1 < gameObjectMap.size(); ++it1)
	{
		if (gameObjectMap[it1]->ContainsType(GameObjectType::COLLIDABLE) == false || gameObjectMap[it1]->GetEnabled() == false)
		{
			continue;
		}
		pcollidable1 = dynamic_cast<CollidableGameObject*>(gameObjectMap[it1]);
		pcollidable1->UpdateShapeDimensions();

		if (pcollidable1->GetShape() != nullptr && pcollidable1->GetShape()->IsRendered())
		{
			XMFLOAT3 p = pcollidable1->GetTransform()->GetWorldPosition();
			XMFLOAT2 d = pcollidable1->GetShape()->GetShapeDimensions();

#if _DEBUG
			DebugDrawManager::GetInstance()->CreateWorldSpaceDebugRect(p, XMFLOAT3(d.x, d.y, 1), DebugDrawManager::DebugColour::PURPLE);
#endif
		}
	}

	for (int it1 = 0; it1 < gameObjectMap.size(); ++it1)
	{
		if (gameObjectMap[it1]->GetEnabled() == false)
		{
			continue;
		}

		for (int it2 = it1+1; it2 < gameObjectMap.size(); ++it2)
		{
			if (gameObjectMap[it1]->ContainsType(GameObjectType::COLLIDABLE) == false || gameObjectMap[it2]->ContainsType(GameObjectType::COLLIDABLE) == false || gameObjectMap[it2]->GetEnabled() == false)
			{
				continue;
			}

			pcollidable1 = dynamic_cast<CollidableGameObject*>(gameObjectMap[it1]);
			pcollidable2 = dynamic_cast<CollidableGameObject*>(gameObjectMap[it2]);

			if (pcollidable1->GetShape() == nullptr || pcollidable2->GetShape() == nullptr)
			{
				continue;
			}

			// Whether to just collide or collide with response
			if (pcollidable1->GetShape()->IsCollidable() && pcollidable2->GetShape()->IsCollidable())
			{
				bool hasCollided = pcollidable1->GetShape()->CollideResponse(pcollidable2->GetShape());
				if (hasCollided)
				{
					EventManager::Instance()->AddEvent(new CollisionHoldEvent(pcollidable1, pcollidable2));
				}
			}
			else if (pcollidable1->GetShape()->IsTrigger() && pcollidable2->GetShape()->IsTrigger() || pcollidable1->GetShape()->IsCollidable() && pcollidable2->GetShape()->IsTrigger() || pcollidable2->GetShape()->IsCollidable() && pcollidable1->GetShape()->IsTrigger())
			{
				bool hasCollided = pcollidable1->GetShape()->Collide(pcollidable2->GetShape());
				if (hasCollided)
				{
					EventManager::Instance()->AddEvent(new TriggerHoldEvent(pcollidable1, pcollidable2));
				}
			}

		}
	}
}

void Collision::Update(vector<GameObject*> gameObjectMap, vector<ObjectEntry<BulletGameObject>*> bulletMap)
{
	CollidableGameObject* pcollidable = nullptr;
	BulletGameObject* pbullet = nullptr;

	for (int itObj = 0; itObj < gameObjectMap.size(); ++itObj)
	{
		for (int itBullet = 0; itBullet < bulletMap.size(); ++itBullet)
		{
			if (itObj != itBullet)
			{
				if (gameObjectMap[itObj]->ContainsType(GameObjectType::COLLIDABLE) == false)
				{
					continue;
				}

				pcollidable = dynamic_cast<CollidableGameObject*>(gameObjectMap[itObj]);
				pbullet = bulletMap[itBullet]->m_pObject;

				if (pcollidable->GetShape() == nullptr || pbullet->GetShape() == nullptr || !bulletMap[itBullet]->m_Active)
				{
					continue;
				}

				if (pcollidable->GetShape()->IsTrigger() && pbullet->GetShape()->IsTrigger() || pcollidable->GetShape()->IsCollidable() && pbullet->GetShape()->IsTrigger() || pbullet->GetShape()->IsCollidable() && pcollidable->GetShape()->IsTrigger())
				{
					bool hasCollided = pcollidable->GetShape()->Collide(pbullet->GetShape());
					if (hasCollided)
					{
						EventManager::Instance()->AddEvent(new TriggerHoldEvent(pcollidable, pbullet));
					}
				}
			}
		}
	}
}

void Collision::Update(vector<GameObject*>& gameObjectMap, vector<Tile*>& tiles)
{
	CollidableGameObject* pcollidable = nullptr;

	for (int i = 0; i < gameObjectMap.size(); ++i)
	{
		for (int j = 0; j < tiles.size(); ++j)
		{
			if (gameObjectMap[i]->ContainsType(GameObjectType::COLLIDABLE) == false || tiles[j] == nullptr)
			{
				continue;
			}

			pcollidable = dynamic_cast<CollidableGameObject*>(gameObjectMap[i]);

			if (pcollidable->GetShape() == nullptr || tiles[j]->GetShape() == nullptr)
			{
				continue;
			}

			if (pcollidable->GetShape() != nullptr && pcollidable->GetShape()->IsRendered())
			{
				XMFLOAT3 p = pcollidable->GetTransform()->GetWorldPosition();
				XMFLOAT2 d = pcollidable->GetShape()->GetShapeDimensions();

#if _DEBUG
				DebugDrawManager::GetInstance()->CreateWorldSpaceDebugRect(p, XMFLOAT3(d.x, d.y, 1), DebugDrawManager::DebugColour::PURPLE);
#endif
			}

			if (tiles[j]->GetShape() != nullptr && tiles[j]->GetShape()->IsRendered())
			{
				XMFLOAT3 p = tiles[j]->GetTransform()->GetWorldPosition();
				XMFLOAT2 d = tiles[j]->GetShape()->GetShapeDimensions();

#if _DEBUG
				DebugDrawManager::GetInstance()->CreateWorldSpaceDebugRect(p, XMFLOAT3(d.x, d.y, 1), DebugDrawManager::DebugColour::PURPLE);
#endif
			}

			if (pcollidable->GetShape()->IsCollidable() == true)
			{
				bool hasCollided = pcollidable->GetShape()->CollideResponse(tiles[j]->GetShape());

				if (hasCollided)
				{
					EventManager::Instance()->AddEvent(new TriggerHoldEvent(pcollidable, tiles[j]));
				}
			}
		}
	}
}
