#include "EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"


EnemyGameObject::EnemyGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
    m_gameObjectType = GameObjectType::ENEMY;

	string playerName = "Player";
	PlayerGameObject* pPlayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(playerName);
	m_pUnitStateController = new StateController(this, pPlayer);




	//
	// Example trigger 2 versions of adding to the controller
	// 


	//Example setup of a State. Need to add triggers to the state otherwise the state will never trigger on. This would be an example of "Trigger when distance from player is below 15 units.
	//StateAttackMelee* melee = new StateAttackMelee(this, pPlayer);
	//melee->AddTrigger(StateTriggers::DistanceFromPlayer, 15, StateTriggerValueAt::Below);
	//m_pUnitStateController->AddState(melee);

	//Can also add the trigger post init with below lines (results in the same trigger as above)
	//StateTrigger* trig = new StateTrigger(StateTriggers::DistanceFromPlayer, 15, StateTriggerValueAt::Below);
	//m_pUnitStateController->GetAllStates()[enemyState::AttackMelee]->AddTrigger(trig);


}


void EnemyGameObject::Update()
{	
	if (m_pUnitStateController != nullptr)
	{
		m_pUnitStateController->Update();
	}

	if (!m_curPath.empty())
	{

		//Uses directional movement for calculations, should be moved to charactergameobject then update called from here

		XMFLOAT3 desiredDirection = MathHelper::Minus(m_curPath.back()->m_pos, m_transform->GetPosition());
		desiredDirection = MathHelper::Normalize(desiredDirection);
		m_direction = MathHelper::Plus(m_direction, desiredDirection);

		if (m_curPath.back() == Pathfinding::GetInstance()->FindClosestNode(m_transform->GetPosition()))
		{
			m_curPath.pop_back();
			LOG("New Node");
		}


		float step = m_moveSpeed * GameManager::GetInstance()->GetTimer()->DeltaTime();

		m_direction = MathHelper::Normalize(m_direction);

		XMFLOAT3 stepPos = MathHelper::Multiply(m_direction, step);
		stepPos = MathHelper::Plus(stepPos, m_transform->GetPosition());
		m_transform->SetPosition(stepPos);

	}
	else
	{
		//LOG("NO PATH");
		//Pathfinding::GetInstance()->FindPath(m_transform->GetPosition(), XMFLOAT3(1000, 200, 0), m_curPath); //test function
	}





}

void EnemyGameObject::SetPath(vector<node*> path)
{
	m_curPath = path;
}

const vector<node*> EnemyGameObject::GetPath() const
{
	return m_curPath;
}
