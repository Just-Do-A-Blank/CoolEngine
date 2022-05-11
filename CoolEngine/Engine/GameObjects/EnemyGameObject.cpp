#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"


EnemyGameObject::EnemyGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::ENEMY;
}

EnemyGameObject::EnemyGameObject(json data, int index) : CharacterGameObject(data, index)
{

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

void EnemyGameObject::Serialize(json& jsonData)
{
	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };


	GameObject::Serialize(jsonData);
	jsonData[std::to_string((int)m_gameObjectType)]["Position"].push_back(position);
	jsonData[std::to_string((int)m_gameObjectType)]["Rotation"].push_back(rotation);
	jsonData[std::to_string((int)m_gameObjectType)]["Scale"].push_back(scale);
	jsonData[std::to_string((int)m_gameObjectType)]["Layers"].push_back(m_health);
	jsonData[std::to_string((int)m_gameObjectType)]["Health"].push_back(m_layer);
	jsonData[std::to_string((int)m_gameObjectType)]["Movement Speed"].push_back(m_moveSpeed);
}

void EnemyGameObject::Deserialize(json& jsonData)
{
}
