#pragma once
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/AI/Pathfinding.h"

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

    void OnExit();
    void OnEntry();


    //Getters
    const bool GetIsAlive() const;
    const bool GetIsAttacking() const;
    const EnemyMovementStates GetMovementState() const;


    //Setters
    void SetAllProperties(bool alive, EnemyMovementStates movementState, bool attacking);
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
    
    const XMFLOAT3 GetCurrentDestination()const;
    
    void Update(float health, XMFLOAT3 pos);
    void ProcessMovement(XMFLOAT3 pos);

private:
    EnemyStateObserver m_observer;

    vector<node*> m_pPath;
    
    State* m_pCurrentState = nullptr;

    State m_attackPlayer;
    State m_huntPlayer;
    State m_fleePlayer;
    State m_patrolArea;
    State m_WanderArea;
    State m_idle;
};

class EnemyStateObserver : public Observer
{
public:

    EnemyStateObserver();
    ~EnemyStateObserver();

    void Handle(Event* e);



private:

    


};