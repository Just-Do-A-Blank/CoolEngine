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
	CharacterGameObject::Update();

	m_stateMachine.Update();

	if (m_stateMachine.IsStateActive(FuzzyStateType::WANDER) == true)
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

	FuzzyState* pstate = new MeleeMovementState(this);
	m_stateMachine.AddState(pstate);

	pstate = new MeleeAttackState(this);
	m_stateMachine.AddState(pstate);

	pstate = new WanderState(this);
	m_stateMachine.AddState(pstate);

	pstate = new RangeMovementState(this);
	m_stateMachine.AddState(pstate);

	pstate = new RangeAttackState(this);
	m_stateMachine.AddState(pstate);

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}

void EnemyGameObject::SetWeaponPositionAgro()
{
	if (m_pweapon == nullptr || m_pplayer == nullptr)
	{
		return;
	}

	m_pweapon->SetIsPointingAtPlayer(true);
	m_pweapon->SetTargetPosition(XMFLOAT2(m_pplayer->GetTransform()->GetWorldPosition().x, m_pplayer->GetTransform()->GetWorldPosition().y));
}

void EnemyGameObject::SetWeaponPositionWander()
{
	if (m_pweapon == nullptr || MathHelper::SquareMagnitude(m_direction) == 0.0f)
	{
		return;
	}

	m_pweapon->SetIsPointingAtPlayer(false);

	float weaponOffsetDistance = 50.0f;

	XMFLOAT3 weaponPos = MathHelper::Multiply(m_direction, weaponOffsetDistance);
	weaponPos = MathHelper::Plus(m_transform->GetWorldPosition(), weaponPos);

	m_pweapon->GetTransform()->SetWorldPosition(weaponPos);
}
