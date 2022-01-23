#include "GameObject.h"

#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Physics/Shape.h"
#include "Engine/Physics/Circle.h"
#include "Engine/Physics/Box.h"

#include <iostream>

GameObject::GameObject()
{
	InitGraphics();
}

GameObject::GameObject(string identifier)
{
	m_identifier = identifier;

	InitGraphics();
}

void GameObject::InitGraphics()
{
	m_pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	m_ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(DEFAULT_PIXEL_SHADER_NAME);

	m_pmesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);
}

const bool& GameObject::IsRenderable()
{
	return m_isRenderable;
}

void GameObject::Render(RenderStruct& renderStruct)
{
	//Update CB
	PerInstanceCB cb;
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(m_transform.GetWorldMatrix()));

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

void GameObject::Update()
{
	if (m_pcurrentAnimation != nullptr && m_pcurrentAnimation->GetFrames() != nullptr)
	{
		m_pcurrentAnimation->Update();
	}
}

void GameObject::ShowEngineUI(ID3D11Device* pdevice)
{
	ImGui::Begin("Properties");

	CreateEngineUI(pdevice);

	ImGui::End();
}

void GameObject::CreateEngineUI(ID3D11Device* pdevice)
{
	ImGui::Separator();
	ImGui::Spacing();

	m_transform.CreateEngineUI();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::TextUnformatted("Texture");

	if (ImGui::ImageButton((void*)(intptr_t)m_palbedoSRV, DEFAULT_IMGUI_IMAGE_SIZE))
	{
		EditorUI::OpenFileExplorer(L"DDS files\0*.dds\0", m_texNameBuffer, _countof(m_texNameBuffer));

		wstring relativePath = m_texNameBuffer;

		//Check if that path points to an asset in the resources folder
		int index = relativePath.find(L"Resources");

		if (index == -1)
		{
			LOG("The resource specified isn't stored in a resource folder!");
		}
		else
		{
			//Get relative file path
			relativePath = wstring(m_texNameBuffer).substr(index);

			relativePath.copy(m_texNameBuffer, relativePath.size());

			m_texNameBuffer[relativePath.size()] = L'\0';

			//Load texture if not loaded
			if (GraphicsManager::GetInstance()->IsTextureLoaded(m_texNameBuffer) == true)
			{
				GraphicsManager::GetInstance()->LoadTextureFromFile(m_texNameBuffer);
			}

			m_palbedoSRV = GraphicsManager::GetInstance()->GetShaderResourceView(m_texNameBuffer);
		}
	}

	ImGui::Spacing();

	EditorUI::DragInt("Layer", m_layer, 100.0f, 0.1f, 0, GraphicsManager::GetInstance()->GetNumLayers() - 1);

	ImGui::Spacing();

	EditorUI::Checkbox("Renderable", m_isRenderable);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::TextUnformatted("Animation");

	for (std::unordered_map<std::string, SpriteAnimation>::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		if (ImGui::TreeNode(it->first.c_str()) == true)
		{
			//Animation text
			strcpy_s(m_animName, it->first.c_str());

			if (IMGUI_LEFT_LABEL(ImGui::InputText, "Name", m_animName, ANIM_NAME_SIZE) == true)
			{
				if (m_animations.count(m_animName) == 0)
				{
					m_animUpdateName = it->first;

					m_animNewName = m_animName;

					m_updateAnimName = true;
				}
				else
				{
					LOG("Tried to add an animation with the same local name as one that already exists!");
				}
			}

			//Animation images
			if (ImGui::ImageButton((void*)(intptr_t)it->second.GetCurrentFrame(), DEFAULT_IMGUI_IMAGE_SIZE) == true)
			{
				EditorUI::OpenFolderExplorer(m_animFilepath, _countof(m_animFilepath));

				wstring relativePath = m_animFilepath;

				m_updateAnim = relativePath != L"";

				m_animUpdateName = it->first;
			}

			ImGui::TreePop();
		}
	}

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

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	string currentSelected;

	if (m_collider != nullptr)
	{
		currentSelected = Shape::ShapeTypeToString(m_collider->GetShapeType());
	}
	else
	{
		currentSelected = Shape::ShapeTypeToString(ShapeType::COUNT);
	}

	if (IMGUI_LEFT_LABEL(ImGui::BeginCombo, "Collider", currentSelected.c_str()) == true)
	{
		if (ImGui::Selectable(Shape::ShapeTypeToString(ShapeType::COUNT).c_str(), m_collider == nullptr))
		{
			delete m_collider;
			m_collider = nullptr;
		}
		else if (ImGui::Selectable(Shape::ShapeTypeToString(ShapeType::BOX).c_str(), m_collider->GetShapeType() == ShapeType::BOX))
		{
			delete m_collider;
			m_collider = new Box(&m_transform);
		}
		else if (ImGui::Selectable(Shape::ShapeTypeToString(ShapeType::CIRCLE).c_str(), m_collider->GetShapeType() == ShapeType::CIRCLE))
		{
			delete m_collider;
			m_collider = new Circle(&m_transform, 1.0f);
		}

		ImGui::EndCombo();
	}

	if (m_collider != nullptr)
	{
		m_collider->CreateEngineUI();
	}

	if (m_updateAnimName == true)
	{
		SpriteAnimation tempAnim = m_animations[m_animUpdateName];

		m_animations.erase(m_animUpdateName);

		m_animations.insert(pair<string, SpriteAnimation>(m_animNewName, tempAnim));

		m_updateAnimName = false;
	}

	if (m_updateAnim == true)
	{
		SpriteAnimation anim = GraphicsManager::GetInstance()->GetAnimation(m_animFilepath);

		if (anim.GetFrames() == nullptr)
		{
			if (GraphicsManager::GetInstance()->LoadAnimationFromFile(m_animFilepath) == false)
			{
				m_animations[m_animUpdateName] = GraphicsManager::GetInstance()->GetAnimation(m_animFilepath);

				PlayAnimation(m_animUpdateName);
			}
			else
			{
				LOG("Failed to load the animation!");
			}
		}
		else
		{
			m_animations[m_animUpdateName] = anim;

			PlayAnimation(m_animUpdateName);
		}

		m_updateAnim = false;
	}
}

Mesh* GameObject::GetMesh() const
{
	return m_pmesh;
}

SpriteAnimation& GameObject::GetAnimation(std::string name)
{
	return m_animations[name];
}

unordered_map<std::string, SpriteAnimation>* GameObject::GetAnimations()
{
	return &m_animations;
}

int GameObject::GetLayer() const
{
	return m_layer;
}

bool GameObject::PlayAnimation(std::string name)
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

SpriteAnimation* GameObject::GetCurrentAnimation()
{
	return m_pcurrentAnimation;
}

ID3D11ShaderResourceView* GameObject::GetAlbedoSRV() const
{
	return m_palbedoSRV;
}

ID3D11VertexShader* GameObject::GetVertexShader() const
{
	return m_pvertexShader;
}

ID3D11PixelShader* GameObject::GetPixelShader() const
{
	return m_ppixelShader;
}

Transform* GameObject::GetTransform()
{
	return &m_transform;
}

bool GameObject::SetMesh(std::wstring meshName)
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

void GameObject::SetMesh(Mesh* pmesh)
{
	m_pmesh = pmesh;
}

bool GameObject::SetAlbedo(std::wstring albedoName)
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

void GameObject::SetAlbedo(ID3D11ShaderResourceView* psRV)
{
	m_palbedoSRV = psRV;
}

bool GameObject::SetVertexShader(std::wstring shaderName)
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

void GameObject::SetVertexShader(ID3D11VertexShader* pvertexShader)
{
	m_pvertexShader = pvertexShader;
}

bool GameObject::SetPixelShader(std::wstring shaderName)
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

void GameObject::SetPixelShader(ID3D11PixelShader* ppixelShader)
{
	m_ppixelShader = ppixelShader;
}

void GameObject::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

void GameObject::SetLayer(int layer)
{
	if (layer >= GraphicsManager::GetInstance()->GetNumLayers() || layer < 0)
	{
		LOG("Tried to set gameobject to a layer that doesn't exist!");

		return;
	}

	m_layer = layer;
}

bool GameObject::AddAnimation(string animName, SpriteAnimation& anim)
{
	if (m_animations.count(animName) != 0)
	{
		LOG("Tried to add an animation but one with that name already exists!");

		return false;
	}

	m_animations[animName] = anim;

	return true;
}

bool GameObject::AddAnimation(string localAnimName, wstring animName)
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

bool GameObject::RemoveAnimation(string animName)
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

bool GameObject::OverwriteAnimation(string animName, SpriteAnimation& anim)
{
	m_animations[animName] = anim;

	return true;
}

void GameObject::SetShape(Shape* collider)
{
	m_collider = collider;
}

const string& GameObject::GetIdentifier()
{
	return m_identifier;
}

bool GameObject::IsAnimated()
{
	return m_pcurrentAnimation == nullptr;
}

Shape* GameObject::GetShape()
{
	return m_collider;
}
