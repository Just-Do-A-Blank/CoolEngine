#include "FuzzyState.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/GameObjects/EnemyGameObject.h"

std::string FuzzyState::s_typeNames[(int)FuzzyStateType::COUNT + 1] =
{
	"Melee Movement",
	"Range Movement",
	"Melee Attack",
	"Range Attack",
	"Wander",
	"Invalid State"
};

FuzzyState::FuzzyState(EnemyGameObject* penemy)
{
	m_penemy = penemy;
}

FuzzyState::FuzzyState(const nlohmann::json& data, EnemyGameObject* penemy)
{
	m_penemy = penemy;
	Deserialize(data);
}

FuzzyState::FuzzyState(const FuzzyState* pother, EnemyGameObject* penemy)
{
	m_activationLevel = pother->m_activationLevel;
	m_penemy = penemy;
}

#if EDITOR
void FuzzyState::CreateEngineUI()
{
	EditorUIFloatParameters params;
	params.m_minValue = 0.01f;
	params.m_maxValue = 1;
	params.m_tooltipText = "The level at which the state becomes active";

	EditorUI::DragFloat("Activation Level", m_activationLevel, params);
}
#endif

void FuzzyState::SetActivationLevel(float activationLevel)
{
	m_activationLevel = activationLevel;
}

float FuzzyState::GetActivationLevel() const
{
	return m_activationLevel;
}

void FuzzyState::Serialize(nlohmann::json& data)
{
	data["ActivationLevel"] = m_activationLevel;
}

void FuzzyState::Deserialize(const nlohmann::json& data)
{
	m_activationLevel = data["ActivationLevel"];
}

FuzzyStateType FuzzyState::GetStateType() const
{
	return m_stateType;
}

std::string FuzzyState::StateTypeToString(FuzzyStateType type)
{
	return FuzzyState::s_typeNames[(int)type];
}

FuzzyStateType FuzzyState::StringToStateType(std::string typeString)
{
	for (int i = 0; i < (int)FuzzyStateType::COUNT; ++i)
	{
		if (FuzzyState::s_typeNames[i] == typeString)
		{
			return (FuzzyStateType)i;
		}
	}

	return FuzzyStateType::COUNT;
}
