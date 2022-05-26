#pragma once
#include <unordered_set>

struct PickupResource
{
    string key;
    float strength;
    PickupResource()
    {
        key = "";
        strength = 0;
    }

    PickupResource(string Key, float Strength)
    {
        key = Key;
        strength = Strength;
    }

    PickupResource(string Key)
    {
        key = Key;
    }
};

class PickupResourceInterface
{
public:
    PickupResourceInterface(list<PickupResource*>* resources);

#if EDITOR
    virtual void CreateEngineUI();
#endif

    list<PickupResource*>* GetEffects() { return m_pPickupEffects; }


    const string m_popupKeyResourceKeyFound = "PickupResource_ResourceErrorKey";

private:
	void AddResourceToList(string key);
    void RemoveResourceFromList(string key);

    string m_errorMsg;
    bool m_errorLastEntry;
    string m_entryKey;

    PickupResource* m_deleteOnNextLoop;

    list<PickupResource*>* m_pPickupEffects;
    unordered_set<string>* m_pFullEffectsList;


    void RemoveEffect(PickupResource* resource);
    void TrimSpacesEitherSideOfText(string& text);
    void TextToLower(string& text);

#if EDITOR
    /// Displays interface for the creation / mangement section
    void CreateNewResourceInterface();

    /// Displays all resources for editing / deleting
    void DisplayResourcesForEdit();
#endif
};

