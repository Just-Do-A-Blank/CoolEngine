#include "Engine/GameObjects/Gameplay/Player/PlayerResource.h"

PlayerResource::PlayerResource(string key)
{
	m_value = 0;
	m_maxValue = 0;
	m_minValue = 0;
	m_key = key;
    m_defaultValue = 0;
}

/*string PlayerResource::GetKey() const
{
	return m_key;
}

int& PlayerResource::GetValue()
{
	return m_value;
}

int& PlayerResource::GetMaxValue()
{
	return m_maxValue;
}

void PlayerResource::SetValue(int value)
{
	m_value = value;
}

void PlayerResource::SetMaxValue(int value)
{
	m_maxValue = value;
}

int& PlayerResource::GetMinValue()
{
	return m_minValue;
}

void PlayerResource::SetMinValue(int value)
{
	m_minValue = value;
}*/