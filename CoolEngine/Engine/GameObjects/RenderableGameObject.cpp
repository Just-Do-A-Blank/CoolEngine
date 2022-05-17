#include "RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Graphics/Mesh.h"

RenderableGameObject::RenderableGameObject() : GameObject()
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;
}

RenderableGameObject::RenderableGameObject(RenderableGameObject const& other) : GameObject(other)
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
	for (int i = 0; i < ANIM_NAME_SIZE; ++i)
	{
		m_createDeleteAnimName[i] = other.m_createDeleteAnimName[i];
	}
	m_animations = other.m_animations;

	if (other.m_pcurrentAnimation)
	{
		m_pcurrentAnimation = new SpriteAnimation(*other.m_pcurrentAnimation);
	}
	m_currentAnimationName = other.m_currentAnimationName;

}

RenderableGameObject::RenderableGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{
	InitGraphics();

	m_gameObjectType |= GameObjectType::RENDERABLE;
}

RenderableGameObject::RenderableGameObject(const nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
	GameObject::Init(data, uuid);

	m_layer = data["Layers"];
	m_isRenderable = data["IsRenderable"];

	std::string tempPath = data["TexturePath"];
	m_texFilepath = std::wstring(tempPath.begin(), tempPath.end());

	SetAlbedo(m_texFilepath);

	for (int i = 0; i < data["AnimNames"].size(); ++i)
	{
		std::string tempAnimPath = data["AnimPaths"][i];

		AddAnimation(data["AnimNames"][i], std::wstring(tempAnimPath.begin(), tempAnimPath.end()));
	}

	m_gameObjectType |= GameObjectType::RENDERABLE;

	InitGraphics();

}

RenderableGameObject::~RenderableGameObject()
{
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
	if (m_isRenderable == false)
	{
		return;
	}

	if (m_pcurrentAnimation != nullptr)
	{
		GraphicsManager::GetInstance()->RenderQuad(m_pcurrentAnimation->GetCurrentFrame(), m_transform->GetWorldPosition(), m_transform->GetWorldScale(), m_transform->GetWorldRotation().z, m_layer);
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
	if (m_pcurrentAnimation != nullptr && m_pcurrentAnimation->GetFrames() != nullptr)
	{
		m_pcurrentAnimation->Update();
	}
}

void RenderableGameObject::EditorUpdate()
{
}


#if EDITOR
void RenderableGameObject::CreateEngineUI()
{
	GameObject::CreateEngineUI();

	if (EditorUI::CollapsingSection("Renderable", true))
	{
		ImGui::Spacing();

		EditorUI::Texture("Texture", m_texFilepath, m_palbedoSRV);

		ImGui::Spacing();

		auto layerParameters = EditorUIIntParameters();
		layerParameters.m_minValue = 0;
		layerParameters.m_maxValue = GraphicsManager::GetInstance()->GetNumLayers() - 1;

		EditorUI::DragInt("Layer", m_layer, layerParameters);

		ImGui::Spacing();

		EditorUI::Checkbox("Renderable", m_isRenderable);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		CreateAnimationEditorUI();
	}

	
}

/// <summary>
/// Creates the animation editor section of the renderable UI
/// </summary>
void RenderableGameObject::CreateAnimationEditorUI()
{
	if (EditorUI::CollapsingSection("Animations", false))
	{
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

void RenderableGameObject::Serialize(nlohmann::json& data)
{
	GameObject::Serialize(data);

	std::string texPath = std::string(m_texFilepath.begin(), m_texFilepath.end());

	data["Layers"] = m_layer;
	data["IsRenderable"] = m_isRenderable;
	data["TexturePath"] = texPath;

	data["AnimNames"] = {};
	data["AnimPaths"] = {};

	for (std::unordered_map<std::string, SpriteAnimation>::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		data["AnimNames"].push_back(it->first);

		std::wstring animPath = it->second.GetAnimPath();
		std::string tempAnimPath = std::string(animPath.begin(), animPath.end());

		data["AnimPaths"].push_back(tempAnimPath);
	}
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