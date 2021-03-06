#include "Tile.h"

#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/Graphics/AnimationState.h"
#include "Engine/Physics/Box.h"

#if EDITOR
TileMap* Tile::s_ptileMap = nullptr;
#endif

Tile::Tile() : RenderableCollidableGameObject()
{
}

Tile::Tile(string identifier, CoolUUID uuid) : RenderableCollidableGameObject(identifier, uuid)
{
}

void Tile::Init(string identifier, CoolUUID uuid)
{
	SetIdentifier(identifier);
	SetUUID(uuid);
}

Tile::Tile(wstring path, string identifier, CoolUUID uuid) : RenderableCollidableGameObject(identifier, uuid)
{
	InitAnimation(path);
}

void Tile::InitAnimation(wstring animPath)
{
	AnimationState* pstate = new AnimationState();
	pstate->SetName("default");
	pstate->SetAnimation(animPath);

	m_panimationStateMachine->AddState("default", pstate);
	m_panimationStateMachine->SetActiveState("default");
}

const bool& Tile::GetIsPassable() const
{
	return m_isPassable;
}

int Tile::GetSpriteIndex() const
{
	return m_spriteIndex;
}

int Tile::GetAnimIndex() const
{
	return m_animIndex;
}

void Tile::SetIsPassable(bool passable)
{
	m_isPassable = passable;

	if (m_isPassable == false)
	{
		m_pcollider = new Box(this);
	}
	else if (m_pcollider != nullptr)
	{
		delete m_pcollider;
		m_pcollider = nullptr;
	}

}

void Tile::SetSpriteIndex(int index)
{
	m_spriteIndex = index;
}

void Tile::SetAnimIndex(int index)
{
	m_animIndex = index;
}

#if EDITOR
void Tile::CreateEngineUI()
{
	ImGui::Separator();
	ImGui::Spacing();

	if (EditorUI::Texture("Texture", m_spritePath, m_palbedoSRV) == true)
	{
		s_ptileMap->AddSpritePath(this, m_spritePath);
	}

	ImGui::Spacing();

	if (m_panimationStateMachine->GetActiveState() == nullptr)
	{
		if (EditorUI::Animation("Animation", m_animPath, nullptr) == true)
		{
			SpriteAnimation anim = GraphicsManager::GetInstance()->GetAnimation(m_animPath);

			if (anim.GetFrames() != nullptr)
			{
				AnimationState* panimState = new AnimationState();
				panimState->SetName("default");
				panimState->SetAnimation(m_animPath);

				m_panimationStateMachine->AddState("default", panimState);
				m_panimationStateMachine->SetActiveState("default");
			}
		}
	}
	else
	{
		AnimationState* pstate = (AnimationState*)m_panimationStateMachine->GetActiveState();

		if (EditorUI::Animation("Animation", m_animPath, pstate->GetAnimation()->GetCurrentFrame()) == true)
		{
			pstate->SetAnimation(m_animPath);
		}
	}

	ImGui::Spacing();

    auto layerParameters = EditorUIIntParameters();
    layerParameters.m_minValue = 0;
    layerParameters.m_maxValue = GraphicsManager::GetInstance()->GetNumLayers() - 1;

	EditorUI::DragInt("Layer", m_layer, layerParameters);

	ImGui::Spacing();

	EditorUI::Checkbox("Passable", m_isPassable);

	ImGui::Spacing();
	ImGui::Separator();
}

void Tile::CopyTile(Tile* ptile)
{
	SetSpriteIndex(ptile->m_spriteIndex);
	SetAnimIndex(ptile->m_animIndex);

	SetLayer(ptile->m_layer);
	SetIsPassable(ptile->m_isPassable);

	SetAlbedo(ptile->m_palbedoSRV);

	delete m_panimationStateMachine;
	m_panimationStateMachine = new AnimationStateMachine(ptile->m_panimationStateMachine);
}
#endif