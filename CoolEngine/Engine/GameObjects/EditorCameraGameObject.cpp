#include "EditorCameraGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/EditorUI/EditorUI.h"

EditorCameraGameObject::EditorCameraGameObject(string identifier, CoolUUID uuid) : CameraGameObject(identifier, uuid)
{
	m_areControlledByUser = true;

	Initialize();
}

EditorCameraGameObject::EditorCameraGameObject(const nlohmann::json& data, CoolUUID uuid) : CameraGameObject(data, uuid)
{
	m_areControlledByUser = true;
	m_speedBoost = data["Speed Boost"];
	m_moveSpeed = data["Movement Speed"];
}

EditorCameraGameObject::EditorCameraGameObject(EditorCameraGameObject const& other) : CameraGameObject(other)
{
	m_moveSpeed = other.m_moveSpeed;
	m_speedBoost = other.m_speedBoost;
}

EditorCameraGameObject::~EditorCameraGameObject()
{
}

void EditorCameraGameObject::Initialize()
{
	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
}

void EditorCameraGameObject::Handle(Event* e)
{
	switch (e->GetEventID())
	{
	case EventType::KeyPressed:
		KeyPressed((KeyPressedEvent*)e);
		break;

	case EventType::KeyReleased:
		KeyReleased((KeyReleasedEvent*)e);
		break;
	}
}

void EditorCameraGameObject::KeyPressed(KeyPressedEvent* e)
{
	if (!m_areControlledByUser)
	{
		return;
	}

	if (e->GetKeyCode() == VK_SHIFT)
	{
		m_speedBoost = 5.0f;
	}

	// Player movement
	m_movement = XMFLOAT3(0, 0, 0);

	if (e->GetKeyCode() == 'W')
	{
		m_movement.y = 1.0f;
	}
	if (e->GetKeyCode() == 'S')
	{
		m_movement.y = -1.0f;
	}
	if (e->GetKeyCode() == 'A')
	{
		m_movement.x = -1.0f;
	}
	if (e->GetKeyCode() == 'D')
	{
		m_movement.x = 1.0f;
	}	
}

void EditorCameraGameObject::KeyReleased(KeyReleasedEvent* e)
{
	if (!m_areControlledByUser)
	{

	}

	if (e->GetKeyCode() == VK_SHIFT)
	{
		m_speedBoost = 1.0f;
	}
}

void EditorCameraGameObject::Serialize(nlohmann::json& jsonData)
{
	CameraGameObject::Serialize(jsonData);

	jsonData["Speed Boost"] = m_speedBoost;
	jsonData["Movement Speed"] = m_moveSpeed;
}

void EditorCameraGameObject::EditorUpdate()
{
	if (m_movement.x != 0.0f || m_movement.y != 0.0f)
	{
		XMStoreFloat3(&m_movement, XMVector3Normalize(XMLoadFloat3(&m_movement)) * m_moveSpeed * m_speedBoost * GameManager::GetInstance()->GetTimer()->DeltaTime());
		GetTransform()->Translate(m_movement);
	}
}

#if EDITOR
	/// <summary>
	/// Shows engine UI
	/// </summary>
	void EditorCameraGameObject::CreateEngineUI()
	{
		CameraGameObject::CreateEngineUI();

		if (EditorUI::CollapsingSection("Camera controller", false))
		{
			EditorUINonSpecificParameters cameraParameters = EditorUINonSpecificParameters();
			cameraParameters.m_tooltipText = "Ticked means you can control the camera, Unticked means it will act more like the game.";
			EditorUI::Checkbox("Controlled by User", m_areControlledByUser, cameraParameters);
		}
	}
#endif