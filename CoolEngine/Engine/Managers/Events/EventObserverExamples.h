#pragma once
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/CollisionEvents.h"
#include "Engine/Managers/Events/EventManager.h"


class CollisionObserver : public Observer
{
public:
	void TriggerEnter(TriggerEnterEvent* e)
	{
		//LOG("Trigger enter!");
	}

	void TriggerHold(TriggerHoldEvent* e)
	{
		//LOG("Trigger hold!");
	}

	void TriggerExit(TriggerExitEvent* e)
	{
		//LOG("Trigger exit!");
	}

	void CollisionEnter(CollisionEnterEvent* e)
	{
		//LOG("Collision enter!");
	}

	void CollisionHold(CollisionHoldEvent* e)
	{
		//LOG("Collision hold!");
	}

	void CollisionExit(CollisionExitEvent* e)
	{
		//LOG("Collision exit!");
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

	CollisionObserver()
	{
		EventManager::Instance()->AddClient(EventType::TriggerEnter, this);
		EventManager::Instance()->AddClient(EventType::TriggerHold, this);
		EventManager::Instance()->AddClient(EventType::TriggerExit, this);
		EventManager::Instance()->AddClient(EventType::CollisionEnter, this);
		EventManager::Instance()->AddClient(EventType::CollisionHold, this);
		EventManager::Instance()->AddClient(EventType::CollisionExit, this);
	}
};