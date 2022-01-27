#include "ParticleManager.h"

#include "Engine/Managers/GameManager.h"

ParticleManager::ParticleManager(wstring meshName, wstring vShaderName, wstring pShaderName)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		string systemName = "particleSystem" + to_string(i);

		m_pParticleSystems[i] = GameManager::GetInstance()->CreateGameObject<ParticleSystem>(systemName);
	}

	Mesh* pmesh = GraphicsManager::GetInstance()->GetMesh(meshName);
	if (pmesh == nullptr)
	{
		LOG("Failed to set the mesh as one with that name doesn't exist!");
	}
	m_pMesh = pmesh;

	ID3D11VertexShader* pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(vShaderName);
	if (pvertexShader == nullptr)
	{
		LOG("Failed to find vertex shader with that name " << vShaderName.c_str() << "!");
	}
	m_pVertexShader = pvertexShader;

	ID3D11PixelShader* ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(pShaderName);
	if (ppixelShader == nullptr)
	{
		LOG("Failed to find pixel shader with that name " << pShaderName.c_str() << "!");
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

void ParticleManager::AddSystem(Transform trans, float life, wstring albedoName, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, float number)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (!m_pParticleSystems[i]->GetActive())
		{
			// Initialise one, then break from loop so more are not made
			ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(albedoName);
			if (psRV == nullptr)
			{
				LOG("Failed to set the albedo SRV as one with that name doesn't exist!");
				return;
			}

			m_pParticleSystems[i]->Initialise(trans, life, psRV, vel, accel, partLife, interval, number);
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
			m_pParticleSystems[i]->Initialise(*system->GetTrans(), system->GetLife(), system->GetTexture());
			break;
		}
	}
}