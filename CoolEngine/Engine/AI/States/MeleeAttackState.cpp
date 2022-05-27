#include "MeleeAttackState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/EditorUI/EditorUI.h"

MeleeAttackState::MeleeAttackState(EnemyGameObject* penemy) : FuzzyState(penemy)
{
	m_stateType = FuzzyStateType::MELEE_ATTACK;
}

MeleeAttackState::MeleeAttackState(const nlohmann::json& data, EnemyGameObject* penemy) : FuzzyState(data, penemy)
{
	m_stateType = FuzzyStateType::MELEE_ATTACK;

	Deserialize(data);
}

MeleeAttackState::MeleeAttackState(MeleeAttackState const* other, EnemyGameObject* penemy) : FuzzyState(other, penemy)
{
	m_stateType = FuzzyStateType::MELEE_ATTACK;

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
	if (!m_penemy || m_penemy->GetWeapon() == nullptr || m_penemy->GetWeapon()->ContainsType(GameObjectType::MELEE_WEAPON) == false)
	{
		return 0.0f;
	}
	else
	{
		m_pweapon = (MeleeWeaponGameObject*)m_penemy->GetWeapon();
	}

	MeleeWeaponGameObject* pmeleeWeapon = (MeleeWeaponGameObject*)m_penemy->GetWeapon();

	float distanceSq = MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	float varianceSq = MathHelper::RandomNumber(0.0f, m_attackRangeVariance) - (m_attackRangeVariance * 0.5f);
	varianceSq *= std::abs(varianceSq);

	if (distanceSq + varianceSq <= pmeleeWeapon->GetRadius() * pmeleeWeapon->GetRadius())
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
	if (m_attackedLastFrame == true && m_pweapon->GetIsSwinging() == false)
	{
		XMFLOAT2 enemyPos = XMFLOAT2(m_penemy->GetTransform()->GetWorldPosition().x, m_penemy->GetTransform()->GetWorldPosition().y);
		XMFLOAT2 playerPos = XMFLOAT2(m_pplayer->GetTransform()->GetWorldPosition().x, m_pplayer->GetTransform()->GetWorldPosition().y);

		XMFLOAT2 toWeapon = MathHelper::Minus(playerPos, enemyPos);

		if (MathHelper::SquareMagnitude(toWeapon) != 0)
		{
			toWeapon = MathHelper::Normalize(toWeapon);

			m_pweapon->SetWeaponPosition(toWeapon);
		}
	}

	m_pweapon->Attack();

	m_attackedLastFrame = m_pweapon->GetIsSwinging();
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
