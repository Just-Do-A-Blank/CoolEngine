#pragma once

#include "Engine\Structure\Manager.h"

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}


#define NUM_FMOD_CHANNELS 512

class AudioManager : public Manager<AudioManager>
{
public:

	bool Init();

	//Music functions
	bool LoadMusic(string name);

	bool PlayMusic(string name, float volume, bool loop);

	//Sound functions
	bool Load(string name);

	bool Play(string name, float volume);
	bool Play(string name, XMFLOAT3 position, float volume, FMOD::Channel** ppchannel = nullptr);

	void Update();

	bool Delete(string name);

	void SetListenerPosition(XMFLOAT3 position);

	bool Destroy();

	bool SetMasterVolume(float volume);
	float GetMasterVolume();

	void Serialize(nlohmann::json& data) override;
	void Deserialize(nlohmann::json& data) override;

protected:

private:
	FMOD::System* m_psystem = nullptr;

	unordered_map<string, FMOD::Sound*> m_sounds;
	unordered_map<string, FMOD::Sound*> m_music;

	FMOD::Channel* m_pmusicChannel = nullptr;
	FMOD::Sound* m_pcurrentMusic = nullptr;

	float m_musicVolume = 0.5f;

	bool m_loopMusic = false;

	float m_masterVolume = 1.0f;
};

