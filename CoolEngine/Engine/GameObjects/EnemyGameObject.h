#pragma once
#include "Engine/GameObjects/GameObject.h"
#include "Engine/AI/Pathfinding.h"

//Add FsSM states
enum class EnemyState
{
    WANDER,
    ATTACK,
    RUN,
    PATROL,
    STAY,
};

//Change getters and setters for this when setting up FsSM
struct EnemyStateMachine
{
    float WANDER;
    float ATTACK;
    float RUN;
    float PATROL;
    float STAY;

    //FSM
    EnemyState state;
};

class EnemyGameObject :
    public GameObject
{

public:
    EnemyGameObject(string identifier); 
    void Update();


    //Setters
    void SetPath(vector<node*> path);
    void SetTarget(XMFLOAT3 target);
    void SetSpeed(float speed);
    void SetDirection(XMFLOAT3 dir);
    void SetEnemyState(EnemyState state); //will be changed to FsSM states in the future


    //Getters
    const vector<node*> GetPath() const;
    const XMFLOAT3 GetTarget() const;
    const float GetSpeed() const;
    const XMFLOAT3 GetDirection() const;
    const EnemyState GetEnemyState() const; //will be changed to FsSM states in the future 


private:

    vector<node*> m_curPath;
    float m_speed;
    XMFLOAT3 m_direction;
   
    EnemyStateMachine m_enemyState;


};

