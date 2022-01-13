#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		m_pParticleSystems[i] = new ParticleSystem();
	}
}

ParticleManager::~ParticleManager()
{
	delete[] m_pParticleSystems;
	delete m_pMesh;

	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
}

void ParticleManager::Update(const float dTime)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (m_pParticleSystems[i]->IsActive())
		{
			m_pParticleSystems[i]->Update(dTime);
		}
	}
}

void ParticleManager::Render(ID3D11DeviceContext* pContext, ConstantBuffer<PerInstanceCB>* pConstantBuffer)
{
	pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);

	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (m_pParticleSystems[i]->IsActive())
		{
			m_pParticleSystems[i]->Render(pContext, pConstantBuffer, m_pMesh);
		}
	}

	// Unbind resources
	pContext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
	pContext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 0, nullptr);
}

void ParticleManager::AddSystem(Transform trans, float life, SYSTEM_TYPE type, wstring albedoName)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (!m_pParticleSystems[i]->IsActive())
		{
			// Initialise one, then break from loop so more are not made
			ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(albedoName);
			if (psRV == nullptr)
			{
				cout << "Failed to set the albedo SRV as one with that name doesn't exist!" << endl;
				return;
			}

			m_pParticleSystems[i]->Initialise(trans, life, type, psRV);
			break;
		}
	}
}

bool ParticleManager::SetVertexShader(wstring shaderName)
{
	ID3D11VertexShader* pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(shaderName);

	if (pvertexShader == nullptr)
	{
		std::cout << "Failed to find vertex shader with that name " << shaderName.c_str() << "!" << std::endl;
		return false;
	}

	m_pVertexShader = pvertexShader;
	return true;
}

bool ParticleManager::SetPixelShader(wstring shaderName)
{
	ID3D11PixelShader* ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(shaderName);

	if (ppixelShader == nullptr)
	{
		std::cout << "Failed to find pixel shader with that name " << shaderName.c_str() << "!" << std::endl;
		return false;
	}

	m_pPixelShader = ppixelShader;
	return true;
}

bool ParticleManager::SetMesh(wstring meshName)
{
	Mesh* pmesh = GraphicsManager::GetInstance()->GetMesh(meshName);

	if (pmesh == nullptr)
	{
		cout << "Failed to set the mesh as one with that name doesn't exist!" << endl;
		return false;
	}

	m_pMesh = pmesh;
	return true;
}