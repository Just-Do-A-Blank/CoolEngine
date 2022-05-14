#include "ParticleManager.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Managers/GameManager.h"

ParticleManager::ParticleManager()
{
	for (unsigned int i = 0; i < PARTICLE_SIZE; ++i)
	{
		m_pParticles[i] = new Particle();
	}
	
	Mesh* pmesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);
	if (pmesh == nullptr)
	{
		LOG("Failed to set the mesh as one with that name doesn't exist!");
	}
	m_pMesh = pmesh;

	ID3D11VertexShader* pvertexShader = GraphicsManager::GetInstance()->GetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	if (pvertexShader == nullptr)
	{
		LOG("Failed to find vertex shader" << "!");
	}
	m_pVertexShader = pvertexShader;

	ID3D11PixelShader* ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	if (ppixelShader == nullptr)
	{
		LOG("Failed to find pixel shader" << "!");
	}

	m_pPixelShader = ppixelShader;
}

ParticleManager::~ParticleManager()
{
	delete[] m_pParticles;
	m_pParticleSystems.clear();
	delete m_pMesh;

	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
}

void ParticleManager::Update(const float dTime)
{
	for (unsigned int i = 0; i < PARTICLE_SIZE; ++i)
	{
		if (m_pParticles[i]->GetActive())
		{
			m_pParticles[i]->Update(dTime);
		}
	}
}

void ParticleManager::Render(ID3D11DeviceContext* pContext)
{
	for (unsigned int i = 0; i < PARTICLE_SIZE; ++i)
	{
		if (m_pParticles[i]->GetActive())
		{
			m_pParticles[i]->Render(pContext, m_pMesh);
		}
	}
}

void ParticleManager::AddSystem(Transform trans, float life, wstring albedoName, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, float number, float randPos, float randVel, float randAccel, float randLife, int layer)
{
	string systemName = "particleSystem" + to_string(m_pParticleSystems.size());
	m_pParticleSystems.push_back( GameManager::GetInstance()->CreateGameObject<ParticleSystem>(systemName) );

	ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(albedoName);
	if (psRV == nullptr)
	{
		LOG("Failed to set the albedo SRV as one with that name doesn't exist!");
		return;
	}
	m_pParticleSystems.at(m_pParticleSystems.size() - 1)->Initialise(trans, life, psRV, vel, accel, partLife, interval, number, randPos, randVel, randAccel, randLife, layer);
}

void ParticleManager::AddSystem(ParticleSystem* system)
{
	string systemName = "particleSystem" + to_string(m_pParticleSystems.size());
	m_pParticleSystems.push_back(GameManager::GetInstance()->CreateGameObject<ParticleSystem>(systemName));

	m_pParticleSystems.at(m_pParticleSystems.size() - 1)->Initialise(	*system->GetTrans(), system->GetLife(), system->GetTexture(),
																		system->GetVelocity(), system->GetAccel(), system->GetParticleLife(), system->GetSpawnInterval(), system->GetSpawnNumber(),
																		system->GetRandomPos(), system->GetRandomVelocity(), system->GetRandomAccel(), system->GetRandomLife(), system->GetLayer());
}

void ParticleManager::AddParticle(Transform trans, float life, ID3D11ShaderResourceView* tex, XMFLOAT2 vel, XMFLOAT2 accel, float randPos, float randVel, float randAccel, float randLife, int layer)
{
	for (unsigned int i = 0; i < PARTICLE_SIZE; ++i)
	{
		if (!m_pParticles[i]->GetActive())
		{
			// Initialise one, then break from loop so more are not made
			if (randPos != 0.0f)
			{
				XMFLOAT3 pos = trans.GetWorldPosition();
				float rand = MathHelper::RandomNumber(-abs(randPos), abs(randPos));
				pos.x += rand;
				rand = MathHelper::RandomNumber(-abs(randPos), abs(randPos));
				pos.y += rand;
				trans.SetWorldPosition(pos);
			}
			if (randVel != 0.0f)
			{
				float rand = MathHelper::RandomNumber(-abs(randVel), abs(randVel));
				vel.x += rand;
				rand = MathHelper::RandomNumber(-abs(randVel), abs(randVel));
				vel.y += rand;
			}
			if (randAccel != 0.0f)
			{
				float rand = MathHelper::RandomNumber(-abs(randAccel), abs(randAccel));
				accel.x += rand;
				rand = MathHelper::RandomNumber(-abs(randAccel), abs(randAccel));
				accel.y += rand;
			}
			if (randLife != 0.0f)
			{
				float rand = MathHelper::RandomNumber(-abs(randLife), abs(randLife));
				life += rand;
				if (life < 0.0f)
				{
					life = 0.0f;
				}
			}

			m_pParticles[i]->Initialise(trans, vel, accel, life, tex, layer);

			break;
		}
	}
}