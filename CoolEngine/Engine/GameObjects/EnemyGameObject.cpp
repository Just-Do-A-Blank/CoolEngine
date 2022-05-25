#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/AI/States/MeleeMovementState.h"
#include "Engine/AI/States/MeleeAttackState.h"
#include "Engine/AI/States/WanderState.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Physics/Shape.h"

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

	SetWeaponPosition();
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
	PrefabGameObject::Start();

	MeleeMovementState* pstate = new MeleeMovementState(this);

	m_stateMachine.AddState(pstate);

	MeleeAttackState* pattackState = new MeleeAttackState(this);

	m_stateMachine.AddState(pattackState);

	WanderState* pwanderState = new WanderState(this);

	m_stateMachine.AddState(pwanderState);

	m_pweapon = GameManager::GetInstance()->CreateGameObject<MeleeWeaponGameObject>(m_identifier + "_TestWeapon");
	m_pweapon->SetAlbedo(TEST2);
	m_pweapon->GetTransform()->SetLocalScale(XMFLOAT3(20, 20, 20));
	m_pweapon->SetLayer(3);
	m_pweapon->GetShape()->SetIsTrigger(true);
	m_pweapon->GetShape()->SetIsCollidable(false);

	m_pplayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
}

void EnemyGameObject::SetWeaponPosition()
{
	if (m_pweapon == nullptr || m_pplayer == nullptr)
	{
		return;
	}

	XMFLOAT2 posWorld = XMFLOAT2(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y);
	XMFLOAT2 playerPosWorld = XMFLOAT2(m_pplayer->GetTransform()->GetWorldPosition().x, m_pplayer->GetTransform()->GetWorldPosition().y);
	XMFLOAT2 toWeapon = MathHelper::Minus(playerPosWorld, posWorld);
	toWeapon = MathHelper::Normalize(toWeapon);
	float weaponOffsetDistance = 50.0f;

	XMFLOAT2 weaponPosition = MathHelper::Multiply(toWeapon, weaponOffsetDistance);
	weaponPosition = MathHelper::Plus(posWorld, weaponPosition);

	float angle = MathHelper::DotProduct(toWeapon, XMFLOAT2(0, 1));
	angle = (std::acosf(angle) * 180.0f) / XM_PI;

	if (toWeapon.x > 0.0f)
	{
		angle *= -1.0f;
	}

	m_pweapon->GetTransform()->SetWorldPosition(XMFLOAT3(weaponPosition.x, weaponPosition.y, 0.0f));
	m_pweapon->GetTransform()->SetWorldRotation(XMFLOAT3(0, 0, angle));
}
