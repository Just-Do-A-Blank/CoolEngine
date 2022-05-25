#include "WeaponGameObject.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/EventManager.h"

WeaponGameObject::WeaponGameObject() : TriggerableGameObject()
{
    m_gameObjectType |= GameObjectType::WEAPON;
}

WeaponGameObject::WeaponGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}

WeaponGameObject::WeaponGameObject(const nlohmann::json& data, CoolUUID uuid) : TriggerableGameObject(data, uuid)
{
    m_gameObjectType |= GameObjectType::WEAPON;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

WeaponGameObject::WeaponGameObject(WeaponGameObject const& other) : TriggerableGameObject(other)
{
	m_level = other.m_level;
	m_strength = other.m_strength;
	m_damage = other.m_damage;

	m_shotCount = other.m_shotCount;
	m_timeLethal = other.m_timeLethal;
	m_distanceTravelled = other.m_distanceTravelled;

	ELEMENTS m_element = other.m_element;
	STATUSES m_statusEffect = other.m_statusEffect;
}

WeaponGameObject::~WeaponGameObject()
{

}

void WeaponGameObject::Serialize(nlohmann::json& data)
{
	TriggerableGameObject::Serialize(data);

    SaveLocalData(data);
}

void WeaponGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("WeaponLevel"))
    {
        m_level = jsonData["WeaponLevel"];
        m_strength = jsonData["WeaponStrength"];
        m_damage = jsonData["WeaponDamage"];
        m_shotCount = jsonData["WeaponShotCount"];
        m_timeLethal = jsonData["WeaponTimeLethal"];
        m_distanceTravelled = jsonData["WeaponDistanceTravelled"];
        m_element = (ELEMENTS)jsonData["WeaponElement"];
        m_statusEffect = (STATUSES)jsonData["WeaponStatus"];
    }
}

void WeaponGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["WeaponLevel"] = m_level;
    jsonData["WeaponStrength"] = m_strength;
    jsonData["WeaponDamage"] = m_damage;
    jsonData["WeaponShotCount"] = m_shotCount;
    jsonData["WeaponTimeLethal"] = m_timeLethal;
    jsonData["WeaponDistanceTravelled"] = m_distanceTravelled;
    jsonData["WeaponElement"] = (int)m_element;
    jsonData["WeaponStatus"] = (int)m_statusEffect;
}

void WeaponGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
    TriggerableGameObject::LoadAllPrefabData(jsonData);
}

void WeaponGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    TriggerableGameObject::SaveAllPrefabData(jsonData);
}

void WeaponGameObject::Attack()
{
}

void WeaponGameObject::CalculateWeaponStrength()
{
    m_strength = ((float)GetIsDualType() + 0.5f) * (m_statusEffect == STATUSES::NONE ? 0.5f : 1.5f) * RoundUp( (10.0f * m_distanceTravelled / m_timeLethal) * m_damage * m_shotCount );
}

void WeaponGameObject::SetLevel(int lvl)
{
    m_level = lvl;
}

void WeaponGameObject::SetStrength(int strength)
{
    m_strength = strength;
}

void WeaponGameObject::SetDamage(int dam)
{
    m_damage = dam;
}

void WeaponGameObject::SetElement(ELEMENTS element)
{
    m_element = element;
}

void WeaponGameObject::SetStatusEffect(STATUSES effect)
{
    m_statusEffect = effect;
}

void WeaponGameObject::SetShotCount(int count)
{
    m_shotCount = count;
}

void WeaponGameObject::SetTimeLethal(float time)
{
    m_timeLethal = time;
}

void WeaponGameObject::SetDistanceTravelled(float dist)
{
    m_distanceTravelled = dist;
}

void WeaponGameObject::SetIsPlayerWeapon(bool player)
{
    m_isPlayerWeapon = player;
}

void WeaponGameObject::SetBulletTexturePath(wstring path)
{
    m_bulletTexturePath = path;
}

void WeaponGameObject::SetBulletScale(XMFLOAT3 scale)
{
    m_bulletScale = scale;
}

void WeaponGameObject::SetCollisionScale(XMFLOAT2 scale)
{
    m_collisionScale = scale;
}

int WeaponGameObject::GetLevel()
{
    return m_level;
}

int WeaponGameObject::GetStrength()
{
    return m_strength;
}

int WeaponGameObject::GetDamage()
{
    return m_damage;
}

ELEMENTS WeaponGameObject::GetElement()
{
    return m_element;
}

STATUSES WeaponGameObject::GetStatusEffect()
{
    return m_statusEffect;
}

int WeaponGameObject::GetShotCount()
{
    return m_shotCount;
}

float WeaponGameObject::GetTimeLethal()
{
    return m_timeLethal;
}

float WeaponGameObject::GetDistanceTravelled()
{
    return m_distanceTravelled;
}

bool WeaponGameObject::GetIsPlayerWeapon()
{
    return m_isPlayerWeapon;
}

wstring WeaponGameObject::GetBulletTexturePath()
{
    return m_bulletTexturePath;
}

XMFLOAT3 WeaponGameObject::GetBulletScale()
{
    return m_bulletScale;
}

XMFLOAT2 WeaponGameObject::GetCollisionScale()
{
    return m_collisionScale;
}

bool WeaponGameObject::GetIsDualType()
{
    return ((int)m_element >= (int)ELEMENTS::POISONFIRE);
}

int WeaponGameObject::RoundUp(float value)
{
    int temp = (int)value;
    if ((float)temp != value)
    {
        // If not a whole number already
        return (temp + 1);
    }

    return temp;
}

void WeaponGameObject::Handle(Event* e)
{
	switch (e->GetEventID())
	{
	case EventType::MouseButtonPressed:
		{
			MouseButtonPressedEvent* pmouseEvent = (MouseButtonPressedEvent*)e;

			if (pmouseEvent->GetButton() == VK_LBUTTON)
			{
				Attack();
			}
		}
		break;
	}
}

void WeaponGameObject::RegisterForEvents()
{
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
}

void WeaponGameObject::UnregisterForEvents()
{
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
}
