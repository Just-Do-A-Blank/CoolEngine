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

// Occurs when two objects collide without collision on. Fired on enter.
class TriggerEnterEvent : public CollisionEvent
{
public:
	TriggerEnterEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerEnter) {}
};

// Occurs when two objects collide without collision on. Fired every frame.
class TriggerHoldEvent : public CollisionEvent
{
public:
	TriggerHoldEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerHold) {}
};

// Occurs when two objects collide without collision on. Fired the frame the two stop colliding.
class TriggerExitEvent : public CollisionEvent
{
public:
	TriggerExitEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerExit) {}
};

// Occurs when two objects collide with collision on. Fired on enter.
class CollisionEnterEvent : public CollisionEvent
{
public:
	CollisionEnterEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionEnter) {}
};

// Occurs when two objects collide with collision on. Fired every frame.
class CollisionHoldEvent : public CollisionEvent
{
public:
	CollisionHoldEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionHold) {}
};

// Occurs when two objects collide with collision on. Fired the frame the two stop colliding.
class CollisionExitEvent : public CollisionEvent
{
public:
	CollisionExitEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionExit) {}
};