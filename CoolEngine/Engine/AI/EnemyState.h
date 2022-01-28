#pragma once
#include "Engine/Managers/Events/EventObserver.h"

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
    const bool GetIsAlive() const;
    const bool GetIsAttacking() const;
    const EnemyMovementStates GetMovementState() const;

    //Setters
    void SetAllProperties(bool alive, EnemyMovementStates movementState);
    void SetAlive(bool alive);
    void SetMovementState(EnemyMovementStates movement);


private:

    bool m_isAlive;
    bool m_isAttacking;
    EnemyMovementStates m_movementState;

};

class EnemyState
{

public:
    EnemyState();
    ~EnemyState() {};

    State* GetCurrentState() { return m_pCurrentState; }
    void Update(float health);
    EnemyStateObserver m_observer;
private:



    State* m_pCurrentState = nullptr;

    State m_attackPlayer;
    State m_fleePlayer;
    State m_patrolArea;
    State m_WanderArea;
    State m_idle;
};

class EnemyStateObserver : Observer
{
public:

    EnemyStateObserver();
    ~EnemyStateObserver();

    void Handle(Event* e);



private:

    


};