#pragma once
#include "Engine/Includes/json.hpp"

enum class FuzzyStateType
{
	MELEE_MOVEMENT = 0,
	RANGE_MOVEMENT,
	MELEE_ATTACK,
	RANGE_ATTACK,
	WANDER,

	COUNT
};

class FuzzyState
{
public:
	FuzzyState();
	FuzzyState(const FuzzyState* pother);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update() = 0;

	virtual float CalculateActivation() = 0;

#if EDITOR
	virtual void CreateEngineUI();
#endif

	void SetActivationLevel(float activationLevel);
	float GetActivationLevel() const;

	virtual void Serialize(nlohmann::json& data);
	virtual void Deserialize(const nlohmann::json& data);

	FuzzyStateType GetStateType() const;

protected:
	FuzzyStateType m_stateType = FuzzyStateType::COUNT;

private:
	float m_activationLevel = 0.5f;
};

