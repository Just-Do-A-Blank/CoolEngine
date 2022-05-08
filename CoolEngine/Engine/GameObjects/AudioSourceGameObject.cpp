#include "AudioSourceGameObject.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Includes/FMOD/fmod.hpp"
#include "Engine/Includes/FMOD/fmod_errors.h"

AudioSourceGameObject::AudioSourceGameObject() : GameObject()
{
	m_gameObjectType |= GameObjectType::SOUND;
}

AudioSourceGameObject::AudioSourceGameObject(string identifier) : GameObject(identifier)
{
	m_gameObjectType |= GameObjectType::SOUND;
}

const float& AudioSourceGameObject::GetVolume() const
{
	return m_volume;
}

const std::string& AudioSourceGameObject::GetSoundPath() const
{
	return m_relativePath;
}

const bool& AudioSourceGameObject::IsLooping() const
{
	return m_loop;
}

const bool& AudioSourceGameObject::IsEnabled() const
{
	return m_enabled;
}

void AudioSourceGameObject::SetVolume(float volume)
{
	m_volume = volume;
}

void AudioSourceGameObject::SetSoundPath(std::string path)
{
	m_relativePath = path;
}

void AudioSourceGameObject::SetIsLooping(bool looping)
{
	m_loop = looping;
}

void AudioSourceGameObject::SetIsEnabled(bool enabled)
{
	m_enabled = enabled;
}

#if EDITOR

void AudioSourceGameObject::CreateEngineUI()
{
	GameObject::CreateEngineUI();

	ImGui::Spacing();

	EditorUI::RelativePath("Audio Path", m_relativePath);

	ImGui::Spacing();

	EditorUI::DragFloat("Volume", m_volume, 100, 0.01f, 0, 1);

	ImGui::Spacing();

	EditorUI::Checkbox("Looping", m_loop);

	ImGui::Spacing();

	EditorUI::Checkbox("Enabled", m_enabled);

	ImGui::Spacing();

	if (ImGui::Button("Play") == true)
	{
		Play();
	}

	ImGui::Spacing();
}

void AudioSourceGameObject::Update()
{
	if (m_enabled == false)
	{
		return;
	}

	if (m_loop)
	{
		bool isPlaying = false;

		if (m_pchannel != nullptr)
		{
			m_pchannel->isPlaying(&isPlaying);
		}

		if (m_pchannel == nullptr || isPlaying == false)
		{
			Play();
		}
	}
}

void AudioSourceGameObject::Play()
{
	if (m_relativePath != "")
	{
		AudioManager::GetInstance()->Play(m_relativePath, m_transform->GetPosition(), m_volume, &m_pchannel);
	}
	else
	{
		LOG("Tried to play sound when the sound path hasn't been set");
	}
}

#endif