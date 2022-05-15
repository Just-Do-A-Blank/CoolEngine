#pragma once
#include "GameUIComponent.h"

struct FontAtlasStruct;
class TextComponent : public GameUIComponent
{
	friend FileIO;
private:
	ID3D11Buffer* m_pvertexBuffer = nullptr;
	ID3D11Buffer* m_pindexBuffer = nullptr;

	string m_text = "";
	string m_fontName;
	int m_fontSize;

#if EDITOR
	ID3D11Device* m_pdevice;
#endif

	XMFLOAT4 m_colour;
	vector<FontAtlasStruct*> m_fontAtlas;
public:
	virtual void Render(RenderStruct& renderStruct)override;

	void Init(string text, string fontName, int fontSize, XMFLOAT4 colour, ID3D11Device* pdevice);
	TextComponent(string identifier, CoolUUID uuid);
	TextComponent(nlohmann::json& data, CoolUUID uuid, ID3D11Device* pdevice);
	void CreateVertexBuffer(ID3D11Device* pdevice);
	void UpdateFont(string fontName, int fontSize);

	void Serialize(nlohmann::json& data) override;

#if EDITOR
	void CreateEngineUI() override;
#endif
};

