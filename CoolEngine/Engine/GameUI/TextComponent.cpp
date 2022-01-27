#include "TextComponent.h"
#include "Engine/Managers/FontManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Managers/GraphicsManager.h"
#include"Engine/ResourceDefines.h"

TextComponent::TextComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation):GameUIComponent(identifier, position, scale, rotation)
{	
	m_ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(TEXT_PIXEL_SHADER_NAME);
}

void TextComponent::UpdateFont(string fontName, int fontSize)
{
	m_fontAtlas = FontManager::GetInstance()->GetFontAtlas(fontName, fontSize);
	m_ptexture = m_fontAtlas[0]->fontTexture;
}

void TextComponent::Render(RenderStruct& renderStruct)
{
	//Update CB
	PerInstanceCB cb;
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(m_transform->GetWorldMatrix()));

	GraphicsManager::GetInstance()->m_pperInstanceCB->Update(cb, renderStruct.m_pcontext);

	ID3D11Buffer* pcbBuffer = GraphicsManager::GetInstance()->m_pperInstanceCB->GetBuffer();

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
}

void TextComponent::Init(string text, string fontName, int fontSize, ID3D11Device* pdevice)
{
	m_text = text;
	m_fontName = fontName;
	UpdateFont(fontName, fontSize);
	CreateVertexBuffer(pdevice);
}

void TextComponent::CreateVertexBuffer(ID3D11Device* pdevice)
{
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
	int vertexIndex = 0;
	for (int i = 0; i < numLetters; ++i)
	{
		int letter = ((int)m_text.at(i)-32);

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x, position.y + m_fontAtlas[letter]->charDimension.y, 0.0f);	//Top left	0
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2(m_fontAtlas[letter]->charTexPosition.x / textureDimension.x, (m_fontAtlas[letter]->charTexPosition.y) / textureDimension.y);
		LOG("X = " << vertexBuffer[vertexIndex].m_texCoords.x << ", Y = " << vertexBuffer[vertexIndex].m_texCoords.y);
		++vertexIndex;

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x + m_fontAtlas[letter]->charDimension.x, position.y + m_fontAtlas[letter]->charDimension.y, 0.0f);	//top right	1
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2((m_fontAtlas[letter]->charTexPosition.x + m_fontAtlas[letter]->charDimension.x) / textureDimension.x, m_fontAtlas[letter]->charTexPosition.y / textureDimension.y);
		LOG("X = " << vertexBuffer[vertexIndex].m_texCoords.x << ", Y = " << vertexBuffer[vertexIndex].m_texCoords.y);
		++vertexIndex;

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x, position.y, 0.0f);	//Bottom left	2
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2(m_fontAtlas[letter]->charTexPosition.x / textureDimension.x, (m_fontAtlas[letter]->charTexPosition.y + m_fontAtlas[letter]->charDimension.y) / textureDimension.y);
		LOG("X = " << vertexBuffer[vertexIndex].m_texCoords.x << ", Y = " << vertexBuffer[vertexIndex].m_texCoords.y);
		++vertexIndex;

		vertexBuffer[vertexIndex].m_pos = XMFLOAT3(position.x + m_fontAtlas[letter]->charDimension.x, position.y, 0.0f);	//Bottom right	3
		vertexBuffer[vertexIndex].m_texCoords = XMFLOAT2((m_fontAtlas[letter]->charTexPosition.x + m_fontAtlas[letter]->charDimension.x) / textureDimension.x, (m_fontAtlas[letter]->charTexPosition.y + m_fontAtlas[letter]->charDimension.y) / textureDimension.y);
		LOG("X = " << vertexBuffer[vertexIndex].m_texCoords.x << ", Y = " << vertexBuffer[vertexIndex].m_texCoords.y);
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

		// Update the x location for drawing by the size of the letter and spacing before next letter.
		position.x = position.x + m_fontAtlas[letter]->charDimension.x + m_fontAtlas[letter]->spacing.y;
	}


	//ID3D11Buffer* pvertexBuffer;

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

	//ID3D11Buffer* pindexBuffer;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT16) * numLetters*6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	pdevice->CreateBuffer(&bd, &InitData, &m_pindexBuffer);
}
