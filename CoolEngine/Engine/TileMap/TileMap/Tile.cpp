#include "Tile.h"

Tile::Tile() : GameObject()
{
}

Tile::Tile(string identifier) : GameObject(identifier)
{
	//SetAlbedo(DEFAULT_TILE);
}

Tile::Tile(wstring path, int ID, string identifier) : GameObject(identifier)
{
	InitAnimation(path);

	m_ID = ID;
}

Tile::Tile(int ID, string identifier) : GameObject(identifier)
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