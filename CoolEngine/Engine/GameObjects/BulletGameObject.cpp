#include "BulletGameObject.h"
#include "Engine/Managers/GameManager.h"


BulletGameObject::BulletGameObject() : WeaponGameObject()
{
	m_gameObjectType |= GameObjectType::BULLET;
}

BulletGameObject::BulletGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::BULLET;
}

BulletGameObject::BulletGameObject(const nlohmann::json& data, CoolUUID uuid) : WeaponGameObject(data, uuid)
{
    m_gameObjectType |= GameObjectType::BULLET;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

BulletGameObject::~BulletGameObject()
{

}

void BulletGameObject::Serialize(nlohmann::json& data)
{
    SaveLocalData(data);
	WeaponGameObject::Serialize(data);
}

void BulletGameObject::Update()
{
	float dTime = GameManager::GetInstance()->GetTimer()->DeltaTime();
	XMFLOAT3 translate = XMFLOAT3(m_direction.x * dTime * m_speed, m_direction.y * dTime * m_speed, 0.0f);
	m_transform->Translate(translate);

	m_currentTravelTime += dTime;
	if (m_currentTravelTime >= m_totalTravelTime)
	{
		// Lifetime over
		m_isActive = false;
	}
}

void BulletGameObject::LoadLocalData(const nlohmann::json& jsonData)
{

}

void BulletGameObject::SaveLocalData(nlohmann::json& jsonData)
{

}

void BulletGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
    WeaponGameObject::LoadAllPrefabData(jsonData);
}

void BulletGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    WeaponGameObject::SaveAllPrefabData(jsonData);
}