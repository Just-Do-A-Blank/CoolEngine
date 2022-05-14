#pragma once
#include "GameObject.h"

class Shape;

class CollidableGameObject : virtual public GameObject
{
public:
	CollidableGameObject();
	CollidableGameObject(string identifier, CoolUUID uuid);
	CollidableGameObject(const json& data, CoolUUID uuid);
	virtual ~CollidableGameObject()override;

	//Getters
	Shape* GetShape();

	//Setters
	void SetShape(Shape* collider);
	void SetShapeDimensions(XMFLOAT3 scale);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	virtual void Serialize(json& jsonData) override;

protected:
	Shape* m_pcollider = nullptr;

private:

};

