#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"
#include "Engine/AI/Pathfinding.h"
#include "Engine/AI/AIState.h"


class EnemyGameObject :
    public CharacterGameObject
{

public:
    EnemyGameObject(string identifier, CoolUUID uuid);
    void Update();


    //Setters
    void SetPath(vector<node*> path);
    void SetTarget(XMFLOAT3 target);
    void SetDirection(XMFLOAT3 dir);


    //Getters
    const vector<node*> GetPath() const;
    const XMFLOAT3 GetTarget() const;
    const XMFLOAT3 GetDirection() const;


private:

    vector<node*> m_curPath;

    StateController* m_pUnitStateController;
    

};
