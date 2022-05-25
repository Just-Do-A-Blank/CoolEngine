#pragma once
#include "Engine/Managers/Events/Events.h"
#include "Engine/GameObjects/PickupGameObject.h"

class PickupEvent : public Event
{
public:
	PickupEvent(list<PickupResource*> ConsumableData) : m_consumableData(ConsumableData), Event(EventType::Pickup) {}
	~PickupEvent() {}

	inline list<PickupResource*>  GetConsumableData() { return m_consumableData; }
private:
	list<PickupResource*>  m_consumableData;


};