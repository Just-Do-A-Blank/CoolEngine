#pragma once
#include <vector>

#include "Engine/GameObjects/RenderableCollidableGameObject.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/ResourceDefines.h"

class TileMap;

class Tile : public RenderableCollidableGameObject
{
public:
	//Constructors

	//Default
	Tile();

	// Load from parameters
	Tile(wstring path, string identifier, CoolUUID uuid);

	// Load empty
	Tile(string identifier, CoolUUID uuid);

	void Init(string identifier, CoolUUID uuid);

	//Setup

	void InitAnimation(wstring animPath);

	//Getters
	const bool& GetIsPassable() const;

	int GetSpriteIndex() const;
	int GetAnimIndex() const;

	//Setters
	void SetIsPassable(bool passable);

	void SetSpriteIndex(int index);
	void SetAnimIndex(int index);

#if EDITOR
	void CreateEngineUI() override;
#endif

	void CopyTile(Tile* ptile);

protected:

private:
	// True = walkable, False = Blocked
	bool m_isPassable = true; 

	int m_spriteIndex = -1;
	int m_animIndex = -1;

#if EDITOR
	wstring m_animPath;
	wstring m_spritePath;

	static TileMap* s_ptileMap;

	friend class TileMap;
#endif
};

