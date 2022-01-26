#include "Tile.h"

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
const int& Tile::GetSpriteIndex() const
{
	return m_spriteIndex;
}

const int& Tile::GetAnimIndex() const
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