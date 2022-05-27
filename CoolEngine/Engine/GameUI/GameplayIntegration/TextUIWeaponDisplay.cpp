#include "Engine/GameUI/GameplayIntegration/TextUIWeaponDisplay.h"
#include "Engine/GameUI/TextComponent.h"
#include "Engine/EditorUI/EditorUI.h"

TextUIWeaponDisplay::TextUIWeaponDisplay(TextComponent* textComponent) : GameplayUIWeaponAttachment()
{
#if EDITOR
	m_WeaponStatList = GetWeaponStatAsList();
	m_WeaponStatSelectedItem = GetWeaponStatFromIndex(0);
#endif

	m_displayStat = (EWEAPONTEXTDISPLAYSTAT)0;
	m_displayWhenNoWeapon = "";
	m_textComponent = textComponent;
}

TextUIWeaponDisplay::TextUIWeaponDisplay(nlohmann::json& data, TextComponent* textComponent) : GameplayUIWeaponAttachment(data)
{
#if EDITOR
	m_WeaponStatList = GetWeaponStatAsList();
#endif

	LoadLocalData(data);
#endif

	m_textComponent = textComponent;
}

TextUIWeaponDisplay::TextUIWeaponDisplay(TextUIWeaponDisplay const& other, TextComponent* textComponent) : GameplayUIWeaponAttachment(other)
{
	m_displayStat = other.m_displayStat;
	m_displayWhenNoWeapon = other.m_displayWhenNoWeapon;
	m_textComponent = textComponent;

#if EDITOR
	m_WeaponStatList = GetWeaponStatAsList();
	m_WeaponStatSelectedItem = GetWeaponStatFromIndex((int)m_displayStat);
#endif
}

TextUIWeaponDisplay::~TextUIWeaponDisplay()
{

}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void TextUIWeaponDisplay::Start()
{
	GameplayUIWeaponAttachment::Start();
}

/// <summary>
/// Runs during gameplay with the resource value
/// </summary>
/// <param name="weaponGameObject">The weapon to display</param>
void TextUIWeaponDisplay::Update(WeaponGameObject* weaponGameObject)
{
	if (m_textComponent == nullptr)
	{
		return;
	}

	if (weaponGameObject == nullptr)
	{
		m_textComponent->SetText(m_displayWhenNoWeapon);
		return;
	}

	switch (m_displayStat)
	{
	case EWEAPONTEXTDISPLAYSTAT::KEY:
			m_textComponent->SetText(weaponGameObject->GetUniqueKey());
			break;
		case EWEAPONTEXTDISPLAYSTAT::LEVEL:
			m_textComponent->SetText(to_string(weaponGameObject->GetLevel()));
			break;
		case EWEAPONTEXTDISPLAYSTAT::DAMAGE:
			m_textComponent->SetText(to_string(weaponGameObject->GetDamage()));
			break;
		case EWEAPONTEXTDISPLAYSTAT::STRENGTH:
			m_textComponent->SetText(to_string(weaponGameObject->GetStrength()));
			break;
		case EWEAPONTEXTDISPLAYSTAT::SHOTCOUNT:
			m_textComponent->SetText(to_string(weaponGameObject->GetShotCount()));
			break;
	}
}

#if EDITOR
void TextUIWeaponDisplay::CreateEngineUI()
{
	GameplayUIWeaponAttachment::CreateEngineUI();
	if (EditorUI::CollapsingSection("Weapon display", true))
	{
		EditorUI::FullTitle("Blank/No texture images mean\nnothing displayed or weapon image displayed.");

		if (EditorUI::ComboBox("Stat", m_WeaponStatList, m_WeaponStatSelectedItem))
		{
			m_displayStat = (EWEAPONTEXTDISPLAYSTAT)m_WeaponStatSelectedItem.first;
		}

		EditorUINonSpecificParameters param = EditorUINonSpecificParameters();
		param.m_tooltipText = "When there is no weapon this text is used";
		EditorUI::InputText("No Weapon Text", m_displayWhenNoWeapon, param);
	}
}
#endif

void TextUIWeaponDisplay::Serialize(nlohmann::json& data)
{
	GameplayUIWeaponAttachment::Serialize(data);
	SaveLocalData(data);
}

void TextUIWeaponDisplay::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameplayUIWeaponAttachment::LoadAllPrefabData(jsonData);
	LoadLocalData(jsonData);
}

void TextUIWeaponDisplay::SaveAllPrefabData(nlohmann::json& jsonData)
{
	GameplayUIWeaponAttachment::SaveAllPrefabData(jsonData);
	SaveLocalData(jsonData);
}

void TextUIWeaponDisplay::LoadLocalData(const nlohmann::json& jsonData)
{
	if (jsonData.contains("TextUIWeaponDisplay_StatSelected"))
	{
		m_displayStat = jsonData["TextUIWeaponDisplay_StatSelected"];

#if EDITOR
		m_WeaponStatSelectedItem = GetWeaponStatFromIndex((int)m_displayStat);
#endif
	}

	if (jsonData.contains("TextUIWeaponDisplay_DisplayWhenNoWeapon"))
	{
		m_displayWhenNoWeapon = jsonData["TextUIWeaponDisplay_DisplayWhenNoWeapon"];
	}

}

void TextUIWeaponDisplay::SaveLocalData(nlohmann::json& jsonData)
{
	jsonData["TextUIWeaponDisplay_StatSelected"] = m_displayStat;
	jsonData["TextUIWeaponDisplay_DisplayWhenNoWeapon"] = m_displayWhenNoWeapon;
}

#if EDITOR
list<pair<int, string>> TextUIWeaponDisplay::GetWeaponStatAsList()
{
	list<pair<int, string>> newList = list<pair<int, string>>();
	for (int i = 0; i < (int)EWEAPONTEXTDISPLAYSTAT::COUNT; ++i)
	{
		newList.push_back(GetWeaponStatFromIndex(i));
	}

	return newList;
}

pair<int, string> TextUIWeaponDisplay::GetWeaponStatFromIndex(int index)
{
	pair<int, string> returnPair = pair<int, string>(0, "Key");
	switch (index)
	{
	case 1:
		returnPair = pair<int, string>(1, "Level");
		break;
	case 2:
		returnPair = pair<int, string>(2, "Strength");
		break;
	case 3:
		returnPair = pair<int, string>(3, "Damage");
		break;
	case 4:
		returnPair = pair<int, string>(4, "Shot Count");
		break;
	}

	return returnPair;
}
#endif