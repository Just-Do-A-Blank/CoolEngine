#include "RangeAttackState.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"

RangeAttackState::RangeAttackState(EnemyGameObject* penemy)
{
	m_stateType = FuzzyStateType::RANGE_ATTACK;

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
	m_penemy = penemy;

	if (m_pplayer == nullptr)
	{
		LOG("AI state tried to get the player but couldn't!");
	}
}

void RangeAttackState::SetEnemy(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

void RangeAttackState::Enter()
{
}

void RangeAttackState::Exit()
{
}

float RangeAttackState::CalculateActivation()
{
	if (m_pplayer == nullptr)
	{
		return 0.0f;
	}

	WeaponGameObject* pweapon = m_penemy->GetWeapon();

	if (pweapon != nullptr && pweapon->ContainsType(GameObjectType::RANGE_WEAPON) == true)
	{
		m_pweapon = dynamic_cast<RangedWeaponGameObject*>(m_penemy->GetWeapon());
	}
	else
	{
		return 0.0f;
	}

	float distanceSq = MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	if (distanceSq < m_pweapon->GetDistanceTravelled() * m_pweapon->GetDistanceTravelled())
	{
		return 1.0f;
	}

	return 0.0f;
}

void RangeAttackState::Update()
{
	m_pweapon->Attack();
}

void RangeAttackState::Serialize(nlohmann::json& data)
{
}

void RangeAttackState::Deserialize(const nlohmann::json& data)
{
}
