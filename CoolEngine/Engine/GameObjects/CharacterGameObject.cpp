#include "CharacterGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/Physics/Shape.h"
#include "Engine/Managers/Events/EnemyDeathEvent.h"

CharacterGameObject::CharacterGameObject() : TriggerableGameObject()
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::CharacterGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::CharacterGameObject(const nlohmann::json& data, CoolUUID uuid) : TriggerableGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

CharacterGameObject::CharacterGameObject(CharacterGameObject const& other) : TriggerableGameObject(other)
{
	m_moveSpeed = other.m_moveSpeed;
	m_health = other.m_health;
	m_direction = other.m_direction;
	m_hasWeapon = other.m_hasWeapon;
}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Start()
{
    PrefabGameObject::Start();

	AddWeapon();
}

void CharacterGameObject::AddWeapon()
{
	if (m_isWeaponRanged)
	{
		m_pweapon = GameManager::GetInstance()->CreateGameObject<RangedWeaponGameObject>(m_identifier + "_TestWeapon");
	}
	else
	{
		m_pweapon = GameManager::GetInstance()->CreateGameObject<MeleeWeaponGameObject>(m_identifier + "_TestWeapon");
	}
	m_pweapon->SetAlbedo(TEST2);
	m_pweapon->GetTransform()->SetLocalScale(XMFLOAT3(20, 20, 20));
	m_pweapon->SetLayer(3);
	m_pweapon->GetShape()->SetIsTrigger(false);
	m_pweapon->GetShape()->SetIsCollidable(false);
	m_pweapon->RegisterForEvents();
	m_pweapon->SetDistanceTravelled(500);

	// Sets true if player, false if enemy
	m_pweapon->SetIsPlayerWeapon(ContainsType(GameObjectType::PLAYER));
	//Setting this weapon to be held
	m_pweapon->SetHeld(true);

	m_hasWeapon = true;
}

void CharacterGameObject::Update()
{
	TriggerableGameObject::Update();

	if (m_hasWeapon && !m_pweapon)
	{
		AddWeapon();
	}

	if (m_invincibilityTime > 0.0f)
	{
		m_invincibilityTime -= GameManager::GetInstance()->GetTimer()->DeltaTime();
	}
	else
	{
		m_invincibilityTime = 0;
	}

	// Update weapon position/angle
	if (m_pweapon)
	{
		m_pweapon->SetHolderPosition(XMFLOAT2(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y));
	}
}

void CharacterGameObject::EditorUpdate()
{

}

WeaponGameObject* CharacterGameObject::GetWeapon()
{
	return m_pweapon;
}

void CharacterGameObject::TakeDamage(float damage)
{
	m_health -= damage;
	// To do - Kill at zero health

	if (m_health < 0 && ContainsType(GameObjectType::ENEMY))
	{
		GameManager::GetInstance()->DeleteGameObjectUsingIdentifier(m_identifier);
	}
}

void CharacterGameObject::Serialize(nlohmann::json& jsonData)
{
	TriggerableGameObject::Serialize(jsonData);
    SaveLocalData(jsonData);
}

void CharacterGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    TriggerableGameObject::LoadAllPrefabData(jsonData);
    LoadLocalData(jsonData);
}

void CharacterGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    TriggerableGameObject::SaveAllPrefabData(jsonData);
}

void CharacterGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("Movement Speed"))
    {
        m_moveSpeed = jsonData["Movement Speed"];
        m_health = jsonData["Health"];
    }
}

void CharacterGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["Health"] = m_health;
    jsonData["Movement Speed"] = m_moveSpeed;
}

XMFLOAT3 CharacterGameObject::GetWeaponPosition()
{
    return m_pweapon->GetTransform()->GetWorldPosition();
}

XMFLOAT3 CharacterGameObject::GetWeaponDirection()
{
    // From character to weapon = weapon pos - character pos
    XMFLOAT3 weaponPos = m_pweapon->GetTransform()->GetWorldPosition();
    XMFLOAT3 characterPos = GetTransform()->GetWorldPosition();
    return XMFLOAT3(weaponPos.x - characterPos.x, weaponPos.y - characterPos.y, 0.0f);


}