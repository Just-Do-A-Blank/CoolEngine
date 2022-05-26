#include "FuzzyState.h"
#include "Engine/EditorUI/EditorUI.h"

FuzzyState::FuzzyState()
{
}

FuzzyState::FuzzyState(const FuzzyState* pother)
{
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
