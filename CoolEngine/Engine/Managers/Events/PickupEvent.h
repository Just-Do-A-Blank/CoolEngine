#pragma once
#include "Engine/Managers/Events/Events.h"
#include "Engine/GameObjects/PickupGameObject.h"

class PickupEvent : public Event
{
public:
	PickupEvent(ConsumableData ConsumableData) : m_consumableData(ConsumableData), Event(EventType::Pickup) {}
	~PickupEvent() {}

	inline ConsumableData GetConsumableData() { return m_consumableData; }
private:
	ConsumableData m_consumableData;


};