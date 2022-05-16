#pragma once
#include "Engine/Managers/Events/Events.h"
#include "Engine/GameObjects/GameObject.h"


class AttackEvent : public Event
{
public:
	inline GameObject* GetWeapon() { return m_weapon; }
protected:
	AttackEvent(GameObject* obj, EventType eventID) : Event(eventID)
	{
		m_weapon = obj;
	}
	GameObject* m_weapon;
};

// Occurs when ranged weapon is fired
class CreateBulletEvent : public AttackEvent
{
public:
	CreateBulletEvent(GameObject* obj) : AttackEvent(obj, EventType::CreateBullet) {}
};