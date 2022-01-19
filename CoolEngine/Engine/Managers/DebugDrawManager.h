#if _DEBUG
#pragma once
#include "Engine/Structure/Singleton.h"

class DebugRect;

class DebugDrawManager : public Singleton<DebugDrawManager>
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
		SIZE
	};

	void Init();
	void CreateWorldSpaceDebugRect(string identifier, XMFLOAT3 position, float width, float height, DebugColour colour);
	void CreateScreenSpaceDebugRect(string identifier, XMFLOAT3 position, float width, float height, DebugColour colour);
	void Render(RenderStruct& renderStruct);
	void Update();

private:
	unordered_map<string, DebugRect*> m_debugRectMap;
	unordered_map<DebugColour, wstring> m_albedoMap;


};

#endif //_DEBUG

