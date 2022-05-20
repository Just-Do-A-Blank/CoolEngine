#include "FuzzyState.h"

FuzzyState::FuzzyState()
{
}

FuzzyState::FuzzyState(const FuzzyState* pother)
{
}

void FuzzyState::SetActivationLevel(float activationLevel)
{
	m_activationLevel = activationLevel;
}

float FuzzyState::GetActivationLevel() const
{
	return m_activationLevel;
}
