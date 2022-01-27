#pragma once
#include "Events.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"

/// <summary>
/// 
/// Header File contains listeners
/// Methods for listeners may need 
/// 
/// Listeners are being given the information from the event manager and will be told when to execute their code when the event they have "subscribed to" is called
/// 
/// </summary>

class Observer
{
public:

	Observer() {}

	virtual ~Observer() {}

	virtual void Handle(Event* e) = 0;

};

class CollisionObserver : public Observer
{
public:
	void TriggerEnter(TriggerEnterEvent* e)
	{
		// To do
	}

	void TriggerHold(TriggerHoldEvent* e)
	{
		LOG("Trigger occuring!");
	}

	void TriggerExit(TriggerExitEvent* e)
	{
		// To do
	}

	void CollisionEnter(CollisionEnterEvent* e)
	{
		// To do
	}

	void CollisionHold(CollisionHoldEvent* e)
	{
		LOG("Collision occuring!");
	}

	void CollisionExit(CollisionExitEvent* e)
	{
		// To do
	}

	void Handle(Event* e)
	{
		switch (e->GetEventID())
		{
		case EventType::TriggerEnter:
			TriggerEnter((TriggerEnterEvent*)e);
			break;
		case EventType::TriggerHold:
			TriggerHold((TriggerHoldEvent*)e);
			break;
		case EventType::TriggerExit:
			TriggerExit((TriggerExitEvent*)e);
			break;
		case EventType::CollisionEnter:
			CollisionEnter((CollisionEnterEvent*)e);
			break;
		case EventType::CollisionHold:
			CollisionHold((CollisionHoldEvent*)e);
			break;
		case EventType::CollisionExit:
			CollisionExit((CollisionExitEvent*)e);
			break;
		}
	}

	CollisionObserver(int* i) { this->m_i = i; }

private:
	int* m_i;
};

class ExampleObserver : public Observer
{
public:
	void KeyPressed(KeyPressedEvent* e)
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
			vector = MathHelper::Multiply(XMFLOAT3( (m_playerObject->GetMoveSpeed() * vector.x) / size, (m_playerObject->GetMoveSpeed() * vector.y) / size, 0 ),GameManager::GetInstance()->GetTimer()->DeltaTime());
			m_playerObject->GetTransform()->Translate(vector);
		}
	}

	void KeyReleased(KeyReleasedEvent* e)
	{

	}

	void MouseButtonPressed(MouseButtonPressedEvent* e)
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

	void MouseButtonReleased(MouseButtonReleasedEvent* e)
	{

	}

	void MouseMoved(MouseMovedEvent* e)
	{
		//LOG(e->GetX()); 
		//LOG(e->GetY());
	}

	void Handle(Event* e)
	{
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
	
	//pass in object / values you want the observer to have access to for editing e.g m_playerController* controller; this->m_playerController = playerController then the observer makes all changes basedo on the events. 
	//Event processing all happens in 1 place for each object that wants access to the event classes. Controllers can be placed in their own classes if they have access to the headers for the 
	//Observed class & EventObserver.h for access to the Observer based class for inheritance 
	
	ExampleObserver(int* i, PlayerGameObject* player) { this->m_i = i; this->m_playerObject = player; }

private:
	int* m_i;
	PlayerGameObject* m_playerObject;
};