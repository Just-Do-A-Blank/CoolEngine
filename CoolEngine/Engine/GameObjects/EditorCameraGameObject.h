#pragma once
#include "CameraGameObject.h"
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/KeyEvents.h"


class EditorCameraGameObject : public CameraGameObject, public Observer
{
public:
	EditorCameraGameObject(string identifier, CoolUUID uuid);
	EditorCameraGameObject(const nlohmann::json& data, CoolUUID uuid);
	EditorCameraGameObject(EditorCameraGameObject const& other);
	virtual ~EditorCameraGameObject()override;

	void Initialize(XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector,
		float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth) override;

	void Handle(Event* e) override;

	virtual void Serialize(nlohmann::json& jsonData) override;

#if EDITOR
	/// <summary>
	/// Shows engine UI
	/// </summary>
	virtual void CreateEngineUI() override;
#endif

protected:

private:
	float m_moveSpeed = 100.0f;
	float m_speedBoost = 1;
	
	/// <summary>
	/// True means we take input from the user not following the player
	/// </summary>
	bool m_areControlledByUser;

	void KeyPressed(KeyPressedEvent* e);
	void KeyReleased(KeyReleasedEvent* e);
};

