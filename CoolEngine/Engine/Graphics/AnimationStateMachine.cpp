#include "AnimationStateMachine.h"
#include "Engine/Graphics/AnimationState.h"
#include "Engine/EditorUI/EditorUI.h"

void AnimationStateMachine::Serialize(nlohmann::json& data)
{
	data["AnimationStateMachine"]["ActiveState"] = GetStateName(m_pstate);

	for (std::unordered_map<std::string, FiniteState*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
	{
		it->second->Serialize(data["AnimationStateMachine"], this);
	}
}

void AnimationStateMachine::Deserialize(const nlohmann::json& data)
{
	m_states.clear();

	for (nlohmann::json::const_iterator it = data["AnimationStateMachine"].begin(); it != data["AnimationStateMachine"].end(); ++it)
	{
		m_states[it.key()] = new AnimationState();
	}

	for (nlohmann::json::const_iterator it = data["AnimationStateMachine"].begin(); it != data["AnimationStateMachine"].end(); ++it)
	{
		m_states[it.key()]->Deserialize(data["AnimationStateMachine"], this);
	}
}

#if EDITOR
void AnimationStateMachine::CreateEngineUI()
{
	EditorUI::CollapsingSection("Animation");

	for (std::unordered_map<std::string, FiniteState*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

		if (m_selectedState == it->first)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		int nodeCount = 0;

		if (ImGui::TreeNodeEx(it->first.c_str(), flags) == true)
		{
			it->second->CreateEngineUI();

			ImGui::TreePop();
		}

		if (ImGui::IsItemClicked() == true)
		{
			if (m_selectedState == it->first)
			{
				m_selectedState = "";
			}
			else
			{
				m_selectedState = it->first;
			}
		}
	}

	float windowWidth = ImGui::GetWindowWidth();

	char buffer[FILEPATH_BUFFER_SIZE];
	
	strcpy_s(buffer, m_newStateName.c_str());

	ImGui::PushItemWidth(windowWidth * 0.5f);

	ImGui::Text("Name");

	ImGui::SameLine();

	if (ImGui::InputText("##Name", buffer, FILEPATH_BUFFER_SIZE) == true)
	{
		m_newStateName = std::string(buffer);
	}

	ImGui::PopItemWidth();

	ImGui::SameLine();

	ImGui::PushItemWidth(windowWidth * 0.5f);

	if (ImGui::Button("New"))
	{
		if (m_newStateName != "" && m_states.count(m_newStateName) == 0)
		{
			AnimationState* pstate = new AnimationState();
			AddState(m_newStateName, pstate);
		}
		else
		{
			LOG("A state with that name already exists or the name is empty!");
		}

		m_newStateName = "";
	}
	
	ImGui::SameLine();

	if (ImGui::Button("Delete") == true && m_states.count(m_selectedState) != 0)
	{
		RemoveState(m_selectedState);
		m_selectedState = "";
	}

	ImGui::PopItemWidth();
}
#endif
