#include "RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Graphics/Mesh.h"

RenderableGameObject::RenderableGameObject() : GameObject()
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;
}

RenderableGameObject::RenderableGameObject(string identifier) : GameObject(identifier)
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;
}

void RenderableGameObject::InitGraphics()
{
	m_pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	m_ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(DEFAULT_PIXEL_SHADER_NAME);

	m_pmesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);
}

const bool& RenderableGameObject::IsRenderable()
{
	return m_isRenderable;
}

void RenderableGameObject::Render(RenderStruct& renderStruct)
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

	if (m_pcurrentAnimation == nullptr || m_pcurrentAnimation->GetFrames() == nullptr)
	{
		renderStruct.m_pcontext->PSSetShaderResources(0, 1, &m_palbedoSRV);
	}
	else
	{
		ID3D11ShaderResourceView* psRV = m_pcurrentAnimation->GetCurrentFrame();

		renderStruct.m_pcontext->PSSetShaderResources(0, 1, &psRV);
	}

	//Draw object
	renderStruct.m_pcontext->DrawIndexed(m_pmesh->GetIndexCount(), 0, 0);

	//Unbind resources
	renderStruct.m_pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
	renderStruct.m_pcontext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
}

void RenderableGameObject::Update()
{
	if (m_pcurrentAnimation != nullptr && m_pcurrentAnimation->GetFrames() != nullptr)
	{
		m_pcurrentAnimation->Update();
	}
}


#if EDITOR
void RenderableGameObject::CreateEngineUI()
{
	GameObject::CreateEngineUI();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::Texture("Texture", m_texFilepath, m_palbedoSRV);

	ImGui::Spacing();

	EditorUI::DragInt("Layer", m_layer, 100.0f, 0.1f, 0, GraphicsManager::GetInstance()->GetNumLayers() - 1);

	ImGui::Spacing();

	EditorUI::Checkbox("Renderable", m_isRenderable);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::Animations("Animation", m_animations);

	ImGui::Spacing();

	//Create button for adding animations to object
	IMGUI_LEFT_LABEL(ImGui::InputText, "Name", m_createDeleteAnimName, ANIM_NAME_SIZE);

	ImGui::SameLine();

	if (ImGui::Button("New") == true && m_animations.count(m_createDeleteAnimName) == 0)
	{
		if (AddAnimation(m_createDeleteAnimName, SpriteAnimation()) == true)
		{
			//Add string terminator so it appears the field has been wiped
			m_createDeleteAnimName[0] = '\0';
		}
	}

	//Create button for deleting animations from object
	ImGui::SameLine();

	if (ImGui::Button("Delete") == true)
	{
		if (RemoveAnimation(m_createDeleteAnimName) == true)
		{
			//Add string terminator so it appears the field has been wiped
			m_createDeleteAnimName[0] = '\0';
		}
	}
}
#endif

Mesh* RenderableGameObject::GetMesh() const
{
	return m_pmesh;
}

SpriteAnimation& RenderableGameObject::GetAnimation(std::string name)
{
	return m_animations[name];
}

unordered_map<std::string, SpriteAnimation>* RenderableGameObject::GetAnimations()
{
	return &m_animations;
}

int RenderableGameObject::GetLayer() const
{
	return m_layer;
}

bool RenderableGameObject::PlayAnimation(std::string name)
{
	m_pcurrentAnimation = &m_animations[name];

	if (m_pcurrentAnimation == nullptr)
	{
		LOG("Couldn't find an animation with that name!");

		return false;
	}

	m_currentAnimationName = name;

	m_pcurrentAnimation->Play();

	return true;
}

SpriteAnimation* RenderableGameObject::GetCurrentAnimation()
{
	return m_pcurrentAnimation;
}

ID3D11ShaderResourceView* RenderableGameObject::GetAlbedoSRV() const
{
	return m_palbedoSRV;
}

ID3D11VertexShader* RenderableGameObject::GetVertexShader() const
{
	return m_pvertexShader;
}

ID3D11PixelShader* RenderableGameObject::GetPixelShader() const
{
	return m_ppixelShader;
}

bool RenderableGameObject::SetMesh(std::wstring meshName)
{
	Mesh* pmesh = GraphicsManager::GetInstance()->GetMesh(meshName);

	if (pmesh == nullptr)
	{
		cout << "Failed to set the mesh as one with that name doesn't exist!" << endl;

		return false;
	}

	m_pmesh = pmesh;

	return true;
}

void RenderableGameObject::SetMesh(Mesh* pmesh)
{
	m_pmesh = pmesh;
}

bool RenderableGameObject::SetAlbedo(std::wstring albedoName)
{
	ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(albedoName);

	if (psRV == nullptr)
	{
		cout << "Failed to set the albedo SRV as one with that name doesn't exist!" << endl;

		return false;
	}

	m_palbedoSRV = psRV;

	return true;
}

void RenderableGameObject::SetAlbedo(ID3D11ShaderResourceView* psRV)
{
	m_palbedoSRV = psRV;
}

bool RenderableGameObject::SetVertexShader(std::wstring shaderName)
{
	ID3D11VertexShader* pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(shaderName);

	if (pvertexShader == nullptr)
	{
		std::cout << "Failed to find vertex shader with that name " << shaderName.c_str() << "!" << std::endl;

		return false;
	}

	m_pvertexShader = pvertexShader;

	return true;
}

void RenderableGameObject::SetVertexShader(ID3D11VertexShader* pvertexShader)
{
	m_pvertexShader = pvertexShader;
}

bool RenderableGameObject::SetPixelShader(std::wstring shaderName)
{
	ID3D11PixelShader* ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(shaderName);

	if (ppixelShader == nullptr)
	{
		LOG("Failed to find pixel shader with that name!");

		return false;
	}

	m_ppixelShader = ppixelShader;

	return true;
}

void RenderableGameObject::SetPixelShader(ID3D11PixelShader* ppixelShader)
{
	m_ppixelShader = ppixelShader;
}

void RenderableGameObject::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

void RenderableGameObject::SetLayer(int layer)
{
	if (layer >= GraphicsManager::GetInstance()->GetNumLayers() || layer < 0)
	{
		LOG("Tried to set gameobject to a layer that doesn't exist!");

		return;
	}

	m_layer = layer;
}

bool RenderableGameObject::AddAnimation(string animName, SpriteAnimation& anim)
{
	if (m_animations.count(animName) != 0)
	{
		LOG("Tried to add an animation but one with that name already exists!");

		return false;
	}

	m_animations[animName] = anim;

	return true;
}

bool RenderableGameObject::AddAnimation(string localAnimName, wstring animName)
{
	SpriteAnimation anim = GraphicsManager::GetInstance()->GetAnimation(animName);

	if (anim.GetFrames() == nullptr)
	{
		std::cout << "Failed to find animation with that name!" << std::endl;

		return false;
	}

	if (m_animations.count(localAnimName) != 0)
	{
		LOG("Tried to add an animation but one with that name already exists!");

		return false;
	}

	m_animations[localAnimName] = anim;

	return true;
}

bool RenderableGameObject::RemoveAnimation(string animName)
{
	if (m_animations.count(animName) == 0)
	{
		LOG("Tried to remove an animation but one with that name doesn't exist!");

		return false;
	}

	if (m_currentAnimationName == animName)
	{
		m_pcurrentAnimation = nullptr;
		m_currentAnimationName = "";
	}

	m_animations.erase(animName);

	return true;
}

bool RenderableGameObject::OverwriteAnimation(string animName, SpriteAnimation& anim)
{
	m_animations[animName] = anim;

	return true;
}

bool RenderableGameObject::IsAnimated()
{
	return m_pcurrentAnimation == nullptr;
}