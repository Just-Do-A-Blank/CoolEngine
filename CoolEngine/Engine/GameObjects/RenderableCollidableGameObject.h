#pragma once
#include "CollidableGameObject.h"
#include "Engine/GameObjects/RenderableGameObject.h"

class RenderableCollidableGameObject : public CollidableGameObject, public RenderableGameObject
{
public:
	RenderableCollidableGameObject();
	RenderableCollidableGameObject(string identifier);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

protected:

private:

};

