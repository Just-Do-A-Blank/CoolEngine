#include "FiniteState.h"
#include "Engine/AI/FiniteStateMachine.h"
#include "Engine/EditorUI/EditorUI.h"

FiniteState::FiniteState()
{
}

FiniteState::FiniteState(const FiniteState* pother)
{
	m_name = pother->m_name;
}

void FiniteState::Enter()
{
	for (std::unordered_map<std::string, TransitionInfo<bool>>::iterator it = m_boolTransitionInfo.begin(); it != m_boolTransitionInfo.end(); ++it)
	{
		it->second.CurrentValue = it->second.ResetValue;
	}

	for (std::unordered_map<std::string, TransitionInfo<int>>::iterator it = m_intTransitionInfo.begin(); it != m_intTransitionInfo.end(); ++it)
	{
		it->second.CurrentValue = it->second.ResetValue;
	}

	for (std::unordered_map<std::string, TransitionInfo<float>>::iterator it = m_floatTransitionInfo.begin(); it != m_floatTransitionInfo.end(); ++it)
	{
		it->second.CurrentValue = it->second.ResetValue;
	}
}

bool FiniteState::SetBool(std::string name, bool value)
{
    if (m_boolTransitionInfo.count(name) == 0)
    {
        LOG("A bool with that name doesn't exist!");

        return false;
    }

    m_boolTransitionInfo[name].CurrentValue = value;

    return true;
}

bool FiniteState::SetFloat(std::string name, float value)
{
    if (m_floatTransitionInfo.count(name) == 0)
    {
        LOG("A float with that name doesn't exist!");

        return false;
    }

    m_floatTransitionInfo[name].CurrentValue = value;

    return true;
}

bool FiniteState::SetInt(std::string name, int value)
{
    if (m_intTransitionInfo.count(name) == 0)
    {
        LOG("An int with that name doesn't exist!");

        return false;
    }

    m_intTransitionInfo[name].CurrentValue = value;

    return true;
}

bool FiniteState::CreateBoolTransition(std::string name, bool transitionValue, bool initialValue, FiniteState* pnewState)
{
    if (m_boolTransitionInfo.count(name) != 0)
    {
        LOG("A bool condition with that name already exists!");

        return false;
    }

    TransitionInfo<bool> info;
    info.CurrentValue = initialValue;
    info.TransitionValue = transitionValue;
    info.NewState = pnewState;

    m_boolTransitionInfo[name] = info;

    return true;
}

bool FiniteState::CreateFloatTransition(std::string name, float transitionValue, float initialValue, FiniteState* pnewState)
{
    if (m_floatTransitionInfo.count(name) != 0)
    {
        LOG("A float with that name already exists!");

        return false;
    }

    TransitionInfo<float> info;
    info.CurrentValue = initialValue;
    info.TransitionValue = transitionValue;
    info.NewState = pnewState;

    m_floatTransitionInfo[name] = info;

    return true;
}

bool FiniteState::CreateIntTransition(std::string name, int transitionValue, int initialValue, FiniteState* pnewState)
{
    if (m_intTransitionInfo.count(name) != 0)
    {
        LOG("An int with that name already exists!");

        return false;
    }

    TransitionInfo<int> info;
    info.CurrentValue = initialValue;
    info.TransitionValue = transitionValue;
    info.NewState = pnewState;

    m_intTransitionInfo[name] = info;

    return true;
}

bool FiniteState::Transition(FiniteState*& pnewState)
{
    for (std::unordered_map<std::string, TransitionInfo<bool>>::iterator it = m_boolTransitionInfo.begin(); it != m_boolTransitionInfo.end(); ++it)
    {
        if (it->second.CurrentValue == it->second.TransitionValue)
        {
            pnewState = it->second.NewState;

            return true;
        }
    }

    for (std::unordered_map<std::string, TransitionInfo<int>>::iterator it = m_intTransitionInfo.begin(); it != m_intTransitionInfo.end(); ++it)
    {
        if (it->second.CurrentValue == it->second.TransitionValue)
        {
            pnewState = it->second.NewState;

            return true;
        }
    }

    for (std::unordered_map<std::string, TransitionInfo<float>>::iterator it = m_floatTransitionInfo.begin(); it != m_floatTransitionInfo.end(); ++it)
    {
        if (std::abs(it->second.CurrentValue - it->second.TransitionValue) <= FLT_EPSILON)
        {
            pnewState = it->second.NewState;

            return true;
        }
    }

    return false;
}

void FiniteState::CreateEngineUI()
{
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Bool Transitions");

    CreateBoolTransitionUI();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Float Transitions");

    CreateFloatTransitionUI();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Int Transitions");

    CreateIntTransitionUI();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void FiniteState::Serialize(nlohmann::json& data, FiniteStateMachine* pstateMachine)
{
	data[m_name]["BoolTransitions"] = {};

	for (std::unordered_map<std::string, TransitionInfo<bool>>::iterator it = m_boolTransitionInfo.begin(); it != m_boolTransitionInfo.end(); ++it)
	{
		data[m_name]["BoolTransitions"][it->first].push_back({ it->second.CurrentValue, it->second.TransitionValue, pstateMachine->GetStateName(it->second.NewState) });
	}

	data[m_name]["IntTransitions"] = {};

	for (std::unordered_map<std::string, TransitionInfo<int>>::iterator it = m_intTransitionInfo.begin(); it != m_intTransitionInfo.end(); ++it)
	{
		data[m_name]["IntTransitions"][it->first].push_back({ it->second.CurrentValue, it->second.TransitionValue, pstateMachine->GetStateName(it->second.NewState) });
	}

	data[m_name]["FloatTransitions"] = {};

	for (std::unordered_map<std::string, TransitionInfo<float>>::iterator it = m_floatTransitionInfo.begin(); it != m_floatTransitionInfo.end(); ++it)
	{
		data[m_name]["FloatTransitions"][it->first].push_back({ it->second.CurrentValue, it->second.TransitionValue, pstateMachine->GetStateName(it->second.NewState) });
	}
}

void FiniteState::Deserialize(const nlohmann::json& data, FiniteStateMachine* pstateMachine)
{
	for (nlohmann::json::const_iterator it = data[m_name]["BoolTransitions"].begin(); it != data[m_name]["BoolTransitions"].end(); ++it)
	{
		TransitionInfo<bool> info;
		info.CurrentValue = data[m_name]["BoolTransitions"][it.key()][0][0];
		info.ResetValue = info.CurrentValue;
		info.TransitionValue = data[m_name]["BoolTransitions"][it.key()][0][1];
		info.NewState = pstateMachine->GetState(data[m_name]["BoolTransitions"][it.key()][0][2]);

		m_boolTransitionInfo[it.key()] = info;
	}

	for (nlohmann::json::const_iterator it = data[m_name]["IntTransitions"].begin(); it != data[m_name]["IntTransitions"].end(); ++it)
	{
		TransitionInfo<int> info;
		info.CurrentValue = data[m_name]["IntTransitions"][it.key()][0][0];
		info.ResetValue = info.CurrentValue;
		info.TransitionValue = data[m_name]["IntTransitions"][it.key()][0][1];
		info.NewState = pstateMachine->GetState(data[m_name]["IntTransitions"][it.key()][0][2]);

		m_intTransitionInfo[it.key()] = info;
	}

	for (nlohmann::json::const_iterator it = data[m_name]["FloatTransitions"].begin(); it != data[m_name]["FloatTransitions"].end(); ++it)
	{
		TransitionInfo<float> info;
		info.CurrentValue = data[m_name]["FloatTransitions"][it.key()][0][0];
		info.ResetValue = info.CurrentValue;
		info.TransitionValue = data[m_name]["FloatTransitions"][it.key()][0][1];
		info.NewState = pstateMachine->GetState(data[m_name]["FloatTransitions"][it.key()][0][2]);

		m_floatTransitionInfo[it.key()] = info;
	}
}

void FiniteState::SetName(std::string name)
{
    m_name = name;
}

std::string FiniteState::GetName()
{
    return m_name;
}

void FiniteState::RemoveTransitions(FiniteState* pstate)
{
    for (std::unordered_map<std::string, TransitionInfo<bool>>::iterator it = m_boolTransitionInfo.begin(); it != m_boolTransitionInfo.end(); ++it)
    {
        if (it->second.NewState == pstate)
        {
            it->second.NewState = nullptr;
        }
    }

    for (std::unordered_map<std::string, TransitionInfo<int>>::iterator it = m_intTransitionInfo.begin(); it != m_intTransitionInfo.end(); ++it)
    {
        if (it->second.NewState == pstate)
        {
            it->second.NewState = nullptr;
        }
    }

    for (std::unordered_map<std::string, TransitionInfo<float>>::iterator it = m_floatTransitionInfo.begin(); it != m_floatTransitionInfo.end(); ++it)
    {
        if (it->second.NewState == pstate)
        {
            it->second.NewState = nullptr;
        }
    }
}

void FiniteState::InitTransitions(FiniteState const* other, FiniteStateMachine* pmachine)
{
	for (std::unordered_map<std::string, TransitionInfo<bool>>::const_iterator it = other->m_boolTransitionInfo.begin(); it != other->m_boolTransitionInfo.end(); ++it)
	{
		TransitionInfo<bool> info;
		info.CurrentValue = it->second.CurrentValue;
		info.ResetValue = it->second.ResetValue;
		info.TransitionValue = it->second.TransitionValue;

		if (it->second.NewState == nullptr)
		{
			info.NewState = nullptr;
		}
		else
		{
			info.NewState = pmachine->GetState(it->second.NewState->GetName());
		}


		m_boolTransitionInfo[it->first] = info;
	}

	for (std::unordered_map<std::string, TransitionInfo<int>>::const_iterator it = other->m_intTransitionInfo.begin(); it != other->m_intTransitionInfo.end(); ++it)
	{
		TransitionInfo<int> info;
		info.CurrentValue = it->second.CurrentValue;
		info.ResetValue = it->second.ResetValue;
		info.TransitionValue = it->second.TransitionValue;

		if (it->second.NewState == nullptr)
		{
			info.NewState = nullptr;
		}
		else
		{
			info.NewState = pmachine->GetState(it->second.NewState->GetName());
		}


		m_intTransitionInfo[it->first] = info;
	}

	for (std::unordered_map<std::string, TransitionInfo<float>>::const_iterator it = other->m_floatTransitionInfo.begin(); it != other->m_floatTransitionInfo.end(); ++it)
	{
		TransitionInfo<float> info;
		info.CurrentValue = it->second.CurrentValue;
		info.ResetValue = it->second.ResetValue;
		info.TransitionValue = it->second.TransitionValue;

		if (it->second.NewState == nullptr)
		{
			info.NewState = nullptr;
		}
		else
		{
			info.NewState = pmachine->GetState(it->second.NewState->GetName());
		}


		m_floatTransitionInfo[it->first] = info;
	}
}

void FiniteState::CreateBoolTransitionUI()
{
    ImGui::PushID("BoolTrans");

    float windowWidth = ImGui::GetWindowWidth();

    for (std::unordered_map<std::string, TransitionInfo<bool>>::iterator it = m_boolTransitionInfo.begin(); it != m_boolTransitionInfo.end(); ++it)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

        if (m_selectedTransitionName == it->first)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        if (ImGui::TreeNodeEx(it->first.c_str(), flags) == true)
        {
            ImGui::Text("Current Value");
            ImGui::SameLine();
            ImGui::Checkbox("##Current Value", &it->second.CurrentValue);

            ImGui::Text("Transition Value");
            ImGui::SameLine();
            ImGui::Checkbox("##Transition Value", &it->second.TransitionValue);

            ImGui::Text("New State: ");
            ImGui::SameLine();

            if (it->second.NewState == nullptr)
            {
                ImGui::Text("Null");
            }
            else
            {
                ImGui::Text(it->second.NewState->GetName().c_str());
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("AnimationState");

                if (ppayload != nullptr)
                {
                    FiniteState* pstate = *((FiniteState**)ppayload->Data);

                    if (pstate != this)
                    {
                        it->second.NewState = pstate;
                    }
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::TreePop();
        }

        if (ImGui::IsItemClicked() == true)
        {
            if (m_selectedTransitionName == it->first)
            {
                m_selectedTransitionName = "";
            }
            else
            {
                m_selectedTransitionName = it->first;
            }
        }
    }

    ImGui::Spacing();

    char buffer[FILEPATH_BUFFER_SIZE];
    strcpy_s(buffer, m_boolTransitionName.c_str());

    ImGui::PushItemWidth(windowWidth * 0.5f);

    if (ImGui::InputText("##Name", buffer, FILEPATH_BUFFER_SIZE) == true)
    {
        m_boolTransitionName = std::string(buffer);
    }

    ImGui::PopItemWidth();

    ImGui::SameLine();

    ImGui::PushItemWidth(windowWidth * 0.5f);

    if (ImGui::Button("New"))
    {
        if (m_boolTransitionName != "" && m_boolTransitionInfo.count(m_boolTransitionName) == 0)
        {
            TransitionInfo<bool> info;
            info.CurrentValue = false;
            info.TransitionValue = true;
            info.NewState = nullptr;

            m_boolTransitionInfo[m_boolTransitionName] = info;
        }
        else
        {
            LOG("A bool transition with that name already exists or the name is empty!");
        }

        m_boolTransitionName = "";
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete") == true && m_boolTransitionInfo.count(m_selectedTransitionName) != 0)
    {
        m_boolTransitionInfo.erase(m_selectedTransitionName);
    }

    ImGui::PopItemWidth();

    ImGui::PopID();
}

void FiniteState::CreateFloatTransitionUI()
{
    ImGui::PushID("FloatTrans");

    float windowWidth = ImGui::GetWindowWidth();

    for (std::unordered_map<std::string, TransitionInfo<float>>::iterator it = m_floatTransitionInfo.begin(); it != m_floatTransitionInfo.end(); ++it)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

        if (m_selectedTransitionName == it->first)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        if (ImGui::TreeNodeEx(it->first.c_str(), flags) == true)
        {
            ImGui::PushItemWidth(windowWidth * 0.2f);

            ImGui::Text("Current Value");
            ImGui::SameLine();
            ImGui::DragFloat("##Current Value", &it->second.CurrentValue);

            ImGui::Text("Transition Value");
            ImGui::SameLine();
            ImGui::DragFloat("##Transition Value", &it->second.TransitionValue);

            ImGui::Text("New State: ");
            ImGui::SameLine();

            if (it->second.NewState == nullptr)
            {
                ImGui::Text("Null");
            }
            else
            {
                ImGui::Text(it->second.NewState->GetName().c_str());
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("AnimationState");

                if (ppayload != nullptr)
                {
                    FiniteState* pstate = *((FiniteState**)ppayload->Data);
                    
                    if (pstate != this)
                    {
                        it->second.NewState = pstate;
                    }
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::PopItemWidth();

            ImGui::TreePop();
        }

        if (ImGui::IsItemClicked() == true)
        {
            if (m_selectedTransitionName == it->first)
            {
                m_selectedTransitionName = "";
            }
            else
            {
                m_selectedTransitionName = it->first;
            }
        }
    }

    ImGui::Spacing();

    char buffer[FILEPATH_BUFFER_SIZE];
    strcpy_s(buffer, m_floatTransitionName.c_str());

    ImGui::PushItemWidth(windowWidth * 0.5f);

    if (ImGui::InputText("##Name", buffer, FILEPATH_BUFFER_SIZE) == true)
    {
        m_floatTransitionName = std::string(buffer);
    }

    ImGui::PopItemWidth();

    ImGui::SameLine();

    ImGui::PushItemWidth(windowWidth * 0.5f);

    if (ImGui::Button("New"))
    {
        if (m_floatTransitionName != "" && m_floatTransitionInfo.count(m_floatTransitionName) == 0)
        {
            TransitionInfo<float> info;
            info.CurrentValue = 0;
            info.TransitionValue = 1;
            info.NewState = nullptr;

            m_floatTransitionInfo[m_floatTransitionName] = info;
        }
        else
        {
            LOG("A float transition with that name already exists or the name is empty!");
        }

        m_floatTransitionName = "";
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete") == true && m_floatTransitionInfo.count(m_selectedTransitionName) != 0)
    {
        m_floatTransitionInfo.erase(m_selectedTransitionName);
    }

    ImGui::PopItemWidth();

    ImGui::PopID();
}

void FiniteState::CreateIntTransitionUI()
{
    ImGui::PushID("IntTrans");

    float windowWidth = ImGui::GetWindowWidth();

    for (std::unordered_map<std::string, TransitionInfo<int>>::iterator it = m_intTransitionInfo.begin(); it != m_intTransitionInfo.end(); ++it)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

        if (m_selectedTransitionName == it->first)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        if (ImGui::TreeNodeEx(it->first.c_str(), flags) == true)
        {
            ImGui::PushItemWidth(windowWidth * 0.2f);

            ImGui::Text("Current Value");
            ImGui::SameLine();
            ImGui::DragInt("##Current Value", &it->second.CurrentValue);

            ImGui::Text("Transition Value");
            ImGui::SameLine();
            ImGui::DragInt("##Transition Value", &it->second.TransitionValue);

            ImGui::Text("New State: ");
            ImGui::SameLine();

            if (it->second.NewState == nullptr)
            {
                ImGui::Text("Null");
            }
            else
            {
                ImGui::Text(it->second.NewState->GetName().c_str());
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("AnimationState");

                if (ppayload != nullptr)
                {
                    FiniteState* pstate = *((FiniteState**)ppayload->Data);

                    if (pstate != this)
                    {
                        it->second.NewState = pstate;
                    }
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::PopItemWidth();

            ImGui::TreePop();
        }

        if (ImGui::IsItemClicked() == true)
        {
            if (m_selectedTransitionName == it->first)
            {
                m_selectedTransitionName = "";
            }
            else
            {
                m_selectedTransitionName = it->first;
            }
        }
    }

    ImGui::Spacing();

    char buffer[FILEPATH_BUFFER_SIZE];
    strcpy_s(buffer, m_intTransitionName.c_str());

    ImGui::PushItemWidth(windowWidth * 0.5f);

    if (ImGui::InputText("##Name", buffer, FILEPATH_BUFFER_SIZE) == true)
    {
        m_intTransitionName = std::string(buffer);
    }

    ImGui::PopItemWidth();

    ImGui::SameLine();

    ImGui::PushItemWidth(windowWidth * 0.5f);

    if (ImGui::Button("New"))
    {
        if (m_intTransitionName != "" && m_intTransitionInfo.count(m_intTransitionName) == 0)
        {
            TransitionInfo<int> info;
            info.CurrentValue = 0;
            info.TransitionValue = 1;
            info.NewState = nullptr;

            m_intTransitionInfo[m_intTransitionName] = info;
        }
        else
        {
            LOG("A float transition with that name already exists or the name is empty!");
        }

        m_intTransitionName = "";
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete") == true && m_intTransitionInfo.count(m_selectedTransitionName) != 0)
    {
        m_intTransitionInfo.erase(m_selectedTransitionName);
    }

    ImGui::PopItemWidth();

    ImGui::PopID();
}
