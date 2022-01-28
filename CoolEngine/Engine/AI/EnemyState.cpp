#include "EnemyState.h"
#include "Engine/Managers/Events/EventManager.h"

EnemyState::EnemyState()
{
	
    m_attackPlayer.SetAllProperties(true,EnemyMovementStates::ATTACK);
    m_fleePlayer.SetAllProperties(true,EnemyMovementStates::FLEE);
    m_patrolArea.SetAllProperties(true,EnemyMovementStates::PATROL);
    m_WanderArea.SetAllProperties(true, EnemyMovementStates::WANDER);
    m_idle.SetAllProperties(true,EnemyMovementStates::IDLE);

    m_pCurrentState = &m_idle;

    EventManager::Instance()->AddClient(EventType::CharacterPosUpdate, &m_observer);

}

void EnemyState::Update(float health)
{

    if (health < 20)
    {
        m_pCurrentState = &m_fleePlayer;
    }
}




//----------------------------------------------------//


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

void State::SetAllProperties(bool alive, EnemyMovementStates movementState)
{
    m_isAlive = alive;
    m_movementState = movementState;
}

void State::SetAlive(bool alive)
{
    m_isAlive = alive;
}

void State::SetMovementState(EnemyMovementStates movement)
{
    m_movementState = movement;
}
