#pragma once
#include "Engine/Includes/json.hpp"

class EnemyGameObject;
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
	FuzzyState(EnemyGameObject* penemy);
	FuzzyState(const nlohmann::json& data, EnemyGameObject* penemy);
	FuzzyState(const FuzzyState* pother, EnemyGameObject* penemy);

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

	static std::string StateTypeToString(FuzzyStateType type);
	static FuzzyStateType StringToStateType(std::string typeString);

	virtual void Start() = 0;

	static std::string s_typeNames[(int)FuzzyStateType::COUNT + 1];


protected:
	FuzzyStateType m_stateType = FuzzyStateType::COUNT;
	EnemyGameObject* m_penemy = nullptr;

private:
	float m_activationLevel = 0.5f;
};

