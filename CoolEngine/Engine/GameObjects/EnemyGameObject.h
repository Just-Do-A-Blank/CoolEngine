#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"
#include "Engine/AI/Pathfinding.h"
#include  "Engine/AI/EnemyStateMachine.h"

class EnemyGameObject : public CharacterGameObject
{
public:
    EnemyGameObject(string identifier, CoolUUID uuid);
    EnemyGameObject(const nlohmann::json& data, CoolUUID uuid);
	EnemyGameObject(EnemyGameObject const& other);
	virtual ~EnemyGameObject()override;

    virtual void Update();
    virtual void EditorUpdate();

    virtual void Serialize(nlohmann::json& jsonData) override;

	void CalculateMovement(node* pnode);

	void Start() override;

private:
    EnemyStateMachine m_stateMachine;
};
