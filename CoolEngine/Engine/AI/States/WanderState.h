#pragma once
#include "FuzzyState.h"

class PlayerGameObject;
class EnemyGameObject;

struct node;

class WanderState : public FuzzyState
{
public:
	WanderState(EnemyGameObject* penemy);
	WanderState(const nlohmann::json& data);
	WanderState(WanderState const* other, EnemyGameObject* penemy);

	void SetEnemy(EnemyGameObject* penemy);

	void Enter() override;
	void Exit() override;

	float CalculateActivation() override;

	void Update() override;

	void Serialize(nlohmann::json& data) override;
	void Deserialize(const nlohmann::json& data) override;

#if EDITOR
	void CreateEngineUI() override;
#endif

	void Start() override;

protected:

private:
	PlayerGameObject* m_pplayer = nullptr;
	EnemyGameObject* m_penemy = nullptr;

	std::vector<node*> m_path;

	float m_activationDistance = 750;
	float m_nodePopDistance = 3.0f;
	float m_wanderDistance = 200.0f;
	float m_waitTime = 3.0f;
	float m_waitTimeVariance = 1.0f;
	float m_waitTimeStamp = -1.0f;
	float m_currentWaitTime = -1.0f;
};

