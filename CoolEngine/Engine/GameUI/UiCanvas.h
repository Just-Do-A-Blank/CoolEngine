#pragma once
#include "Engine/Managers/SceneGraph.h"
#include "Engine/GameObjects/RenderableGameObject.h"

class UiElement;
class Transform;

class Mesh;

class UiCanvas
{
private:
	string m_casvasIdentifier;
	SceneGraph<UiElement>* m_pUiSceneGraph;

	//Graphics variables
	ID3D11VertexShader* m_pvertexShader = nullptr;
	ID3D11PixelShader* m_ppixelShader = nullptr;

	Mesh* m_pmesh = nullptr;

	int m_layer = 0;

	//Flags
	bool m_isRenderable = true;

protected:
	Transform* m_transform;

	virtual void CreateEngineUI(ID3D11Device* pdevice);
public:
	UiCanvas(string canvasIdentifier);
	void InitGraphics();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	//Editor UI
	virtual void ShowEngineUI(ID3D11Device* pdevice);

	//Setters
	void SetIsRenderable(bool& condition);

	//Getters
	const string& GetIdentifier();
	Transform* GetTransform();
};

