#pragma once
#include "CameraGameObject.h"
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/KeyEvents.h"


class TileMapCameraGameObject : public CameraGameObject, public Observer
{
public:
	TileMapCameraGameObject(string identifier, CoolUUID uuid);
	virtual ~TileMapCameraGameObject();

	void Initialize(XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector,
		float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth) override;

	void Handle(Event* e) override;

protected:

private:
	float m_moveSpeed = 100.0f;
	float m_speedBoost = 1;

	void KeyPressed(KeyPressedEvent* e);
	void KeyReleased(KeyReleasedEvent* e);
};

