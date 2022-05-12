#include "RangedWeaponGameObject.h"

RangedWeaponGameObject::RangedWeaponGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{

}

RangedWeaponGameObject::~RangedWeaponGameObject()
{
}

void RangedWeaponGameObject::SetShotCount(int count)
{
	m_shotCount = count;
}

void RangedWeaponGameObject::SetAngleInterval(float angle)
{
	m_angleInterval = angle;
}

void RangedWeaponGameObject::SetIsShot(bool shot)
{
	m_isShot = shot;
}

int RangedWeaponGameObject::GetShotCount()
{
	return m_shotCount;
}

float RangedWeaponGameObject::GetAngleInterval()
{
	return m_angleInterval;
}

bool RangedWeaponGameObject::GetIsShot()
{
	return m_isShot;
}
