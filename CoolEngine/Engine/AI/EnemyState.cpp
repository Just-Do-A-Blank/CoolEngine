#include "EnemyState.h"
#include "Engine/Managers/Events/EventManager.h"

EnemyState::EnemyState()
{
	
    m_attackPlayer.SetAllProperties(true,EnemyMovementStates::ATTACK, true);
    m_fleePlayer.SetAllProperties(true,EnemyMovementStates::FLEE, false);
    m_patrolArea.SetAllProperties(true,EnemyMovementStates::PATROL, false);
    m_WanderArea.SetAllProperties(true, EnemyMovementStates::WANDER, false);
    m_idle.SetAllProperties(true,EnemyMovementStates::IDLE, false);
    m_huntPlayer.SetAllProperties(true, EnemyMovementStates::ATTACK, false);

    m_pCurrentState = &m_idle;

    EventManager::Instance()->AddClient(EventType::CharacterPosUpdate, &m_observer);

}

void EnemyState::Update(float health, XMFLOAT3 pos)
{

    if (m_pPath.empty())
    {
        switch (m_pCurrentState->GetMovementState())
        {
        case EnemyMovementStates::WANDER:
            //m_pPath = Pathfinding::GetInstance()->FindPath(pos, 
            break;
        case EnemyMovementStates::PATROL:
            break;
        case EnemyMovementStates::ATTACK:
            break;
        case EnemyMovementStates::FLEE:
            break;
        case EnemyMovementStates::IDLE:
            break;


        }
    }

    switch (m_pCurrentState->GetMovementState())
    {
    case EnemyMovementStates::WANDER:
        ProcessMovement(pos);
        break;
    case EnemyMovementStates::PATROL:
        ProcessMovement(pos);
        break;
    case EnemyMovementStates::ATTACK:
        ProcessMovement(pos);
        break;
    case EnemyMovementStates::FLEE:
        ProcessMovement(pos);
        break;
    case EnemyMovementStates::IDLE:
        m_pPath.clear();
        m_pPath.push_back(Pathfinding::GetInstance()->FindClosestNode(pos));
        break;
    }
}

void EnemyState::ProcessMovement(XMFLOAT3 pos)
{
    if (m_pPath.back() == Pathfinding::GetInstance()->FindClosestNode(pos))
    {
        m_pPath.pop_back();
        LOG("New Node");
    }

}


    /*
    if (health < 20)
    {
        m_pCurrentState = &m_fleePlayer;
    }

*/





//----------------------------------------------------//
//State Class


void State::OnExit()
{

}

void State::OnEntry()
{

}

const bool State::GetIsAlive() const
{
    return m_isAlive;
}

const bool State::GetIsAttacking() const
{
    return m_isAttacking;
}

const EnemyMovementStates State::GetMovementState() const
{
    return m_movementState;
}

void State::SetAllProperties(bool alive, EnemyMovementStates movementState, bool attacking)
{
    m_isAlive = alive;
    m_movementState = movementState;
    m_isAttacking = attacking;
}

void State::SetAlive(bool alive)
{
    m_isAlive = alive;
}

void State::SetMovementState(EnemyMovementStates movement)
{
    m_movementState = movement;
}
