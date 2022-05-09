#if _DEBUG

#include "DebugDrawManager.h"
#include "Engine/GameObjects/DebugRect.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/ResourceDefines.h"

void DebugDrawManager::Init(ID3D11Device* pd3dDevice)
{
	wstring textureName;
	//Load Textures PerColour
	for (int i = 0; i < (int)DebugColour::SIZE; ++i)
	{
		textureName = DEBUG_COLOUR + to_wstring(i) + L".dds";
		if (GraphicsManager::GetInstance()->LoadTextureFromFile(textureName))
		{
			m_albedoMap.insert(pair<DebugColour, wstring>(DebugColour(i), textureName));
		}
	}	
}

void DebugDrawManager::CreateWorldSpaceDebugRect(string identifier, XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour)
{
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, identifier, false);
	debugRect->GetTransform()->SetPosition(position);
	debugRect->GetTransform()->SetScale(dimension);

	m_debugRectMap.insert(pair<string, DebugRect*>(identifier, debugRect));
}

void DebugDrawManager::CreateScreenSpaceDebugRect(string identifier, XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour)
{
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, identifier, true);
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

void DebugDrawManager::Serialize(nlohmann::json& data)
{
}

void DebugDrawManager::Deserialize(nlohmann::json& data)
{
}

#endif //_DEBUG
