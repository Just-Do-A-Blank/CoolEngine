#if _DEBUG

#include "DebugDrawManager.h"
#include "Engine/GameObjects/DebugRect.h"
#include "Engine/Managers/GraphicsManager.h"

void DebugDrawManager::Init(ID3D11Device* pd3dDevice)
{
	wstring textureName;
	//Load Textures PerColour
	for (int i = 0; i < (int)DebugColour::SIZE; ++i)
	{
		textureName = L"Resources\\Debug\\DebugColour" + to_wstring(i) + L".dds";
		GraphicsManager::GetInstance()->LoadTextureFromFile(textureName, pd3dDevice);

		m_albedoMap.insert(pair<DebugColour, wstring>(DebugColour(i), textureName));
	}	
}

void DebugDrawManager::CreateWorldSpaceDebugRect(string identifier, XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour)
{
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, false);
	debugRect->GetTransform()->SetPosition(position);
	debugRect->GetTransform()->SetScale(dimension);

	m_debugRectMap.insert(pair<string, DebugRect*>(identifier, debugRect));
}

void DebugDrawManager::CreateScreenSpaceDebugRect(string identifier, XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour)
{
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, true);
	debugRect->GetTransform()->SetPosition(position);
	debugRect->GetTransform()->SetScale(dimension);

	m_debugRectMap.insert(pair<string, DebugRect*>(identifier, debugRect));
}

void DebugDrawManager::Render(RenderStruct& renderStruct)
{
	for (unordered_map<string, DebugRect*>::iterator it = m_debugRectMap.begin(); it != m_debugRectMap.end(); ++it)
	{
		it->second->Render(renderStruct);
	}
}

void DebugDrawManager::Update()
{
	for (unordered_map<string, DebugRect*>::iterator it = m_debugRectMap.begin(); it != m_debugRectMap.end(); ++it)
	{
		it->second->Update();
	}
}

#endif //_DEBUG
