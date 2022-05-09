#pragma once
#include <Engine/Managers/Events/Events.h>
#include <Engine/GameObjects/GameObject.h>

class CollisionEvent : public Event
{
public:
	inline GameObject* GetGameObject(int i) { return m_objects[i]; }
protected:
	CollisionEvent(GameObject* obj1, GameObject* obj2, EventType eventID) : Event(eventID)
	{
		m_objects[0] = obj1;
		m_objects[1] = obj2;
	}
	GameObject* m_objects[2];
};

class TriggerEnterEvent : public CollisionEvent
{
public:
	TriggerEnterEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerEnter) {}
};

class TriggerHoldEvent : public CollisionEvent
{
public:
	TriggerHoldEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerHold) {}
};

class TriggerExitEvent : public CollisionEvent
{
public:
	TriggerExitEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerExit) {}
};

class CollisionEnterEvent : public CollisionEvent
{
public:
	CollisionEnterEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionEnter) {}
};

class CollisionHoldEvent : public CollisionEvent
{
public:
	CollisionHoldEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionHold) {}
};

class CollisionExitEvent : public CollisionEvent
{
public:
	CollisionExitEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionExit) {}
};