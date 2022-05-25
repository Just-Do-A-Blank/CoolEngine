#include "RangeMovementState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"

RangeMovementState::RangeMovementState(EnemyGameObject* penemy)
{
	m_stateType = FuzzyStateType::RANGE_MOVEMENT;

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
	m_penemy = penemy;

	if (m_pplayer == nullptr)
	{
		LOG("AI state tried to get the player but couldn't!");
	}
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

	if ((distanceSq > upperDistance || distanceSq < lowerDistance) && distanceSq < m_maxActivationDistanceSq)
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

		if (MathHelper::DistanceSquared(m_penemy->GetTransform()->GetWorldPosition(), m_path.back()->m_pos) < m_nodePopDistanceSq)
		{
			m_path.pop_back();
		}
	}
}

void RangeMovementState::Serialize(nlohmann::json& data)
{
}

void RangeMovementState::Deserialize(const nlohmann::json& data)
{
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
