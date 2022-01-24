#if _DEBUG

#pragma once
#include "GameObject.h"
#include "Engine/Managers/DebugDrawManager.h"

class DebugRect :
    public GameObject
{
private:
    DebugDrawManager::DebugColour m_colour;

public:
    DebugRect(wstring albedoName, string identifier, bool screenSpace);
    void SetDebugColour(DebugDrawManager::DebugColour colour);
    void Render(RenderStruct& renderStruct);
    void Update();
};

#endif //DEBUG

