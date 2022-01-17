#pragma once
#include <vector>

#include "Engine/GameObjects/GameObject.h"

class Tile : public GameObject
{
public:
	//Constructors

	//Default
	Tile();

	// Load from parameters
	Tile(wstring path, int animFrames, int ID, string identifier);

	// Load empty
	Tile(int a, string identifier);

	//Setup

	void InitAnimation(wstring animPath);

	//Getters

	int GetID() { return m_ID; }
	bool SetPassable() { return m_Passable; }

	//Setters

	void SetID(int id) { m_ID = id; }
	void SetPassable(bool passable);

protected:

private:
	int	 m_ID = -25;
	// True = walkable, False = Blocked
	bool m_Passable = false; 
};

