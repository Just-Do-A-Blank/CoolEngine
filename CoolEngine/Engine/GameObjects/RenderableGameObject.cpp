#include "RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Graphics/Mesh.h"

RenderableGameObject::RenderableGameObject() : GameObject()
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;
}

RenderableGameObject::RenderableGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;
}

RenderableGameObject::RenderableGameObject(json data, int index) : GameObject(data[index]["Name"], CoolUUID(data[index]["GUID"]))
{
	if (m_gameObjectType == GameObjectType::BASE)
	{
		m_gameObjectType |= GameObjectType::RENDERABLE;
		m_transform->SetPosition(XMFLOAT3(data[index]["Position"][0], data[index]["Position"][1], data[index]["Position"][2]));
		m_transform->SetRotation(XMFLOAT3(data[index]["Rotation"][0], data[index]["Rotation"][1], data[index]["Rotation"][2]));
		m_transform->SetScale(XMFLOAT3(data[index]["Scale"][0], data[index]["Scale"][1], data[index]["Scale"][2]));
		m_layer = data[index]["Layers"];
		InitGraphics();
	}
	else
	{
		InitGraphics();
	}
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
	if (m_palbedoSRV == nullptr)
	{
		return;
	}

	if (m_pcurrentAnimation != nullptr)
	{
		GraphicsManager::GetInstance()->RenderQuad(m_pcurrentAnimation->GetCurrentFrame(), m_transform->GetPosition(), m_transform->GetScale(), m_transform->GetRotation().z, m_layer);
	}
	else
	{
		XMMATRIX worldMatrix = m_transform->GetWorldMatrix();
		XMVECTOR scaleVector;
		XMVECTOR rotationVector;
		XMVECTOR positionVector;
		DirectX::XMMatrixDecompose(&scaleVector, &rotationVector, &positionVector, worldMatrix);

		XMFLOAT3 scale;
		XMFLOAT3 position;
		XMStoreFloat3(&scale, scaleVector);
		XMStoreFloat3(&position, positionVector);

		GraphicsManager::GetInstance()->RenderQuad(m_palbedoSRV, position, scale, m_transform->GetAccumulatedRotation().z, m_layer);
	}
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

SpriteAnimation RenderableGameObject::GetAnimation(std::string name)
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

void RenderableGameObject::Serialize(json& jsonData)
{
	GameObject::Serialize(jsonData);

	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };


	jsonData[std::to_string((int)m_gameObjectType)]["Position"].push_back(position);
	jsonData[std::to_string((int)m_gameObjectType)]["Rotation"].push_back(rotation);
	jsonData[std::to_string((int)m_gameObjectType)]["Scale"].push_back(scale);
	jsonData[std::to_string((int)m_gameObjectType)]["Layers"].push_back(m_layer);
}

void RenderableGameObject::Deserialize(json& jsonData)
{
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