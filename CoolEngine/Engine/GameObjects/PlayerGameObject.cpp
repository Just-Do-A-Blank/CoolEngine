#include "PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/Physics/Shape.h"
#include "Engine/GameObjects/PickupGameObject.h"
#include "Engine/Managers/Events/PickupEvent.h"


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

	m_resourceManager = new PlayerResourceManager();

	m_pInventory = new Inventory();

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);
	EventManager::Instance()->AddClient(EventType::Pickup, this);
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
	dodge.m_keyCodes.push_back(EVIRTUALKEYCODE::KC_SHIFT);

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
	gameplayButtons.push_back(dodge);
	gameplayButtons.push_back(roll);

	InputsAsGameplayButtons* buttons = new InputsAsGameplayButtons(gameplayButtons);
	m_playerController = new PlayerController(buttons, this);


	m_pInventory = new Inventory();

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseMoved, this);
	EventManager::Instance()->AddClient(EventType::Pickup, this);

	m_gameObjectType |= GameObjectType::PLAYER;

	//GameManager* pgameManager = GameManager::GetInstance();
	//pgameManager->CreateGameObject<CameraGameObject>("Camera"); //  use - GameManager::GetInstance()->GetCamera(); - to set camera to editor camera
	//GameManager::GetInstance()->SetCamera(m_cameraRef);
    m_resourceManager = new PlayerResourceManager();
    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

PlayerGameObject::PlayerGameObject(PlayerGameObject const& other) : CharacterGameObject(other)
{
	m_playerController = new PlayerController(*other.m_playerController, this);

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseMoved, this);
	EventManager::Instance()->AddClient(EventType::Pickup, this);

	m_pInventory = new Inventory(*other.m_pInventory);
    m_resourceManager = new PlayerResourceManager(*other.m_resourceManager);
}

PlayerGameObject::~PlayerGameObject()
{
	EventManager::Instance()->RemoveClientEvent(EventType::KeyPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::KeyReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseMoved, this);
	EventManager::Instance()->RemoveClientEvent(EventType::Pickup, this);

	if (m_pweapon != nullptr)
	{
		m_pweapon->UnregisterForEvents();
	}

    delete m_playerController;
	m_playerController = nullptr;

	delete m_resourceManager;

	delete m_pInventory;
	m_pInventory = nullptr;
}


void PlayerGameObject::Start()
{
	CharacterGameObject::Start();

	m_resourceManager->Start();
}


void PlayerGameObject::Serialize(nlohmann::json& jsonData)
{
	CharacterGameObject::Serialize(jsonData);
    SaveLocalData(jsonData);
}

void PlayerGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    CharacterGameObject::LoadAllPrefabData(jsonData);
    LoadLocalData(jsonData);
}

void PlayerGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    CharacterGameObject::SaveAllPrefabData(jsonData);
}

void PlayerGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    m_playerController->LoadAllPrefabData(jsonData);
	m_resourceManager->LoadData(jsonData);
	m_pInventory->LoadData(jsonData);
}

void PlayerGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    m_playerController->SaveAllPrefabData(jsonData);
    m_resourceManager->SaveData(jsonData);
	m_pInventory->SaveData(jsonData);
}

void PlayerGameObject::OnTriggerHold(GameObject* obj1, GameObject* obj2)
{
	//If the player has interacted with the object
	if (obj1->ContainsType(GameObjectType::PLAYER))
	{
		//If ths object is a pickup object
		if ((obj2->ContainsType(GameObjectType::PICKUP)))
		{
			EventManager::Instance()->AddEvent(new PickupEvent(obj2));
			obj2->SetEnabled(false);

		}
		//if the object is a weapon
		else if ((obj2->ContainsType(GameObjectType::WEAPON))) 
		{
			WeaponGameObject* weapon = dynamic_cast<WeaponGameObject*>(obj2);
			//If this weapon is not currently being held by someone
			if (!weapon->GetHeld())
			{
				//Pick it up
				EventManager::Instance()->AddEvent(new PickupEvent(obj2));
				obj2->SetEnabled(false);
			}
		}

	}
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
	case EventType::Pickup:	
		PickupEvent* pickupEvent = (PickupEvent*)e;
		m_pInventory->AddItemToInventory((pickupEvent->GetConsumableData()));
		break;
	}
}

/// <summary>
/// Update loop for the gameobject
/// </summary>
void PlayerGameObject::Update()
{
	CharacterGameObject::Update();

    m_playerController->Update();
}

void PlayerGameObject::TakeDamage(float damage)
{
    CharacterGameObject::TakeDamage(damage);

	m_resourceManager->TakeWeaponDamage(damage);
	m_resourceManager->CheckForPlayerDeath();
}

void PlayerGameObject::RunPlayerDeadSequence()
{
    LOG("PLAYER IS DEAD");
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
		m_resourceManager->CreateEngineUI();

		m_playerController->CreateEngineUI();
	}
}

#endif

/// <summary>
/// Gets the player resource (such as health)
/// </summary>
/// <returns>The resource manager</returns>
PlayerResourceManager* PlayerGameObject::GetPlayerResources()
{
	return m_resourceManager;
}
