#pragma once
#include "GameUIComponent.h"

struct FontAtlasStruct;
class TextComponent : public GameUIComponent
{
	friend FileIO;
private:
	ID3D11Buffer* m_pvertexBuffer = nullptr;
	ID3D11Buffer* m_pindexBuffer = nullptr;

	string m_text;
	wstring m_texturePath;
	string m_fontName;
	XMVECTORF32 m_colour;
	vector<FontAtlasStruct*> m_fontAtlas;
public:
	virtual void Render(RenderStruct& renderStruct)override;

	void Init(string text, string fontName, int fontSize, XMVECTORF32 colour, ID3D11Device* pdevice);
	TextComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	void CreateVertexBuffer(ID3D11Device* pdevice);
	void UpdateFont(string fontName, int fontSize);
};

