#include "MeleeAttackState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/EditorUI/EditorUI.h"

MeleeAttackState::MeleeAttackState(EnemyGameObject* penemy) : FuzzyState()
{
	m_stateType = FuzzyStateType::MELEE_ATTACK;

	m_penemy = penemy;
}

MeleeAttackState::MeleeAttackState(const nlohmann::json& data) : FuzzyState(data)
{
	m_stateType = FuzzyStateType::MELEE_ATTACK;

	Deserialize(data);
}

MeleeAttackState::MeleeAttackState(MeleeAttackState const* other, EnemyGameObject* penemy) : FuzzyState(other)
{
	m_stateType = FuzzyStateType::MELEE_ATTACK;

	m_penemy = penemy;

	m_attackRangeVariance = other->m_attackRangeVariance;
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
	if (m_penemy->GetWeapon() == nullptr || m_penemy->GetWeapon()->ContainsType(GameObjectType::MELEE_WEAPON) == false)
	{
		return 0.0f;
	}

	MeleeWeaponGameObject* pmeleeWeapon = (MeleeWeaponGameObject*)m_penemy->GetWeapon();

	float distanceSq = MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	float variance = MathHelper::RandomNumber(0.0f, m_attackRangeVariance) - (m_attackRangeVariance * 0.5f);

	if (distanceSq + variance <= pmeleeWeapon->GetRadius() * pmeleeWeapon->GetRadius())
	{
		return 1.0f;
	}

	return 0.0f;
}

#if EDITOR
void MeleeAttackState::CreateEngineUI()
{
	FuzzyState::CreateEngineUI();

	ImGui::Spacing();

	EditorUIFloatParameters params;
	params.m_minValue = 0;
	params.m_maxValue = 10000;
	params.m_tooltipText = "Variance around the distance at which the enemy swings the weapon. The higher it is the more likely the enemy will be to miss.";

	EditorUI::DragFloat("Attack Range Variance", m_attackRangeVariance, params);
}
#endif

void MeleeAttackState::Update()
{
	//Add code to swing weapon
}

void MeleeAttackState::Serialize(nlohmann::json& data)
{
	FuzzyState::Serialize(data);

	data["AttackRangeVariance"] = m_attackRangeVariance;
}

void MeleeAttackState::Deserialize(const nlohmann::json& data)
{
	FuzzyState::Deserialize(data);

	m_attackRangeVariance = data["AttackRangeVariance"];
}

void MeleeAttackState::Start()
{
	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}
