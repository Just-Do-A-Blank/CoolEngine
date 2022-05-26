#pragma once
#include "FuzzyStateMachine.h"

class EnemyGameObject;
enum class FuzzyStateType;

class EnemyStateMachine : public FuzzyStateMachine
{
public:
	EnemyStateMachine();

	void Deserialize(const nlohmann::json& data) override;

	bool IsStateActive(FuzzyStateType type) const;

protected:

private:
};

