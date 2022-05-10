#include "TextComponent.h"
#include "Engine/Managers/FontManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Managers/GraphicsManager.h"
#include"Engine/ResourceDefines.h"
#include "Engine/EditorUI/EditorUI.h"

TextComponent::TextComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation):GameUIComponent(identifier, uuid, position, scale, rotation)
{	
	m_ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(TEXT_PIXEL_SHADER_NAME);
}

void TextComponent::UpdateFont(string fontName, int fontSize)
{
	m_fontAtlas = FontManager::GetInstance()->GetFontAtlas(fontName, fontSize);
	m_ptexture = m_fontAtlas[0]->fontTexture;
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
				CreateVertexBuffer(m_pdevice);
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();
	
	if (EditorUI::InputText("Text", m_text) == true)
	{
		CreateVertexBuffer(m_pdevice);
	}

	ImGui::Spacing();

	XMFLOAT3 colour = XMFLOAT3(m_colour.f[0], m_colour.f[1], m_colour.f[2]);
	EditorUI::DragFloat3("Colour", colour, 100.0f, 0.01f, 0.0f, 1.0f );

	m_colour.f[0] = colour.x;
	m_colour.f[1] = colour.y;
	m_colour.f[2] = colour.z;

	ImGui::Spacing();
	ImGui::Separator();

}
#endif

void TextComponent::Render(RenderStruct& renderStruct)
{
	//Update CB
	TextPerInstanceCB cb;

	cb.colour = XMFLOAT3(m_colour.f[0], m_colour.f[1], m_colour.f[2]);

	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(m_transform->GetWorldMatrix()));

	GraphicsManager::GetInstance()->m_ptextPerInstanceCB->Update(cb, renderStruct.m_pcontext);

	ID3D11Buffer* pcbBuffer = GraphicsManager::GetInstance()->m_ptextPerInstanceCB->GetBuffer();

	//Bind CB and appropriate resources
	renderStruct.m_pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);
	renderStruct.m_pcontext->VSSetShader(m_pvertexShader, nullptr, 0);

	renderStruct.m_pcontext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);
	renderStruct.m_pcontext->PSSetShader(m_ppixelShader, nullptr, 0);

	renderStruct.m_pcontext->PSSetShaderResources(0, 1, &m_ptexture);
	UINT stride = sizeof(Vertex_PosTex);
	UINT offset = 0;
	renderStruct.m_pcontext->IASetVertexBuffers(0, 1, &m_pvertexBuffer, &stride, &offset);
	renderStruct.m_pcontext->IASetIndexBuffer(m_pindexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//Draw object
	renderStruct.m_pcontext->DrawIndexed(m_text.length() * 6, 0, 0);

	//Unbind resources
	renderStruct.m_pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
	renderStruct.m_pcontext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);


	m_pmesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);

	ID3D11Buffer* pindexBuffer = m_pmesh->GetIndexBuffer();
	ID3D11Buffer* pvertexBuffer = m_pmesh->GetVertexBuffer();

	stride = m_pmesh->GetVBStride();
	offset = m_pmesh->GetVBOffset();

	renderStruct.m_pcontext->IASetVertexBuffers(0, 1, &pvertexBuffer, &stride, &offset);
	renderStruct.m_pcontext->IASetIndexBuffer(m_pmesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
}

void TextComponent::Init(string text, string fontName, int fontSize, XMVECTORF32 colour, ID3D11Device* pdevice)
{
	m_colour = colour;
	m_text = text;
	m_fontName = fontName;
	m_fontSize = fontSize;
#if EDITOR
	m_pdevice = pdevice;
#endif
	UpdateFont(fontName, fontSize);
	CreateVertexBuffer(pdevice);
}

void TextComponent::CreateVertexBuffer(ID3D11Device* pdevice)
{
	if (m_pvertexBuffer)
	{
		m_pvertexBuffer->Release();
		m_pvertexBuffer = nullptr;

		m_pindexBuffer->Release();
		m_pindexBuffer = nullptr;
	}

	XMFLOAT2 windowDimension = GraphicsManager::GetInstance()->GetWindowDimensions();

	XMFLOAT3 position = m_transform->GetPosition();

	int numLetters = m_text.length();

	wstring filePath = FontManager::GetInstance()->GetFontTextureFilePath(m_fontName);

	Vertex_PosTex* vertexBuffer = new Vertex_PosTex[numLetters * 4];
	
	ID3D11Texture2D* texture2d = nullptr;
	HRESULT hr = GraphicsManager::GetInstance()->GetResource(filePath)->QueryInterface(&texture2d);

	XMFLOAT2 textureDimension(0, 0);
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		texture2d->GetDesc(&desc);
		textureDimension.x = static_cast<float>(desc.Width);
		textureDimension.y = static_cast<float>(desc.Height);
	}

	UINT16* indices = new UINT16[numLetters * 6];
	int indicesIndex = 0;

	// Draw each letter onto a quad.
	XMFLOAT3 offsetPos = XMFLOAT3();

	int vertexIndex = 0;
	float biggestHeight = 0.0f;
	for (int i = 0; i < numLetters; ++i)
	{
		int letter = ((int)m_text.at(i)-32);

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x + offsetPos.x, position.y + m_fontAtlas[letter]->charDimension.y + offsetPos.y, 0.0f);	//Top left	0
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2(m_fontAtlas[letter]->charTexPosition.x / textureDimension.x, (m_fontAtlas[letter]->charTexPosition.y) / textureDimension.y);
		++vertexIndex;

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x + m_fontAtlas[letter]->charDimension.x + offsetPos.x, position.y + m_fontAtlas[letter]->charDimension.y + offsetPos.y, 0.0f);	//top right	1
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2((m_fontAtlas[letter]->charTexPosition.x + m_fontAtlas[letter]->charDimension.x) / textureDimension.x, m_fontAtlas[letter]->charTexPosition.y / textureDimension.y);
		++vertexIndex;

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x + offsetPos.x, position.y + offsetPos.y, 0.0f);	//Bottom left	2
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2(m_fontAtlas[letter]->charTexPosition.x / textureDimension.x, (m_fontAtlas[letter]->charTexPosition.y + m_fontAtlas[letter]->charDimension.y) / textureDimension.y);
		++vertexIndex;

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x + offsetPos.x + m_fontAtlas[letter]->charDimension.x, position.y + offsetPos.y, 0.0f);	//Bottom right	3
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2((m_fontAtlas[letter]->charTexPosition.x + m_fontAtlas[letter]->charDimension.x) / textureDimension.x, (m_fontAtlas[letter]->charTexPosition.y + m_fontAtlas[letter]->charDimension.y) / textureDimension.y);
		++vertexIndex;

		indices[indicesIndex] = vertexIndex - 4;	//Top left
		indicesIndex++;

		indices[indicesIndex] = vertexIndex - 1;	//Bottom right
		indicesIndex++;

		indices[indicesIndex] = vertexIndex - 2;	//Bottom left
		indicesIndex++;

		indices[indicesIndex] = vertexIndex - 4;	//Top Left
		indicesIndex++;

		indices[indicesIndex] = vertexIndex - 3;	//Top right
		indicesIndex++;

		indices[indicesIndex] = vertexIndex - 1;	//Bottom right
		indicesIndex++;

		offsetPos.x += m_fontAtlas[letter]->charDimension.x + m_fontAtlas[letter]->spacing.y;
		
		if (biggestHeight < m_fontAtlas[letter]->charDimension.y)
		{
			biggestHeight = m_fontAtlas[letter]->charDimension.y;
		}
	}

	for (int i = 0; i < numLetters * 4; ++i)
	{
		vertexBuffer[i].m_pos.x -= offsetPos.x * 0.5f;
		vertexBuffer[i].m_pos.y -= biggestHeight * 0.5f;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex_PosTex) * (numLetters * 4);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertexBuffer;

	pdevice->CreateBuffer(&bd, &InitData, &m_pvertexBuffer);

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT16) * numLetters*6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	pdevice->CreateBuffer(&bd, &InitData, &m_pindexBuffer);
}
