#include "EditorCameraGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/GameManager.h"

EditorCameraGameObject::EditorCameraGameObject(string identifier, CoolUUID uuid) : CameraGameObject(identifier, uuid)
{

}

EditorCameraGameObject::EditorCameraGameObject(const nlohmann::json& data, CoolUUID uuid) : CameraGameObject(data, uuid)
{
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

void EditorCameraGameObject::Initialize(XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector, float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth)
{
	CameraGameObject::Initialize(position, forwardVector, upVector, windowWidth, windowHeight, nearDepth, farDepth);

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
}

void EditorCameraGameObject::Handle(Event* e)
{
	/*switch (e->GetEventID())
	{
	case EventType::KeyPressed:
		KeyPressed((KeyPressedEvent*)e);
		break;

	case EventType::KeyReleased:
		KeyReleased((KeyReleasedEvent*)e);
		break;
	}*/
}

void EditorCameraGameObject::KeyPressed(KeyPressedEvent* e)
{
	if (e->GetKeyCode() == VK_SHIFT)
	{
		m_speedBoost = 5.0f;
	}

	// Player movement
	XMFLOAT3 movement = XMFLOAT3(0, 0, 0);

	if (e->GetKeyCode() == 'W')
	{
		movement.y = 1.0f;
	}
	if (e->GetKeyCode() == 'S')
	{
		movement.y = -1.0f;
	}
	if (e->GetKeyCode() == 'A')
	{
		movement.x = -1.0f;
	}
	if (e->GetKeyCode() == 'D')
	{
		movement.x = 1.0f;
	}

	if (movement.x != 0.0f || movement.y != 0.0f)
	{
		XMStoreFloat3(&movement, XMVector3Normalize(XMLoadFloat3(&movement)) * m_moveSpeed * m_speedBoost * GameManager::GetInstance()->GetTimer()->DeltaTime());
		GetTransform()->Translate(movement);
	}
}

void EditorCameraGameObject::KeyReleased(KeyReleasedEvent* e)
{
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
