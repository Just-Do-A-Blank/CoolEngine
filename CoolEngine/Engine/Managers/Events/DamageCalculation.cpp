#include "Engine/Managers/Events/DamageCalculation.h"
#include "Engine/GameObjects/CharacterGameObject.h"
#include "Engine/GameObjects/BulletGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"


DamageCalculation::DamageCalculation()
{
	EventManager::Instance()->AddClient(EventType::TriggerEnter, this);
	EventManager::Instance()->AddClient(EventType::TriggerHold, this);
	EventManager::Instance()->AddClient(EventType::TriggerExit, this);
}

DamageCalculation::~DamageCalculation()
{
	EventManager::Instance()->RemoveClientEvent(EventType::TriggerEnter, this);
	EventManager::Instance()->RemoveClientEvent(EventType::TriggerHold, this);
	EventManager::Instance()->RemoveClientEvent(EventType::TriggerExit, this);
}

float DamageCalculation::CalculateDamage(float weaponDamage, ELEMENTS weaponElement, ELEMENTS characterElement, ELEMENTALSTATUSES characterStatus)
{
	float multiplier = 1.0f;
	if ((characterElement == ElementWeakTo[(int)weaponElement] || characterElement == DualElementWeakTo[(int)weaponElement]) && characterElement != ELEMENTS::NONE)
	{
		// Character weak to attack
		multiplier *= 2.0f;
		if ((int)characterElement >= (int)ELEMENTS::POISONFIRE)
		{
			// Dual element character is extra weak
			multiplier *= 2.0f;
		}
	}
	else if ((characterElement == ElementResistantTo[(int)weaponElement] || characterElement == DualElementResistantTo[(int)weaponElement]) && characterElement != ELEMENTS::NONE)
	{
		// Character resistant to attack
		multiplier /= 2.0f;
		if ((int)characterElement >= (int)ELEMENTS::POISONFIRE)
		{
			// Dual element character is extra weak
			multiplier /= 2.0f;
		}
	}

	if ((characterElement == ElementWeakTo[(int)characterStatus] || characterElement == DualElementWeakTo[(int)characterStatus]) && characterElement != ELEMENTS::NONE)
	{
		multiplier *= 2.0f;
	}

	if (multiplier > 4.0f)
		multiplier = 4.0f;

	return (weaponDamage * multiplier);
}

void DamageCalculation::TriggerHold(TriggerHoldEvent* e)
{
#if EDITOR
	if (GameManager::GetInstance()->GetViewState() == ViewState::EDITOR_VIEW)
	{
		return;
	}
#endif

	int bulletID = e->GetGameObject(0)->ContainsType(GameObjectType::BULLET) ? 0 : 1;
	int characterID = e->GetGameObject(0)->ContainsType(GameObjectType::CHARACTER) ? 0 : 1;

	if (e->GetGameObject(bulletID)->ContainsType(GameObjectType::BULLET))
	{
		// Ranged
		BulletGameObject* pBullet = dynamic_cast<BulletGameObject*>(e->GetGameObject(bulletID));

		if (e->GetGameObject(characterID)->ContainsType(GameObjectType::CHARACTER))
		{
			// If character, only hit bullet if not invincible
			CharacterGameObject* pCharacter = dynamic_cast<CharacterGameObject*>(e->GetGameObject(characterID));
			if ((pCharacter->GetInvincibilityTime() <= 0.0f) && ((pCharacter->ContainsType(GameObjectType::PLAYER) && pBullet->GetIsPlayerWeapon() == false) || (pCharacter->ContainsType(GameObjectType::ENEMY) && pBullet->GetIsPlayerWeapon())))
			{
				pCharacter->TakeDamage(CalculateDamage(pBullet->GetDamage(), pBullet->GetElement(), pCharacter->GetElement(), pCharacter->GetElementalStatus()));
				pCharacter->SetInvincibilityTime(INVINCIBLE_TIME);
				pBullet->SetActive(false);
			}
		}
		else
		{
			// If not a character, just delete bullet
			dynamic_cast<BulletGameObject*>(e->GetGameObject(bulletID))->SetActive(false);
		}
	}
	else if (e->GetGameObject(bulletID)->ContainsType(GameObjectType::MELEE_WEAPON))
	{
		// Melee
		MeleeWeaponGameObject* pMelee = dynamic_cast<MeleeWeaponGameObject*>(e->GetGameObject(bulletID));

		if (e->GetGameObject(characterID)->ContainsType(GameObjectType::CHARACTER))
		{
			// If character, only hit melee if not invincible
			CharacterGameObject* pCharacter = dynamic_cast<CharacterGameObject*>(e->GetGameObject(characterID));
			if ((pCharacter->GetInvincibilityTime() <= 0.0f) && ((pCharacter->ContainsType(GameObjectType::PLAYER) && pMelee->GetIsPlayerWeapon() == false) || (pCharacter->ContainsType(GameObjectType::ENEMY) && pMelee->GetIsPlayerWeapon())))
			{
				pCharacter->TakeDamage(CalculateDamage(pMelee->GetDamage(), pMelee->GetElement(), pCharacter->GetElement(), pCharacter->GetElementalStatus()));
				pCharacter->SetInvincibilityTime(INVINCIBLE_TIME);
			}
		}
	}
}

void DamageCalculation::CollisionHold(CollisionHoldEvent* e)
{

}

void DamageCalculation::Handle(Event* e)
{
	switch (e->GetEventID())
	{
	case EventType::TriggerHold:
		TriggerHold((TriggerHoldEvent*)e);
		break;
	case EventType::CollisionHold:
		CollisionHold((CollisionHoldEvent*)e);
		break;
	}
}