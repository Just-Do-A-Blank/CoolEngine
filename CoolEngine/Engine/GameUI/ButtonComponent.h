#pragma once
#include "GameUIComponent.h"
#include "Engine/Managers/Events/EventObserver.h"

#define NUM_BUTTON_TEXTURES 3

enum class ButtonState
{
	PRESSED = 0,
	HOVERED,
	RELEASED,
	COUNT
};

class ButtonComponent : public GameUIComponent, public Observer
{
	friend FileIO;
private:
	ID3D11ShaderResourceView* m_pButtonTextures[(int)ButtonState::COUNT];
	wstring m_buttonTexFilepath[(int)ButtonState::COUNT];

	ButtonState m_currentButtonState = ButtonState::RELEASED;
	bool m_isHovered = false;
	bool m_buttonClicked = false;

	void (*m_OnClickFunction)(void*) = nullptr;
	void* m_OnClickFunctionArg = nullptr;
public:
	ButtonComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	ButtonComponent(nlohmann::json& data, CoolUUID uuid);
	~ButtonComponent();

	virtual void Update()override;

	void SetTexture(std::wstring wsfilepath, ButtonState textureType);

	void Handle(Event* e) override;
	void SetButtonOnClick(void (*onClick)(void*), void* argument);

#if EDITOR
	void CreateEngineUI() override;
#endif

	void SetButtonState(ButtonState buttonState);
};