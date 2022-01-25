#include "Tile.h"

Tile::Tile() : RenderableGameObject()
{
}

Tile::Tile(string identifier) : RenderableGameObject(identifier)
{
}

Tile::Tile(wstring path, int ID, string identifier) : RenderableGameObject(identifier)
{
	InitAnimation(path);

	m_ID = ID;
}

Tile::Tile(int ID, string identifier) : RenderableGameObject(identifier)
{
	m_ID = ID;
}

void Tile::InitAnimation(wstring animPath)
{
	AddAnimation("default", animPath);
	return;
}

void Tile::SetPassable(bool passable)
{
	m_Passable = passable;
}