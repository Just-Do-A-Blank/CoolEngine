#pragma once
#include "Engine/Managers/Events/Events.h"
#include "Engine/GameObjects/GameObject.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"


class AttackEvent : public Event
{
public:
	inline RangedWeaponGameObject* GetObj() { return m_obj; }
	inline XMFLOAT3 GetDirection() { return m_direction; }
	inline XMFLOAT3 GetStartPos() { return m_startPos; }

protected:
	AttackEvent(RangedWeaponGameObject* obj, XMFLOAT3 dir, XMFLOAT3 startPos, EventType eventID) : Event(eventID)
	{
		m_obj = obj;
		m_direction = dir;
		m_startPos = startPos;
	}
	RangedWeaponGameObject* m_obj;
	XMFLOAT3 m_direction;
	XMFLOAT3 m_startPos;
};

// Occurs when ranged weapon is fired
class CreateBulletEvent : public AttackEvent
{
public:
	CreateBulletEvent(RangedWeaponGameObject* obj, XMFLOAT3 dir, XMFLOAT3 startPos) : AttackEvent(obj, dir, startPos, EventType::CreateBullet) {}
};