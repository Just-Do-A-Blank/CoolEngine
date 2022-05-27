#include "AudioSourceGameObject.h"
#include "Engine/GameObjects/CharacterGameObject.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Includes/FMOD/fmod.hpp"
#include "Engine/Includes/FMOD/fmod_errors.h"

AudioSourceGameObject::AudioSourceGameObject() : TriggerableGameObject()
{
	m_gameObjectType |= GameObjectType::SOUND;
}

AudioSourceGameObject::AudioSourceGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::SOUND;
}

AudioSourceGameObject::AudioSourceGameObject(const nlohmann::json& data, CoolUUID uuid) : TriggerableGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::SOUND;

	m_volume = data["Volume"];
	m_soundName = data["SoundName"];
	m_loop = data["Loop"];
	m_playOnOverlap = data["PlayOnOverlap"];
}

AudioSourceGameObject::AudioSourceGameObject(AudioSourceGameObject const& other) : TriggerableGameObject(other)
{
	m_volume = other.m_volume;
	m_soundName = other.m_soundName;
	m_loop = other.m_loop;
	m_playOnOverlap = other.m_playOnOverlap;
}

const float& AudioSourceGameObject::GetVolume() const
{
	return m_volume;
}

const std::string& AudioSourceGameObject::GetSoundPath() const
{
	return m_soundName;
}

const bool& AudioSourceGameObject::IsLooping() const
{
	return m_loop;
}

void AudioSourceGameObject::SetVolume(float volume)
{
	m_volume = volume;
}

void AudioSourceGameObject::SetSoundPath(std::string path)
{
	m_soundName = path;
}

void AudioSourceGameObject::SetIsLooping(bool looping)
{
	m_loop = looping;
}

void AudioSourceGameObject::Serialize(nlohmann::json& data)
{
	TriggerableGameObject::Serialize(data);

	data["Volume"] = m_volume;
	data["SoundName"] = m_soundName;
	data["Loop"] = m_loop;
	data["PlayOnOverlap"] = m_playOnOverlap;
}

#if EDITOR
void AudioSourceGameObject::CreateEngineUI()
{
	TriggerableGameObject::CreateEngineUI();

	ImGui::Spacing();

	if (EditorUI::CollapsingSection("Audio") == true)
	{
		EditorUI::InputText("Audio Name", m_soundName);

		ImGui::Spacing();

		EditorUIFloatParameters params;
		params.m_minValue = 0.0f;
		params.m_maxValue = 1.0f;

		EditorUI::DragFloat("Volume", m_volume, params);

		ImGui::Spacing();

		EditorUI::Checkbox("Looping", m_loop);

		ImGui::Spacing();

		if (ImGui::Button("Play") == true)
		{
			Play();
		}
	}
}
#endif

void AudioSourceGameObject::Update()
{
	if (m_loop && m_playedOnce == true)
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

bool AudioSourceGameObject::Play()
{
	if (m_soundName != "")
	{
		bool played = AudioManager::GetInstance()->Play(m_soundName, m_transform->GetWorldPosition(), m_volume, &m_pchannel);

		if (played == true)
		{
			m_playedOnce = true;
		}

		return played;
	}
	else
	{
		LOG("Tried to play sound when the sound path hasn't been set");

		return false;
	}
}

void AudioSourceGameObject::OnTriggerHold(GameObject* obj1, GameObject* obj2)
{
	bool isPlaying = false;
	FMOD_RESULT result = FMOD_OK;

	if (m_pchannel != nullptr)
	{
		result = m_pchannel->isPlaying(&isPlaying);
	}

	if (m_playOnOverlap == false || isPlaying == true || result != FMOD_OK)
	{
		return;
	}

	CharacterGameObject* pcharacter = nullptr;

	if (obj1->ContainsType(GameObjectType::CHARACTER) == true)
	{
		pcharacter = dynamic_cast<CharacterGameObject*>(obj1);
	}
	else if (obj2->ContainsType(GameObjectType::CHARACTER) == true)
	{
		pcharacter = dynamic_cast<CharacterGameObject*>(obj2);
	}

	if (pcharacter == nullptr)
	{
		return;
	}

	Play();
}