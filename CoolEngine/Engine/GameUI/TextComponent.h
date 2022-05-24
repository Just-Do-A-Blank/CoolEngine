#pragma once
#include "GameUIComponent.h"
#include "Engine/Includes/DirectXTK/SimpleMath.h"
#include "Engine/GameUI/GameplayIntegration/GameplayUIResourceAttachment.h"

struct FontAtlasStruct;
class TextComponent : public GameUIComponent
{
	friend FileIO;
private:
	vector<DirectX::SimpleMath::Rectangle> m_characterSourceRects;
	vector<DirectX::SimpleMath::Rectangle> m_characterDestinationRects;

	XMFLOAT2 m_dimensions;
	XMFLOAT2 m_fontTextureDimension;

	float m_cachedZRotation = -1.0f;
	XMFLOAT3 m_cachedPosition = XMFLOAT3();
	XMFLOAT3 m_cachedScale = XMFLOAT3();

	string m_text = "";
	string m_fontName;
	int m_fontSize;

	XMFLOAT4 m_colour;
	vector<FontAtlasStruct*> m_fontAtlas;
public:
	virtual void Render(RenderStruct& renderStruct)override;

	void Init(string text, string fontName, int fontSize, XMFLOAT4 colour);
	TextComponent(string identifier, CoolUUID uuid);
	TextComponent(nlohmann::json& data, CoolUUID uuid, ID3D11Device* pdevice);
	TextComponent(TextComponent const& other);

	void CreateTextQuads();
	void UpdateFont(string fontName, int fontSize);

	virtual void Update()override;
	virtual void EditorUpdate()override;
	void Serialize(nlohmann::json& data) override;

#if EDITOR
	void CreateEngineUI() override;
#endif

protected:
	virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
	virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

private:
	void LoadLocalData(const nlohmann::json& jsonData);
	void SaveLocalData(nlohmann::json& jsonData);

	GameplayUIResourceAttachment* m_resourceAttachement;
};

