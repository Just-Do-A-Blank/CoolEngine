#pragma once
#include "Engine/GameObjects/PrefabGameObject.h"

class Shape;

class CollidableGameObject : public PrefabGameObject
{
public:
	CollidableGameObject();
	CollidableGameObject(string identifier, CoolUUID uuid);
	CollidableGameObject(const nlohmann::json& data, CoolUUID uuid);
	CollidableGameObject(CollidableGameObject const& other);
	virtual ~CollidableGameObject()override;

	//Getters
	Shape* GetShape();

	//Setters
	void SetShape(Shape* collider);
	void SetShapeDimensions(XMFLOAT3 scale);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	virtual void Serialize(nlohmann::json& jsonData) override;

    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

protected:
	Shape* m_pcollider = nullptr;

    void LoadLocalData(const nlohmann::json& jsonData);
private:

    void SaveLocalData(nlohmann::json& jsonData);
};

