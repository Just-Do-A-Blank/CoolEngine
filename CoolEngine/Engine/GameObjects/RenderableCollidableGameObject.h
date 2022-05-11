#pragma once
#include "CollidableGameObject.h"
#include "Engine/GameObjects/RenderableGameObject.h"

class RenderableCollidableGameObject : public RenderableGameObject, public CollidableGameObject
{
public:
	RenderableCollidableGameObject();
	RenderableCollidableGameObject(string identifier, CoolUUID uuid);
	virtual ~RenderableCollidableGameObject();

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

protected:

private:

};

