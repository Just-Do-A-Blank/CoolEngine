#pragma once
#include "GameUIComponent.h"

#define NUM_BUTTON_TEXTURES 3

enum class ButtonTexture
{
	PRESSED,
	RELEASED,
	HOVERED,
	COUNT	
};

class ButtonComponent : public GameUIComponent
{
	friend FileIO;
private:
	ID3D11ShaderResourceView* m_pButtonTexture[(int)ButtonTexture::COUNT];
	wstring m_buttonTexFilepath[(int)ButtonTexture::COUNT];
public:
	ButtonComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	ButtonComponent(nlohmann::json& data, CoolUUID uuid);

	void SetTexture(std::wstring wsfilepath, ButtonTexture textureType);

#if EDITOR
	void CreateEngineUI() override;
#endif
};

