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
	inline std::wstring GetTextureName() { return m_textureName; }

protected:
	AttackEvent(RangedWeaponGameObject* obj, XMFLOAT3 dir, XMFLOAT3 startPos, std::wstring texName, EventType eventID) : Event(eventID)
	{
		m_obj = obj;
		m_direction = dir;
		m_startPos = startPos;
		m_textureName = texName;
	}
	RangedWeaponGameObject* m_obj;
	XMFLOAT3 m_direction;
	XMFLOAT3 m_startPos;
	std::wstring m_textureName;
};

// Occurs when ranged weapon is fired
class CreateBulletEvent : public AttackEvent
{
public:
	CreateBulletEvent(RangedWeaponGameObject* obj, XMFLOAT3 dir, XMFLOAT3 startPos, std::wstring texName) : AttackEvent(obj, dir, startPos, texName, EventType::CreateBullet) {}
};