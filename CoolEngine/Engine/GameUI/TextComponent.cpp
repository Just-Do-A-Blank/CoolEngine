#include "TextComponent.h"
#include "Engine/Managers/FontManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Managers/GraphicsManager.h"
#include"Engine/ResourceDefines.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/GameUI/GameplayIntegration/TextUIResourceDisplay.h"

TextComponent::TextComponent(string identifier, CoolUUID uuid) : GameUIComponent(identifier, uuid)
{	
	m_uiComponentType |= UIComponentType::TEXT;

	m_resourceAttachement = new TextUIResourceDisplay();
}

TextComponent::TextComponent(nlohmann::json& data, CoolUUID uuid, ID3D11Device* pdevice) : GameUIComponent(data, uuid)
{
	m_uiComponentType |= UIComponentType::TEXT;

	std::string uuidString = to_string(*m_UUID);

	if (GameUIComponent::IsPrefab())
	{
		LoadLocalData(GameUIComponent::GetPrefabDataLoadedAtCreation());
		m_resourceAttachement = new TextUIResourceDisplay(GameUIComponent::GetPrefabDataLoadedAtCreation());
	}
	else
	{
		LoadLocalData(data);
		m_resourceAttachement = new TextUIResourceDisplay(data);
	}
}

void TextComponent::UpdateFont(string fontName, int fontSize)
{
	m_fontAtlas = FontManager::GetInstance()->GetFontAtlas(fontName, fontSize);
	m_ptexture = m_fontAtlas[0]->fontTexture;
}

void TextComponent::Serialize(nlohmann::json& data)
{
	GameUIComponent::Serialize(data);

	std::string uuidString = to_string(*m_UUID);

	SaveLocalData(data);
	m_resourceAttachement->Serialize(data);
}

void TextComponent::LoadLocalData(const nlohmann::json& jsonData)
{
	if (jsonData.contains("Text"))
	{
		XMFLOAT4 colour;
		colour = XMFLOAT4(jsonData["Colour"][0], jsonData["Colour"][1], jsonData["Colour"][2], jsonData["Colour"][3]);

		Init(jsonData["Text"], jsonData["FontName"], jsonData["FontSize"], colour);
	}
}

void TextComponent::SaveLocalData(nlohmann::json& jsonData)
{
	jsonData["Text"] = m_text;
	jsonData["FontName"] = m_fontName;
	jsonData["FontSize"] = m_fontSize;
	jsonData["Colour"] = { m_colour.x, m_colour.y, m_colour.z, m_colour.w };
}

void TextComponent::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameUIComponent::LoadAllPrefabData(jsonData);
	LoadLocalData(jsonData);
	m_resourceAttachement->LoadFromTopLevel(jsonData);
}

void TextComponent::SaveAllPrefabData(nlohmann::json& jsonData)
{
	SaveLocalData(jsonData);
	m_resourceAttachement->SaveFromTopLevel(jsonData);
	GameUIComponent::SaveAllPrefabData(jsonData);
}

#if EDITOR
void TextComponent::CreateEngineUI()
{
	GameUIComponent::CreateEngineUI();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	vector<string> fontList = FontManager::GetInstance()->GetFontNames();

	if (IMGUI_LEFT_LABEL(ImGui::BeginCombo, "Font", m_fontName.c_str()) == true)
	{
		for (int i = 0; i < fontList.size(); ++i)
		{
			if (ImGui::Selectable(fontList[i].c_str(), m_fontName == fontList[i].c_str()))
			{
				m_fontName = fontList[i];
				UpdateFont(fontList[i], m_fontSize);
				CreateTextQuads();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();
	
	if (EditorUI::InputText("Text", m_text) == true)
	{
		CreateTextQuads();
	}

	ImGui::Spacing();
    EditorUIFloatParameters colorParameters = EditorUIFloatParameters();
    colorParameters.m_columnWidth = 100;
    colorParameters.m_speed = 0.01f;
    colorParameters.m_minValue = 0;
    colorParameters.m_maxValue = 1;

	XMFLOAT3 colour = XMFLOAT3(m_colour.x, m_colour.y, m_colour.z);
	EditorUI::DragFloat3("Colour", colour, colorParameters);

	m_colour.x = colour.x;
	m_colour.y = colour.y;
	m_colour.z = colour.z;

	ImGui::Spacing();
	ImGui::Separator();


	m_resourceAttachement->CreateEngineUI();
}
#endif

void TextComponent::Update()
{
	XMFLOAT3 position = m_transform->GetWorldPosition();
	float zRotation = m_transform->GetWorldRotation().z;
	XMFLOAT3 scale = m_transform->GetWorldPosition();

	if (zRotation != m_cachedZRotation || !MathHelper::Equals(position, m_cachedPosition) || !MathHelper::Equals(scale, m_cachedScale))
	{
		CreateTextQuads();

		m_cachedPosition = m_transform->GetWorldPosition();
		m_cachedZRotation = m_transform->GetWorldRotation().z;
		m_cachedScale = m_transform->GetWorldScale();
	}
}

void TextComponent::EditorUpdate()
{
	Update();
}

TextComponent::TextComponent(TextComponent const& other) : GameUIComponent(other)
{
	m_characterSourceRects = other.m_characterSourceRects;
	m_characterDestinationRects = other.m_characterDestinationRects;

	m_dimensions = other.m_dimensions;
	m_fontTextureDimension = other.m_fontTextureDimension;

	m_cachedZRotation = other.m_cachedZRotation;
	m_cachedPosition = other.m_cachedPosition;
	m_cachedScale = other.m_cachedScale;

	m_text = other.m_text;
	m_fontName = other.m_fontName;
	m_fontSize = other.m_fontSize;

	m_colour = other.m_colour;
	m_fontAtlas = other.m_fontAtlas;
}


void TextComponent::Render(RenderStruct& renderStruct)
{
	for (int i = 0; i < m_characterSourceRects.size(); ++i)
	{
		XMFLOAT3 position = m_transform->GetWorldPosition();
		XMFLOAT2 offset;
		GraphicsManager::GetInstance()->RenderOffsettedColouredSpriteSheetQuad(m_ptexture, &((RECT)m_characterSourceRects[i]), (RECT)m_characterDestinationRects[i], m_transform->GetWorldRotation().z, XMFLOAT2(), m_colour, m_layer);
	}
}

void TextComponent::Init(string text, string fontName, int fontSize, XMFLOAT4 colour)
{
	m_colour = colour;
	m_text = text;
	m_fontName = fontName;
	m_fontSize = fontSize;

	UpdateFont(fontName, fontSize);
	CreateTextQuads();
}

void TextComponent::CreateTextQuads()
{
	XMFLOAT2 windowDimension = GraphicsManager::GetInstance()->GetWindowDimensions();

	XMFLOAT3 position = m_transform->GetWorldPosition();
	XMFLOAT3 scale = m_transform->GetWorldScale();

	int numLetters = m_text.length();

	wstring filePath = FontManager::GetInstance()->GetFontTextureFilePath(m_fontName);

	ID3D11Texture2D* texture2d = nullptr;
	HRESULT hr = GraphicsManager::GetInstance()->GetResource(filePath)->QueryInterface(&texture2d);

	
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		texture2d->GetDesc(&desc);
		m_fontTextureDimension.x = static_cast<float>(desc.Width);
		m_fontTextureDimension.y = static_cast<float>(desc.Height);
	}

	// Draw each letter onto a quad.
	XMFLOAT3 offsetPos = XMFLOAT3();

	float biggestHeight = 0.0f;
	m_characterSourceRects.resize(numLetters);
	m_characterDestinationRects.resize(numLetters);

	ID3D11Resource* pResource = nullptr;
	ID3D11Texture2D* pTexture2D = nullptr;
	D3D11_TEXTURE2D_DESC desc;

	m_ptexture->GetResource(&pResource);
	pResource->QueryInterface(&pTexture2D);
	pTexture2D->GetDesc(&desc);

	for (int i = 0; i < numLetters; ++i)
	{
		int letter = ((int)m_text.at(i)-32);

		XMFLOAT3 bottomLeftPosition = XMFLOAT3(position.x + offsetPos.x, position.y + offsetPos.y, 0.0f);
		XMFLOAT3 topRightPosition = XMFLOAT3(position.x + (m_fontAtlas[letter]->charDimension.x * scale.x) + offsetPos.x, position.y + (m_fontAtlas[letter]->charDimension.y * scale.y)+ offsetPos.y, 0.0f);

		XMFLOAT2 topLeftTexPos = XMFLOAT2(m_fontAtlas[letter]->charTexPosition.x, (m_fontAtlas[letter]->charTexPosition.y));
		XMFLOAT2 bottomRightTexPos = XMFLOAT2((m_fontAtlas[letter]->charTexPosition.x + m_fontAtlas[letter]->charDimension.x), (m_fontAtlas[letter]->charTexPosition.y + m_fontAtlas[letter]->charDimension.y));

		m_characterDestinationRects[i].width = (topRightPosition.x - bottomLeftPosition.x);
		m_characterDestinationRects[i].height = (topRightPosition.y - bottomLeftPosition.y);

		m_characterSourceRects[i].width = bottomRightTexPos.x - topLeftTexPos.x;
		m_characterSourceRects[i].height = topLeftTexPos.y - bottomRightTexPos.y;

		m_characterDestinationRects[i].x = bottomLeftPosition.x + m_characterDestinationRects[i].width * 0.5f;
		m_characterDestinationRects[i].y = bottomLeftPosition.y + m_characterDestinationRects[i].height * 0.5f;

		m_characterSourceRects[i].x = topLeftTexPos.x;
		m_characterSourceRects[i].y = desc.Height - topLeftTexPos.y;

		offsetPos.x += m_characterDestinationRects[i].width + (m_fontAtlas[letter]->spacing.y * scale.x);
		
		if (biggestHeight < m_fontAtlas[letter]->charDimension.y * scale.y)
		{
			biggestHeight = m_fontAtlas[letter]->charDimension.y * scale.y;
		}
	}

	m_dimensions.x = offsetPos.x;
	m_dimensions.y = biggestHeight;

	for (int i = 0; i < numLetters; ++i)
	{
		m_characterDestinationRects[i].x -= m_dimensions.x * 0.5f;
		m_characterDestinationRects[i].y -= m_dimensions.y * 0.5f;

		XMFLOAT2 relativePos;
		relativePos.x = m_characterDestinationRects[i].x - position.x;
		relativePos.y = m_characterDestinationRects[i].y - position.y;

		XMFLOAT3 worldRotation = m_transform->GetWorldRotation();
		XMVECTOR translatedPos = XMVector4Transform(XMVectorSet(relativePos.x, relativePos.y, 0.0f, 1.0f), XMMatrixRotationRollPitchYaw((worldRotation.x / 180.0f) * XM_PI, (worldRotation.y / 180.0f) * XM_PI, (worldRotation.z / 180.0f) * XM_PI));
		XMFLOAT2 destPos;
		XMStoreFloat2(&destPos, translatedPos);

		m_characterDestinationRects[i].x = position.x + destPos.x;
		m_characterDestinationRects[i].y = position.y + destPos.y;
	}
}