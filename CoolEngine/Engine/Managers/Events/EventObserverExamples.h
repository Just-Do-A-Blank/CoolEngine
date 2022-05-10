#pragma once
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/CollisionEvents.h"

class CollisionObserver : public Observer
{
public:
	void TriggerEnter(TriggerEnterEvent* e)
	{
		LOG("Trigger enter!");
	}

	void TriggerHold(TriggerHoldEvent* e)
	{
		LOG("Trigger hold!");
	}

	void TriggerExit(TriggerExitEvent* e)
	{
		LOG("Trigger exit!");
	}

	void CollisionEnter(CollisionEnterEvent* e)
	{
		LOG("Collision enter!");
	}

	void CollisionHold(CollisionHoldEvent* e)
	{
		LOG("Collision hold!");
	}

	void CollisionExit(CollisionExitEvent* e)
	{
		LOG("Collision exit!");
	}

	void Handle(Event* e)
	{
		switch (e->GetEventID())
		{
		case EventType::TriggerEnter:
			TriggerEnter((TriggerEnterEvent*)e);
			break;
		case EventType::TriggerHold:
			TriggerHold((TriggerHoldEvent*)e);
			break;
		case EventType::TriggerExit:
			TriggerExit((TriggerExitEvent*)e);
			break;
		case EventType::CollisionEnter:
			CollisionEnter((CollisionEnterEvent*)e);
			break;
		case EventType::CollisionHold:
			CollisionHold((CollisionHoldEvent*)e);
			break;
		case EventType::CollisionExit:
			CollisionExit((CollisionExitEvent*)e);
			break;
		}
	}

	CollisionObserver() { }
};