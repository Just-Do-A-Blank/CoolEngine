#if _DEBUG

#include "DebugDrawManager.h"
#include "Engine/GameObjects/DebugRect.h"
#include "GraphicsManager.h"
#include <Engine/Main.cpp>

void DebugDrawManager::Init()
{
	wstring textureName;
	//Load Textures PerColour
	for (int i = 0; i < (int)DebugColour::SIZE; ++i)
	{
		textureName = L"Resources\\Sprites\\DebugColour" + to_wstring(i) + L".dds";
		GraphicsManager::GetInstance()->LoadTextureFromFile(textureName, g_pd3dDevice);

		m_albedoMap.insert(pair<DebugColour, wstring>(DebugColour(i), textureName));
	}	
}

void DebugDrawManager::CreateWorldSpaceDebugRect(string identifier, XMFLOAT3 position, float width, float height, DebugColour colour)
{
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, false);
	debugRect->GetTransform()->SetPosition(position);
	debugRect->GetTransform()->SetScale(XMFLOAT3(width, height, 1.0f));

	m_debugRectMap.insert(pair<string, DebugRect*>(identifier, debugRect));
}

void DebugDrawManager::CreateScreenSpaceDebugRect(string identifier, XMFLOAT3 position, float width, float height, DebugColour colour)
{
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, true);
	debugRect->GetTransform()->SetPosition(position);
	debugRect->GetTransform()->SetScale(XMFLOAT3(width, height, 1.0f));

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