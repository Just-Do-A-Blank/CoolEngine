#include "RangeAttackState.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/EditorUI/EditorUI.h"

RangeAttackState::RangeAttackState(EnemyGameObject* penemy) : FuzzyState()
{
	m_stateType = FuzzyStateType::RANGE_ATTACK;

	m_penemy = penemy;
}

RangeAttackState::RangeAttackState(const nlohmann::json& data) : FuzzyState(data)
{
	m_stateType = FuzzyStateType::RANGE_ATTACK;

	Deserialize(data);
}

RangeAttackState::RangeAttackState(RangeAttackState const* other, EnemyGameObject* penemy) : FuzzyState(other)
{
	m_stateType = FuzzyStateType::RANGE_ATTACK;

	m_penemy = penemy;

	m_attackRangeVariance = other->m_attackRangeVariance;
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

	float attackVarianceSq = MathHelper::RandomNumber(-m_attackRangeVariance * 0.5f, m_attackRangeVariance * 0.5f);
	attackVarianceSq *= attackVarianceSq;

	if (distanceSq < m_pweapon->GetDistanceTravelled() * m_pweapon->GetDistanceTravelled() + attackVarianceSq)
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
	FuzzyState::Serialize(data);

	data["AttackRangeVariance"] = m_attackRangeVariance;
}

void RangeAttackState::Deserialize(const nlohmann::json& data)
{
	FuzzyState::Deserialize(data);

	m_attackRangeVariance = data["AttackRangeVariance"];
}

void RangeAttackState::CreateEngineUI()
{
	FuzzyState::CreateEngineUI();

	ImGui::Spacing();

	EditorUIFloatParameters params;
	params.m_minValue = 0;
	params.m_maxValue = 10000;
	params.m_tooltipText = "Variance around the distance at which the enemy fires the weapon. The higher it is the more likely the enemy will be to fire too early.";

	EditorUI::DragFloat("Attack Range Variance", m_attackRangeVariance, params);
}

void RangeAttackState::Start()
{
	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}