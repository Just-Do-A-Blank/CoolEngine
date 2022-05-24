#include "WanderState.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/GameManager.h"

WanderState::WanderState(EnemyGameObject* penemy) : FuzzyState()
{
	m_stateType = FuzzyStateType::WANDER;

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));

	m_penemy = penemy;
}

void WanderState::SetEnemy(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

void WanderState::Enter()
{
	XMFLOAT3 randomPos = XMFLOAT3(MathHelper::RandomNumber(-m_wanderHalfRange, m_wanderHalfRange), MathHelper::RandomNumber(-m_wanderHalfRange, m_wanderHalfRange), MathHelper::RandomNumber(-m_wanderHalfRange, m_wanderHalfRange));

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

	if (distanceSq > m_activationDistanceSq)
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

		XMFLOAT3 randomPos = XMFLOAT3(MathHelper::RandomNumber(-m_wanderHalfRange, m_wanderHalfRange), MathHelper::RandomNumber(-m_wanderHalfRange, m_wanderHalfRange), MathHelper::RandomNumber(-m_wanderHalfRange, m_wanderHalfRange));

		Pathfinding::GetInstance()->FindPath(m_penemy->GetTransform()->GetWorldPosition(), randomPos, m_path);

		m_currentWaitTime = MathHelper::RandomNumber(m_waitTime - m_waitTimeHalfRange, m_waitTime + m_waitTimeHalfRange);
	}

	if (m_path.size() != 0)
	{
		m_penemy->CalculateMovement(m_path.back());

		if (MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_path.back()->m_pos) < m_nodePopDistanceSq)
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
}

void WanderState::Deserialize(const nlohmann::json& data)
{
}
