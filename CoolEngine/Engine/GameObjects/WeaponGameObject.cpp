#include "WeaponGameObject.h"

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

	m_level = data["WeaponLevel"];
	m_strength = data["WeaponStrength"];
	m_damage = data["WeaponDamage"];
	m_shotCount = data["WeaponShotCount"];
	m_timeLethal = data["WeaponTimeLethal"];
	m_distanceTravelled = data["WeaponDistanceTravelled"];
	m_element = (ELEMENTS)data["WeaponElement"];
	m_statusEffect = (STATUSES)data["WeaponStatus"];
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

	data["WeaponLevel"] = m_level;
	data["WeaponStrength"] = m_strength;
	data["WeaponDamage"] = m_damage;
	data["WeaponShotCount"] = m_shotCount;
	data["WeaponTimeLethal"] = m_timeLethal;
	data["WeaponDistanceTravelled"] = m_distanceTravelled;
	data["WeaponElement"] = (int)m_element;
	data["WeaponStatus"] = (int)m_statusEffect;
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
