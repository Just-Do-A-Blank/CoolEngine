#pragma once
#include "Engine/Managers/Events/Events.h"

class PickupEvent : public Event
{
public:
	PickupEvent(void* data) : Event(EventType::Pickup, data) {}
	~PickupEvent() {}


private:


};