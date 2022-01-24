#include "ParticleManager.h"

ParticleManager::ParticleManager(wstring meshName, wstring vShaderName, wstring pShaderName)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		m_pParticleSystems[i] = new ParticleSystem();
	}

	Mesh* pmesh = GraphicsManager::GetInstance()->GetMesh(meshName);
	if (pmesh == nullptr)
	{
		cout << "Failed to set the mesh as one with that name doesn't exist!" << endl;
	}
	m_pMesh = pmesh;

	ID3D11VertexShader* pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(vShaderName);
	if (pvertexShader == nullptr)
	{
		std::cout << "Failed to find vertex shader with that name " << vShaderName.c_str() << "!" << std::endl;
	}
	m_pVertexShader = pvertexShader;

	ID3D11PixelShader* ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(pShaderName);
	if (ppixelShader == nullptr)
	{
		std::cout << "Failed to find pixel shader with that name " << pShaderName.c_str() << "!" << std::endl;
	}

	m_pPixelShader = ppixelShader;
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
		if (m_pParticleSystems[i]->GetActive())
		{
			m_pParticleSystems[i]->Update(dTime);
		}
	}
}

void ParticleManager::Render(ID3D11DeviceContext* pContext)
{
	pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);

	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (m_pParticleSystems[i]->GetActive())
		{
			m_pParticleSystems[i]->Render(pContext, m_pMesh);
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
		if (!m_pParticleSystems[i]->GetActive())
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

void ParticleManager::AddSystem(ParticleSystem* system)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (!m_pParticleSystems[i]->GetActive())
		{
			m_pParticleSystems[i]->Initialise(*system->GetTrans(), system->GetLife(), system->GetType(), system->GetTexture());
			break;
		}
	}
}