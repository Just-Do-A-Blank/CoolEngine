#pragma once
#include <vector>

#include "Engine/GameObjects/GameObject.h"

class TileGameObject : public GameObject
{
public:
	TileGameObject(wstring path, int animFrames, int ID);
	TileGameObject(int a);

	int GetID() 
	{ 
		if (m_ID != NULL)
		{
			return m_ID;
		}
		return -25;
	}

	void SetID(int id) { m_ID = id; }

	void SetEdges(bool N, bool E, bool S, bool W);

	void SetEdgeN(bool N);
	void SetEdgeS(bool S);
	void SetEdgeW(bool W);
	void SetEdgeE(bool E);

	void InitSprites();
	void LoadAnimSprites();

protected:

private:
	bool m_edgeN, m_edgeS, m_edgeW, m_edgeE;
	int	 m_ID = -25;

	int  m_animFrames = 1;
	vector<ID3D11ShaderResourceView*> m_sprites[1] = {};

	void InitEdges();
};

