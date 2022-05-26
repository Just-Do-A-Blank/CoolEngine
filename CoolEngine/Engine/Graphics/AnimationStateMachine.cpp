#include "AnimationStateMachine.h"
#include "Engine/Graphics/AnimationState.h"
#include "Engine/EditorUI/EditorUI.h"

AnimationStateMachine::AnimationStateMachine()
{
}

AnimationStateMachine::AnimationStateMachine(AnimationStateMachine const* other)
{
	for (std::unordered_map<std::string, FiniteState*>::const_iterator it = other->m_states.begin(); it != other->m_states.end(); ++it)
	{
		m_states[it->first] = new AnimationState((const AnimationState*)it->second);
	}

	for (std::unordered_map<std::string, FiniteState*>::const_iterator it = other->m_states.begin(); it != other->m_states.end(); ++it)
	{
		m_states[it->first]->InitTransitions(it->second, this);
	}

	if (other->m_pstate != nullptr)
	{
		m_pstate = m_states[other->m_pstate->GetName()];
	}
	else
	{
		m_pstate = nullptr;
	}
}

void AnimationStateMachine::Serialize(nlohmann::json& data)
 {
	data["AnimationStateMachine"]["ActiveState"] = GetStateName(m_pstate);
	data["AnimationStateMachine"]["States"] = {};

	for (std::unordered_map<std::string, FiniteState*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
	{
		it->second->Serialize(data["AnimationStateMachine"]["States"], this);
	}
	int i = 0;
}

void AnimationStateMachine::Deserialize(const nlohmann::json& data)
{
	m_states.clear();

	if (!data.contains("AnimationStateMachine"))
	{
		return;
	}

	if (data["AnimationStateMachine"]["States"] != NULL)
	{
		for (nlohmann::json::const_iterator it = data["AnimationStateMachine"]["States"].begin(); it != data["AnimationStateMachine"]["States"].end(); ++it)
		{
			m_states[it.key()] = new AnimationState();
			m_states[it.key()]->SetName(it.key());
		}

		for (nlohmann::json::const_iterator it = data["AnimationStateMachine"]["States"].begin(); it != data["AnimationStateMachine"]["States"].end(); ++it)
		{
			m_states[it.key()]->Deserialize(data["AnimationStateMachine"]["States"], this);
		}
	}

	std::string activeStateName = data["AnimationStateMachine"]["ActiveState"];

	if (activeStateName == "")
	{
		m_pstate = nullptr;
	}
	else
	{
		m_pstate = m_states[activeStateName];
	}
}

#if EDITOR
void AnimationStateMachine::CreateEngineUI()
{
	if (EditorUI::CollapsingSection("Animation"))
	{
		for (std::unordered_map<std::string, FiniteState*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

			if (m_selectedState == it->first)
			{
				flags |= ImGuiTreeNodeFlags_Selected;
			}

			bool treeNodeOpen = ImGui::TreeNodeEx(it->first.c_str(), flags);

			if (treeNodeOpen == true)
			{
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID) == true)
				{
					ImGui::SetDragDropPayload("AnimationState", &it->second, sizeof(it->second), ImGuiCond_Once);
					ImGui::EndDragDropSource();
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

				it->second->CreateEngineUI();

				ImGui::TreePop();
			}

			if (treeNodeOpen == false && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID) == true)
			{
				ImGui::SetDragDropPayload("AnimationState", &it->second, sizeof(it->second), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (treeNodeOpen == false && ImGui::IsItemClicked() == true)
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

		ImGui::Text("Active State: ");
		ImGui::SameLine();

		if (m_pstate == nullptr)
		{
			ImGui::Text("Null");
		}
		else
		{
			ImGui::Text(m_pstate->GetName().c_str());
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("AnimationState");

			if (ppayload != nullptr)
			{
				FiniteState* pstate = *((FiniteState**)ppayload->Data);

				m_pstate = pstate;
			}

			ImGui::EndDragDropTarget();
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
				pstate->SetName(m_newStateName);

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
}
#endif
