#include "ItemTypes.h"
#include <Engine/GameObjects/Gameplay/Interactables/HealthConsum.h>
#include <Engine/GameObjects/Gameplay/Interactables/EInteractableType.h>
#include <Engine/GameObjects/CharacterGameObject.h>
#include <Engine/GameObjects/Gameplay/Interactables/Interactables.h>
#include "Engine/GameObjects/GameObject.h"

ItemTypes::ItemTypes() : Interactables(m_gameObjectType, GetType())
{
	m_gameObjectType |= GameObjectType::INTERACTABLE;
}

ItemTypes::~ItemTypes()
{
}
