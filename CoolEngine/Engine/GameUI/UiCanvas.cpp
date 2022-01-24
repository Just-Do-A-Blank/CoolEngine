#include "UiCanvas.h"
#include "Engine/GameUI/UiElement.h"
#include "Engine/Managers/GraphicsManager.h"

void UiCanvas::CreateEngineUI(ID3D11Device* pdevice)
{
}

UiCanvas::UiCanvas(string canvasIdentifier)
{
	m_casvasIdentifier = canvasIdentifier;

	InitGraphics();
}

void UiCanvas::InitGraphics()
{
	m_pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(PASSTHROUGH_VERTEX_SHADER_NAME);
	m_ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(DEFAULT_PIXEL_SHADER_NAME);

	m_pmesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);
}

void UiCanvas::Render(RenderStruct& renderStruct)
{
	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		unordered_map<string, UiElement*> uiElementList = m_pUiSceneGraph->GetAllGameObjects();
		for (unordered_map<string, UiElement*>::iterator it = uiElementList.begin(); it != uiElementList.end(); ++it)
		{
			if (it->second->IsRenderable() == false || it->second->GetLayer() != i)
			{
				continue;
			}

			it->second->Render(renderStruct);
		}
	}
}


void UiCanvas::Update()
{
	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		unordered_map<string, UiElement*> uiElementList = m_pUiSceneGraph->GetAllGameObjects();
		for (unordered_map<string, UiElement*>::iterator it = uiElementList.begin(); it != uiElementList.end(); ++it)
		{
			it->second->Update();
		}
	}
}

void UiCanvas::ShowEngineUI(ID3D11Device* pdevice)
{
}

void UiCanvas::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

const string& UiCanvas::GetIdentifier()
{
	return m_casvasIdentifier;
}

Transform* UiCanvas::GetTransform()
{
	return m_transform;
}