#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

class WeaponGameObject : public TriggerableGameObject
{
public:
	WeaponGameObject(string identifier, CoolUUID uuid);
};