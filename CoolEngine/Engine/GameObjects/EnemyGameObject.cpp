#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"


EnemyGameObject::EnemyGameObject()
{
    m_gameObjectType |= GameObjectType::ENEMY;
}

EnemyGameObject::EnemyGameObject(string identifier) : CharacterGameObject(identifier)
{
    m_gameObjectType |= GameObjectType::ENEMY;
}


void EnemyGameObject::Update()
{
	if (!m_curPath.empty())
	{

		//Uses directional movement for calculations, should be moved to charactergameobject then update called from here

		XMFLOAT3 desiredDirection = MathHelper::Minus(m_curPath.back()->m_pos, m_transform->GetPosition());
		desiredDirection = MathHelper::Normalize(desiredDirection);
		m_direction = MathHelper::Plus(m_direction, desiredDirection);

		if (m_curPath.back() == Pathfinding::GetInstance()->FindClosestNode(m_transform->GetPosition()))
		{
			m_curPath.pop_back();
			LOG("New Node");
		}


		float step = m_moveSpeed * GameManager::GetInstance()->GetTimer()->DeltaTime();

		m_direction = MathHelper::Normalize(m_direction);

		XMFLOAT3 stepPos = MathHelper::Multiply(m_direction, step);
		stepPos = MathHelper::Plus(stepPos, m_transform->GetPosition());
		m_transform->SetPosition(stepPos);
		
	}
	else
	{
		//LOG("NO PATH");
		//Pathfinding::GetInstance()->FindPath(m_transform->GetPosition(), XMFLOAT3(1000, 200, 0), m_curPath); //test function
	}

}

void EnemyGameObject::SetPath(vector<node*> path)
{
	m_curPath = path;
}

const vector<node*> EnemyGameObject::GetPath() const
{
	return m_curPath;
}
