#if _DEBUG

#include "DebugDrawManager.h"
#include "Engine/GameObjects/DebugRect.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Managers/GameManager.h"

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

void DebugDrawManager::CreateWorldSpaceDebugRect(XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour)
{
	string name = "DebugRect";
	name += to_string(m_debugRectMap.size());

	CoolUUID uuid;
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, name, uuid, false);
	debugRect->GetTransform()->SetWorldPosition(position);
	debugRect->GetTransform()->SetScale(dimension);

	m_debugRectMap.insert(pair<string, DebugRect*>(name, debugRect));
}

void DebugDrawManager::CreateScreenSpaceDebugRect(XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour)
{
	string name = "DebugRect";
	name += to_string(m_debugRectMap.size());

	CoolUUID uuid;
	DebugRect* debugRect = new DebugRect(m_albedoMap.find(colour)->second, name, uuid, true);
	debugRect->GetTransform()->SetWorldPosition(position);
	debugRect->GetTransform()->SetScale(dimension);

	m_debugRectMap.insert(pair<string, DebugRect*>(name, debugRect));
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
		delete it->second;
	}

	m_debugRectMap.clear();
}

void DebugDrawManager::Serialize(nlohmann::json& data)
{
}

void DebugDrawManager::Deserialize(nlohmann::json& data)
{
}

#endif //_DEBUG
