#include "CameraGameObject.h"

void CameraGameObject::Initialize(XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector,
	float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth)
{
	m_windowHeight = windowHeight;
	m_windowWidth = windowWidth;

	m_transform.Initialize(position, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_transform.SetForwardVector(forwardVector);
	m_transform.SetUpVector(upVector);

	CreateViewMatrix();
	CreateProjectionMatrix();
}

void CameraGameObject::CreateViewMatrix()
{
	XMFLOAT3 position = m_transform.GetPosition();
	XMFLOAT3 forwardVector = m_transform.GetForwardVector();
	XMFLOAT3 upVector = m_transform.GetUpVector();

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookToLH(XMLoadFloat3(&position),
		XMLoadFloat(&forwardVector.z),	XMLoadFloat3(&upVector)));
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

void CameraGameObject::Update(float deltaTime)
{
	CreateViewMatrix();
}
