#include "CameraGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/EditorUI/EditorUI.h"

CameraGameObject::CameraGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::CAMERA;

	XMFLOAT3 cameraPos = XMFLOAT3(0, 0, -5);
	XMFLOAT3 cameraForward = XMFLOAT3(0, 0, 1);
	XMFLOAT3 cameraUp = XMFLOAT3(0, 1, 0);

	float windowWidth = GraphicsManager::GetInstance()->GetWindowDimensions().x;
	float windowHeight = GraphicsManager::GetInstance()->GetWindowDimensions().y;

	float nearDepth = 0.01f;
	float farDepth = 1000.0f;

	Initialize(cameraPos, cameraForward, cameraUp, windowWidth, windowHeight, nearDepth, farDepth);
}

CameraGameObject::CameraGameObject(const nlohmann::json& data, CoolUUID index) : GameObject(data, index)
{
	m_gameObjectType |= GameObjectType::CAMERA;

    LoadLocalData(data);
}

CameraGameObject::CameraGameObject(CameraGameObject const& other):GameObject(other)
{
	m_viewMatrix = other.m_viewMatrix;
	m_projectionMatrix = other.m_projectionMatrix;

	m_windowHeight = other.m_windowHeight;
	m_windowWidth = other.m_windowWidth;
	m_nearDepth = other.m_nearDepth;
	m_farDepth = other.m_farDepth;
}

CameraGameObject::~CameraGameObject()
{
}

void CameraGameObject::Initialize(const XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector,
	float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth)
{
	m_windowHeight = windowHeight;
	m_windowWidth = windowWidth;

	m_nearDepth = nearDepth;
	m_farDepth = farDepth;

	m_transform->SetForwardVector(forwardVector);
	m_transform->SetUpVector(upVector);

	CreateViewMatrix();
	CreateProjectionMatrix();
}

void CameraGameObject::CreateViewMatrix()
{
	XMFLOAT3 position = m_transform->GetWorldPosition();
	XMFLOAT3 forwardVector = m_transform->GetForwardVector();
	XMFLOAT3 upVector = m_transform->GetUpVector();

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookToLH(XMLoadFloat3(&position),
		XMLoadFloat3(&forwardVector),	XMLoadFloat3(&upVector)));
}

void CameraGameObject::CreateProjectionMatrix()
{
	XMStoreFloat4x4(&m_projectionMatrix, XMMatrixOrthographicLH(m_windowWidth, m_windowHeight, m_nearDepth, m_farDepth));
}

void CameraGameObject::ReshapeCamera(float newWindowWidth, float newWindowHeight, float newNearDepth, float newFarDepth)
{
	m_windowWidth = newWindowWidth;
	m_windowHeight = newWindowHeight;
	m_nearDepth = newNearDepth;
	m_farDepth = newFarDepth;

	CreateProjectionMatrix();
}

void CameraGameObject::Update()
{
	CreateViewMatrix();
	CreateProjectionMatrix();
}

void CameraGameObject::EditorUpdate()
{
}

void CameraGameObject::Serialize(nlohmann::json& jsonData)
{
	GameObject::Serialize(jsonData);

    SaveLocalData(jsonData);
}

void CameraGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("Window Height"))
    {
        m_windowHeight = jsonData["Window Height"];
        m_windowWidth = jsonData["Window Width"];
        m_nearDepth = jsonData["Near Depth"];
        m_farDepth = jsonData["Far Depth"];

        Initialize(m_transform->GetWorldPosition(), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), m_windowWidth, m_windowHeight, m_nearDepth, m_farDepth);

        CreateViewMatrix();
        CreateProjectionMatrix();
    }
}

void CameraGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["Window Height"] = m_windowHeight;
    jsonData["Window Width"] = m_windowWidth;
    jsonData["Near Depth"] = m_nearDepth;
    jsonData["Far Depth"] = m_farDepth;
}

XMFLOAT4X4 CameraGameObject::GetView() const
{
	return m_viewMatrix;
}

XMFLOAT4X4 CameraGameObject::GetProjection() const
{
	return m_projectionMatrix;
}

XMFLOAT4X4 CameraGameObject::GetViewProjection() const
{
	XMFLOAT4X4 viewProj;

	XMStoreFloat4x4(&viewProj, XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projectionMatrix)));

	return viewProj;
}

XMFLOAT2& CameraGameObject::GetMousePositionInWorldSpace()const
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(*GraphicsManager::GetInstance()->GetHWND(), &point);

	float x = 0;
	float y = 0;

#if EDITOR
	DirectX::XMFLOAT2 relativePos = EditorUI::GetViewportPosition();
	relativePos = DirectX::XMFLOAT2(point.x - relativePos.x, point.y - relativePos.y);

	x = ((2.0f * relativePos.x) / EditorUI::GetViewportSize().x) - 1.0f;
	y = 1.0f - ((2.0f * relativePos.y) / EditorUI::GetViewportSize().y);
#else
	x = ((2.0f * point.x) / GraphicsManager::GetInstance()->GetWindowDimensions().x) - 1.0f;
	y = 1.0f - ((2.0f * point.y) / GraphicsManager::GetInstance()->GetWindowDimensions().y);
#endif

	XMFLOAT2 clickPosWorld;

	XMStoreFloat2(&clickPosWorld, XMVector2Transform(XMVectorSet(x, y, 0, 0), XMMatrixInverse(nullptr, XMLoadFloat4x4(&GameManager::GetInstance()->GetCamera()->GetViewProjection()))));

	return clickPosWorld;
}
