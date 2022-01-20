#pragma once
#include <vector>

#include "Engine/GameObjects/GameObject.h"

class Tile : public GameObject
{
public:
	//Constructors

	//Default
	Tile();

	Tile(wstring path, int animFrames, int ID, string identifier);
	Tile(int a, string identifier);

	//Setup

	void InitAnimation(wstring animPath);

	//Getters

	int GetID() { return m_ID; }

	//Setters

	void SetID(int id) { m_ID = id; }

	void SetEdges(bool N, bool E, bool S, bool W);

	void SetEdgeN(bool N);
	void SetEdgeS(bool S);
	void SetEdgeW(bool W);
	void SetEdgeE(bool E);

	

protected:

private:
	bool m_edgeN, m_edgeS, m_edgeW, m_edgeE;
	int	 m_ID = -25;

	void InitEdges();
};

