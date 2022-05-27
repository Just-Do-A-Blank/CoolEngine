#include "MeleeMovementState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/AI/Pathfinding.h"
#include "Engine/EditorUI/EditorUI.h"

MeleeMovementState::MeleeMovementState(EnemyGameObject* penemy) : FuzzyState(penemy)
{
	m_stateType = FuzzyStateType::MELEE_MOVEMENT;
}

void MeleeMovementState::SetEnemy(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

MeleeMovementState::MeleeMovementState(const nlohmann::json& data, EnemyGameObject* penemy) : FuzzyState(data, penemy)
{
	m_stateType = FuzzyStateType::MELEE_MOVEMENT;

	Deserialize(data);
}

MeleeMovementState::MeleeMovementState(MeleeMovementState const* other, EnemyGameObject* penemy) : FuzzyState(other, penemy)
{
	m_stateType = FuzzyStateType::MELEE_MOVEMENT;

	m_activationDistance = other->m_activationDistance;
	m_maxActivationDistance = other->m_maxActivationDistance;
	m_nodePopDistance = other->m_nodePopDistance;
	m_replanPathTime = other->m_replanPathTime;
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

	if (distanceSq > m_activationDistance * m_activationDistance && distanceSq < m_maxActivationDistance * m_maxActivationDistance)
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

		if (MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_path.back()->m_pos) < m_nodePopDistance * m_nodePopDistance)
		{
			m_path.pop_back();
		}
	}
}

void MeleeMovementState::Serialize(nlohmann::json& data)
{
	FuzzyState::Serialize(data);

	data["ActivationDistance"] = m_activationDistance;
	data["NodePopDistance"] = m_nodePopDistance;
	data["MaxActivationDistance"] = m_maxActivationDistance;
	data["ReplanPathTime"] = m_replanPathTime;
}

void MeleeMovementState::Deserialize(const nlohmann::json& data)
{
	FuzzyState::Deserialize(data);

	m_activationDistance = data["ActivationDistance"];
	m_nodePopDistance = data["NodePopDistance"];
	m_maxActivationDistance = data["MaxActivationDistance"];
	m_replanPathTime = data["ReplanPathTime"];
}

#if EDITOR
void MeleeMovementState::CreateEngineUI()
{
	FuzzyState::CreateEngineUI();

	ImGui::Spacing();

	EditorUIFloatParameters params;
	params.m_minValue = 0;
	params.m_maxValue = 10000;
	params.m_tooltipText = "The distance at which this state will activate";

	EditorUI::DragFloat("Activation Distance", m_activationDistance, params);

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.0f;
	params.m_maxValue = 10000.0f;
	params.m_tooltipText = "The maximum distance the player can be from the enemy for this state to activate.";

	EditorUI::DragFloat("Max Activation Distance", m_maxActivationDistance);

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.1f;
	params.m_maxValue = 100.0f;
	params.m_tooltipText = "The distance at which the enemy must be from the pathfinding node before it's classed as being visited.";

	EditorUI::DragFloat("Node Pop Distance", m_nodePopDistance, params);

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.0f;
	params.m_maxValue = 100.0f;
	params.m_tooltipText = "The time spent between replanning the path to the player in seconds.";

	EditorUI::DragFloat("Path Replan Time", m_replanPathTime, params);
}
#endif

void MeleeMovementState::Start()
{
	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}