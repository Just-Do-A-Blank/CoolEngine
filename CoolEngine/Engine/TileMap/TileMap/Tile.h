#pragma once
#include <vector>

#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/ResourceDefines.h"

class Tile : public RenderableGameObject
{
public:
	//Constructors

	//Default
	Tile();

	// Load from parameters
	Tile(wstring path, int ID, string identifier);

	// Load empty
	Tile(int a, string identifier);

	Tile(string identifier);

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

