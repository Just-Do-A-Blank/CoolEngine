#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

class InteractableGameObject : public TriggerableGameObject
{
public:
	InteractableGameObject(string identifier, CoolUUID uuid);
	InteractableGameObject(const json& data, CoolUUID index);
	virtual ~InteractableGameObject()override;

	virtual void Serialize(json& jsonData) override;
};