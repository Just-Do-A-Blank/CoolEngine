#pragma once
#include "Engine/GameUI/GameUIComponent.h"


class UICanvas : public GameUIComponent
{
private:
	string m_canvasIdentifier;

	int m_layer = 0;

	//Flags
	bool m_isRenderable = true;

protected:

	virtual void CreateEngineUI(ID3D11Device* pdevice);
public:
	UICanvas(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	//Editor UI
	virtual void ShowEngineUI(ID3D11Device* pdevice);

	//Setters
	void SetIsRenderable(bool& condition);

	//Getters
	const string& GetCanvasIdentifier();
	Transform* GetTransform();
};

