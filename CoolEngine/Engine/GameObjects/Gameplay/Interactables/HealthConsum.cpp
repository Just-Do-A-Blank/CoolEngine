#include "HealthConsum.h"
#include "EInteractableType.h"
#include <Engine/GameObjects/CharacterGameObject.h>
#include "Interactables.h"
#include "Engine/GameObjects/GameObject.h"


/// <summary>
/// Passes the GameObjectType and Enumerated Type to the Interactables Class
/// </summary>
HealthConsum::HealthConsum() : Interactables(m_gameObjectType, GetType())
{
	m_gameObjectType |= GameObjectType::INTERACTABLE;
}

HealthConsum::~HealthConsum()
{
}

void HealthConsum()
{

}

void HealthConsum::AddHealthConsum()
{
}


