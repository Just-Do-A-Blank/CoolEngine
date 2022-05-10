#include "GameUIComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

GameUIComponent::GameUIComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
{
	m_componentType |= UIComponentType::BASE;

	m_uiElementIdentifier = identifier;
	m_uuid = uuid;
	m_transform = new Transform();
	InitGraphics();

	m_transform->SetScale(scale);
	m_transform->SetPosition(position);
	m_transform->SetRotation(rotation);

}

void GameUIComponent::InitGraphics()
{
	m_pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	m_ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(DEFAULT_PIXEL_SHADER_NAME);

	m_pmesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);
}

void GameUIComponent::Render(RenderStruct& renderStruct)
{
	//Update CB
	PerInstanceCB cb;
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(m_transform->GetWorldMatrix()));

	GraphicsManager::GetInstance()->m_pperInstanceCB->Update(cb, renderStruct.m_pcontext);

	ID3D11Buffer* pcbBuffer = GraphicsManager::GetInstance()->m_pperInstanceCB->GetBuffer();

	//Bind CB and appropriate resources
	renderStruct.m_pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);
	renderStruct.m_pcontext->VSSetShader(m_pvertexShader, nullptr, 0);

	renderStruct.m_pcontext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);
	renderStruct.m_pcontext->PSSetShader(m_ppixelShader, nullptr, 0);

	renderStruct.m_pcontext->PSSetShaderResources(0, 1, &m_ptexture);
	

	//Draw object
	renderStruct.m_pcontext->DrawIndexed(m_pmesh->GetIndexCount(), 0, 0);

	//Unbind resources
	renderStruct.m_pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
	renderStruct.m_pcontext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
}

void GameUIComponent::Update()
{
}

void GameUIComponent::Serialize(nlohmann::json& data)
{
}

void GameUIComponent::Deserialize(nlohmann::json& data)
{
}

void GameUIComponent::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

void GameUIComponent::SetLayer(const int& layer)
{
	m_layer = layer;
}

void GameUIComponent::SetTexture(std::wstring wsfilepath)
{
	m_ptexture = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	if (m_ptexture == nullptr)
	{
		LOG("Tried to get a texture that doesn't exist!");
	}
	else
	{
		m_texFilepath = wsfilepath;
	}
}

#if EDITOR
void GameUIComponent::ShowEngineUI()
{
	ImGui::Begin("Properties");

	EditorUI::IdentifierText("Identifier", m_uiElementIdentifier);

	ImGui::Spacing();
	ImGui::Separator();

	CreateEngineUI();

	ImGui::End();
}
void GameUIComponent::CreateEngineUI()
{
	ImGui::Spacing();

	m_transform->CreateEngineUI();
}
#endif

int& GameUIComponent::GetLayer()
{
	return m_layer;
}

const bool& GameUIComponent::IsRenderable()
{
	return m_isRenderable;
}

Transform* GameUIComponent::GetTransform()
{
	return m_transform;
}

const string& GameUIComponent::GetIdentifier()
{
	return m_uiElementIdentifier;
}

const CoolUUID& GameUIComponent::GetUUID() const
{
	return m_uuid;
}
