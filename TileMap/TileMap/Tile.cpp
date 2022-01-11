#include "Tile.h"

Tile::Tile()
{
	m_edgeN = false;
	m_edgeS = false;
	m_edgeW = false;
	m_edgeE = false;
}

Tile::Tile(int a)
{
	m_edgeN = false;
	m_edgeS = false;
	m_edgeW = false;
	m_edgeE = false;

	SetID(a);
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