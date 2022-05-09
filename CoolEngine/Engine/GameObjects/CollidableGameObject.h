#pragma once
#include "GameObject.h"

class Shape;

class CollidableGameObject : virtual public GameObject
{
public:
	CollidableGameObject();
	CollidableGameObject(string identifier);

	//Getters
	Shape* GetShape();

	//Setters
	void SetShape(Shape* collider);
	void SetShapeDimensions(XMFLOAT3 scale);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

protected:
	Shape* m_pcollider = nullptr;

private:

};

