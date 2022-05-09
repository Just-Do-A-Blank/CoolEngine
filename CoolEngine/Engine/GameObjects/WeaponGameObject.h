#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

class WeaponGameObject : public TriggerableGameObject
{
public:
	WeaponGameObject();
	WeaponGameObject(string identifier);
};