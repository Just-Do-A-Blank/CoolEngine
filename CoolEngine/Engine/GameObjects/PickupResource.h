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

    bool operator==(const PickupResource& ob)const
    {
        bool equal = false;
        if (key == ob.key && strength == ob.strength)
        {
            equal = true;
        }
            

        return equal;
    }
};

class PickupResourceInterface
{
public:
    PickupResourceInterface(unordered_set<PickupResource*>* resources);
    virtual void CreateEngineUI();

    unordered_set<PickupResource*>* GetEffects() { return m_pPickupEffects; }


    const string m_popupKeyResourceKeyFound = "PickupResource_ResourceErrorKey";

private:
	void AddResourceToList(string key);
    void RemoveResourceFromList(string key);

    string m_errorMsg;
    bool m_errorLastEntry;
    string m_entryKey;

    PickupResource* m_deleteOnNextLoop;

    unordered_set<PickupResource*>* m_pPickupEffects;
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

