#pragma once
#include "CollidableGameObject.h"
#include "Engine/GameObjects/RenderableGameObject.h"

class RenderableCollidableGameObject : public RenderableGameObject, public CollidableGameObject
{
public:
	RenderableCollidableGameObject();
	RenderableCollidableGameObject(string identifier, CoolUUID uuid);
	RenderableCollidableGameObject(json data, CoolUUID index);
	virtual ~RenderableCollidableGameObject()override;

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	void Serialize(json& jsonData);
	void Deserialize(json& jsonData);

protected:

private:

};

