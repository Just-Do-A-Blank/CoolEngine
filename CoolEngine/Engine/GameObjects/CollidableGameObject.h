#pragma once
#include "GameObject.h"

class Shape;

class CollidableGameObject : virtual public GameObject
{
public:
	CollidableGameObject();
	CollidableGameObject(string identifier, CoolUUID uuid);
	CollidableGameObject(json data, CoolUUID index);
	~CollidableGameObject();
	virtual ~CollidableGameObject()override;

	//Getters
	Shape* GetShape();

	//Setters
	void SetShape(Shape* collider);
	void SetShapeDimensions(XMFLOAT3 scale);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	void Serialize(json& jsonData);
	void Deserialize(json& jsonData);

protected:
	Shape* m_pcollider = nullptr;

private:

};

