#include "PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/Physics/Shape.h"
#include "Engine/GameObjects/PickupGameObject.h"
#include "Engine/Managers/Events/PickupEvent.h"
#include "Engine/Helpers/Timer.h"

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

	m_myInventory = new Inventory(); // creates the inventory object
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

    delete m_playerController;
	m_playerController = nullptr;

	delete m_resourceManager;
}


void PlayerGameObject::Start()
{
	PrefabGameObject::Start();

	m_resourceManager->Start();

	bool rendered;
	if (m_isWeaponRanged)
	{
		m_pweapon = GameManager::GetInstance()->CreateGameObject<RangedWeaponGameObject>("TestWeapon");
		rendered = true;
	}
	else
	{
		m_pweapon = GameManager::GetInstance()->CreateGameObject<MeleeWeaponGameObject>("TestWeapon");
		rendered = false;
	}
	m_pweapon->SetAlbedo(TEST2);
	m_pweapon->GetTransform()->SetLocalScale(XMFLOAT3(20, 20, 20));
	m_pweapon->SetLayer(3);
	m_pweapon->GetShape()->SetIsTrigger(false);
	m_pweapon->GetShape()->SetIsCollidable(false);
	m_pweapon->SetIsRenderable(rendered);
	m_pweapon->SetIsPlayerWeapon(ContainsType(GameObjectType::PLAYER));
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
}

void PlayerGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    m_playerController->SaveAllPrefabData(jsonData);
    m_resourceManager->SaveData(jsonData);
}

void PlayerGameObject::MouseButtonPressed(MouseButtonPressedEvent* e)
{
	// Melee attack
	if ((!m_isWeaponRanged) && (!m_isSwingingMelee))
	{
		if (m_pweapon == nullptr)
		{
			return;
		}

		// Can begin attack
		XMFLOAT2 playerPosWorld = XMFLOAT2(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y);
		XMFLOAT2 toWeapon = MathHelper::Minus(GameManager::GetInstance()->GetCamera()->GetMousePositionInWorldSpace(), playerPosWorld);
		toWeapon = MathHelper::Normalize(toWeapon);

		// Starting angle is offset from mouse direction (in degrees)
		m_currentSwingAngle = MathHelper::DotProduct(toWeapon, XMFLOAT2(1, 0));
		m_currentSwingAngle = (std::acosf(m_currentSwingAngle) * 180.0f) / XM_PI;
		if (toWeapon.y < 0)
		{
			m_currentSwingAngle *= -1;
		}
		m_currentSwingAngle -= (dynamic_cast<MeleeWeaponGameObject*>(m_pweapon)->GetSwingAngle() / 2);

		// Used to increase angle
		m_swingTime = dynamic_cast<MeleeWeaponGameObject*>(m_pweapon)->GetSwingTime();
		m_swingSpeed = dynamic_cast<MeleeWeaponGameObject*>(m_pweapon)->GetSwingAngle() / m_swingTime;

		m_isSwingingMelee = true;
		bool rendered = true;
		m_pweapon->SetIsRenderable(rendered);
		m_pweapon->GetShape()->SetIsTrigger(true);
	}
}

void PlayerGameObject::SetWeaponPosition(XMFLOAT2 toWeapon, float weaponRadius)
{
	if (m_pweapon == nullptr)
	{
		return;
	}

	XMFLOAT2 playerPosWorld = XMFLOAT2(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y);
	XMFLOAT2 weaponPosition = MathHelper::Multiply(toWeapon, weaponRadius);
	weaponPosition = MathHelper::Plus(playerPosWorld, weaponPosition);

	float angle = MathHelper::DotProduct(toWeapon, XMFLOAT2(0, 1));
	angle = (std::acosf(angle) * 180.0f) / XM_PI;

	if (toWeapon.x > 0.0f)
	{
		angle *= -1.0f;
	}

	m_pweapon->GetTransform()->SetWorldPosition(XMFLOAT3(weaponPosition.x, weaponPosition.y, 0.0f));
	m_pweapon->GetTransform()->SetWorldRotation(XMFLOAT3(0, 0, angle));
}

void PlayerGameObject::OnTriggerHold(GameObject* obj1, GameObject* obj2)
{
	if ((obj1->ContainsType(GameObjectType::PLAYER)) && (obj2->ContainsType(GameObjectType::PICKUP)))
	{
		PickupGameObject* pickup = dynamic_cast<PickupGameObject*>(obj2);


		//Send event with the data for the player or anyone else that wants to know about to listen to. The pickup data related is also added when cast back to the event
		EventManager::Instance()->AddEvent(new PickupEvent(pickup->GetConsumableData()));
		//This object should be removed from the scene after firing this event

		pickup->SetToBeDeleted(true);
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
	case EventType::Pickup://///////////////////////////////////////////////////////////////
		PickupEvent* test = (PickupEvent*)e;
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

	if (m_invincibilityTime > 0.0f)
	{
		m_invincibilityTime -= GameManager::GetInstance()->GetTimer()->DeltaTime();
	}
	else
	{
		m_invincibilityTime = 0;
	}

	if (m_isWeaponRanged)
	{
		XMFLOAT2 playerPosWorld = XMFLOAT2(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y);
		XMFLOAT2 toWeapon = MathHelper::Minus(GameManager::GetInstance()->GetCamera()->GetMousePositionInWorldSpace(), playerPosWorld);
		toWeapon = MathHelper::Normalize(toWeapon);

		SetWeaponPosition(toWeapon, 50.0f);
	}
	else if (m_isSwingingMelee && m_swingTime > 0.0f)
	{
		XMFLOAT2 toWeapon = XMFLOAT2(std::cosf(XM_PI * m_currentSwingAngle / 180.0f), std::sinf(XM_PI * m_currentSwingAngle / 180.0f));

		SetWeaponPosition(toWeapon, dynamic_cast<MeleeWeaponGameObject*>(m_pweapon)->GetRadius());

		float dTime = GameManager::GetInstance()->GetTimer()->DeltaTime();
		m_currentSwingAngle += (m_swingSpeed * dTime);
		m_swingTime -= dTime;
		if (m_swingTime <= 0.0f)
		{
			m_isSwingingMelee = false;
			bool rendered = false;
			m_pweapon->SetIsRenderable(rendered);
			m_pweapon->GetShape()->SetIsTrigger(false);
		}
	}
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
