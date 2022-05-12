#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

class WeaponGameObject : public TriggerableGameObject
{
public:
	WeaponGameObject(string identifier, CoolUUID uuid);
	WeaponGameObject(json& data, int index);
};