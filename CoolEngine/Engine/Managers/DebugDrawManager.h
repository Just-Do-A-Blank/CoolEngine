#if _DEBUG
#pragma once
#include "Engine/Structure/Manager.h"

class DebugRect;
class RenderStruct;

class DebugDrawManager : public Manager<DebugDrawManager>
{
public:
	enum class DebugColour
	{
		RED,
		BLUE,
		YELLOW,
		PURPLE,
		GREEN,
		ORANGE,
		BEIGE,
		SIZE
	};

	void Init(ID3D11Device* pd3dDevice);
	void CreateWorldSpaceDebugRect(string identifier, XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour);
	void CreateScreenSpaceDebugRect(string identifier, XMFLOAT3& position, XMFLOAT3& dimension, DebugColour colour);
	void Render(RenderStruct& renderStruct);
	void Update();

	void Serialize(nlohmann::json& data) override;
	void Deserialize(nlohmann::json& data) override;

private:
	unordered_map<string, DebugRect*> m_debugRectMap;
	unordered_map<DebugColour, wstring> m_albedoMap;


};

#endif //_DEBUG

