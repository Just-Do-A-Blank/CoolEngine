#pragma once
#include "Engine/Managers/Events/Events.h"
#include "Engine/GameObjects/PickupGameObject.h"

class PickupEvent : public Event
{
public:
	PickupEvent(GameObject* object) : m_PickedupObject(object), Event(EventType::Pickup) {}
	~PickupEvent() {}

	inline GameObject* GetConsumableData() { return m_PickedupObject; }
private:
	GameObject* m_PickedupObject;


};