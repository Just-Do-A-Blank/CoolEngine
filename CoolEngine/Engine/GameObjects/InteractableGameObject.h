#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

class InteractableGameObject : public TriggerableGameObject
{
public:
	InteractableGameObject();
	InteractableGameObject(string identifier);
};