#pragma once

#include "Engine\Structure\Singleton.h"

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}


#define NUM_FMOD_CHANNELS 512

class AudioManager : public Singleton<AudioManager>
{
public:

	bool Init();

	//Music functions
	bool LoadMusic(string relativePath);

	bool PlayMusic(string relativePath, float volume, bool loop);

	//Sound functions
	bool Load(string relativePath);

	bool Play(string relativePath, float volume);
	bool Play(string relativePath, XMFLOAT3 position, float volume);

	void Update();

	bool Delete(string relativePath);

	void SetListenerPosition(XMFLOAT3 position);

	bool Destroy();

protected:

private:
	FMOD::System* m_psystem = nullptr;

	unordered_map<string, FMOD::Sound*> m_sounds;
	unordered_map<string, FMOD::Sound*> m_music;

	FMOD::Channel* m_pmusicChannel = nullptr;
	FMOD::Sound* m_pcurrentMusic = nullptr;

	float m_musicVolume = 0.5f;

	bool m_loopMusic = false;
};

