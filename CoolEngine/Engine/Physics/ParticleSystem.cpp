#include "ParticleSystem.h"
#include "Engine/Physics/ParticleManager.h"
#include "Engine/Managers/GameManager.h"

#include "Engine/EditorUI/EditorUI.h"

ParticleSystem::ParticleSystem(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::PARTICLE_SYSTEM;

	m_systemLife = 0.0f;
	m_timer = 0.0f;
	m_isActive = false;

	// Particle properties
	m_velocity = { 0, 0 };
	m_accel = { 0, 0 };
	m_particleLife = 1.0f;
	m_spawnInterval = 1.0f;
	m_spawnNumber = 1;

	// Randomness
	m_positionRand = 0;
	m_velocityRand = 0;
	m_accelRand = 0;
	m_lifeRand = 0;

	ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(DEFAULT_TILE);
	if (psRV == nullptr)
	{
		LOG("Failed to set the albedo SRV as one with that name doesn't exist!");
		return;
	}
	m_pTexture = psRV;
}

ParticleSystem::ParticleSystem(const nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::PARTICLE_SYSTEM;

	m_systemLife = data["SystemLife"];
	m_timer = data["Timer"];
	m_isActive = data["IsActive"];
	m_velocity = XMFLOAT2(data["ParticleVelocity"][0], data["ParticleVelocity"][1]);
	m_accel = XMFLOAT2(data["ParticleAcceleration"][0], data["ParticleAcceleration"][1]);
	m_particleLife = data["ParticleLife"];
	m_spawnInterval = data["SpawnInterval"];
	m_spawnNumber = data["SpawnNumber"];

	std::string tempPath = data["TexturePath"];

	m_texFilepath = std::wstring(tempPath.begin(), tempPath.end());
	m_positionRand = data["PositionRandom"];
	m_velocityRand = data["VelocityRandom"];
	m_accelRand = data["AccelerationRandom"];
	m_lifeRand = data["LifeRandom"];
	m_layer = data["ParticleLayer"];

	ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(m_texFilepath);

	if (psRV == nullptr)
	{
		LOG("Failed to set the albedo SRV as one with that name doesn't exist!");
		return;
	}

	m_pTexture = psRV;
}

ParticleSystem::ParticleSystem(ParticleSystem const& other) : GameObject(other)
{
	m_systemLife = other.m_systemLife;
	m_timer = other.m_timer;
	m_isActive = other.m_isActive;

	m_velocity = other.m_velocity;
	m_accel = other.m_accel;
	m_particleLife = other.m_particleLife;
	m_spawnInterval = other.m_spawnInterval;
	m_spawnNumber = other.m_spawnNumber;

	m_pTexture = other.m_pTexture;
	m_positionRand = other.m_positionRand;
	m_velocityRand = other.m_velocityRand;
	m_accelRand = other.m_accelRand;
	m_lifeRand = other.m_lifeRand;

	m_texFilepath = other.m_texFilepath;

	m_layer = other.m_layer;
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Serialize(nlohmann::json& data)
{
	GameObject::Serialize(data);

	data["SystemLife"] = m_systemLife;
	data["Timer"] = m_timer;
	data["IsActive"] = m_isActive;
	data["ParticleVelocity"] = { m_velocity.x, m_velocity.y };
	data["ParticleAcceleration"] = { m_accel.x, m_accel.y };
	data["ParticleLife"] = m_particleLife;
	data["SpawnInterval"] = m_spawnInterval;
	data["SpawnNumber"] = m_spawnNumber;
	data["TexturePath"] = std::string(m_texFilepath.begin(), m_texFilepath.end());
	data["PositionRandom"] = m_positionRand;
	data["VelocityRandom"] = m_velocityRand;
	data["AccelerationRandom"] = m_accelRand;
	data["LifeRandom"] = m_lifeRand;
	data["ParticleLayer"] = m_layer;
}

void ParticleSystem::Initialise(Transform trans, float life, std::wstring texPath, int layer)
{
	*m_transform = trans;
	m_transform->UpdateMatrix();

	ID3D11ShaderResourceView* psRV = GraphicsManager::GetInstance()->GetShaderResourceView(texPath);

	if (psRV == nullptr)
	{
		LOG("Failed to set the albedo SRV as one with that name doesn't exist!");
		return;
	}

	m_texFilepath = texPath;
	m_pTexture = psRV;
	m_systemLife = life;
	m_timer = 0.0f;
	m_isActive = true;
	m_layer = layer;
}

void ParticleSystem::Initialise(Transform trans, float life, std::wstring texPath, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, int number, float randPos, float randVel, float randAccel, float randLife, int layer)
{
	Initialise(trans, life, texPath, layer);
	SetProperties(vel, accel, partLife, interval, number);
	SetRandomness(randPos, randVel, randAccel, randLife);
}

void ParticleSystem::SetProperties(XMFLOAT2 vel, XMFLOAT2 accel, float life, float interval, int number)
{
	m_velocity = vel;
	m_accel = accel;
	m_particleLife = life;
	m_spawnInterval = interval;
	m_spawnNumber = number;
}

void ParticleSystem::SetRandomness(float randPos, float randVel, float randAccel, float randLife)
{
	m_positionRand = randPos;
	m_velocityRand = randVel;
	m_accelRand = randAccel;
	m_lifeRand = randLife;
}

void ParticleSystem::Update()
{
	if (m_isActive)
	{
		const float dTime = GameManager::GetInstance()->GetTimer()->DeltaTime();

		// Particle generation process
		if (m_timer >= m_spawnInterval)
		{
			for (unsigned int i = 0; i < m_spawnNumber; ++i)
			{
				ParticleManager::GetInstance()->AddParticle(*m_transform, m_particleLife, m_pTexture, m_velocity, m_accel, m_positionRand, m_velocityRand, m_accelRand, m_lifeRand, m_layer);
			}
			m_timer = 0;
		}
		m_timer += dTime;

		m_systemLife -= dTime;
		if (m_systemLife <= 0.0f)
		{
			m_isActive = false;
		}
	}
}

void ParticleSystem::EditorUpdate()
{
	Update();
}

#if EDITOR
void ParticleSystem::CreateEngineUI()
{
	GameObject::CreateEngineUI();

	ImGui::Spacing();

    auto velocityAccelerationParameters = EditorUIFloatParameters();
    velocityAccelerationParameters.m_columnWidth = 150;
    velocityAccelerationParameters.m_speed = 0.1f;

	EditorUI::DragFloat2("Velocity", m_velocity, velocityAccelerationParameters);
	ImGui::Spacing();
	EditorUI::DragFloat2("Acceleration", m_accel, velocityAccelerationParameters);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

    auto particleParameters = EditorUIFloatParameters();
    particleParameters.m_columnWidth = 150;
    particleParameters.m_speed = 0.02f;
    particleParameters.m_minValue = 0;
    particleParameters.m_maxValue = 10000;

	EditorUI::DragFloat("Particle Lifetime", m_particleLife, particleParameters);
	ImGui::Spacing();
	EditorUI::DragFloat("Particle Interval", m_spawnInterval, particleParameters);

    auto quantityParameters = EditorUIIntParameters();
    quantityParameters.m_columnWidth = 150;
    quantityParameters.m_speed = 0.05f;
    quantityParameters.m_minValue = 0;
    quantityParameters.m_maxValue = 128;

	ImGui::Spacing();
	EditorUI::DragInt("Particle Quantity", m_spawnNumber, quantityParameters);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

    auto spreadParameters = EditorUIFloatParameters();
    spreadParameters.m_columnWidth = 150;
    spreadParameters.m_speed = 0.1f;
    spreadParameters.m_minValue = 0;
    spreadParameters.m_maxValue = 10000;

    auto lifetimeSpreadParameters = EditorUIFloatParameters();
    lifetimeSpreadParameters.m_columnWidth = 150;
    lifetimeSpreadParameters.m_speed = 0.005f;
    lifetimeSpreadParameters.m_minValue = 0;
    lifetimeSpreadParameters.m_maxValue = 10000;

	EditorUI::DragFloat("Position Spread", m_positionRand, spreadParameters);
	ImGui::Spacing();
	EditorUI::DragFloat("Velocity Spread", m_velocityRand, spreadParameters);
	ImGui::Spacing();
	EditorUI::DragFloat("Acceleration Spread", m_accelRand, spreadParameters);
	ImGui::Spacing();
	EditorUI::DragFloat("Lifetime Spread", m_lifeRand, lifetimeSpreadParameters);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::Texture("Texture", m_texFilepath, m_pTexture, 150.0f);
	ImGui::Spacing();

    auto layerParameters = EditorUIIntParameters();
    layerParameters.m_minValue = 0;
    layerParameters.m_maxValue = GraphicsManager::GetInstance()->GetNumLayers();

	EditorUI::DragInt("Layer", m_layer, layerParameters);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::DragFloat("System Lifetime", m_systemLife, spreadParameters);
	ImGui::Spacing();

    auto activeParameters = EditorUINonSpecificParameters();
    activeParameters.m_columnWidth = 150;

	EditorUI::Checkbox("System Active?", m_isActive, activeParameters);

	ImGui::Spacing();
}
const std::wstring& ParticleSystem::GetTexturePath() const
{
	return m_texFilepath;
}
#endif
