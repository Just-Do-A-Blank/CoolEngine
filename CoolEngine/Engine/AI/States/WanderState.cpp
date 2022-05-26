#include "WanderState.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/EditorUI/EditorUI.h"

WanderState::WanderState(EnemyGameObject* penemy) : FuzzyState(penemy)
{
	m_stateType = FuzzyStateType::WANDER;
}
WanderState::WanderState(const nlohmann::json& data, EnemyGameObject* penemy) : FuzzyState(data, penemy)
{
	m_stateType = FuzzyStateType::WANDER;

	Deserialize(data);
}

WanderState::WanderState(WanderState const* other, EnemyGameObject* penemy) : FuzzyState(other, penemy)
{
	m_stateType = FuzzyStateType::WANDER;

	m_activationDistance = other->m_activationDistance;
	m_nodePopDistance = other->m_nodePopDistance;
	m_wanderDistance = other->m_wanderDistance;
	m_waitTime = other->m_waitTime;
	m_waitTimeVariance = other->m_waitTimeVariance;
}

void WanderState::SetEnemy(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

void WanderState::Enter()
{
	XMFLOAT3 randomPos = XMFLOAT3(MathHelper::RandomNumber(-m_wanderDistance * 0.5f, m_wanderDistance * 0.5f), MathHelper::RandomNumber(-m_wanderDistance * 0.5f, m_wanderDistance * 0.5f), MathHelper::RandomNumber(-m_wanderDistance * 0.5f, m_wanderDistance * 0.5f));

	Pathfinding::GetInstance()->FindPath(m_penemy->GetTransform()->GetWorldPosition(), randomPos, m_path);
}

void WanderState::Exit()
{
	m_path.clear();

	m_waitTimeStamp = -1;
}

float WanderState::CalculateActivation()
{
	if (m_pplayer == nullptr)
	{
		return 0.0f;
	}

	float distanceSq = MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_pplayer->GetTransform()->GetWorldPosition());

	if (distanceSq > m_activationDistance * m_activationDistance)
	{
		return 1.0f;
	}

	return 0.0f;
}

void WanderState::Update()
{
	if (m_path.size() == 0)
	{
		if (GameManager::GetInstance()->GetTimer()->GameTime() - m_waitTimeStamp < m_currentWaitTime)
		{
			m_currentWaitTime -= GameManager::GetInstance()->GetTimer()->DeltaTime();

			return;
		}

		XMFLOAT3 randomPos = XMFLOAT3(MathHelper::RandomNumber(-m_wanderDistance * 0.5f, m_wanderDistance * 0.5f), MathHelper::RandomNumber(-m_wanderDistance * 0.5f, m_wanderDistance * 0.5f), MathHelper::RandomNumber(-m_wanderDistance * 0.5f, m_wanderDistance * 0.5f));

		Pathfinding::GetInstance()->FindPath(m_penemy->GetTransform()->GetWorldPosition(), randomPos, m_path);

		m_currentWaitTime = MathHelper::RandomNumber(m_waitTime - (m_waitTimeVariance * 0.5f), m_waitTime + (m_waitTimeVariance * 0.5f));
	}

	if (m_path.size() != 0)
	{
		m_penemy->CalculateMovement(m_path.back());

		if (MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_path.back()->m_pos) < m_nodePopDistance * m_nodePopDistance)
		{
			m_path.pop_back();
		}

		if (m_path.size() == 0)
		{
			m_waitTimeStamp = GameManager::GetInstance()->GetTimer()->GameTime();
		}
	}
}

void WanderState::Serialize(nlohmann::json& data)
{
	FuzzyState::Serialize(data);

	data["activationDistance"] = m_activationDistance;
	data["NodePopDistance"] = m_nodePopDistance;
	data["WanderDistance"] = m_wanderDistance;
	data["WaitTime"] = m_waitTime;
	data["WaitTimeVariance"] = m_waitTimeVariance;
}

void WanderState::Deserialize(const nlohmann::json& data)
{
	FuzzyState::Deserialize(data);

	m_activationDistance = data["activationDistance"];
	m_nodePopDistance = data["NodePopDistance"];
	m_wanderDistance = data["WanderDistance"];
	m_waitTime = data["WaitTime"];
	m_waitTimeVariance = data["WaitTimeVariance"];
}

void WanderState::CreateEngineUI()
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
	params.m_minValue = 0.1f;
	params.m_maxValue = 100.0f;
	params.m_tooltipText = "The distance at which the enemy must be from the pathfinding node before it's classed as being visited.";

	EditorUI::DragFloat("Node Pop Distance", m_nodePopDistance, params);

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.0f;
	params.m_maxValue = 10000.0f;
	params.m_tooltipText = "The maximum distance in each axis the AI can wander";

	EditorUI::DragFloat("Max Wander Distance", m_wanderDistance, params);

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.0f;
	params.m_maxValue = 100.0f;
	params.m_tooltipText = "The amount of time the AI will wait before wandering to a different position.";

	EditorUI::DragFloat("Wait Time", m_waitTime, params);

	ImGui::Spacing();

	params = EditorUIFloatParameters();
	params.m_minValue = 0.0f;
	params.m_maxValue = 100.0f;
	params.m_tooltipText = "The amount of variance in the time that the AI waits.";

	EditorUI::DragFloat("Wait Time Variance", m_waitTimeVariance, params);
}

void WanderState::Start()
{
	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}