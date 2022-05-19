#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

class InteractableGameObject : public TriggerableGameObject
{
public:
	InteractableGameObject(string identifier, CoolUUID uuid);
	InteractableGameObject(const nlohmann::json& data, CoolUUID index);
	InteractableGameObject(InteractableGameObject const& other);
	virtual ~InteractableGameObject()override;

	virtual void Serialize(nlohmann::json& jsonData) override;
};