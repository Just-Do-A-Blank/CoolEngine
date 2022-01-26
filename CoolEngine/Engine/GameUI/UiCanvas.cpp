#include "UiCanvas.h"
#include "Engine/GameUI/UiElement.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/ResourceDefines.h"

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
		vector<UiElement*> uiElementList = m_pUiSceneGraph->GetAllGameObjects();
		for (int it = 0; it < uiElementList.size(); ++it)
		{
			if (uiElementList[it]->IsRenderable() == false || uiElementList[it]->GetLayer() != i)
			{
				continue;
			}

			uiElementList[it]->Render(renderStruct);
		}
	}
}


void UiCanvas::Update()
{
	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		vector<UiElement*> uiElementList = m_pUiSceneGraph->GetAllGameObjects();
		for (int it = 0; it < uiElementList.size(); ++it)
		{
			uiElementList[it]->Update();
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
