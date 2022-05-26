#include "RangedWeaponGameObject.h"
#include "Engine/Managers/Events/AttackEvents.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/GameManager.h"

RangedWeaponGameObject::RangedWeaponGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::RANGE_WEAPON;
    m_shotSpeed = 50.0f;
}

RangedWeaponGameObject::RangedWeaponGameObject(const nlohmann::json& data, CoolUUID uuid) : WeaponGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::RANGE_WEAPON;
    m_shotSpeed = 50.0f;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

RangedWeaponGameObject::RangedWeaponGameObject(RangedWeaponGameObject const& other) : WeaponGameObject(other)
{
	m_angleInterval = other.m_angleInterval;
	m_shotSpeed = other.m_shotSpeed;

	m_isShot = other.m_isShot;
	m_timeBetweenShots = other.m_timeBetweenShots;
}

RangedWeaponGameObject::~RangedWeaponGameObject()
{
}

void RangedWeaponGameObject::SetAngleInterval(float angle)
{
	m_angleInterval = angle;
}

void RangedWeaponGameObject::SetIsShot(bool shot)
{
	m_isShot = shot;
}

void RangedWeaponGameObject::SetSpeed(float speed)
{
	m_shotSpeed = speed;
}

void RangedWeaponGameObject::SetTimeBetweenShots(float shotTime)
{
	m_timeBetweenShots = shotTime;
}

float RangedWeaponGameObject::GetAngleInterval()
{
	return m_angleInterval;
}

bool RangedWeaponGameObject::GetIsShot()
{
	return m_isShot;
}

float RangedWeaponGameObject::GetSpeed()
{
	return m_shotSpeed;
}

float RangedWeaponGameObject::GetTimeBetweenShots()
{
	return m_timeBetweenShots;
}

void RangedWeaponGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);

    SaveLocalData(data);
}

void RangedWeaponGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("AngleInterval"))
    {
        m_angleInterval = jsonData["AngleInterval"];
        m_shotSpeed = jsonData["ShotSpeed"];
    }
}
void RangedWeaponGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["AngleInterval"] = m_angleInterval;
    jsonData["ShotSpeed"] = m_shotSpeed;
}

void RangedWeaponGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
    WeaponGameObject::LoadAllPrefabData(jsonData);
}

void RangedWeaponGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    WeaponGameObject::SaveAllPrefabData(jsonData);
}

void RangedWeaponGameObject::Attack()
{
	if (GameManager::GetInstance()->GetTimer()->GameTime() - m_lastShotTimestamp >= m_timeBetweenShots)
	{
		EventManager::Instance()->AddEvent(new CreateBulletEvent(this, m_transform->GetForwardVector(), m_transform->GetWorldPosition()));

		m_lastShotTimestamp = GameManager::GetInstance()->GetTimer()->GameTime();
	}
}

void RangedWeaponGameObject::Update()
{
    XMFLOAT2 toWeapon = MathHelper::Minus(GameManager::GetInstance()->GetCamera()->GetMousePositionInWorldSpace(), GetHolderPosition());
    toWeapon = MathHelper::Normalize(toWeapon);

    SetWeaponPosition(toWeapon);
}