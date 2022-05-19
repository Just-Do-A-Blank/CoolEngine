#if _DEBUG
#include "DebugRect.h"
#include "Engine/ResourceDefines.h"

DebugRect::DebugRect(wstring albedoName, string identifier, CoolUUID uuid, bool screenSpace) : RenderableGameObject(identifier, uuid)
{
	SetAlbedo(albedoName);

	if (screenSpace)
	{
		SetVertexShader(SCREENSPACE_VERTEX_SHADER_NAME);
		SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	}
	else
	{
		SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
		SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	}
}

DebugRect::DebugRect(DebugRect const& other) : RenderableGameObject(other)
{
	m_colour = other.m_colour;
}

DebugRect::~DebugRect()
{
}

void DebugRect::SetDebugColour(DebugDrawManager::DebugColour colour)
{
	m_colour = colour;
}

void DebugRect::Render(RenderStruct& renderStruct)
{
	RenderableGameObject::Render(renderStruct);
}

void DebugRect::Update()
{
}

void DebugRect::EditorUpdate()
{
}

#endif //_DEBUG
