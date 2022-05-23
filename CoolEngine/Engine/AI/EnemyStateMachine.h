#pragma once
#include "FuzzyStateMachine.h"

class EnemyGameObject;

class EnemyStateMachine : public FuzzyStateMachine
{
public:
	EnemyStateMachine();

	void Deserialize(const nlohmann::json& data) override;

protected:

private:
};

