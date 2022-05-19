#pragma once
#include "GameObject.h"

class Shape;

class CollidableGameObject : virtual public GameObject
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

protected:
	Shape* m_pcollider = nullptr;

private:

};

