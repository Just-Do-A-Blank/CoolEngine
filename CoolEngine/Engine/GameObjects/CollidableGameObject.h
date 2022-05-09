#pragma once
#include "GameObject.h"

class Shape;

class CollidableGameObject : virtual public GameObject
{
public:
	CollidableGameObject();
	CollidableGameObject(string identifier, CoolUUID uuid);

	//Getters
	Shape* GetShape();

	//Setters
	void SetShape(Shape* collider);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

protected:
	Shape* m_pcollider = nullptr;

private:

};

