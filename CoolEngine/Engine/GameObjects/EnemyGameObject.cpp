#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/AI/States/MeleeMovementState.h"
#include "Engine/AI/States/RangeMovementState.h"
#include "Engine/AI/States/RangeAttackState.h"
#include "Engine/AI/States/MeleeAttackState.h"
#include "Engine/AI/States/WanderState.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/PlayerGameObject.h"


EnemyGameObject::EnemyGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::ENEMY;

	m_pAIStateMachine = new EnemyStateMachine(this);
}

EnemyGameObject::EnemyGameObject(const nlohmann::json& data, CoolUUID uuid) : CharacterGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::ENEMY;

	if (data.contains("EnemyStateMachine") == true)
	{
		m_pAIStateMachine = new EnemyStateMachine(data, this);
	}
	else
	{
		m_pAIStateMachine = new EnemyStateMachine(this);
	}
}

EnemyGameObject::EnemyGameObject(EnemyGameObject const& other) : CharacterGameObject(other)
{
	m_pAIStateMachine = new EnemyStateMachine(other.m_pAIStateMachine, this);
}

EnemyGameObject::~EnemyGameObject()
{
	delete m_pAIStateMachine;
	m_pAIStateMachine = nullptr;
}


void EnemyGameObject::Update()
{
	CharacterGameObject::Update();
  
	m_pAIStateMachine->Update();

	if (m_pAIStateMachine->IsStateActive(FuzzyStateType::WANDER) == true)
	{
		SetWeaponPositionWander();
	}
	else
	{
		SetWeaponPositionAgro();
	}
}

void EnemyGameObject::EditorUpdate()
{
}

void EnemyGameObject::Serialize(nlohmann::json& data)
{
	CharacterGameObject::Serialize(data);

	m_pAIStateMachine->Serialize(data);
}

void EnemyGameObject::CalculateMovement(node* pnode)
{
	XMFLOAT3 desiredDirection = MathHelper::Minus(pnode->m_pos, m_transform->GetWorldPosition());
	desiredDirection = MathHelper::Normalize(desiredDirection);
	m_direction = MathHelper::Plus(m_direction, desiredDirection);

	float step = m_moveSpeed * GameManager::GetInstance()->GetTimer()->DeltaTime();

	if (MathHelper::SquareMagnitude(m_direction) != 0)
	{
		m_direction = MathHelper::Normalize(m_direction);

		XMFLOAT3 stepPos = MathHelper::Multiply(m_direction, step);
		stepPos = MathHelper::Plus(stepPos, m_transform->GetWorldPosition());
		m_transform->SetWorldPosition(stepPos);
	}
}

void EnemyGameObject::Start()
{
	CharacterGameObject::Start();

	m_pAIStateMachine->Start();

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}

void EnemyGameObject::SetWeaponPositionAgro()
{
	if (m_pweapon == nullptr || m_pplayer == nullptr)
	{
		return;
	}

	m_pweapon->SetTargetPosition(XMFLOAT2(m_pplayer->GetTransform()->GetWorldPosition().x, m_pplayer->GetTransform()->GetWorldPosition().y));
}

void EnemyGameObject::SetWeaponPositionWander()
{
	if (m_pweapon == nullptr || MathHelper::SquareMagnitude(m_direction) == 0.0f)
	{
		return;
	}

	XMFLOAT3 targetPos = MathHelper::Multiply(m_direction, m_pweapon->GetRadius() * 2);
	targetPos = MathHelper::Plus(m_transform->GetWorldPosition(), targetPos);

	m_pweapon->SetTargetPosition(XMFLOAT2(targetPos.x, targetPos.y));
}

void EnemyGameObject::CreateEngineUI()
{
	CharacterGameObject::CreateEngineUI();

	m_pAIStateMachine->CreateEngineUI();
}
