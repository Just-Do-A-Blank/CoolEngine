#include "PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include <Engine/Managers/Events/EventManager.h>

PlayerGameObject::PlayerGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::PLAYER;

    GameplayButton up = 
    {
        EGAMEPLAYBUTTONCLASS::MoveUp,
        list<int>(57),
    };
    GameplayButton down =
    {
        EGAMEPLAYBUTTONCLASS::MoveDown,
        list<int>(53),
    };
    GameplayButton left =
    {
        EGAMEPLAYBUTTONCLASS::MoveLeft,
        list<int>(41),
    };
    GameplayButton right =
    {
        EGAMEPLAYBUTTONCLASS::MoveRight,
        list<int>(44),
    };
    list< GameplayButton> gameplayButtons;
    gameplayButtons.push_back(up);
    gameplayButtons.push_back(down);
    gameplayButtons.push_back(left);
    gameplayButtons.push_back(right);

    InputsAsGameplayButtons* buttons = new InputsAsGameplayButtons(gameplayButtons);
    m_playerController = new PlayerController(buttons, GetTransform());

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);

    EventManager::Instance()->AddClient(EventType::CollisionEnter, this);
    EventManager::Instance()->AddClient(EventType::CollisionExit, this);
    EventManager::Instance()->AddClient(EventType::CollisionHold, this);
    EventManager::Instance()->AddClient(EventType::TriggerEnter, this);
    EventManager::Instance()->AddClient(EventType::TriggerExit, this);
    EventManager::Instance()->AddClient(EventType::TriggerHold, this);
}

PlayerGameObject::~PlayerGameObject()
{
	EventManager::Instance()->RemoveClientEvent(EventType::KeyPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::KeyReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseMoved, this);

    EventManager::Instance()->RemoveClientEvent(EventType::CollisionEnter, this);
    EventManager::Instance()->RemoveClientEvent(EventType::CollisionExit, this);
    EventManager::Instance()->RemoveClientEvent(EventType::CollisionHold, this);
    EventManager::Instance()->RemoveClientEvent(EventType::TriggerEnter, this);
    EventManager::Instance()->RemoveClientEvent(EventType::TriggerExit, this);
    EventManager::Instance()->RemoveClientEvent(EventType::TriggerHold, this);

    delete m_playerController;
}

/// <summary>
/// Handles events from the Observations
/// </summary>
void PlayerGameObject::Handle(Event* e)
{
    CharacterGameObject::Handle(e);

	switch (e->GetEventID())
	{
    case EventType::CollisionEnter:
        m_playerController->Handle(e);
        break;
    case EventType::CollisionExit:
        m_playerController->Handle(e);
        break;
    case EventType::CollisionHold:
        m_playerController->Handle(e);
        break;
    case EventType::TriggerEnter:
        m_playerController->Handle(e);
        break;
    case EventType::TriggerExit:
        m_playerController->Handle(e);
        break;
    case EventType::TriggerHold:
        m_playerController->Handle(e);
        break;
	case EventType::KeyPressed:
        
		//KeyPressed((KeyPressedEvent*)e);
		break;
	case EventType::KeyReleased:
		//KeyReleased((KeyReleasedEvent*)e);
		break;
	case EventType::MouseButtonPressed:
		//MouseButtonPressed((MouseButtonPressedEvent*)e);
		break;
	case EventType::MouseButtonReleased:
		//MouseButtonReleased((MouseButtonReleasedEvent*)e);
		break;
	case EventType::MouseMoved:
		//MouseMoved((MouseMovedEvent*)e);
		break;

	}
}

