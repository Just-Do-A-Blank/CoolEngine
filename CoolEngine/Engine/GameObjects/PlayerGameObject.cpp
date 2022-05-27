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




	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
	EventManager::Instance()->AddClient(EventType::MouseMoved, this);

	m_gameObjectType |= GameObjectType::PLAYER;

	//GameManager* pgameManager = GameManager::GetInstance();
	//pgameManager->CreateGameObject<CameraGameObject>("Camera"); //  use - GameManager::GetInstance()->GetCamera(); - to set camera to editor camera
	//GameManager::GetInstance()->SetCamera(m_cameraRef);
    m_resourceManager = new PlayerResourceManager();
	m_pInventory = new Inventory();
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

    m_resourceManager = new PlayerResourceManager(*other.m_resourceManager);
	m_pInventory = new Inventory(*other.m_pInventory);
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

	//Need to init the inventory after all other objects have been inited. This loads all the objects into the inventory
	EventManager::Instance()->AddClient(EventType::Pickup, this);
	m_pInventory->Start();
	
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

void PlayerGameObject::UseResource(unordered_set<PickupResource*> consumable)
{

	unordered_set<PickupResource*>::iterator it;
	for (it = consumable.begin(); it != consumable.end(); it++)
	{
		m_resourceManager->GiveResource((*it)->key, (*it)->strength);
	}
}

void PlayerGameObject::MouseButtonPressed(MouseButtonPressedEvent* e)
{
	switch (e->GetButton())
	{
	case VK_LBUTTON:
		if (m_pweapon != nullptr)
		{
			m_pweapon->Attack();
		}
		break;
	}
}

void PlayerGameObject::OnTriggerHold(GameObject* obj1, GameObject* obj2)
{
	//If the player has interacted with the object
	if (obj1->ContainsType(GameObjectType::PLAYER))
	{
		//If ths object is a pickup object
		if ((obj2->ContainsType(GameObjectType::PICKUP)))
		{
			PickupGameObject* pickup = dynamic_cast<PickupGameObject*>(obj2);
			if (pickup->GetEnabled())
			{
				//if this item is a consumable, dont add it to the event list and afftect the player's resource by the same amount. Can be changed if this is wanted to be affected elsewhere
				if (pickup->GetConsumeOnPickup())
				{
					UseResource(pickup->GetConsumableData());
				}
				else
				{
					EventManager::Instance()->AddEvent(new PickupEvent(obj2));
					pickup->SetEnabled(false);
				}

			}



		}
		//if the object is a weapon
		else if ((obj2->ContainsType(GameObjectType::WEAPON))) 
		{
			WeaponGameObject* weapon = dynamic_cast<WeaponGameObject*>(obj2);
			//If this weapon is not currently being held by someone
			if (!weapon->GetHeld())
			{
				if (weapon->GetEnabled())
				{
					//Putting weapon into inventory if we have a weapon
					if (m_pweapon != nullptr)
					{
						//Pick it up
						EventManager::Instance()->AddEvent(new PickupEvent(obj2));
						obj2->SetEnabled(false);
					}
					//The player doesnt have a weapon currently (its nullptr)
					else
					{
						//Equip it
						m_pweapon = weapon;
						m_pweapon->SetHeld(true);
					}


				}
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
		MouseButtonPressed((MouseButtonPressedEvent*)e);
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

	AudioManager::GetInstance()->SetListenerPosition(m_transform->GetWorldPosition());

	XMFLOAT2 pos = XMFLOAT2(m_transform->GetWorldPosition().x, m_transform->GetWorldPosition().y);

    GameManager* gamemanager = GameManager::GetInstance();
	XMFLOAT2 toWeapon = MathHelper::Minus(gamemanager->GetCamera()->GetMousePositionInWorldSpace(), pos);
	toWeapon = MathHelper::Normalize(toWeapon);

	m_pweapon->SetWeaponPosition(toWeapon);

    float timeDelta = gamemanager->GetTimer()->DeltaTime();
    m_resourceManager->Update(timeDelta);
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
	AudioManager::GetInstance()->SetListenerPosition(m_transform->GetWorldPosition());
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

/// <summary>
/// Gets the inventory on the player.
/// </summary>
/// <returns>The player's inventory</returns>
Inventory* PlayerGameObject::GetInventory()
{
    return m_pInventory;
}