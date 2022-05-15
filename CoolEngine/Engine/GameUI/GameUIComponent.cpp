#include "GameUIComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

GameUIComponent::GameUIComponent(string identifier, CoolUUID uuid):GameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_componentType |= UIComponentType::BASE;

	m_transform = new Transform();
	InitGraphics();
}

GameUIComponent::GameUIComponent(nlohmann::json& data, CoolUUID uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_componentType |= UIComponentType::BASE;

	m_UUID = CoolUUID(*uuid);
	std::string uuidString = to_string(*uuid);

	SetIdentifier(data["GameUI"][(int)m_componentType][uuidString]["Identifier"]);
	m_transform = new Transform();
	InitGraphics();

	m_transform->SetPosition(XMFLOAT3(data["GameUI"][(int)m_componentType][uuidString]["Position"][0], data["GameUI"][(int)m_componentType][uuidString]["Position"][1], data["GameUI"][(int)m_componentType][uuidString]["Position"][2]));
	m_transform->SetRotation(XMFLOAT3(data["GameUI"][(int)m_componentType][uuidString]["rotation"][0], data["GameUI"][(int)m_componentType][uuidString]["rotation"][1], data["GameUI"][(int)m_componentType][uuidString]["rotation"][2]));
	m_transform->SetScale(XMFLOAT3(data["GameUI"][(int)m_componentType][uuidString]["scale"][0], data["GameUI"][(int)m_componentType][uuidString]["scale"][1], data["GameUI"][(int)m_componentType][uuidString]["scale"][2]));

	m_isRenderable = data["GameUI"][(int)m_componentType][uuidString]["IsRendering"];
	m_layer = data["GameUI"][(int)m_componentType][uuidString]["Layer"];

	SetTexture(data["GameUI"][(int)m_componentType][uuidString]["TexturePath"]);
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
	std::string uuidString = to_string(*m_UUID);

	data["GameUI"][(int)m_componentType][uuidString]["Identifier"].push_back(m_identifier);
	data["GameUI"][(int)m_componentType][uuidString]["Position"].push_back({ m_transform->GetLocalPosition().x, m_transform->GetLocalPosition().y, m_transform->GetLocalPosition().z });
	data["GameUI"][(int)m_componentType][uuidString]["Rotation"].push_back({ m_transform->GetLocalRotation().x, m_transform->GetLocalRotation().y, m_transform->GetLocalRotation().z });
	data["GameUI"][(int)m_componentType][uuidString]["Scale"].push_back({ m_transform->GetLocalScale().x, m_transform->GetLocalScale().y, m_transform->GetLocalScale().z });
	data["GameUI"][(int)m_componentType][uuidString]["TexturePath"].push_back(m_texFilepath);
	data["GameUI"][(int)m_componentType][uuidString]["Layer"].push_back(m_layer);
	data["GameUI"][(int)m_componentType][uuidString]["IsRendering"].push_back(m_isRenderable);
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

	EditorUI::IdentifierText("Identifier", m_identifier);

	ImGui::Spacing();
	ImGui::Separator();

	CreateEngineUI();

	ImGui::End();
}
void GameUIComponent::CreateEngineUI()
{
	GameObject::CreateEngineUI();
}
UIComponentType GameUIComponent::GetUIComponentType() const
{
	return m_componentType;
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

const UIComponentType& GameUIComponent::GetComponentType() const
{
	return m_componentType;
}

bool GameUIComponent::ContainsType(UIComponentType type)
{
	return (m_componentType & type) == type;
}

