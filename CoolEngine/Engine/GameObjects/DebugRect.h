#if _DEBUG

#pragma once
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/DebugDrawManager.h"

class DebugRect : public RenderableGameObject
{
private:
    DebugDrawManager::DebugColour m_colour;

public:
    DebugRect(wstring albedoName, string identifier, CoolUUID uuid, bool screenSpace);
	DebugRect(DebugRect const& other);
	virtual ~DebugRect()override;

    void SetDebugColour(DebugDrawManager::DebugColour colour);
    void Render(RenderStruct& renderStruct);
    virtual void Update()override;
    virtual void EditorUpdate()override;
};

#endif //DEBUG

