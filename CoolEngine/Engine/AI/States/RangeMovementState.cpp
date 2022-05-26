#include "RangeMovementState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/EditorUI/EditorUI.h"

RangeMovementState::RangeMovementState(EnemyGameObject* penemy) : FuzzyState()
{
	m_stateType = FuzzyStateType::RANGE_MOVEMENT;

	m_penemy = penemy;
}

RangeMovementState::RangeMovementState(const nlohmann::json& data) : FuzzyState(data)
{
	m_stateType = FuzzyStateType::RANGE_MOVEMENT;

	Deserialize(data);
}

RangeMovementState::RangeMovementState(RangeMovementState const* other, EnemyGameObject* penemy) : FuzzyState(other)
{
	m_stateType = FuzzyStateType::RANGE_MOVEMENT;

	m_penemy = penemy;

	m_maxActivationDistance = other->m_maxActivationDistance;
	m_nodePopDistance = other->m_nodePopDistance;
	m_replanPathTime = other->m_replanPathTime;
	m_upperOptimalDistanceMultiplier = other->m_upperOptimalDistanceMultiplier;
	m_lowerOptimalDistanceMultiplier = other->m_lowerOptimalDistanceMultiplier;
}

void RangeMovementState::SetEnemy(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

void RangeMovementState::Enter()
{
	Pathfinding::GetInstance()->FindPath(m_penemy->GetTransform()->GetWorldPosition(), CalculateTargetPosition(), m_path);

	m_replanPathTimeStamp = GameManager::GetInstance()->GetTimer()->GameTime();
}

void RangeMovementState::Exit()
{
	m_path.clear();
}

float RangeMovementState::CalculateActivation()
{
	if (m_pplayer == nullptr)
	{
		return 0.0f;
	}

	WeaponGameObject* pweapon = m_penemy->GetWeapon();

	if (pweapon != nullptr && pweapon->ContainsType(GameObjectType::RANGE_WEAPON) == true)
	{
		m_prangeWeapon = dynamic_cast<RangedWeaponGameObject*>(m_penemy->GetWeapon());
	}
	else
	{
		return 0.0f;
	}

	float distanceSq = MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	float upperDistance = m_prangeWeapon->GetDistanceTravelled() * m_prangeWeapon->GetDistanceTravelled() * m_upperOptimalDistanceMultiplier * m_upperOptimalDistanceMultiplier;
	float lowerDistance = m_prangeWeapon->GetDistanceTravelled() * m_prangeWeapon->GetDistanceTravelled() * m_lowerOptimalDistanceMultiplier * m_lowerOptimalDistanceMultiplier;

	if ((distanceSq > upperDistance || distanceSq < lowerDistance) && distanceSq < m_maxActivationDistance * m_maxActivationDistance)
	{
		return 1.0f;
	}

	return 0.0f;
}

void RangeMovementState::Update()
{
	if (m_path.size() == 0 || (GameManager::GetInstance()->GetTimer()->GameTime() - m_replanPathTimeStamp) > m_replanPathTime)
	{
		m_path.clear();

		Pathfinding::GetInstance()->FindPath(m_penemy->GetTransform()->GetWorldPosition(), CalculateTargetPosition(), m_path);

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

void RangeMovementState::Serialize(nlohmann::json& data)
{
	FuzzyState::Serialize(data);

	data["MaxActivationDistance"] = m_maxActivationDistance;
	data["NodePopDistance"] = m_nodePopDistance;
	data["ReplanPathTime"] = m_replanPathTime;
	data["UpperOptimalDistance"] = m_upperOptimalDistanceMultiplier;
	data["LowerOptimalDistance"] = m_lowerOptimalDistanceMultiplier;
}

void RangeMovementState::Deserialize(const nlohmann::json& data)
{
	FuzzyState::Deserialize(data);

	m_maxActivationDistance = data["MaxActivationDistance"];
	m_nodePopDistance = data["NodePopDistance"];
	m_replanPathTime = data["ReplanPathTime"];
	m_upperOptimalDistanceMultiplier = data["UpperOptimalDistance"];
	m_lowerOptimalDistanceMultiplier = data["LowerOptimalDistance"];
}

void RangeMovementState::CreateEngineUI()
{
	FuzzyState::CreateEngineUI();

	ImGui::Spacing();

	EditorUIFloatParameters params;
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

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.0f;
	params.m_maxValue = 1.0f;
	params.m_speed = 0.01f;
	params.m_tooltipText = "The percentage of the range of the range weapon currently being held at which the enemy will run away from the player.";

	EditorUI::DragFloat("Minimum Activation Multiplier", m_lowerOptimalDistanceMultiplier, params);

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.0f;
	params.m_maxValue = 1.0f;
	params.m_speed = 0.01f;
	params.m_tooltipText = "The percentage of the range of the range weapon currently being held at which the enemy will run towards the player.";

	EditorUI::DragFloat("Maximum Activation Multiplier", m_upperOptimalDistanceMultiplier, params);
}

XMFLOAT3 RangeMovementState::CalculateTargetPosition() const
{
	XMFLOAT3 targetPos = MathHelper::Minus(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	if (MathHelper::SquareMagnitude(targetPos) == 0.0f)
	{
		return m_pplayer->GetTransform()->GetWorldPosition();
	}

	float optimalDistanceMultiplier = MathHelper::RandomNumber(m_lowerOptimalDistanceMultiplier, m_upperOptimalDistanceMultiplier);

	targetPos = MathHelper::Normalize(targetPos);
	targetPos = MathHelper::Multiply(targetPos, m_prangeWeapon->GetDistanceTravelled() * optimalDistanceMultiplier);

	targetPos = MathHelper::Plus(m_pplayer->GetTransform()->GetWorldPosition(), targetPos);

	return targetPos;
}

void RangeMovementState::Start()
{
	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}