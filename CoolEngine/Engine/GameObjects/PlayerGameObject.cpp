#include "PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include <Engine/Managers/Events/EventManager.h>

PlayerGameObject::PlayerGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::PLAYER;

    GameplayButton up = 
    {
        EGAMEPLAYBUTTONCLASS::MoveUp,
        list<int>(),
        list<int>(),
    };
    up.m_keyCodes.push_back('W');
    GameplayButton down =
    {
        EGAMEPLAYBUTTONCLASS::MoveDown,
        list<int>(),
        list<int>(),
    };
    down.m_keyCodes.push_back('S');

    GameplayButton left =
    {
        EGAMEPLAYBUTTONCLASS::MoveLeft,
        list<int>(),
        list<int>(),
    };
    left.m_keyCodes.push_back('A');

    GameplayButton right =
    {
        EGAMEPLAYBUTTONCLASS::MoveRight,
        list<int>(),
        list<int>(),
    };
    right.m_keyCodes.push_back('D');

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
}

PlayerGameObject::PlayerGameObject(const json& data, CoolUUID uuid) : CharacterGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::PLAYER;

	GameplayButton up =
	{
		EGAMEPLAYBUTTONCLASS::MoveUp,
		list<int>(),
		list<int>(),
	};
	up.m_keyCodes.push_back('W');
	GameplayButton down =
	{
		EGAMEPLAYBUTTONCLASS::MoveDown,
		list<int>(),
		list<int>(),
	};
	down.m_keyCodes.push_back('S');

	GameplayButton left =
	{
		EGAMEPLAYBUTTONCLASS::MoveLeft,
		list<int>(),
		list<int>(),
	};
	left.m_keyCodes.push_back('A');

	GameplayButton right =
	{
		EGAMEPLAYBUTTONCLASS::MoveRight,
		list<int>(),
		list<int>(),
	};
	right.m_keyCodes.push_back('D');

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
}

PlayerGameObject::~PlayerGameObject()
{
	EventManager::Instance()->RemoveClientEvent(EventType::KeyPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::KeyReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseMoved, this);

    delete m_playerController;
}



void PlayerGameObject::Serialize(json& jsonData)
{
	CharacterGameObject::Serialize(jsonData);
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

/// <summary>
/// Update loop for the gameobject
/// </summary>
void PlayerGameObject::Update()
{
    m_playerController->Update();
}

#if EDITOR
/// <summary>
/// Shows engine UI
/// </summary>
void PlayerGameObject::CreateEngineUI()
{
    CharacterGameObject::CreateEngineUI();

    m_playerController->CreateEngineUI();
}

#endif
