#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"


EnemyGameObject::EnemyGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::ENEMY;
}

EnemyGameObject::EnemyGameObject(const nlohmann::json& data, CoolUUID uuid) : CharacterGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::ENEMY;
}

EnemyGameObject::EnemyGameObject(EnemyGameObject const& other) : CharacterGameObject(other)
{
	m_curPath = other.m_curPath;
	m_enemyState = other.m_enemyState;
}

EnemyGameObject::~EnemyGameObject()
{
}


void EnemyGameObject::Update()
{
	if (!m_curPath.empty())
	{

		//Uses directional movement for calculations, should be moved to charactergameobject then update called from here

		XMFLOAT3 desiredDirection = MathHelper::Minus(m_curPath.back()->m_pos, m_transform->GetWorldPosition());
		desiredDirection = MathHelper::Normalize(desiredDirection);
		m_direction = MathHelper::Plus(m_direction, desiredDirection);

		if (m_curPath.back() == Pathfinding::GetInstance()->FindClosestNode(m_transform->GetWorldPosition()))
		{
			m_curPath.pop_back();
			LOG("New Node");
		}


		float step = m_moveSpeed * GameManager::GetInstance()->GetTimer()->DeltaTime();

		m_direction = MathHelper::Normalize(m_direction);

		XMFLOAT3 stepPos = MathHelper::Multiply(m_direction, step);
		stepPos = MathHelper::Plus(stepPos, m_transform->GetWorldPosition());
		m_transform->SetWorldPosition(stepPos);

		if (m_invincibilityTime > 0.0f)
		{
			m_invincibilityTime -= GameManager::GetInstance()->GetTimer()->DeltaTime();
		}
		else
		{
			m_invincibilityTime = 0;
		}
	}
	else
	{
		//LOG("NO PATH");
		//Pathfinding::GetInstance()->FindPath(m_transform->GetPosition(), XMFLOAT3(1000, 200, 0), m_curPath); //test function
	}

}

void EnemyGameObject::EditorUpdate()
{
}

void EnemyGameObject::SetPath(vector<node*> path)
{
	m_curPath = path;
}

const vector<node*> EnemyGameObject::GetPath() const
{
	return m_curPath;
}

void EnemyGameObject::Serialize(nlohmann::json& jsonData)
{
	CharacterGameObject::Serialize(jsonData);
}