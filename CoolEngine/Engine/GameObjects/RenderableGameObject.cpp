#include "RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/AnimationState.h"

RenderableGameObject::RenderableGameObject() : PrefabGameObject()
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;

	m_panimationStateMachine = new AnimationStateMachine();
}

RenderableGameObject::RenderableGameObject(RenderableGameObject const& other) : PrefabGameObject(other)
{
	GameObject::Init(other);

	m_palbedoSRV = other.m_palbedoSRV;
	m_pvertexShader = other.m_pvertexShader;
	m_ppixelShader = other.m_ppixelShader;

	if (other.m_pmesh)
	{
		m_pmesh = new Mesh(*other.m_pmesh);
	}
	m_layer = other.m_layer;
	m_isRenderable = other.m_isRenderable;

	m_texFilepath = other.m_texFilepath;

	m_panimationStateMachine = new AnimationStateMachine(other.m_panimationStateMachine);
}

RenderableGameObject::RenderableGameObject(string identifier, CoolUUID uuid) : PrefabGameObject(identifier, uuid)
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;

	m_panimationStateMachine = new AnimationStateMachine();
}

RenderableGameObject::RenderableGameObject(const nlohmann::json& data, CoolUUID uuid) : PrefabGameObject(data, uuid)
{
    GameObject::Init(data, uuid);

    if (IsPrefab())
    {
        LoadLocalData(GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }

	m_gameObjectType |= GameObjectType::RENDERABLE;

	InitGraphics();

}

RenderableGameObject::~RenderableGameObject()
{
	if (m_panimationStateMachine != nullptr)
	{
		delete m_panimationStateMachine;
		m_panimationStateMachine = nullptr;
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
	if (!m_isRenderable)
	{
		return;
	}

	AnimationState* pstate = (AnimationState*)m_panimationStateMachine->GetActiveState();

	if (m_panimationStateMachine->GetActiveState() && pstate->GetAnimation() != nullptr)
	{
		GraphicsManager::GetInstance()->RenderQuad(pstate->GetAnimation()->GetCurrentFrame(), m_transform->GetWorldPosition(), m_transform->GetWorldScale(), m_transform->GetWorldRotation().z, m_layer);
	}
	else
	{
		if (m_palbedoSRV == nullptr)
		{
			return;
		}

		GraphicsManager::GetInstance()->RenderQuad(m_palbedoSRV, m_transform->GetWorldPosition(), m_transform->GetWorldScale(), m_transform->GetWorldRotation().z, m_layer);
	}
}

void RenderableGameObject::Update()
{
	m_panimationStateMachine->Update();
}

void RenderableGameObject::EditorUpdate()
{
}


#if EDITOR
void RenderableGameObject::CreateEngineUI()
{
    PrefabGameObject::CreateEngineUI();

	if (EditorUI::CollapsingSection("Renderable"))
	{
		ImGui::Spacing();

		EditorUI::Texture("Texture", m_texFilepath, m_palbedoSRV);

		ImGui::Spacing();

		EditorUIIntParameters layerParameters = EditorUIIntParameters();
		layerParameters.m_minValue = 0;
		layerParameters.m_maxValue = GraphicsManager::GetInstance()->GetNumLayers() - 1;

		EditorUI::DragInt("Layer", m_layer, layerParameters);

		ImGui::Spacing();

		EditorUI::Checkbox("Renderable", m_isRenderable);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		m_panimationStateMachine->CreateEngineUI();
	}

}
#endif

Mesh* RenderableGameObject::GetMesh() const
{
	return m_pmesh;
}

SpriteAnimation RenderableGameObject::GetAnimation(std::string name)
{
	AnimationState* pstate = (AnimationState*)m_panimationStateMachine->GetState(name);

	if (pstate == nullptr)
	{
		return SpriteAnimation(nullptr, L"");
	}

	return *pstate->GetAnimation();
}

int RenderableGameObject::GetLayer() const
{
	return m_layer;
}

void RenderableGameObject::PlayAnimation()
{
	AnimationState* pstate = (AnimationState*)m_panimationStateMachine->GetActiveState();

	if (pstate == nullptr)
	{
		LOG("Tried to play an animation when the object doesn't have any!");

		return;
	}

	pstate->Play();
}

void RenderableGameObject::PauseAnimation()
{
	AnimationState* pstate = (AnimationState*)m_panimationStateMachine->GetActiveState();

	if (pstate == nullptr)
	{
		LOG("Tried to pause an animation when the object doesn't have any!");

		return;
	}

	pstate->Pause();
}

AnimationStateMachine* RenderableGameObject::GetAnimationStateMachine()
{
	return m_panimationStateMachine;
}

void RenderableGameObject::Serialize(nlohmann::json& data)
{
    PrefabGameObject::Serialize(data);

    SaveLocalData(data);
}

void RenderableGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("Layers"))
    {
        m_layer = jsonData["Layers"];
        m_isRenderable = jsonData["IsRenderable"];

        std::string tempPath = jsonData["TexturePath"];
        m_texFilepath = std::wstring(tempPath.begin(), tempPath.end());

        SetAlbedo(m_texFilepath);

        if (m_panimationStateMachine == nullptr)
        {
            delete m_panimationStateMachine;
        }

        m_panimationStateMachine = new AnimationStateMachine();
        m_panimationStateMachine->Deserialize(jsonData);
    }
}

void RenderableGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    std::string texPath = std::string(m_texFilepath.begin(), m_texFilepath.end());

    jsonData["Layers"] = m_layer;
    jsonData["IsRenderable"] = m_isRenderable;
    jsonData["TexturePath"] = texPath;

    m_panimationStateMachine->Serialize(jsonData);
}

void RenderableGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    PrefabGameObject::LoadAllPrefabData(jsonData);
    LoadLocalData(jsonData);
}

void RenderableGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    PrefabGameObject::SaveAllPrefabData(jsonData);
}

const SpriteAnimation* RenderableGameObject::GetCurrentAnimation()
{
	const AnimationState* pstate = (const AnimationState*)m_panimationStateMachine->GetActiveState();

	if (pstate == nullptr)
	{
		LOG("Tried to get the current animation of an object that doesn't have one!");

		return nullptr;
	}

	return pstate->GetAnimation();
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

	m_texFilepath = albedoName;

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

bool RenderableGameObject::AddAnimationState(string stateName, AnimationState* panimState, bool isActive)
{
	if (m_panimationStateMachine->AddState(stateName, panimState) == false)
	{
		return false;
	}

	if (isActive == true)
	{
		m_panimationStateMachine->SetActiveState(stateName);
	}

	return true;
}

bool RenderableGameObject::RemoveAnimationState(string stateName)
{
	return m_panimationStateMachine->RemoveState(stateName);
}

bool RenderableGameObject::IsAnimated()
{
	AnimationState* pstate = (AnimationState*)m_panimationStateMachine->GetActiveState();

	if (pstate == nullptr)
	{
		return false;
	}

	return pstate->GetAnimation()->IsPaused() == false;
}
