#include "MeleeWeaponGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Physics/Shape.h"
#include "Engine/EditorUI/EditorUI.h"

MeleeWeaponGameObject::MeleeWeaponGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::MELEE_WEAPON;
}

MeleeWeaponGameObject::MeleeWeaponGameObject(const nlohmann::json& data, CoolUUID uuid) : WeaponGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::MELEE_WEAPON;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

MeleeWeaponGameObject::MeleeWeaponGameObject(MeleeWeaponGameObject const& other) : WeaponGameObject(other)
{
    m_totalSwingAngle = other.m_totalSwingAngle;
	m_totalChargeTime = other.m_totalChargeTime;
	m_isBlunt = other.m_isBlunt;
}

MeleeWeaponGameObject::~MeleeWeaponGameObject()
{
}

void MeleeWeaponGameObject::SetSwingAngle(float angle)
{
    m_totalSwingAngle = angle;
}

void MeleeWeaponGameObject::SetChargeTime(float time)
{
    m_totalChargeTime = time;
}

void MeleeWeaponGameObject::SetIsBlunt(bool blunt)
{
    m_isBlunt = blunt;
}

void MeleeWeaponGameObject::SetSwingTime(float time)
{
    m_totalSwingTime = time;
}

void MeleeWeaponGameObject::SetIsSwinging(bool swing)
{
    m_isSwinging = swing;
}

void MeleeWeaponGameObject::SetCurrentSwingTime(float time)
{
    m_currentSwingTime = time;
}

void MeleeWeaponGameObject::SetSwingSpeed(float speed)
{
    m_swingSpeed = speed;
}

void MeleeWeaponGameObject::SetCurrentSwingAngle(float angle)
{
    m_currentSwingAngle = angle;
}

void MeleeWeaponGameObject::SetCurrentChargeTime(float time)
{
    m_currentChargeTime = time;
}

float MeleeWeaponGameObject::GetSwingAngle()
{
    return m_totalSwingAngle;
}

float MeleeWeaponGameObject::GetChargeTime()
{
    return m_totalChargeTime;
}

bool MeleeWeaponGameObject::GetIsBlunt()
{
    return m_isBlunt;
}

float MeleeWeaponGameObject::GetSwingTime()
{
    return m_totalSwingTime;
}

bool MeleeWeaponGameObject::GetIsSwinging()
{
    return m_isSwinging;
}

float MeleeWeaponGameObject::GetCurrentSwingTime()
{
    return m_currentSwingTime;
}

float MeleeWeaponGameObject::GetSwingSpeed()
{
    return m_swingSpeed;
}

float MeleeWeaponGameObject::GetCurrentSwingAngle()
{
    return m_currentSwingAngle;
}

float MeleeWeaponGameObject::GetCurrentChargeTime()
{
    return m_currentChargeTime;
}

void MeleeWeaponGameObject::Attack()
{
    if (!m_isSwinging)
    {
        // Can begin attack
		XMFLOAT2 toWeapon = XMFLOAT2(GetTransform()->GetForwardVector().x, GetTransform()->GetForwardVector().y);

        // Starting angle is offset from mouse direction (in degrees)
        m_currentSwingAngle = MathHelper::DotProduct(toWeapon, XMFLOAT2(1, 0));
        m_currentSwingAngle = (std::acosf(m_currentSwingAngle) * 180.0f) / XM_PI;
        if (toWeapon.y < 0)
        {
            m_currentSwingAngle *= -1;
        }
        m_currentSwingAngle -= (m_totalSwingAngle / 2);

        // Used to increase angle
        m_currentSwingTime = m_totalSwingTime;
        m_swingSpeed = m_totalSwingAngle / m_totalSwingTime;
        m_currentChargeTime = m_totalChargeTime;

        m_isSwinging = true;
        GetShape()->SetIsTrigger(false);
    }
}

void MeleeWeaponGameObject::Update()
{
    if (m_isSwinging && m_currentSwingTime > 0.0f)
    {
        XMFLOAT2 toWeapon = XMFLOAT2(std::cosf(XM_PI * m_currentSwingAngle / 180.0f), std::sinf(XM_PI * m_currentSwingAngle / 180.0f));
        SetWeaponPosition(toWeapon);

        float dTime = GameManager::GetInstance()->GetTimer()->DeltaTime();

        if (m_currentChargeTime > 0.0f)
        {
            // Just before attack
            m_currentChargeTime -= dTime;
            if (m_currentChargeTime <= 0.0f)
            {
                GetShape()->SetIsTrigger(true);
            }
        }
        else
        {
            // Swinging weapon
            m_currentSwingAngle += (m_swingSpeed * dTime);
            m_currentSwingTime -= dTime;
            if (m_currentSwingTime <= 0.0f)
            {
                if (m_isBlunt && (!m_isSecondSwing))
                {
                    // Prepare for 2nd swing
                    m_currentSwingTime = m_totalSwingTime;
                    m_swingSpeed = m_totalSwingAngle / m_totalSwingTime;
                    m_swingSpeed *= -1;
                    m_currentChargeTime = m_totalChargeTime;

                    m_isSecondSwing = true;
                    GetShape()->SetIsTrigger(false);
                }
                else
                {
                    // End swing
                    m_isSwinging = false;
                    GetShape()->SetIsTrigger(false);

                    m_isSecondSwing = false;
                }
            }
        }
    }
}

void MeleeWeaponGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);

    SaveLocalData(data);
}

#if EDITOR
void MeleeWeaponGameObject::CreateEngineUI()
{
    WeaponGameObject::CreateEngineUI();

    if (EditorUI::CollapsingSection("Melee Weapon", true))
    {
        EditorUIFloatParameters floatParam = EditorUIFloatParameters();
        EditorUINonSpecificParameters nonParam = EditorUINonSpecificParameters();

        floatParam.m_minValue = 0;
        floatParam.m_maxValue = 720.0f;
        EditorUI::DragFloat("Swing Angle", m_totalSwingAngle, floatParam);

        floatParam.m_minValue = 0;
        floatParam.m_maxValue = 10.0f;
        EditorUI::DragFloat("Swing Time", m_totalSwingTime, floatParam);

        floatParam.m_minValue = 0;
        floatParam.m_maxValue = 5.0f;
        EditorUI::DragFloat("Charge Time", m_totalChargeTime, floatParam);

        EditorUI::Checkbox("Is Blunt", m_isBlunt, nonParam);
    }
}
#endif

void MeleeWeaponGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("SwingAngle"))
    {
        m_totalSwingAngle = jsonData["SwingAngle"];
        m_totalChargeTime = jsonData["ChargeTime"];
        m_isBlunt = jsonData["IsBlunt"];
    }
}

void MeleeWeaponGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["SwingAngle"] = m_totalSwingAngle;
    jsonData["ChargeTime"] = m_totalChargeTime;
    jsonData["IsBlunt"] = m_isBlunt;
}

void MeleeWeaponGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
    WeaponGameObject::LoadAllPrefabData(jsonData);
}

void MeleeWeaponGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    WeaponGameObject::SaveAllPrefabData(jsonData);
}
