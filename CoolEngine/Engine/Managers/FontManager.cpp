#include "FontManager.h"
#include "Engine/Includes/RapidXML/rapidxml.hpp"
#include "Engine/Managers/GraphicsManager.h"

using namespace rapidxml;

void FontManager::LoadFont(string fontInfoFilePath, wstring fontTextureFilePath, string fontName)
{
	ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(fontTextureFilePath + L".dds");

	if (psRV == nullptr)
	{
		cout << "Failed to set the albedo SRV as one with that name doesn't exist!" << endl;

		return;
	}
	
	
	m_fontList.push_back(fontName);

	FILE* readFile;
	fopen_s(&readFile, (fontInfoFilePath + ".xml").c_str(), "rb");

	fseek(readFile, 0, SEEK_END);
	int fileSize = ftell(readFile);
	fileSize += 1;
	fseek(readFile, 0, SEEK_SET);

	char* xml_copy = new char[fileSize];

	fread(xml_copy, fileSize - 1, 1, readFile);
	xml_copy[fileSize - 1] = '\0';
	xml_document<> doc;

	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	std::string encoding = doc.first_node()->first_attribute("encoding")->value();

	xml_node<>* cur_node = doc.first_node("font");
	cur_node = cur_node->first_node("char");

	XMINT2 textueDimension = XMINT2();

	vector<FontAtlasStruct*> fontAtlasData;
	while (cur_node != nullptr)
	{
		FontAtlasStruct* fontAtlas = new FontAtlasStruct;
		fontAtlas->fontTexture = psRV;

		fontAtlas->charId = std::stoi(cur_node->first_attribute("id")->value());
		fontAtlas->charTexPosition.x = stoi(cur_node->first_attribute("x")->value());
		fontAtlas->charTexPosition.y = stoi(cur_node->first_attribute("y")->value());
		fontAtlas->charDimension.x = stoi(cur_node->first_attribute("width")->value());
		fontAtlas->charDimension.y = stoi(cur_node->first_attribute("height")->value());

		textueDimension.x += fontAtlas->charDimension.x;
		textueDimension.y += fontAtlas->charDimension.y;
		cur_node = cur_node->next_sibling();
		if (!cur_node)
		{
			break;
		}
		fontAtlas->spacing.x = stoi(cur_node->first_attribute("a")->value());
		fontAtlas->spacing.y = stoi(cur_node->first_attribute("b")->value());
		fontAtlas->spacing.y = stoi(cur_node->first_attribute("c")->value());


		fontAtlasData.push_back(fontAtlas);

		cur_node = cur_node->next_sibling();
	}

	m_fontAtlasMap[fontName] = fontAtlasData;
	m_fontTexturePathMap[fontName] = fontTextureFilePath;
	m_fontTextureDimension[fontName] = textueDimension;
}

FontAtlasStruct* FontManager::GetFontCharacterData(string fontName, char character)
{
	if (m_fontAtlasMap.count(fontName) == 0)
	{
		LOG("Font : " << fontName << "; was not found in Font Manager ");
		return nullptr;
	}

	vector<FontAtlasStruct*> map = m_fontAtlasMap.find(fontName)->second;

	//Usable ASCII character starts at 32
	return map[(int(character)) - 32];
}

vector<FontAtlasStruct*>& FontManager::GetFontAtlas(string fontName, int fontSize)
{
	if (m_fontAtlasMap.count(fontName) == 0)
	{
		LOG("Font Atlas: " << fontName << "; was not found in Font Manager ");
		return vector<FontAtlasStruct*>();
	}

	return m_fontAtlasMap.find(fontName)->second;
}

XMINT2& FontManager::GetTextureDimensionOfFont(string fontName)
{
	if (m_fontAtlasMap.count(fontName) == 0)
	{
		LOG("Font Atlas: " << fontName << "; was not found in Font Manager ");
		return XMINT2();
	}

	return m_fontTextureDimension[fontName];
}

void FontManager::UpdateVertexBuffer()
{
}

vector<string>& FontManager::GetFontNames()
{
	return m_fontList;
}

void FontManager::Serialize(nlohmann::json& data)
{
	for (std::unordered_map<string, std::wstring>::iterator it = m_fontTexturePathMap.begin(); it != m_fontTexturePathMap.end(); ++it)
	{
		data["FontManager"]["Paths"].push_back(it->first);
	}

	for (std::unordered_map<string, XMINT2>::iterator it = m_fontTextureDimension.begin(); it != m_fontTextureDimension.end(); ++it)
	{
		data["FontManager"]["Dimensions"].push_back(it->first);
	}
}

void FontManager::Deserialize(nlohmann::json& data)
{
}

wstring& FontManager::GetFontTextureFilePath(string fontName)
{
	if (m_fontTexturePathMap.count(fontName) == 0)
	{
		LOG("Font Atlas: " << fontName << "; was not found in Font Manager ");
		return wstring();
	}

	return m_fontTexturePathMap[fontName];
}
