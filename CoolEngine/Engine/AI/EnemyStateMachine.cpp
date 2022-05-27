#include "EnemyStateMachine.h"
#include "FuzzyStateMachine.h"
#include "Engine/AI/States/FuzzyState.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/AI/States/MeleeAttackState.h"
#include "Engine/AI/States/MeleeMovementState.h"
#include "Engine/AI/States/RangeAttackState.h"
#include "Engine/AI/States/RangeMovementState.h"
#include "Engine/AI/States/WanderState.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Includes/json.hpp"

#include <fstream>

EnemyStateMachine::EnemyStateMachine(EnemyGameObject* penemy) : FuzzyStateMachine()
{
	m_penemy = penemy;
}

EnemyStateMachine::EnemyStateMachine(const nlohmann::json& data, EnemyGameObject* penemy)
{
	m_penemy = penemy;

	Deserialize(data);
}

EnemyStateMachine::EnemyStateMachine(EnemyStateMachine const* other, EnemyGameObject* penemy)
{
	m_penemy = penemy;

	m_AIName = other->m_AIName;
	m_editorAIName = other->m_editorAIName;

	for (int i = 0; i < other->m_states.size(); ++i)
	{
		switch (other->m_states[i]->GetStateType())
		{
		case FuzzyStateType::MELEE_ATTACK:
			m_states.push_back(new MeleeAttackState((const MeleeAttackState*)other->m_states[i], m_penemy));
			break;

		case FuzzyStateType::MELEE_MOVEMENT:
			m_states.push_back(new MeleeMovementState((const MeleeMovementState*)other->m_states[i], m_penemy));
			break;

		case FuzzyStateType::RANGE_ATTACK:
			m_states.push_back(new RangeAttackState((const RangeAttackState*)other->m_states[i], m_penemy));
			break;

		case FuzzyStateType::RANGE_MOVEMENT:
			m_states.push_back(new RangeMovementState((const RangeMovementState*)other->m_states[i], m_penemy));
			break;

		case FuzzyStateType::WANDER:
			m_states.push_back(new WanderState((const WanderState*)other->m_states[i], m_penemy));
			break;
		}
	}
}

void EnemyStateMachine::Serialize(nlohmann::json& data)
{
	data["EnemyStateMachine"]["AIName"] = m_AIName;
}

void EnemyStateMachine::Deserialize(const nlohmann::json& data)
{
	m_AIName = data["EnemyStateMachine"]["AIName"];
	m_editorAIName = m_AIName;

	if (m_editorAIName != "")
	{
		Load();
	}
}

bool EnemyStateMachine::IsStateActive(FuzzyStateType type) const
{
	for (int i = 0; i < m_activeStates.size(); ++i)
	{
		if (m_activeStates[i]->GetStateType() == type)
		{
			return true;
		}
	}

	return false;
}

#if EDITOR
void EnemyStateMachine::CreateEngineUI()
{
	if (EditorUI::CollapsingSection("AI"))
	{
		for (int i = 0; i < m_states.size(); ++i)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

			if (m_selectedStateType == m_states[i]->GetStateType())
			{
				flags |= ImGuiTreeNodeFlags_Selected;
			}

			bool treeNodeOpen = ImGui::TreeNodeEx(FuzzyState::StateTypeToString(m_states[i]->GetStateType()).c_str(), flags);

			if (treeNodeOpen == true)
			{
				if (ImGui::IsItemClicked() == true)
				{
					if (m_selectedStateType == m_states[i]->GetStateType())
					{
						m_selectedStateType = FuzzyStateType::COUNT;
					}
					else
					{
						m_selectedStateType = m_states[i]->GetStateType();
					}
				}

				m_states[i]->CreateEngineUI();

				ImGui::TreePop();
			}

			if (treeNodeOpen == false && ImGui::IsItemClicked() == true)
			{
				if (m_selectedStateType == m_states[i]->GetStateType())
				{
					m_selectedStateType = FuzzyStateType::COUNT;
				}
				else
				{
					m_selectedStateType = m_states[i]->GetStateType();
				}
			}
		}

		ImGui::Spacing();

		std::list<std::string> typeNames;
		
		for (int i = 0; i < (int)FuzzyStateType::COUNT; ++i)
		{
			typeNames.push_back(FuzzyState::s_typeNames[i]);
		}

		std::string selectedName = FuzzyState::StateTypeToString(m_comboBoxSelectedStateType);

		if (EditorUI::ComboBox("State Type", typeNames, selectedName) == true)
		{
			m_comboBoxSelectedStateType = FuzzyState::StringToStateType(selectedName);
		}

		EditorButtonCallback callback = EditorUI::BasicDuelButtons("Create", "Delete");

		//Create button
		if (callback.m_leftButton == true)
		{
			bool exists = false;

			for (int i = 0; i < m_states.size(); ++i)
			{
				if (m_states[i]->GetStateType() == m_comboBoxSelectedStateType)
				{
					exists = true;
					break;
				}
			}

			if (exists == false)
			{
				switch (m_comboBoxSelectedStateType)
				{
				case FuzzyStateType::MELEE_ATTACK:
					m_states.push_back(new MeleeAttackState(m_penemy));
					break;

				case FuzzyStateType::MELEE_MOVEMENT:
					m_states.push_back(new MeleeMovementState(m_penemy));
					break;

				case FuzzyStateType::RANGE_ATTACK:
					m_states.push_back(new RangeAttackState(m_penemy));
					break;

				case FuzzyStateType::RANGE_MOVEMENT:
					m_states.push_back(new RangeMovementState(m_penemy));
					break;

				case FuzzyStateType::WANDER:
					m_states.push_back(new WanderState(m_penemy));
					break;
				}
			}
			else
			{
				EditorUI::ErrorPopupBox("State Creation", "Can't create that state as one with that type already exists!");
			}
		}

		//Delete button
		if (callback.m_rightButton == true)
		{
			int i = 0;
			bool shouldDelete = false;

			for (i = 0; i < m_states.size(); ++i)
			{
				if (m_states[i]->GetStateType() == m_selectedStateType)
				{
					shouldDelete = true;
					break;
				}
			}

			if (shouldDelete == true)
			{
				delete m_states[i];
				m_states.erase(m_states.begin() + i);
			}
			else
			{
				EditorUI::ErrorPopupBox("State Deletion", "Can't delete a state as none has been selected!");
			}
		}

		ImGui::Spacing();

		float windowWidth = ImGui::GetContentRegionAvail().x;

		ImGui::PushItemWidth(windowWidth);

		EditorUI::InputText("AI Name", m_editorAIName);

		ImGui::SameLine();

		if (ImGui::Button("Save") == true && m_editorAIName != "")
		{
			Save();
		}

		ImGui::SameLine();

		if (ImGui::Button("Load") == true)
		{
			if (Load() == true)
			{
				m_AIName = m_editorAIName;
			}
		}

		ImGui::PopItemWidth();
	}
}
#endif

void EnemyStateMachine::Start()
{
	for (int i = 0; i < m_states.size(); ++i)
	{
		m_states[i]->Start();
	}
}

void EnemyStateMachine::Save()
{
	std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\AI\\" + m_editorAIName + ".json";

	std::ofstream fileOut(path);

	nlohmann::json data;

	for (int i = 0; i < m_states.size(); ++i)
	{
		m_states[i]->Serialize(data[std::to_string((int)m_states[i]->GetStateType())]);
	}

	fileOut << data;

	fileOut.close();

	m_AIName = m_editorAIName;
}

bool EnemyStateMachine::Load()
{
	std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\AI\\" + m_editorAIName + ".json";

	std::ifstream fileIn(path);

	if (fileIn.is_open() == false)
	{
		LOG("Failed to load AI as couldn't open file!");

		return false;
	}

	nlohmann::json data;

	fileIn >> data;

	fileIn.close();

	for (int i = 0; i < m_states.size(); ++i)
	{
		delete m_states[i];
		m_states[i] = nullptr;
	}

	m_states.clear();

	for (nlohmann::json::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		FuzzyStateType type = (FuzzyStateType)stoi(it.key());

		switch (type)
		{
		case FuzzyStateType::MELEE_ATTACK:
			m_states.push_back(new MeleeAttackState(data[it.key()], m_penemy));
			break;

		case FuzzyStateType::MELEE_MOVEMENT:
			m_states.push_back(new MeleeMovementState(data[it.key()], m_penemy));
			break;

		case FuzzyStateType::RANGE_ATTACK:
			m_states.push_back(new RangeAttackState(data[it.key()], m_penemy));
			break;

		case FuzzyStateType::RANGE_MOVEMENT:
			m_states.push_back(new RangeMovementState(data[it.key()], m_penemy));
			break;

		case FuzzyStateType::WANDER:
			m_states.push_back(new WanderState(data[it.key()], m_penemy));
			break;
		}

		if (GameManager::GetInstance()->GetViewState() == ViewState::GAME_VIEW)
		{
			m_states.back()->Start();
		}
	}

	return true;
}
