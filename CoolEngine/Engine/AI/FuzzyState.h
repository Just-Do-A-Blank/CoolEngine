#pragma once
#include "Engine/Includes/json.hpp"

enum class FuzzyStateType
{

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

	void SetActivationLevel(float activationLevel);
	float GetActivationLevel() const;

	virtual void Serialize(nlohmann::json& data) = 0;
	virtual void Deserialize(const nlohmann::json& data) = 0;

protected:
	FuzzyStateType m_stateType;

private:
	float m_activationLevel = 0.0f;
};

