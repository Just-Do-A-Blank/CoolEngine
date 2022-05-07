#include "Tile.h"

#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/TileMap/TileMap/TileMap.h"

#if EDITOR
TileMap* Tile::s_ptileMap = nullptr;
#endif

Tile::Tile() : RenderableGameObject()
{
}

Tile::Tile(string identifier) : RenderableGameObject(identifier)
{
}

Tile::Tile(wstring path, string identifier) : RenderableGameObject(identifier)
{
	InitAnimation(path);
}

void Tile::InitAnimation(wstring animPath)
{
	AddAnimation("default", animPath);
	return;
}

const bool& Tile::GetIsPassable() const
{
	return m_isPassable;
}

#if TILE_MAP_TOOL
int Tile::GetSpriteIndex() const
{
	return m_spriteIndex;
}

int Tile::GetAnimIndex() const
{
	return m_animIndex;
}
#endif

void Tile::SetIsPassable(bool passable)
{
	m_isPassable = passable;
}

#if TILE_MAP_TOOL
void Tile::SetSpriteIndex(int index)
{
	m_spriteIndex = index;
}

void Tile::SetAnimIndex(int index)
{
	m_animIndex = index;
}
#endif

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

	if (m_pcurrentAnimation == nullptr)
	{
		if (EditorUI::Animation("Animation", m_animPath, nullptr) == true)
		{
			if (AddAnimation("default", m_animPath) == true)
			{
				PlayAnimation("default");
				s_ptileMap->AddAnimPath(this, m_animPath);
			}
		}
	}
	else
	{
		if (EditorUI::Animation("Animation", m_animPath, m_pcurrentAnimation->GetCurrentFrame()) == true)
		{
			if (AddAnimation("default", m_animPath) == true)
			{
				PlayAnimation("default");
				s_ptileMap->AddAnimPath(this, m_animPath);
			}
		}
	}

	ImGui::Spacing();

	EditorUI::DragInt("Layer", m_layer, 100.0f, 0.1f, 0, GraphicsManager::GetInstance()->GetNumLayers() - 1);

	ImGui::Spacing();

	EditorUI::Checkbox("Passable", m_isPassable);

	ImGui::Spacing();
	ImGui::Separator();
}
#endif