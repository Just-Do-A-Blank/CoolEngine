#include "PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include <Engine/Managers/Events/EventManager.h>
#include <Engine/EditorUI/EditorUI.h>

PlayerGameObject::PlayerGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::PLAYER;

    GameplayButton up = 
    {
        EGAMEPLAYBUTTONCLASS::MoveUp,
        list<EVIRTUALKEYCODE>(),
        list<EVIRTUALKEYCODE>(),
    };
    up.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_W);
    GameplayButton down =
    {
        EGAMEPLAYBUTTONCLASS::MoveDown,
        list<EVIRTUALKEYCODE>(),
        list<EVIRTUALKEYCODE>(),
    };
    down.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_S);

    GameplayButton left =
    {
        EGAMEPLAYBUTTONCLASS::MoveLeft,
        list<EVIRTUALKEYCODE>(),
        list<EVIRTUALKEYCODE>(),
    };
    left.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_A);

    GameplayButton right =
    {
        EGAMEPLAYBUTTONCLASS::MoveRight,
        list<EVIRTUALKEYCODE>(),
        list<EVIRTUALKEYCODE>(),
    };
    right.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_D);

	GameplayButton dash =
	{
		EGAMEPLAYBUTTONCLASS::Dodge,
		list<EVIRTUALKEYCODE>(),
		list<EVIRTUALKEYCODE>(),
	};
	dash.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_SHIFT);

    GameplayButton roll =
    {
        EGAMEPLAYBUTTONCLASS::Roll,
        list<EVIRTUALKEYCODE>(),
        list<EVIRTUALKEYCODE>(),
    };
    roll.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_CTRL);

    list< GameplayButton> gameplayButtons;
    gameplayButtons.push_back(up);
    gameplayButtons.push_back(down);
    gameplayButtons.push_back(left);
    gameplayButtons.push_back(right);
    gameplayButtons.push_back(dash);
    gameplayButtons.push_back(roll);

    InputsAsGameplayButtons* buttons = new InputsAsGameplayButtons(gameplayButtons);
    m_playerController = new PlayerController(buttons, this);

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);
}

PlayerGameObject::PlayerGameObject(const nlohmann::json& data, CoolUUID uuid) : CharacterGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::PLAYER;

	GameplayButton up =
	{
		EGAMEPLAYBUTTONCLASS::MoveUp,
		list<EVIRTUALKEYCODE>(),
		list<EVIRTUALKEYCODE>(),
	};
	up.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_W);
	GameplayButton down =
	{
		EGAMEPLAYBUTTONCLASS::MoveDown,
		list<EVIRTUALKEYCODE>(),
		list<EVIRTUALKEYCODE>(),
	};
	down.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_S);

	GameplayButton left =
	{
		EGAMEPLAYBUTTONCLASS::MoveLeft,
		list<EVIRTUALKEYCODE>(),
		list<EVIRTUALKEYCODE>(),
	};
	left.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_A);

	GameplayButton right =
	{
		EGAMEPLAYBUTTONCLASS::MoveRight,
		list<EVIRTUALKEYCODE>(),
		list<EVIRTUALKEYCODE>(),
	};
	right.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_D);

	GameplayButton dodge =
	{
		EGAMEPLAYBUTTONCLASS::Dodge,
		list<EVIRTUALKEYCODE>(),
		list<EVIRTUALKEYCODE>(),
	};
	dodge.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_SHIFT); //VK_LSHIFT

	list< GameplayButton> gameplayButtons;
	gameplayButtons.push_back(up);
	gameplayButtons.push_back(down);
	gameplayButtons.push_back(left);
	gameplayButtons.push_back(right);
	gameplayButtons.push_back(dodge);

	InputsAsGameplayButtons* buttons = new InputsAsGameplayButtons(gameplayButtons);
	m_playerController = new PlayerController(buttons, this);

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseMoved, this);

    m_playerController->LoadPrefabData(PrefabGameObject::GetDataLoadedAtCreation());
}

PlayerGameObject::PlayerGameObject(PlayerGameObject const& other) : CharacterGameObject(other)
{
	m_playerController = new PlayerController(*other.m_playerController, this);

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
	m_playerController = nullptr;
}



void PlayerGameObject::Serialize(nlohmann::json& jsonData)
{
	CharacterGameObject::Serialize(jsonData);
}

void PlayerGameObject::LoadPrefabData(const nlohmann::json& jsonData)
{
    CharacterGameObject::LoadPrefabData(jsonData);
    m_playerController->LoadPrefabData(jsonData);
}

void PlayerGameObject::SavePrefabData(nlohmann::json& jsonData)
{
    CharacterGameObject::SavePrefabData(jsonData);
    m_playerController->SavePrefabData(jsonData);
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

	if (m_invincibilityTime > 0.0f)
	{
		m_invincibilityTime -= GameManager::GetInstance()->GetTimer()->DeltaTime();
	}
	else
	{
		m_invincibilityTime = 0;
	}
}

void PlayerGameObject::EditorUpdate()
{
}

#if EDITOR
/// <summary>
/// Shows engine UI
/// </summary>
void PlayerGameObject::CreateEngineUI()
{
    CharacterGameObject::CreateEngineUI();

	if (EditorUI::CollapsingSection("Player", true))
	{
		m_playerController->CreateEngineUI();
	}
}

#endif