#include "GameObject.h"

#include "Engine/Managers/GraphicsManager.h"

#include <iostream>

void GameObject::CreateRenderableGameObject()
{
	m_isRenderable = true;
}

void GameObject::CreateNonRenderableGameObject()
{
	m_isRenderable = false;
}

void GameObject::AddCollision()
{
	m_isCollidable = true;
}

void GameObject::AddTrigger()
{
	m_isTrigger = true;
}

const bool& GameObject::IsRenderable()
{
	return m_isRenderable;
}

const bool& GameObject::IsCollidable()
{
	return m_isCollidable;
}

const bool& GameObject::IsTrigger()
{
	return m_isTrigger;
}

void GameObject::Render(ID3D11DeviceContext* pcontext, ConstantBuffer<PerInstanceCB>* pconstantBuffer)
{
	//Update CB
	PerInstanceCB cb;
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(m_transform.GetWorldMatrix()));

	pconstantBuffer->Update(cb, pcontext);

	ID3D11Buffer* pcbBuffer = pconstantBuffer->GetBuffer();

	//Bind CB and appropriate resources
	pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);
	pcontext->VSSetShader(m_pvertexShader, nullptr, 0);

	pcontext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);
	pcontext->PSSetShaderResources(0, 1, &m_palbedoSRV);
	pcontext->PSSetShader(m_ppixelShader, nullptr, 0);

	//Draw object
	pcontext->DrawIndexed(m_pmesh->GetIndexCount(), 0, 0);

	//Unbind resources
	pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
	pcontext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
}

Mesh* GameObject::GetMesh() const
{
	return m_pmesh;
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
		std::cout << "Failed to find pixel shader with that name " << shaderName.c_str() << "!" << std::endl;

		return false;
	}

	m_ppixelShader = ppixelShader;

	return true;
}

void GameObject::SetPixelShader(ID3D11PixelShader* ppixelShader)
{
	m_ppixelShader = ppixelShader;
}
