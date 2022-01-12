#pragma once
#include "Transform.h"
#include "Engine/Managers/GameManager.h"

class GameObject
{
	friend class GameManager;
private:
	ID3D11ShaderResourceView* m_texture;

	//Flags
	bool m_isRenderable = false;
	bool m_isCollidable = false;
	bool m_isTrigger = false;

	//GameObject Setup
	void CreateRenderableGameObject();
	void CreateNonRenderableGameObject();

	void AddCollision();
	void AddTrigger();

protected:
	Transform m_transform;

public:
	const bool& IsRenderable();
	const bool& IsCollidable();
	const bool& IsTrigger();
};