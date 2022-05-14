#pragma once
#include "CollidableGameObject.h"
#include "Engine/GameObjects/RenderableGameObject.h"

class RenderableCollidableGameObject : public RenderableGameObject, public CollidableGameObject
{
public:
	RenderableCollidableGameObject();
	RenderableCollidableGameObject(string identifier, CoolUUID uuid);
	RenderableCollidableGameObject(const json& data, CoolUUID uuid);
	virtual ~RenderableCollidableGameObject()override;

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	virtual void Serialize(json& jsonData) override;

protected:

private:

};

