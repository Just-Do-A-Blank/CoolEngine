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

PlayerGameObject::~PlayerGameObject()
{
	EventManager::Instance()->RemoveClientEvent(EventType::KeyPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::KeyReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseMoved, this);
}

// Occurs when two objects collide with collision on. Fired on enter.
void PlayerGameObject::OnCollisionEnter(GameObject* obj1, GameObject* obj2)
{
    //LOG("Player has collided Enter");
}

// Occurs when two objects collide with collision on. Fired the frame the two stop colliding.
void PlayerGameObject::OnCollisionExit(GameObject* obj1, GameObject* obj2)
{
    //LOG("Player has collided Exit");
}

// Occurs when two objects collide with collision on. Fired every frame.
void PlayerGameObject::OnCollisionHold(GameObject* obj1, GameObject* obj2)
{
    //LOG("Player has collided Hold");
}

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

void PlayerGameObject::KeyReleased(KeyReleasedEvent* e)
{

}

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

void PlayerGameObject::MouseButtonReleased(MouseButtonReleasedEvent* e)
{

}

void PlayerGameObject::MouseMoved(MouseMovedEvent* e)
{
	//LOG(e->GetX()); 
	//LOG(e->GetY());
}