#pragma once
#include "Engine/GameObjects/RenderableCollidableGameObject.h"
#include "Engine/Managers/Events/EventObserver.h"

class TriggerableGameObject : public RenderableCollidableGameObject, public Observer
{
public:
	TriggerableGameObject();
	TriggerableGameObject(string identifier);

	virtual void Handle(Event* e) override {}
};

