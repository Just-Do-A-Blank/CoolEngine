#pragma once
#include "Engine/Structure/Manager.h"

struct FontAtlasStruct
{
	int charId;
	XMFLOAT2 charTexPosition;
	XMFLOAT2 charDimension;
	XMFLOAT3 spacing;

	ID3D11ShaderResourceView* fontTexture = nullptr;
};

class FontManager : public Manager<FontManager>
{
private:
	unordered_map<string, vector<FontAtlasStruct*>> m_fontAtlasMap;
	unordered_map<string, XMINT2> m_fontTextureDimension;
	vector<string> m_fontList;
	unordered_map<string, wstring> m_fontTexturePathMap;
public:
	void LoadFont(string fontInfoFilePath, wstring fontTextureFilePath, string fontName);

	FontAtlasStruct* GetFontCharacterData(string fontName, char character);
	vector<FontAtlasStruct*>& GetFontAtlas(string fontName, int fontSize);
	XMINT2& GetTextureDimensionOfFont(string fontName);
	wstring& GetFontTextureFilePath(string fontName);
	void UpdateVertexBuffer();
	vector<string>& GetFontNames();

	void Serialize(nlohmann::json& data) override;
	void Deserialize(nlohmann::json& data) override;
};

