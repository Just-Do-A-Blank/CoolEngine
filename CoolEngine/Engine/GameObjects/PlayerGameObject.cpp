#include "PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include <Engine/Managers/Events/EventManager.h>

PlayerGameObject::PlayerGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseMoved, this);

	m_gameObjectType |= GameObjectType::PLAYER;
}

PlayerGameObject::PlayerGameObject(json data, int index) : CharacterGameObject(data, index)
{
	m_gameObjectType |= GameObjectType::PLAYER;
}

PlayerGameObject::~PlayerGameObject()
{
	EventManager::Instance()->RemoveClientEvent(EventType::KeyPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::KeyReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseMoved, this);
}



void PlayerGameObject::Serialize(json& jsonData)
{
	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };


	GameObject::Serialize(jsonData);
	jsonData[std::to_string((int)m_gameObjectType)]["Position"].push_back(position);
	jsonData[std::to_string((int)m_gameObjectType)]["Rotation"].push_back(rotation);
	jsonData[std::to_string((int)m_gameObjectType)]["Scale"].push_back(scale);
	jsonData[std::to_string((int)m_gameObjectType)]["Layers"].push_back(m_health);
	jsonData[std::to_string((int)m_gameObjectType)]["Health"].push_back(m_layer);
	jsonData[std::to_string((int)m_gameObjectType)]["Movement Speed"].push_back(m_moveSpeed);

}

void PlayerGameObject::Deserialize(json& jsonData)
{
}
/// <summary>
/// Handles events from the Observations
/// </summary>
void PlayerGameObject::Handle(Event* e)
{
    CharacterGameObject::Handle(e);

	switch (e->GetEventID())
	{
	case EventType::KeyPressed:
		KeyPressed((KeyPressedEvent*)e);
		break;
	case EventType::KeyReleased:
		KeyReleased((KeyReleasedEvent*)e);
		break;
	case EventType::MouseButtonPressed:
		MouseButtonPressed((MouseButtonPressedEvent*)e);
		break;
	case EventType::MouseButtonReleased:
		MouseButtonReleased((MouseButtonReleasedEvent*)e);
		break;
	case EventType::MouseMoved:
		MouseMoved((MouseMovedEvent*)e);
		break;

	}
}

/// <summary>
/// Handles any keypresses when they are pressed (frame whilst pressed)
/// </summary>
void PlayerGameObject::KeyPressed(KeyPressedEvent* e)
{
	//Can use 'Letter' or the raw keycode for keyboard inputs.
	if (e->GetKeyCode() == 'C')
	{
		//LOG("C");
	}

	if (e->GetKeyCode() == 0x43)
	{

	}

	if (e->GetKeyCode() == 0x44)
	{
		//LOG("D");
	}

	// Player movement
	XMFLOAT3 vector = XMFLOAT3(0, 0, 0);
	if (e->GetKeyCode() == 'W')
	{
		vector.y = 1.0f;
	}
	if (e->GetKeyCode() == 'S')
	{
		vector.y = -1.0f;
	}
	if (e->GetKeyCode() == 'A')
	{
		vector.x = -1.0f;
	}
	if (e->GetKeyCode() == 'D')
	{
		vector.x = 1.0f;
	}
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		float size = sqrt(vector.x * vector.x + vector.y * vector.y);
		vector = MathHelper::Multiply(XMFLOAT3((GetMoveSpeed() * vector.x) / size, (GetMoveSpeed() * vector.y) / size, 0), GameManager::GetInstance()->GetTimer()->DeltaTime());
		GetTransform()->Translate(vector);
	}
}

/// <summary>
/// Handles any keypresses when they are released (first frame).
/// </summary>
void PlayerGameObject::KeyReleased(KeyReleasedEvent* e)
{

}

/// <summary>
/// Handles any mouse button presses when pressed (frame whilst pressed)
/// </summary>
void PlayerGameObject::MouseButtonPressed(MouseButtonPressedEvent* e)
{
	if (e->GetButton() == VK_LBUTTON)
	{

		//LOG("MB1");

	}

	if (e->GetButton() == VK_RBUTTON)
	{

		//LOG("MB2");

	}


}

/// <summary>
/// Handles any mouse button when they are released (first frame).
/// </summary>
void PlayerGameObject::MouseButtonReleased(MouseButtonReleasedEvent* e)
{

}

/// <summary>
/// Handles the mouse moving across the window
/// </summary>
void PlayerGameObject::MouseMoved(MouseMovedEvent* e)
{
	//LOG(e->GetX()); 
	//LOG(e->GetY());
}
