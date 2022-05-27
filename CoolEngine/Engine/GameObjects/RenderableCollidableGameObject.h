#pragma once
#include "CollidableGameObject.h"
#include "Engine/GameObjects/RenderableGameObject.h"

class RenderableCollidableGameObject : public RenderableGameObject, public CollidableGameObject
{
public:
	RenderableCollidableGameObject();
	RenderableCollidableGameObject(string identifier, CoolUUID uuid);
	RenderableCollidableGameObject(const nlohmann::json& data, CoolUUID uuid);
	RenderableCollidableGameObject(RenderableCollidableGameObject const& other);
	virtual ~RenderableCollidableGameObject()override;

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	virtual void Serialize(nlohmann::json& jsonData) override;

protected:

    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

private:

    void SaveLocalData(nlohmann::json& jsonData);
};

