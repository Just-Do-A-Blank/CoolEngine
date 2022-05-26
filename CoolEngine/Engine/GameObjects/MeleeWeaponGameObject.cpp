#include "MeleeWeaponGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Physics/Shape.h"

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
	m_chargeTime = other.m_chargeTime;
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
    m_chargeTime = time;
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

float MeleeWeaponGameObject::GetSwingAngle()
{
    return m_totalSwingAngle;
}

float MeleeWeaponGameObject::GetChargeTime()
{
    return m_chargeTime;
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

void MeleeWeaponGameObject::Attack()
{
    if (!m_isSwinging)
    {
        // Can begin attack
        XMFLOAT2 playerPosWorld = XMFLOAT2(GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().y);
        XMFLOAT2 toWeapon = MathHelper::Minus(GameManager::GetInstance()->GetCamera()->GetMousePositionInWorldSpace(), playerPosWorld);
        toWeapon = MathHelper::Normalize(toWeapon);

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

        m_isSwinging = true;
        GetShape()->SetIsTrigger(true);
    }
}

void MeleeWeaponGameObject::Update()
{
    if (!m_isSwinging)
    {
        XMFLOAT2 toWeapon;
        if (GetIsPlayerWeapon())
        {
            toWeapon = MathHelper::Minus(GameManager::GetInstance()->GetCamera()->GetMousePositionInWorldSpace(), GetHolderPosition());
        }
        else
        {
            toWeapon = MathHelper::Minus(GetTargetPosition(), GetHolderPosition());
        }
        toWeapon = MathHelper::Normalize(toWeapon);

        SetWeaponPosition(toWeapon);
    }
    else if (m_isSwinging && m_currentSwingTime > 0.0f)
    {
        XMFLOAT2 toWeapon = XMFLOAT2(std::cosf(XM_PI * m_currentSwingAngle / 180.0f), std::sinf(XM_PI * m_currentSwingAngle / 180.0f));

        SetWeaponPosition(toWeapon);

        float dTime = GameManager::GetInstance()->GetTimer()->DeltaTime();
        SetCurrentSwingAngle(m_currentSwingAngle + (m_swingSpeed * dTime));
        SetCurrentSwingTime(m_currentSwingTime - dTime);
        if (m_currentSwingTime <= 0.0f)
        {
            m_isSwinging = false;
            GetShape()->SetIsTrigger(false);
        }
    }
}

void MeleeWeaponGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);

    SaveLocalData(data);
}

void MeleeWeaponGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("SwingAngle"))
    {
        m_totalSwingAngle = jsonData["SwingAngle"];
        m_chargeTime = jsonData["ChargeTime"];
        m_isBlunt = jsonData["IsBlunt"];
    }
}

void MeleeWeaponGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["SwingAngle"] = m_totalSwingAngle;
    jsonData["ChargeTime"] = m_chargeTime;
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
