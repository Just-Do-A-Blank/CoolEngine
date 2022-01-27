#pragma once
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Structure/EditorUIComponent.h"

class Transform;

class GameUIComponent : public EditorUIComponent
{
private:
	//Graphics variables

	
	int m_layer = 0;

	//Flags
	bool m_isRenderable = true;

protected:
	ID3D11VertexShader* m_pvertexShader = nullptr;
	ID3D11PixelShader* m_ppixelShader = nullptr;	
	Mesh* m_pmesh = nullptr;
	string m_uiElementIdentifier;

	Transform* m_transform;
	ID3D11ShaderResourceView* m_ptexture = nullptr;
	wstring m_texFilepath = L"";
public:
	GameUIComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	void InitGraphics();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	//Getters
	int& GetLayer();
	const bool& IsRenderable();
	Transform* GetTransform();
	const string& GetIdentifier();

	//Setters
	void SetIsRenderable(bool& condition);
	void SetLayer(const int& layer);

	void SetTexture(ID3D11ShaderResourceView* ptexture);

#if EDITOR
	virtual void ShowEngineUI();
	virtual void CreateEngineUI() override;
#endif

};

