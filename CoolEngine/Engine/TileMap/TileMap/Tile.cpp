#include "Tile.h"

Tile::Tile() : GameObject()
{}

Tile::Tile(wstring path, int animFrames, int ID, string identifier) : GameObject(identifier)
{
	InitEdges();

	m_ID = ID;
	m_animFrames = animFrames;
	
}

Tile::Tile(int ID, string identifier) : GameObject(identifier)
{
	m_ID = ID;
}


void Tile::SetEdges(bool N, bool S, bool W, bool E)
{
	m_edgeN = N;
	m_edgeS = S;
	m_edgeW = W;
	m_edgeE = E;
}

void Tile::SetEdgeN(bool N)
{
	m_edgeN = N;
}
void Tile::SetEdgeS(bool S)
{
	m_edgeS = S;
}
void Tile::SetEdgeW(bool W)
{
	m_edgeW = W;
}
void Tile::SetEdgeE(bool E)
{
	m_edgeE = E;
}

void Tile::InitSprites()
{
	if (m_animFrames > 1)
	{
		LoadAnimSprites();
	}
	return;
}

void Tile::LoadAnimSprites()
{
	//loadAnimSprite();
}

void Tile::InitEdges()
{
	m_edgeN = false;
	m_edgeS = false;
	m_edgeW = false;
	m_edgeE = false;
}
