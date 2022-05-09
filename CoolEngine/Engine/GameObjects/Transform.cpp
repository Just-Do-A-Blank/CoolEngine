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
	m_rotationMatrix = XMMatrixRotationRollPitchYaw((-m_rotation.x/180.0f)*XM_PI, (-m_rotation.y / 180.0f) * XM_PI, (-m_rotation.z / 180.0f) * XM_PI);
	m_scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_translationalMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	m_worldMatrix = m_scaleMatrix * m_rotationMatrix * m_translationalMatrix;
    m_accumulatedRotation = m_rotation;
	if (m_pparentTransform)
	{
		m_worldMatrix = m_worldMatrix * m_pparentTransform->GetWorldMatrix();
        m_accumulatedRotation = MathHelper::Plus(m_accumulatedRotation, m_pparentTransform->GetRotation());
	}


	for (int i = 0; i < m_childrenTransformList.size(); ++i)
	{
		m_childrenTransformList[i]->UpdateMatrix();
	}

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

const XMFLOAT3& Transform::GetAccumulatedRotation() const
{
    return m_accumulatedRotation;
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

const XMMATRIX& Transform::GetTranslationMatrix() const
{
	return m_translationalMatrix;
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

#if EDITOR
void Transform::CreateEngineUI()
{
	EditorUI::DragFloat3("Position", m_position);
	EditorUI::DragFloat3("Rotation", m_rotation);
	EditorUI::DragFloat3("Scale", m_scale);

	UpdateMatrix();
}
#endif

void Transform::SetParentTransform(Transform* pparentTransform)
{
	m_pparentTransform = pparentTransform;
}

void Transform::AddChildTransform(Transform* pchildTransform)
{
    m_childrenTransformList.push_back(pchildTransform);

}

void Transform::RemoveChildTransform(Transform* pchildTransform)
{
    for (vector<Transform*>::iterator it = m_childrenTransformList.begin(); it != m_childrenTransformList.end(); ++it)
    {
        if (*it._Ptr == pchildTransform)
        {
            m_childrenTransformList.erase(it);
        }
    }
}
