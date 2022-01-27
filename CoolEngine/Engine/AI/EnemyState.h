#pragma once

enum class EnemyMovementStates
{
    WANDER,
    PATROL,
    ATTACK,
    FLEE,
    IDLE,
};

class State
{
public:

    State() {};
    ~State() {};


    //Getters
    bool GetIsAlive();
    EnemyMovementStates GetMovementState();

    //Setters
    void SetAllProperties(bool alive, EnemyMovementStates movementState) { m_isAlive = alive; m_movementState = movementState; }
    void SetAlive(bool alive) { m_isAlive = alive; }
    void SetMovementState(EnemyMovementStates movement) { m_movementState = movement; }


private:

    bool m_isAlive;
    bool m_isAttacking;
    EnemyMovementStates m_movementState;

};

class EnemyState
{

public:
    EnemyState() {};
    ~EnemyState() {};

    State* GetCurrentState() { return m_pCurrentState; }


private:

    State* m_pCurrentState;

    State m_attackPlayer;
    State m_fleePlayer;
    State m_patrolArea;
    State m_WanderArea;
    State m_idle;
};