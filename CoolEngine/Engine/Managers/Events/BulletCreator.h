#pragma once
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/AttackEvents.h"

class BulletCreator : public Observer
{
public:
	void CreateBullet(CreateBulletEvent* e);
	void TestFire(MouseButtonPressedEvent* e);

	void Handle(Event* e);

	unsigned int m_bulletCounter = 0;

	BulletCreator();
};