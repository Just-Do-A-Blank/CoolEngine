#pragma once
#include "TriggerableGameObject.h"

class InteractableGameObject : public TriggerableGameObject
{
public:
	InteractableGameObject();
	InteractableGameObject(string identifier);
};