#include "CameraGameObject.h"

CameraGameObject::CameraGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{

}

CameraGameObject::CameraGameObject(json data, CoolUUID index) : GameObject(data, index)
{

	m_windowHeight = data["Window Height"];
	m_windowWidth = data["Window Width"];
	m_nearDepth = data["Near Depth"];
	m_farDepth = data["Far Depth"];

	CreateViewMatrix();
	CreateProjectionMatrix();
}
CameraGameObject::~CameraGameObject()
{
}

void CameraGameObject::Initialize(XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector,
	float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth)
{
	m_windowHeight = windowHeight;
	m_windowWidth = windowWidth;

	m_nearDepth = nearDepth;
	m_farDepth = farDepth;

	m_transform->Initialize(position, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
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
}

void CameraGameObject::Serialize(json& jsonData)
{
	GameObject::Serialize(jsonData);

	jsonData["Window Height"].push_back(m_windowHeight);
	jsonData["Window Width"].push_back(m_windowWidth);
	jsonData["Near Depth"].push_back(m_nearDepth);
	jsonData["Far Depth"].push_back(m_farDepth);

}

void CameraGameObject::Deserialize(json& jsonData)
{
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
