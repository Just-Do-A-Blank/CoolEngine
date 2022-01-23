#include "Transform.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/EditorUI/EditorUI.h"

void Transform::Initialize(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
{
    m_position = position;
    m_rotation = rotation;
    m_scale = scale;
}

void Transform::UpdateMatrix()
{
	m_scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_translationalMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

    m_worldMatrix = m_scaleMatrix * m_rotationMatrix * m_translationalMatrix;

    UpdateComponentVectors();
}

void Transform::UpdateComponentVectors()
{
    XMVECTOR tempForward, tempUp, tempLeft;
    tempForward = XMVector3TransformNormal(XMLoadFloat3(&m_forwardVector), m_rotationMatrix);
    tempUp = XMVector3TransformNormal(XMLoadFloat3(&m_upVector), m_rotationMatrix);
    tempLeft = XMVector3TransformNormal(XMLoadFloat3(&m_leftVector), m_rotationMatrix);

    XMStoreFloat3(&m_forwardVector, tempForward);
    XMStoreFloat3(&m_upVector, tempUp);
    XMStoreFloat3(&m_leftVector, tempLeft);
}

const XMFLOAT3& Transform::GetPosition() const
{
    return m_position;
}

const XMFLOAT3& Transform::GetRotation() const
{
    return m_rotation;
}

const XMFLOAT3& Transform::GetScale() const
{
    return m_scale;
}

const XMMATRIX& Transform::GetScaleMatrix() const
{
    return m_scaleMatrix;
}

const XMMATRIX& Transform::GetRotationMatrix() const
{
    return m_rotationMatrix;
}

const XMMATRIX& Transform::GetWorldMatrix() const
{
    return m_worldMatrix;
}

const XMFLOAT3& Transform::GetForwardVector() const
{
    return m_forwardVector;
}

const XMFLOAT3& Transform::GetUpVector() const
{
    return m_upVector;
}

const XMFLOAT3& Transform::GetLeftVector() const
{
    return m_leftVector;
}

void Transform::SetPosition(XMFLOAT3& position)
{
    m_position = position;

	UpdateMatrix();
}

void Transform::SetRotation(XMFLOAT3& rotation)
{
    m_rotation = rotation;

    m_rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	UpdateMatrix();
}

void Transform::SetRotationMatrix(XMMATRIX& rotationMatrix)
{
    m_rotationMatrix = rotationMatrix;

	UpdateMatrix();
}

void Transform::SetScale(XMFLOAT3& scale)
{
    m_scale = scale;

	UpdateMatrix();
}

void Transform::SetForwardVector(XMFLOAT3& forwardVector)
{
    m_forwardVector = forwardVector;

	UpdateMatrix();
}

void Transform::SetUpVector(XMFLOAT3& upVector)
{
    m_upVector = upVector;

	UpdateMatrix();
}

void Transform::SetLeftVector(XMFLOAT3& leftVector)
{
    m_leftVector = leftVector;

	UpdateMatrix();
}

void Transform::Translate(XMFLOAT3 vector)
{
    m_position.x += vector.x;
    m_position.y += vector.y;
    m_position.z += vector.z;

    UpdateMatrix();
}

void Transform::CreateEngineUI()
{
	float positionArray[3] =
	{
		m_position.x,
		m_position.y,
		m_position.z
	};

	float rotationArray[3] =
	{
		m_rotation.x,
		m_rotation.y,
		m_rotation.z
	};

	float scaleArray[3] =
	{
		m_scale.x,
		m_scale.y,
		m_scale.z
	};

	if (IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Position", positionArray))
	{
		m_position = XMFLOAT3(positionArray[0], positionArray[1], positionArray[2]);
	}

	if (IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Rotation", rotationArray))
	{
		m_rotation = XMFLOAT3(rotationArray[0], rotationArray[1], rotationArray[2]);
	}

	if (IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Scale", scaleArray))
	{
		m_scale = XMFLOAT3(scaleArray[0], scaleArray[1], scaleArray[2]);
	}
}
