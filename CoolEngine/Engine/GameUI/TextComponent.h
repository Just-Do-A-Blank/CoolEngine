#pragma once
#include "GameUIComponent.h"

struct FontAtlasStruct;
class TextComponent : public GameUIComponent
{
private:
	ID3D11Buffer* m_pvertexBuffer = nullptr;
	ID3D11Buffer* m_pindexBuffer = nullptr;

	string m_text = "";
	wstring m_texturePath;
	string m_fontName;
	int m_fontSize;

#if EDITOR
	ID3D11Device* m_pdevice;
#endif

	XMVECTORF32 m_colour;
	vector<FontAtlasStruct*> m_fontAtlas;
public:
	virtual void Render(RenderStruct& renderStruct)override;

	void Init(string text, string fontName, int fontSize, XMVECTORF32 colour, ID3D11Device* pdevice);
	TextComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	void CreateVertexBuffer(ID3D11Device* pdevice);
	void UpdateFont(string fontName, int fontSize);

#if EDITOR
	void CreateEngineUI() override;
#endif
};

