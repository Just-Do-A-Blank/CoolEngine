#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"
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
    float STAY;

    //FSM
    EnemyState state;
};

class EnemyGameObject :
    public CharacterGameObject
{

public:
    EnemyGameObject(string identifier, CoolUUID uuid);
    EnemyGameObject(json data, int index);
    void Update();


    //Setters
    void SetPath(vector<node*> path);
    void SetTarget(XMFLOAT3 target);
    void SetDirection(XMFLOAT3 dir);
    void SetEnemyState(EnemyState state); //will be changed to FsSM states in the future


    //Getters
    const vector<node*> GetPath() const;
    const XMFLOAT3 GetTarget() const;

    const XMFLOAT3 GetDirection() const;
    const EnemyState GetEnemyState() const; //will be changed to FsSM states in the future 

    void Serialize(json& jsonData);
    void Deserialize(json& jsonData);

private:

    vector<node*> m_curPath;

   
    EnemyStateMachine m_enemyState;

    void DetermineAction();


};

