#include "EnemyGameObject.h"



EnemyGameObject::EnemyGameObject(string identifier) : GameObject(identifier)
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
		}
	}

	//Move, should probably be in Gameobject? (since it'll need to be in the character aswell
	float step = m_speed * GameManager::GetInstance()->GetTimer()->DeltaTime();
	m_direction = MathHelper::Normalize(m_direction);
	m_transform.SetPosition(MathHelper::Multiply(m_direction, step));
	m_transform.SetRotation(MathHelper::Multiply(m_direction, -1));
}

void EnemyGameObject::SetPath(vector<node*> path)
{

	m_curPath = path;


}

const vector<node*> EnemyGameObject::GetPath() const
{
	return m_curPath;
}
