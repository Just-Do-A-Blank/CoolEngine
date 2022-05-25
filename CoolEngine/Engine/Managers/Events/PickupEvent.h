#pragma once
#include "Engine/Managers/Events/Events.h"
#include "Engine/GameObjects/PickupGameObject.h"

class PickupEvent : public Event
{
public:
	PickupEvent(unordered_set<PickupResource*> ConsumableData) : m_consumableData(ConsumableData), Event(EventType::Pickup) {}
	~PickupEvent() {}

	inline unordered_set<PickupResource*>  GetConsumableData() { return m_consumableData; }
private:
	unordered_set<PickupResource*>  m_consumableData;


};