#include "WeaponGameObject.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GraphicsManager.h"

WeaponGameObject::WeaponGameObject() : TriggerableGameObject()
{
    m_gameObjectType |= GameObjectType::WEAPON;
    m_key = "";

#if EDITOR
    m_elementsList = GetElementsAsList();
    m_elementSelectedItem = GetElementsFromIndex((int)m_element);

    m_statusList = GetStatusesAsList();
    m_statusSelectedItem = GetStatusesFromIndex((int)m_statusEffect);
#endif

    CalculateWeaponStrength();
}

WeaponGameObject::WeaponGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::WEAPON;
    m_key = "";

#if EDITOR
    m_elementsList = GetElementsAsList();
    m_elementSelectedItem = GetElementsFromIndex((int)m_element);

    m_statusList = GetStatusesAsList();
    m_statusSelectedItem = GetStatusesFromIndex((int)m_statusEffect);
#endif

    CalculateWeaponStrength();
}

WeaponGameObject::WeaponGameObject(const nlohmann::json& data, CoolUUID uuid) : TriggerableGameObject(data, uuid)
{
    m_gameObjectType |= GameObjectType::WEAPON;

#if EDITOR
    m_elementsList = GetElementsAsList();
    m_statusList = GetStatusesAsList();
#endif

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

WeaponGameObject::WeaponGameObject(WeaponGameObject const& other) : TriggerableGameObject(other)
{
	m_level = other.m_level;
	m_strength = other.m_strength;
	m_damage = other.m_damage;

	m_shotCount = other.m_shotCount;
	m_timeLethal = other.m_timeLethal;
	m_distanceTravelled = other.m_distanceTravelled;
    m_radius = other.m_radius;

	ELEMENTS m_element = other.m_element;
	STATUSES m_statusEffect = other.m_statusEffect;

    m_key = other.m_key;
    m_UITexturePath = other.m_UITexturePath;

#if EDITOR
    m_elementsList = GetElementsAsList();
    m_elementSelectedItem = other.m_elementSelectedItem;

    m_statusList = GetStatusesAsList();
    m_statusSelectedItem = other.m_statusSelectedItem;
#endif

    CalculateWeaponStrength();
}

WeaponGameObject::~WeaponGameObject()
{
    UnregisterForEvents();
}

void WeaponGameObject::Serialize(nlohmann::json& data)
{
	TriggerableGameObject::Serialize(data);

    SaveLocalData(data);
}

#if EDITOR
    void WeaponGameObject::CreateEngineUI()
    {
        TriggerableGameObject::CreateEngineUI();

        if (EditorUI::CollapsingSection("Weapon", true))
        {
            EditorUI::InputText("Unique Key", m_key);

            EditorUIIntParameters numberParam = EditorUIIntParameters();
            numberParam.m_minValue = 0;
            numberParam.m_maxValue = 1000;
            EditorUI::DragInt("Level", m_level, numberParam);

            EditorUIFloatParameters floatParam = EditorUIFloatParameters();
            floatParam.m_minValue = 0;
            floatParam.m_maxValue = 1000;
            EditorUI::DragFloat("Damage", m_damage, floatParam);

            EditorUI::DragFloat("Radius", m_radius, floatParam);

            EditorUI::DragInt("Shots", m_shotCount, numberParam);

            EditorUI::DragFloat("Lethal Time", m_timeLethal, floatParam);
            EditorUI::DragFloat("Travel Distance", m_distanceTravelled, floatParam);

            if (EditorUI::ComboBox("Element", m_elementsList, m_elementSelectedItem))
            {
                m_element = (ELEMENTS)m_elementSelectedItem.first;
            }

            if (EditorUI::ComboBox("Status", m_statusList, m_statusSelectedItem))
            {
                m_statusEffect = (STATUSES)m_statusSelectedItem.first;
            }

            EditorUI::Texture("UI Image", m_UITexturePath, m_ptexture, 100, ImVec2(75,75));
        }
    }
#endif

void WeaponGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("WeaponKey"))
    {
        m_key = jsonData["WeaponKey"];
    }

    if (jsonData.contains("WeaponLevel"))
    {
        m_level = jsonData["WeaponLevel"];
        m_strength = jsonData["WeaponStrength"];
        m_damage = jsonData["WeaponDamage"];
        m_shotCount = jsonData["WeaponShotCount"];
        m_timeLethal = jsonData["WeaponTimeLethal"];
        m_distanceTravelled = jsonData["WeaponDistanceTravelled"];
        m_element = (ELEMENTS)jsonData["WeaponElement"];
        m_statusEffect = (STATUSES)jsonData["WeaponStatus"];
        m_radius = jsonData["Radius"];
        if (jsonData.count("WeaponIsHeld"))
        {
            m_isHeld = jsonData["WeaponIsHeld"];
        }
        else
        {
            m_isHeld = false;
        }


#if EDITOR
        m_elementSelectedItem = GetElementsFromIndex((int)m_element);
        m_statusSelectedItem = GetStatusesFromIndex((int)m_statusEffect);
#endif
    }

    if (jsonData.contains("WeaponUITexturePath"))
    {
        std::string tempPath = jsonData["WeaponUITexturePath"];
        m_UITexturePath = std::wstring(tempPath.begin(), tempPath.end());

        SetUITexture(m_UITexturePath);
    }

    CalculateWeaponStrength();
}

void WeaponGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["WeaponKey"] = m_key;
    jsonData["WeaponLevel"] = m_level;
    jsonData["WeaponStrength"] = m_strength;
    jsonData["WeaponDamage"] = m_damage;
    jsonData["WeaponShotCount"] = m_shotCount;
    jsonData["WeaponTimeLethal"] = m_timeLethal;
    jsonData["WeaponDistanceTravelled"] = m_distanceTravelled;
    jsonData["WeaponElement"] = (int)m_element;
    jsonData["WeaponStatus"] = (int)m_statusEffect;
    jsonData["Radius"] = m_radius;
    jsonData["WeaponIsHeld"] = m_isHeld;
    std::string tempPath = std::string(m_UITexturePath.begin(), m_UITexturePath.end());
    jsonData["WeaponUITexturePath"] = tempPath;
}

void WeaponGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
    TriggerableGameObject::LoadAllPrefabData(jsonData);
}

void WeaponGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    TriggerableGameObject::SaveAllPrefabData(jsonData);
}

void WeaponGameObject::Attack()
{

}

bool WeaponGameObject::GetHeld()
{
    return m_isHeld;
}

void WeaponGameObject::SetHeld(bool isHeld)
{
    m_isHeld = isHeld;
}

void WeaponGameObject::CalculateWeaponStrength()
{
    m_strength = ((float)GetIsDualType() + 0.5f) * (m_statusEffect == STATUSES::NONE ? 0.5f : 1.5f) * RoundUp( (10.0f * m_distanceTravelled / m_timeLethal) * m_damage * m_shotCount );
}

void WeaponGameObject::SetLevel(int lvl)
{
    m_level = lvl;
}

void WeaponGameObject::SetStrength(int strength)
{
    m_strength = strength;
}

void WeaponGameObject::SetDamage(int dam)
{
    m_damage = dam;
}

void WeaponGameObject::SetElement(ELEMENTS element)
{
    m_element = element;
}

void WeaponGameObject::SetStatusEffect(STATUSES effect)
{
    m_statusEffect = effect;
}

void WeaponGameObject::SetShotCount(int count)
{
    m_shotCount = count;
}

void WeaponGameObject::SetTimeLethal(float time)
{
    m_timeLethal = time;
}

void WeaponGameObject::SetDistanceTravelled(float dist)
{
    m_distanceTravelled = dist;
}

void WeaponGameObject::SetIsPlayerWeapon(bool player)
{
    m_isPlayerWeapon = player;
}

void WeaponGameObject::SetBulletTexturePath(wstring path)
{
    m_bulletTexturePath = path;
}

void WeaponGameObject::SetBulletScale(XMFLOAT3 scale)
{
    m_bulletScale = scale;
}

void WeaponGameObject::SetCollisionScale(XMFLOAT2 scale)
{
    m_collisionScale = scale;
}

void WeaponGameObject::SetRadius(float rad)
{
    m_radius = rad;
}

void WeaponGameObject::SetHolderPosition(XMFLOAT2 pos)
{
    m_holderPosition = pos;
}

void WeaponGameObject::SetTargetPosition(XMFLOAT2 pos)
{
    m_targetPosition = pos;
}

string WeaponGameObject::GetUniqueKey()
{
    return m_key;
}

int WeaponGameObject::GetLevel()
{
    return m_level;
}

int WeaponGameObject::GetStrength()
{
    return m_strength;
}

int WeaponGameObject::GetDamage()
{
    return m_damage;
}

ELEMENTS WeaponGameObject::GetElement()
{
    return m_element;
}

STATUSES WeaponGameObject::GetStatusEffect()
{
    return m_statusEffect;
}

int WeaponGameObject::GetShotCount()
{
    return m_shotCount;
}

float WeaponGameObject::GetTimeLethal()
{
    return m_timeLethal;
}

float WeaponGameObject::GetDistanceTravelled()
{
    return m_distanceTravelled;
}

bool WeaponGameObject::GetIsPlayerWeapon()
{
    return m_isPlayerWeapon;
}

wstring WeaponGameObject::GetBulletTexturePath()
{
    return m_bulletTexturePath;
}

XMFLOAT3 WeaponGameObject::GetBulletScale()
{
    return m_bulletScale;
}

XMFLOAT2 WeaponGameObject::GetCollisionScale()
{
    return m_collisionScale;
}

float WeaponGameObject::GetRadius()
{
    return m_radius;
}

XMFLOAT2 WeaponGameObject::GetHolderPosition()
{
    return m_holderPosition;
}

XMFLOAT2 WeaponGameObject::GetTargetPosition()
{
    return m_targetPosition;
}

bool WeaponGameObject::GetIsDualType()
{
    return ((int)m_element >= (int)ELEMENTS::POISONFIRE);
}

std::wstring WeaponGameObject::GetUITexturePath()
{
    return m_UITexturePath;
}

int WeaponGameObject::RoundUp(float value)
{
    int temp = (int)value;
    if ((float)temp != value)
    {
        // If not a whole number already
        return (temp + 1);
    }

    return temp;
}

void WeaponGameObject::Handle(Event* e)
{
	switch (e->GetEventID())
	{
	case EventType::MouseButtonPressed:
		{
			MouseButtonPressedEvent* pmouseEvent = (MouseButtonPressedEvent*)e;

			if (pmouseEvent->GetButton() == VK_LBUTTON && m_isPlayerWeapon)
			{
				Attack();
			}
		}
		break;
	}
}

void WeaponGameObject::RegisterForEvents()
{
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
}

void WeaponGameObject::UnregisterForEvents()
{
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
}

void WeaponGameObject::SetWeaponPosition(XMFLOAT2 toWeapon)
{
    XMFLOAT2 weaponPosition = MathHelper::Multiply(toWeapon, m_radius);
    weaponPosition = MathHelper::Plus(m_holderPosition, weaponPosition);

    float angle = MathHelper::DotProduct(toWeapon, XMFLOAT2(0, 1));
    angle = (std::acosf(angle) * 180.0f) / XM_PI;

    if (toWeapon.x > 0.0f)
    {
        angle *= -1.0f;
    }

    GetTransform()->SetWorldPosition(XMFLOAT3(weaponPosition.x, weaponPosition.y, 0.0f));
    GetTransform()->SetWorldRotation(XMFLOAT3(0, 0, angle));
}

#if EDITOR
list<pair<int, string>> WeaponGameObject::GetElementsAsList()
{
    m_elementsList = list<pair<int, string>>();
    for (int i = 0; i < (int)ELEMENTS::COUNT; ++i)
    {
        m_elementsList.push_back(GetElementsFromIndex(i));
    }

    return m_elementsList;
}

pair<int, string> WeaponGameObject::GetElementsFromIndex(int index)
{
    pair<int, string > returnPair = pair<int, string >(0, "None");
    switch (index)
    {
        case 1:
            returnPair = pair<int, string >(index, "Fire");
            break;
        case 2:
            returnPair = pair<int, string >(index, "Water");
            break;
        case 3:
            returnPair = pair<int, string >(index, "Poison");
            break;
        case 4:
            returnPair = pair<int, string >(index, "Electric");
            break;
        case 5:
            returnPair = pair<int, string >(index, "Poison Fire");
            break;
        case 6:
            returnPair = pair<int, string >(index, "Electric Poison");
            break;
        case 7:
            returnPair = pair<int, string >(index, "Water Electric");
            break;
        case 8:
            returnPair = pair<int, string >(index, "Fire Water");
            break;
    }

    return returnPair;
}

list<pair<int, string>> WeaponGameObject::GetStatusesAsList()
{
    m_statusList = list<pair<int, string>>();
    for (int i = 0; i < (int)STATUSES::COUNT; ++i)
    {
        m_statusList.push_back(GetStatusesFromIndex(i));
    }

    return m_statusList;
}

pair<int, string> WeaponGameObject::GetStatusesFromIndex(int index)
{
    pair<int, string > returnPair = pair<int, string >(0, "None");
    switch (index)
    {
    case 1:
        returnPair = pair<int, string >(index, "Bleeding");
        break;
    case 2:
        returnPair = pair<int, string >(index, "Weakness");
        break;
    case 3:
        returnPair = pair<int, string >(index, "Confusion");
        break;
    case 4:
        returnPair = pair<int, string >(index, "Adrenaline");
        break;
    case 5:
        returnPair = pair<int, string >(index, "Charm");
        break;
    }

    return returnPair;
}
#endif

void WeaponGameObject::SetUITexture(std::wstring wsfilepath)
{
    m_ptexture = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

    if (m_ptexture == nullptr)
    {
        LOG("Tried to get a texture that doesn't exist!");
    }
    else
    {
        m_UITexturePath = wsfilepath;
    }
}
