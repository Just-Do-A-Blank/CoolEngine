#include "Engine/Managers/AudioManager.h"
#include "Engine/Includes/FMOD/fmod.hpp"
#include "Engine/Includes/FMOD/fmod_errors.h"
#include "Engine/Managers/GameManager.h"

bool AudioManager::Init()
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	 FMOD_RESULT result = FMOD::System_Create(&m_psystem, FMOD_VERSION);

	if (result != FMOD_OK)
	{
		LOG("Failed to create fmod system with error " + string(FMOD_ErrorString(result)));

		return false;
	}

	result = m_psystem->init(NUM_FMOD_CHANNELS, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK)
	{
		LOG("Failed to init fmod system with error " + string(FMOD_ErrorString(result)));

		return false;
	}

	m_psystem->set3DNumListeners(1);

	return true;
}

bool AudioManager::LoadMusic(string name)
{
	if (m_music.count(name) != 0)
	{
		LOG("Tried to load music that has already been loaded!");

		return false;
	}

	std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Audio\\Music\\" + name;

	FMOD::Sound* sound;

	FMOD_RESULT result = m_psystem->createStream(path.c_str(), FMOD_DEFAULT, 0, &sound);

	if (result != FMOD_OK)
	{
		LOG("Failed to load music with error " + string(FMOD_ErrorString(result)));

		return false;
	}

	m_music[name] = sound;

	return true;
}

bool AudioManager::PlayMusic(string name, float volume, bool loop)
{
	if (volume < 0 || volume > 1.0f)
	{
		LOG("All values for volume must be between 0 and 1!");

		return false;
	}

	if (m_music.count(name) == 0)
	{
		if (GetInstance()->LoadMusic(name) == false)
		{
			return false;
		}
	}

	FMOD::Channel* pchannel;

	m_psystem->playSound(m_music[name], nullptr, true, &m_pmusicChannel);

	m_pmusicChannel->setVolume(m_masterVolume * volume);

	m_pmusicChannel->setPaused(false);

	m_pcurrentMusic = m_music[name];

	m_loopMusic = loop;

	m_musicVolume = volume;

	return true;
}

bool AudioManager::Load(string name)
{
	if (m_sounds.count(name) != 0)
	{
		LOG("Tried to load a sound that has already been loaded!");

		return true;
	}

	std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Audio\\" + name;

	FMOD::Sound* sound;

	FMOD_RESULT result = m_psystem->createSound(path.c_str(), FMOD_3D, 0, &sound);

	if (result != FMOD_OK)
	{
		LOG("Failed to load sound with error " + string(FMOD_ErrorString(result)));

		return false;
	}

	m_sounds[name] = sound;

	return true;
}

bool AudioManager::Play(string name, float volume)
{
	if (volume < 0 || volume > 1.0f)
	{
		LOG("All values for volume must be between 0 and 1!");

		return false;
	}

	if (m_sounds.count(name) == 0)
	{
		if (GetInstance()->Load(name) == false)
		{
			return false;
		}
	}

	FMOD::Channel* pchannel;

	m_psystem->playSound(m_sounds[name], nullptr, true, &pchannel);

	pchannel->setVolume(m_masterVolume * volume);

	pchannel->setPaused(false);

	return true;
}

bool AudioManager::Play(string name, XMFLOAT3 position, float volume, FMOD::Channel** ppchannel)
{
	if (volume < 0 || volume > 1.0f)
	{
		LOG("All values for volume must be between 0 and 1!");

		return false;
	}

	if (m_sounds.count(name) == 0)
	{
		if (GetInstance()->Load(name) == false)
		{
			return false;
		}
	}

	FMOD::Channel* pchannel;

	m_psystem->playSound(m_sounds[name], nullptr, true, &pchannel);

	pchannel->setMode(FMOD_3D);
	pchannel->setVolume(m_masterVolume * volume);

	FMOD_VECTOR pos;
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	pchannel->set3DAttributes(&pos, nullptr);

	pchannel->setPaused(false);

	if (ppchannel != nullptr)
	{
		*ppchannel = pchannel;
	}

	return true;
}

void AudioManager::Update()
{
	m_psystem->update();

	if (m_loopMusic == true)
	{
		bool isPlaying;
		m_pmusicChannel->isPlaying(&isPlaying);

		if (isPlaying == false)
		{
			m_psystem->playSound(m_pcurrentMusic, nullptr, true, &m_pmusicChannel);

			m_pmusicChannel->setVolume(m_masterVolume * m_musicVolume);

			m_pmusicChannel->setPaused(false);
		}
	}
}

bool AudioManager::Delete(string name)
{
	if (m_sounds.count(name) == 0)
	{
		LOG("Tried to delete a sound that hasn't been loaded!");

		return false;
	}

	m_sounds[name]->release();

	m_sounds.erase(name);

	return false;
}

void AudioManager::SetListenerPosition(XMFLOAT3 position)
{
	FMOD_VECTOR pos;
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	FMOD_RESULT result = m_psystem->set3DListenerAttributes(0, &pos, nullptr, nullptr, nullptr);

	if (result != FMOD_OK)
	{
		LOG("Failed to set the listeners position with the error " + string(FMOD_ErrorString(result)));
	}
}

bool AudioManager::Destroy()
{
	FMOD_RESULT result = m_psystem->release();

	if (result != FMOD_OK)
	{
		LOG("Failed to release FMOD system with error " + string(FMOD_ErrorString(result)));

		return false;
	}

	m_psystem = nullptr;

	CoUninitialize();

	return true;
}

bool AudioManager::SetMasterVolume(float volume)
{
	if (volume < 0 || volume > 1.0f)
	{
		LOG("All values for volume must be between 0 and 1!");

		return false;
	}

	m_masterVolume = volume;

	return true;
}

float AudioManager::GetMasterVolume()
{
	return m_masterVolume;
}

void AudioManager::Serialize(nlohmann::json& data)
{
	for (std::unordered_map<string, FMOD::Sound*>::iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
	{
		data["AudioManager"]["Sounds"].push_back(it->first);
	}

	for (std::unordered_map<string, FMOD::Sound*>::iterator it = m_music.begin(); it != m_music.end(); ++it)
	{
		data["AudioManager"]["Music"].push_back(it->first);
	}
}

void AudioManager::Deserialize(nlohmann::json& data)
{
	for (int i = 0; i < data["AudioManager"]["Sounds"].size(); ++i)
	{
		Load(data["AudioManager"]["Sounds"][i]);
	}

	for (int i = 0; i < data["AudioManager"]["Music"].size(); ++i)
	{
		LoadMusic(data["AudioManager"]["Music"][i]);
	}
}
