#pragma once
#include "FuzzyState.h"

class EnemyGameObject;
class PlayerGameObject;

class RangeAttackState : public FuzzyState
{
public:
	RangeAttackState(EnemyGameObject* penemy);

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

	float m_attackRangeVariance = 900.0f;
};

