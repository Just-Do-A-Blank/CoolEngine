#include "FuzzyState.h"
#include "Engine/EditorUI/EditorUI.h"

std::string FuzzyState::s_typeNames[(int)FuzzyStateType::COUNT + 1] =
{
	"Melee Movement",
	"Range Movement",
	"Melee Attack",
	"Range Attack",
	"Wander",
	"Invalid State"
};

FuzzyState::FuzzyState()
{
}

FuzzyState::FuzzyState(const nlohmann::json& data)
{
	Deserialize(data);
}

FuzzyState::FuzzyState(const FuzzyState* pother)
{
	m_activationLevel = pother->m_activationLevel;
}

#if EDITOR
void FuzzyState::CreateEngineUI()
{
	EditorUIFloatParameters params;
	params.m_minValue = 0;
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
