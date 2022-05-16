#include <Engine/Managers/Events/DamageCalculation.h>
#include <Engine/GameObjects/CharacterGameObject.h>
#include <Engine/GameObjects/WeaponGameObject.h>


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

void DamageCalculation::TriggerEnter(TriggerEnterEvent* e)
{
	//LOG("Trigger enter!");
}

void DamageCalculation::TriggerHold(TriggerHoldEvent* e)
{
	if (e->GetGameObject(0)->ContainsType(GameObjectType::CHARACTER) && e->GetGameObject(1)->ContainsType(GameObjectType::WEAPON))
	{
		CharacterGameObject* pCharacter = dynamic_cast<CharacterGameObject*>(e->GetGameObject(0));
		WeaponGameObject* pWeapon = dynamic_cast<WeaponGameObject*>(e->GetGameObject(1));

		if (!pCharacter->GetInvincibilityTime())
		{
			pCharacter->TakeDamage(CalculateDamage(pWeapon->GetDamage(), pWeapon->GetElement(), pCharacter->GetElement(), pCharacter->GetElementalStatus()));
			pCharacter->SetInvincibilityTime(INVINCIBLE_TIME);
		}
	}
}

void DamageCalculation::TriggerExit(TriggerExitEvent* e)
{
	//LOG("Trigger exit!");
}

void DamageCalculation::Handle(Event* e)
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
	}
}