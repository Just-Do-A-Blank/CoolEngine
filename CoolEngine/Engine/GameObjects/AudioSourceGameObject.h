#pragma once
#include "GameObject.h"
class AudioSourceGameObject : public GameObject
{
public:
	AudioSourceGameObject();
	AudioSourceGameObject(string identifier);

	//Getters
	const float& GetVolume() const;
	const std::string& GetSoundPath() const;
	const bool& IsLooping() const;
	const bool& IsEnabled() const;

	//Setters
	void SetVolume(float volume);
	void SetSoundPath(std::string path);
	void SetIsLooping(bool looping);
	void SetIsEnabled(bool enabled);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	void Update() override;

protected:

private:
	float m_volume = 1.0f;

	std::string m_relativePath = "";

	bool m_loop = false;
	bool m_enabled = false;
};

