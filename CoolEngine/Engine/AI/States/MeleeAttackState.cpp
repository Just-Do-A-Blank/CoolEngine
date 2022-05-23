#include "MeleeAttackState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"

MeleeAttackState::MeleeAttackState(EnemyGameObject* penemy)
{
	m_stateType = FuzzyStateType::MELEE_ATTACK;

	m_penemy = penemy;

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}

void MeleeAttackState::SetEnemy(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

void MeleeAttackState::Enter()
{
}

void MeleeAttackState::Exit()
{
}

float MeleeAttackState::CalculateActivation()
{
	//if (m_penemy->GetWeapon() == nullptr || m_penemy->GetWeapon()->ContainsType(GameObjectType::MELEE_WEAPON) == false)
	//{
	//	return 0.0f;
	//}

	//MeleeWeaponGameObject* pmeleeWeapon = (MeleeWeaponGameObject*)m_penemy->GetWeapon();

	float distanceSq = MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	float variance = MathHelper::RandomNumber(0.0f, m_attackRangeVariance) - (m_attackRangeVariance * 0.5f);

	//if (distanceSq + variance <= pmeleeWeapon->GetRadius() * pmeleeWeapon->GetRadius())
	if (distanceSq + variance <= 400.0f)
	{
		return 1.0f;
	}

	return 0.0f;
}

void MeleeAttackState::Update()
{
	//Add code to swing weapon
}

void MeleeAttackState::Serialize(nlohmann::json& data)
{
}

void MeleeAttackState::Deserialize(const nlohmann::json& data)
{
}
