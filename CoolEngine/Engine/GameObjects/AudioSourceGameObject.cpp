#include "AudioSourceGameObject.h"
#include "Engine/EditorUI/EditorUI.h"

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

	EditorUI::DragFloat("Volume", m_volume, 100, 0.1f, 0, 100);

	ImGui::Spacing();

	EditorUI::Checkbox("Looping", m_loop);

	ImGui::Spacing();

	EditorUI::Checkbox("Enabled", m_enabled);
}

#endif