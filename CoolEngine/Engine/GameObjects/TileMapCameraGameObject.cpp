#include "TileMapCameraGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/GameManager.h"

TileMapCameraGameObject::TileMapCameraGameObject(string identifier, CoolUUID uuid) : CameraGameObject(identifier, uuid)
{

}

void TileMapCameraGameObject::Initialize(XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector, float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth)
{
	CameraGameObject::Initialize(position, forwardVector, upVector, windowWidth, windowHeight, nearDepth, farDepth);

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
}

void TileMapCameraGameObject::Handle(Event* e)
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

void TileMapCameraGameObject::KeyPressed(KeyPressedEvent* e)
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

void TileMapCameraGameObject::KeyReleased(KeyReleasedEvent* e)
{
	if (e->GetKeyCode() == VK_SHIFT)
	{
		m_speedBoost = 1.0f;
	}
}

void TileMapCameraGameObject::Serialize(json& jsonData)
{

	GameObject::Serialize(jsonData);


	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };


	jsonData[std::to_string((int)m_gameObjectType)]["Position"].push_back(position);
	jsonData[std::to_string((int)m_gameObjectType)]["Rotation"].push_back(rotation);
	jsonData[std::to_string((int)m_gameObjectType)]["Scale"].push_back(scale);
	jsonData[std::to_string((int)m_gameObjectType)]["Speed Boost"].push_back(m_speedBoost);
	jsonData[std::to_string((int)m_gameObjectType)]["Movement Speed"].push_back(m_moveSpeed);

}

void TileMapCameraGameObject::Deserialize(json& jsonData)
{
}
