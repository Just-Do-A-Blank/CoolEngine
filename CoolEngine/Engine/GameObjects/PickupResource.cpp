#include "PickupResource.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/PickupsManager.h"
#include <algorithm>


PickupResourceInterface::PickupResourceInterface(list<PickupResource*>* resources)
{
    m_pPickupEffects = resources;
    m_errorMsg = "";
    m_pFullEffectsList = PickupsManager::GetInstance()->GetList();
}

void PickupResourceInterface::AddResourceToList(string key)
{

    m_pFullEffectsList->insert(key);
}

void PickupResourceInterface::RemoveResourceFromList(string key)
{
    m_pFullEffectsList->erase(key);
}


#if EDITOR
void PickupResourceInterface::CreateEngineUI()
{
    if (EditorUI::CollapsingSection("Pick Resource", true))
    {
        CreateNewResourceInterface();

        DisplayResourcesForEdit();
    }
}
#endif



#if EDITOR
void PickupResourceInterface::CreateNewResourceInterface()
{
    EditorUINonSpecificParameters parameters = EditorUINonSpecificParameters();
    parameters.m_columnWidth = 150;
    parameters.m_tooltipText = "A unique key for the resource";
    EditorUI::InputText("Resource Key", m_entryKey, parameters);

    parameters.m_columnWidth = 150;


    m_errorLastEntry = EditorUI::ErrorPopupBox(m_popupKeyResourceKeyFound, m_errorMsg);

    EditorButtonCallback entryButtons = EditorUI::BasicDuelButtons("Add new Type", "Remove Type", parameters);
    if (entryButtons.m_leftButton)
    {
        TrimSpacesEitherSideOfText(m_entryKey);
        TextToLower(m_entryKey);

        if (m_entryKey == "")
        {
            m_errorLastEntry = true;
            m_errorMsg = "Please enter a key!";
            EditorUI::ShowError(m_popupKeyResourceKeyFound);
        }
        else if (m_entryKey == "none")
        {
            m_errorLastEntry = true;
            m_errorMsg = "Resources cannot be called none... sorry about that.";
            EditorUI::ShowError(m_popupKeyResourceKeyFound);
        }
        else
        {
            AddResourceToList(m_entryKey);
            m_entryKey = "";
        }

    }
    else if (entryButtons.m_rightButton)
    {
        TrimSpacesEitherSideOfText(m_entryKey);
        TextToLower(m_entryKey);

        if (m_entryKey == "")
        {
            m_errorLastEntry = true;
            m_errorMsg = "Please enter a key!";
            EditorUI::ShowError(m_popupKeyResourceKeyFound);
        }
        else if (m_entryKey == "none")
        {
            m_errorLastEntry = true;
            m_errorMsg = "Resources cannot be called none... sorry about that.";
            EditorUI::ShowError(m_popupKeyResourceKeyFound);
        }
        else
        {
            RemoveResourceFromList(m_entryKey);
        }
    }
    

    ImGui::Spacing();

    if (EditorUI::BasicButton("Add new effect"))
    {
        m_pPickupEffects->push_back(new PickupResource());
    }

    ImGui::Spacing();
}

/// <summary>
/// Displays all resources for editing / deleting
/// </summary>
void PickupResourceInterface::DisplayResourcesForEdit()
{
    if (m_deleteOnNextLoop != nullptr)
    {
        RemoveEffect(m_deleteOnNextLoop);
        m_deleteOnNextLoop = nullptr;
    }


    list<PickupResource*>::iterator i;
    int j = 0;

    for (i = m_pPickupEffects->begin();i != m_pPickupEffects->end(); i++)
    {
        ImGui::PushID(j);
        //Drop down menu selection
        string currentSelected = (*i)->key;

        unordered_set<string>::iterator it;
        EditorUI::FullTitle("");
        ImGui::Spacing();

        EditorUIFloatParameters floatParameters = EditorUIFloatParameters();
        floatParameters.m_tooltipText = "The strength of the effect (if singular use 1). Negative can subtract";
        floatParameters.m_columnWidth = 150;
        EditorUI::DragFloat("Strength", (*i)->strength, floatParameters);

        if (IMGUI_LEFT_LABEL(ImGui::BeginCombo, "Pickup Effect", currentSelected.c_str()) == true)
        {
            //Iterate players and non-player effects for the menu
            for (it = m_pFullEffectsList->begin(); it != m_pFullEffectsList->end(); it++)
            {
                if (ImGui::Selectable(it->c_str()))
                {
                    (*i)->key = *it;
                }
            }


            ImGui::EndCombo();
        }



        

        ImGui::Spacing();

        if (EditorUI::BasicButton("Delete"))
        {
            m_deleteOnNextLoop = (*i);
        }

        ImGui::PopID();
        j++;
    }
}

#endif


void PickupResourceInterface::TrimSpacesEitherSideOfText(std::string& s)
{
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
}

void PickupResourceInterface::TextToLower(string& text)
{
    for (int i = 0; i < text.length(); ++i)
    {
        text[i] = tolower(text[i]);
    }
}

void PickupResourceInterface::RemoveEffect(PickupResource* resource)
{
    m_pPickupEffects->remove(resource);
}