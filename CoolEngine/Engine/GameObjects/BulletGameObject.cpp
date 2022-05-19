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
}

BulletGameObject::~BulletGameObject()
{

}

void BulletGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);
}

void BulletGameObject::Update()
{
	float dTime = GameManager::GetInstance()->GetTimer()->DeltaTime();
	XMFLOAT3 translate = XMFLOAT3(m_direction.x * dTime * m_speed, m_direction.y * dTime * m_speed, 0.0f);
	m_transform->Translate(translate);
}