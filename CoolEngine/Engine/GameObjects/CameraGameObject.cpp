#include "CameraGameObject.h"

CameraGameObject::CameraGameObject(string identifier) : GameObject(identifier)
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
	XMFLOAT3 position = m_transform->GetPosition();
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

	//int gUID = ;
	std::string gUID = "GUID: ";

	std::string name = "Name: ";
	//name.append();

	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };

	jsonData["CameraGameObject"]["GUID"].push_back(gUID);
	jsonData["CameraGameObject"]["Name"].push_back(name);
	jsonData["CameraGameObject"]["Position"].push_back(position);
	jsonData["CameraGameObject"]["Rotation"].push_back(rotation);
	jsonData["CameraGameObject"]["Scale"].push_back(scale);
	jsonData["CameraGameObject"]["Window Height"].push_back(m_windowHeight);
	jsonData["CameraGameObject"]["Window Width"].push_back(m_windowWidth);
	jsonData["CameraGameObject"]["Near Depth"].push_back(m_nearDepth);
	jsonData["CameraGameObject"]["Far Depth"].push_back(m_farDepth);

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
