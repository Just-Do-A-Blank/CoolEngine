#include "AIState.h"
#include "Engine/GameObjects/EnemyGameObject.h"



UnitClass::UnitClass(EnemyGameObject* enemy, UnitClassType unitClass, bool attacker) : pEnemy(enemy), UnitType(unitClass), canAttack(attacker)
{
	curReaction = None;
	lastReaction = None;
}


void UnitClass::SetDesiredRangeFromPlayer(float min, float max)
{
	minimumDistance = min;
	maximumDistance = max;
	perfectDistance = minimumDistance + maximumDistance / 2;
}

void UnitClass::SetMinimumDistanceFromPlayer(float min)
{
	minimumDistance = min;
	perfectDistance = minimumDistance + maximumDistance / 2;
}

void UnitClass::SetMaximumDistanceFromPlayer(float max)
{
	maximumDistance = max;
	perfectDistance = minimumDistance + maximumDistance / 2;
}

void UnitClass::Update()
{
	XMFLOAT3 playerPos;
	XMFLOAT3 enemyPos = pEnemy->GetTransform()->GetPosition();
	float distanceToPlayer = MathHelper::Distance(playerPos, enemyPos);

	//Check if the player is too close, or too far
	if (distanceToPlayer > maximumDistance)
	{
		curReaction = Towards;
	}
	else if (distanceToPlayer < minimumDistance)
	{
		curReaction = Away;
	}
	else
	{
		curReaction = None;
	}

	//Currently generating a new path for the character on each tick of the game, if this proves to be slow, have a timer for only doing it every second or so
	vector<node*> newPath;
	switch (curReaction)
	{
	case Towards:
		//Generating a new path towards the player with each update loop
		Pathfinding::GetInstance()->FindPath(enemyPos, playerPos, newPath);

		break;
	case Away:
		//Forming a path between the enemy and the minimum radius
		XMFLOAT3 dir = MathHelper::Normalize(MathHelper::Minus(playerPos, enemyPos));
		dir = MathHelper::Multiply(dir, perfectDistance);
		Pathfinding::GetInstance()->FindPath(enemyPos, dir, newPath);

		break;
	case None:
		newPath = pEnemy->GetPath();
		break;
	}
	pEnemy->SetPath(newPath);

}


UnitClassPassive::UnitClassPassive(EnemyGameObject* enemy, UnitClassType type) : UnitClass(enemy, type ,false)
{
	if (curReaction == None)
	{
		//Check if ability can be used
		//Use ability if it can be used
	}
}

void UnitClassPassive::Update()
{
	UnitClass::Update();
}

UnitClassPassiveAvoid::UnitClassPassiveAvoid(EnemyGameObject* enemy) : UnitClassPassive(enemy,Avoid)
{
	//Change these to be the values that we need
	minimumDistance = 0;
	maximumDistance = INT64_MAX;
}

void UnitClassPassiveAvoid::Update()
{
	UnitClassPassive::Update();
}

UnitClassPassiveStationary::UnitClassPassiveStationary(EnemyGameObject* enemy) : UnitClassPassive(enemy,Stationary)
{
	//This unit doesn't move away or towards the player ever. It will just move towards a nearby node on update loop
	minimumDistance = 0;
	maximumDistance = INT64_MAX;
}

void UnitClassPassiveStationary::Update()
{
	//Randomly selects a nearby node to move towards
	vector<node*> path;
	node* closest = Pathfinding::GetInstance()->FindClosestNode(pEnemy->GetTransform()->GetPosition());
	int random = rand() % (closest->m_pNeighbours.size() - 1);

	Pathfinding::GetInstance()->FindPath(pEnemy->GetTransform()->GetPosition(), closest->m_pNeighbours[random]->m_pos, path);

	pEnemy->SetPath(path);

	UnitClassPassive::Update();
}
