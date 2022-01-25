#if _DEBUG

#pragma once
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/DebugDrawManager.h"

class DebugRect : public RenderableGameObject
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

