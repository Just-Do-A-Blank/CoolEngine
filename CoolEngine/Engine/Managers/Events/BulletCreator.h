#pragma once
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/AttackEvents.h"
#include "Engine/Structure/ObjectPool.h"
#include "Engine/GameObjects/BulletGameObject.h"

class BulletCreator : public Observer
{
public:
	BulletCreator();
	~BulletCreator();

	unsigned int m_bulletCounter = 0;
	ObjectPool<BulletGameObject>* m_pBulletPool = nullptr;

	void CreateBullet(CreateBulletEvent* e);
	void TestFire(MouseButtonPressedEvent* e);
	void Handle(Event* e);

	void DeleteBullets();

	void Render(RenderStruct& renderStruct);
	void Update();
};