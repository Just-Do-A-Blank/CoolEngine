#pragma once
#include <vector>

#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/ResourceDefines.h"


class Tile : public RenderableGameObject
{
public:
	//Constructors

	//Default
	Tile();

	// Load from parameters
	Tile(wstring path, string identifier);

	// Load empty
	Tile(string identifier);

	//Setup

	void InitAnimation(wstring animPath);

	//Getters
	const bool& GetIsPassable() const;

#if TILE_MAP_TOOL
	const int& GetSpriteIndex() const;
	const int& GetAnimIndex() const;
#endif

	//Setters
	void SetIsPassable(bool passable);

#if TILE_MAP_TOOL
	void SetSpriteIndex(int index);
	void SetAnimIndex(int index);
#endif

protected:

private:
	// True = walkable, False = Blocked
	bool m_isPassable = false; 

#if TILE_MAP_TOOL
	int m_spriteIndex = -1;
	int m_animIndex = -1;
#endif
};

