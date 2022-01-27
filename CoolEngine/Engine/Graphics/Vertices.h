#pragma once

struct Vertex_PosTex
{
	Vertex_PosTex(XMFLOAT3 pos, XMFLOAT2 tex)
	{
		m_pos = pos;
		m_texCoords = tex;
	}

	XMFLOAT3 m_pos;
	XMFLOAT2 m_texCoords;
};

struct Vertex_PosTexId
{
	Vertex_PosTexId(XMFLOAT3 pos, XMFLOAT2 tex, UINT id)
	{
		m_pos = pos;
		m_texCoords = tex;
		m_id = id;
	}

	XMFLOAT3 m_pos;
	XMFLOAT2 m_texCoords;
	UINT m_id;
};