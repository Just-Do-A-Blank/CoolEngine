#include "Transform.h"

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
