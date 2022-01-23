#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"


EnemyGameObject::EnemyGameObject(string identifier) : CharacterGameObject(identifier)
{

}


void EnemyGameObject::Update()
{
	if (!m_curPath.empty())
	{
		XMFLOAT3 desiredDirection = MathHelper::Minus(m_curPath.back()->pos, m_transform.GetPosition());
		desiredDirection = MathHelper::Normalize(desiredDirection);
		m_direction = MathHelper::Plus(m_direction, desiredDirection);

		if (m_curPath.back() == Pathfinding::Instance()->FindClosestNode(m_transform.GetPosition()))
		{
			m_curPath.pop_back();
			LOG("New Node");
		}


		float step = m_moveSpeed *GameManager::GetInstance()->GetTimer()->DeltaTime();



		m_direction = MathHelper::Normalize(m_direction);

		XMFLOAT3 stepPos = MathHelper::Multiply(m_direction, step);
		stepPos = MathHelper::Plus(stepPos, m_transform.GetPosition());
		m_transform.SetPosition(stepPos);
		
		//CharacterGameObject::Update();


	}
	else
	{
		LOG("NO PATH");
		m_curPath = Pathfinding::Instance()->FindPath(m_transform.GetPosition(), XMFLOAT3(100, 500, 0));
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
