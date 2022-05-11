#pragma once
#include "GameObject.h"

class Shape;

class CollidableGameObject : virtual public GameObject
{
public:
	CollidableGameObject();
	CollidableGameObject(string identifier, CoolUUID uuid);
	CollidableGameObject(json data,  int index);

	//Getters
	Shape* GetShape();

	//Setters
	void SetShape(Shape* collider);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	void Serialize(json& jsonData);
	void Deserialize(json& jsonData);

protected:
	Shape* m_pcollider = nullptr;

private:

};

