#include "MeleeMovementState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/AI/Pathfinding.h"

MeleeMovementState::MeleeMovementState(EnemyGameObject* penemy) : FuzzyState()
{
	m_stateType = FuzzyStateType::MELEE_MOVEMENT;

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
	m_penemy = penemy;

	if (m_pplayer == nullptr)
	{
		LOG("AI state tried to get the player but couldn't!");
	}
}

void MeleeMovementState::SetEnemy(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

void MeleeMovementState::Enter()
{
	Pathfinding::GetInstance()->FindPath(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition(), m_path);

	m_replanPathTimeStamp = GameManager::GetInstance()->GetTimer()->GameTime();
}

void MeleeMovementState::Exit()
{
	m_path.clear();
}

float MeleeMovementState::CalculateActivation()
{
	if (m_pplayer == nullptr)
	{
		return 0.0f;
	}

	WeaponGameObject* pweapon = m_penemy->GetWeapon();

	if (pweapon != nullptr && pweapon->ContainsType(GameObjectType::MELEE_WEAPON) == true)
	{
		m_pweapon = dynamic_cast<MeleeWeaponGameObject*>(m_penemy->GetWeapon());
	}
	else
	{
		return 0.0f;
	}

	float distanceSq = MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	if (distanceSq > m_activationDistanceSq && distanceSq < m_maxActivationDistanceSq)
	{
		return 1.0f;
	}

	return 0.0f;
}

void MeleeMovementState::Update()
{
	if (m_path.size() == 0 || (GameManager::GetInstance()->GetTimer()->GameTime() - m_replanPathTimeStamp) > m_replanPathTime)
	{
		m_path.clear();

		Pathfinding::GetInstance()->FindPath(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition(), m_path);

		m_replanPathTimeStamp = GameManager::GetInstance()->GetTimer()->GameTime();
	}

	if (m_path.size() != 0)
	{
		m_penemy->CalculateMovement(m_path.back());

		if (MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_path.back()->m_pos) < m_nodePopDistanceSq)
		{
			m_path.pop_back();
		}
	}
}

void MeleeMovementState::Serialize(nlohmann::json& data)
{
	data[(int)m_stateType]["ActivationDistanceSq"] = m_activationDistanceSq;
	data[(int)m_stateType]["NodePopDistanceSq"] = m_nodePopDistanceSq;
}

void MeleeMovementState::Deserialize(const nlohmann::json& data)
{
	m_activationDistanceSq = data[(int)m_stateType]["ActivationDistanceSq"];
	m_nodePopDistanceSq = data[(int)m_stateType]["NodePopDistanceSq"];
}
