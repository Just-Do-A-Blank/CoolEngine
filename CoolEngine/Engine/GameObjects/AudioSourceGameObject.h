#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

namespace FMOD
{
	class Channel;
}

class AudioSourceGameObject : public TriggerableGameObject
{
public:
	AudioSourceGameObject();
	AudioSourceGameObject(string identifier, CoolUUID uuid);
	AudioSourceGameObject(const nlohmann::json& data, CoolUUID uuid);
	AudioSourceGameObject(AudioSourceGameObject const& other);

	//Getters
	const float& GetVolume() const;
	const std::string& GetSoundPath() const;
	const bool& IsLooping() const;

	//Setters
	void SetVolume(float volume);
	void SetSoundPath(std::string path);
	void SetIsLooping(bool looping);

	void Serialize(nlohmann::json& data) override;

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	void Update() override;

	bool Play();

protected:
	void OnTriggerHold(GameObject* obj1, GameObject* obj2) override;

private:
	float m_volume = 1.0f;

	std::string m_soundName = "";

	bool m_loop = false;
	bool m_playOnOverlap = true;

	FMOD::Channel* m_pchannel = nullptr;
};

