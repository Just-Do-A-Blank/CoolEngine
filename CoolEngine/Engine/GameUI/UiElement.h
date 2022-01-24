#pragma once

class RenderStruct;
class Transform;

class UiElement
{
private:
	string m_uiElementIdentifier;
	int m_layer = 0;
	Transform* m_transform;

	//Flags
	bool m_isRenderable = true;

public:

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	//Getters
	int& GetLayer();
	const bool& IsRenderable();
	Transform* GetTransform();
	const string& GetIdentifier();

	//Setters
	void SetIsRenderable(bool& condition);

};

