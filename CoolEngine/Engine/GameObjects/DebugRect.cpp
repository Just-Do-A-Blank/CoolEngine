#if _DEBUG
#include "DebugRect.h"
#include "Engine/ResourceDefines.h"

DebugRect::DebugRect(wstring albedoName, bool screenSpace)
{
	SetAlbedo(albedoName);

	if (screenSpace)
	{
		SetVertexShader(nullptr); //TODO
		SetPixelShader(nullptr); //TODO
	}
	else
	{
		SetVertexShader(DEFAULT_VERTEX_SHADER_NAME); //TODO
		SetPixelShader(DEFAULT_PIXEL_SHADER_NAME); //TODO
	}
}

void DebugRect::SetDebugColour(DebugDrawManager::DebugColour colour)
{
	m_colour = colour;
}

void DebugRect::Render(RenderStruct& renderStruct)
{
	GameObject::Render(renderStruct);
}

void DebugRect::Update()
{
}

#endif //_DEBUG