#pragma once
#include "FuzzyState.h"

class PlayerGameObject;
class EnemyGameObject;

struct node;

class WanderState : public FuzzyState
{
public:
	WanderState(EnemyGameObject* penemy);

	void SetEnemy(EnemyGameObject* penemy);

	void Enter() override;
	void Exit() override;

	float CalculateActivation() override;

	void Update() override;

	void Serialize(nlohmann::json& data) override;
	void Deserialize(const nlohmann::json& data) override;

protected:

private:
	PlayerGameObject* m_pplayer = nullptr;
	EnemyGameObject* m_penemy = nullptr;

	std::vector<node*> m_path;

	float m_activationDistanceSq = 90000;
	float m_nodePopDistanceSq = 10.0f;
	float m_wanderHalfRange = 100.0f;
	float m_waitTime = 3.0f;
	float m_waitTimeHalfRange = 1.0f;
	float m_waitTimeStamp = -1.0f;
	float m_currentWaitTime = -1.0f;
};

