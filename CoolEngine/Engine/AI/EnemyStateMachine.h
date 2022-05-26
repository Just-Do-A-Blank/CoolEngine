#pragma once
#include "FuzzyStateMachine.h"
#include "Engine/Includes/json.hpp"

class EnemyGameObject;
enum class FuzzyStateType;

class EnemyStateMachine : public FuzzyStateMachine
{
public:
	EnemyStateMachine(EnemyGameObject* penemy);
	EnemyStateMachine(const nlohmann::json& data, EnemyGameObject* penemy);
	EnemyStateMachine(EnemyStateMachine const* other, EnemyGameObject* penemy);

	void Serialize(nlohmann::json& data) override;
	void Deserialize(const nlohmann::json& data) override;

	bool IsStateActive(FuzzyStateType type) const;

#if EDITOR
	void CreateEngineUI() override;
#endif

	void Start();

	void Save();
	bool Load();

protected:

#if EDITOR
	FuzzyStateType m_selectedStateType;
	FuzzyStateType m_comboBoxSelectedStateType;
#endif

private:
	EnemyGameObject* m_penemy = nullptr;

	std::string m_AIName = "";
	std::string m_editorAIName = "";
};

