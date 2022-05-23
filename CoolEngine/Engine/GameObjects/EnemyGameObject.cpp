#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/AI/States/MeleeMovementState.h"
#include "Engine/AI/States/MeleeAttackState.h"

EnemyGameObject::EnemyGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::ENEMY;
}

EnemyGameObject::EnemyGameObject(const nlohmann::json& data, CoolUUID uuid) : CharacterGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::ENEMY;
}

EnemyGameObject::EnemyGameObject(EnemyGameObject const& other) : CharacterGameObject(other)
{

}

EnemyGameObject::~EnemyGameObject()
{
}


void EnemyGameObject::Update()
{
	m_stateMachine.Update();

	if (m_invincibilityTime > 0.0f)
	{
		m_invincibilityTime -= GameManager::GetInstance()->GetTimer()->DeltaTime();
	}
	else
	{
		m_invincibilityTime = 0;
	}
}

void EnemyGameObject::EditorUpdate()
{
}

void EnemyGameObject::Serialize(nlohmann::json& jsonData)
{
	CharacterGameObject::Serialize(jsonData);
}

void EnemyGameObject::CalculateMovement(node* pnode)
{
	XMFLOAT3 desiredDirection = MathHelper::Minus(pnode->m_pos, m_transform->GetWorldPosition());
	desiredDirection = MathHelper::Normalize(desiredDirection);
	m_direction = MathHelper::Plus(m_direction, desiredDirection);

	float step = m_moveSpeed * GameManager::GetInstance()->GetTimer()->DeltaTime();

	m_direction = MathHelper::Normalize(m_direction);

	XMFLOAT3 stepPos = MathHelper::Multiply(m_direction, step);
	stepPos = MathHelper::Plus(stepPos, m_transform->GetWorldPosition());
	m_transform->SetWorldPosition(stepPos);
}

void EnemyGameObject::Start()
{
	PrefabGameObject::Start();

	MeleeMovementState* pstate = new MeleeMovementState(this);

	m_stateMachine.AddState(pstate);

	MeleeAttackState* pattackState = new MeleeAttackState(this);

	m_stateMachine.AddState(pattackState);
}