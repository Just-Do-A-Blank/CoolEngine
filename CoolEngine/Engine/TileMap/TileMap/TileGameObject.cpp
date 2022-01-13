#include "TileGameObject.h"

TileGameObject::TileGameObject(wstring path, int animFrames, int ID)
{
	InitEdges();

	m_ID = ID;
	m_animFrames = animFrames;
	
}

TileGameObject::TileGameObject(int ID)
{
	m_ID = ID;
}


void TileGameObject::SetEdges(bool N, bool S, bool W, bool E)
{
	m_edgeN = N;
	m_edgeS = S;
	m_edgeW = W;
	m_edgeE = E;
}

void TileGameObject::SetEdgeN(bool N)
{
	m_edgeN = N;
}
void TileGameObject::SetEdgeS(bool S)
{
	m_edgeS = S;
}
void TileGameObject::SetEdgeW(bool W)
{
	m_edgeW = W;
}
void TileGameObject::SetEdgeE(bool E)
{
	m_edgeE = E;
}

void TileGameObject::InitSprites()
{
	if (m_animFrames > 1)
	{
		LoadAnimSprites();
	}
	return;
}

void TileGameObject::LoadAnimSprites()
{
	//loadAnimSprite();
}

void TileGameObject::InitEdges()
{
	m_edgeN = false;
	m_edgeS = false;
	m_edgeW = false;
	m_edgeE = false;
}
