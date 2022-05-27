#include "Engine/GameObjects/Gameplay/Player/PlayerResource.h"

PlayerResource::PlayerResource(string key)
{
	m_value = 0;
	m_maxValue = 0;
	m_minValue = 0;
	m_key = key;
    m_defaultValue = 0;
    m_valueGained = 0;
    m_attachToWeaponDamage = false;
    m_resourceKillOnDrain = false;
}

string PlayerResource::GetKey() const
{
	return m_key;
}

int PlayerResource::GetValue()
{
	return m_value;
}

int PlayerResource::GetMaxValue()
{
	return m_maxValue;
}

int PlayerResource::GetDefaultValue()
{
    return m_defaultValue;
}

int PlayerResource::GetMinValue()
{
    return m_minValue;
}

bool PlayerResource::GetAttachesToWeaponDamage()
{
    return m_attachToWeaponDamage;
}

bool PlayerResource::GetKillsOnDrain()
{
    return m_resourceKillOnDrain;
}

int PlayerResource::GetGain()
{
	return m_valueGained;
}

void PlayerResource::SetValue(int value)
{
	m_value = value;
}

void PlayerResource::SetMaxValue(int value)
{
	m_maxValue = value;
}

void PlayerResource::SetMinValue(int value)
{
	m_minValue = value;
}

void PlayerResource::SetDefaultValue(int value)
{
    m_defaultValue = value;
}

void PlayerResource::SetAttachesToWeaponDamage(bool value)
{
    m_attachToWeaponDamage = value;
}

void PlayerResource::SetKillsOnDrain(bool value)
{
    m_resourceKillOnDrain = value;
}

void PlayerResource::SetGain(int value)
{
	m_valueGained = value;
}