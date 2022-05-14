#include "Transform.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/EditorUI/EditorUI.h"

void Transform::Initialize(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
{
    m_localPosition = position;
    m_localRotation = rotation;
    m_localScale = scale;
}

void Transform::UpdateMatrix()
{
	m_rotationMatrix = XMMatrixRotationRollPitchYaw((m_localRotation.x/180.0f)*XM_PI, (m_localRotation.y / 180.0f) * XM_PI, (m_localRotation.z / 180.0f) * XM_PI);
	m_scaleMatrix = XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);
	m_translationalMatrix = XMMatrixTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);

	m_worldMatrix = m_scaleMatrix * m_rotationMatrix * m_translationalMatrix;
    m_worldRotation = m_localRotation;

	if (m_pparentTransform)
	{
		m_worldMatrix = m_worldMatrix * m_pparentTransform->GetWorldMatrix();
        m_worldRotation = MathHelper::Plus(m_worldRotation, m_pparentTransform->GetWorldRotation());

		XMVECTOR worldPos;
		XMVECTOR worldRotation;
		XMVECTOR worldScale;

		XMMatrixDecompose(&worldScale, &worldRotation, &worldPos, m_worldMatrix);

		XMStoreFloat3(&m_worldPosition, worldPos);
		XMStoreFloat3(&m_worldScale, worldScale);
	}
	else
	{
		m_worldPosition = m_localPosition;
		m_worldRotation = m_localRotation;
		m_worldScale = m_localScale;
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

const XMFLOAT3& Transform::GetWorldPosition() const
{
    return m_worldPosition;
}

const XMFLOAT3& Transform::GetWorldRotation() const
{
    return m_worldRotation;
}

const XMFLOAT3& Transform::GetWorldScale() const
{
    return m_worldScale;
}

const XMFLOAT3& Transform::GetLocalPosition() const
{
	return m_localPosition;
}

const XMFLOAT3& Transform::GetLocalRotation() const
{
	return m_localRotation;
}

const XMFLOAT3& Transform::GetLocalScale() const
{
	return m_localScale;
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

void Transform::SetWorldPosition(XMFLOAT3& position)
{
    m_localPosition = position;

	UpdateMatrix();
}

void Transform::SetLocalPosition(XMFLOAT3& position)
{
	m_localPosition = position;
}

void Transform::SetRotation(XMFLOAT3& rotation)
{
    m_localRotation = rotation;

	UpdateMatrix();
}

void Transform::SetRotationMatrix(XMMATRIX& rotationMatrix)
{
    m_rotationMatrix = rotationMatrix;

	UpdateMatrix();
}

void Transform::SetScale(XMFLOAT3& scale)
{
    m_localScale = scale;

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
    m_localPosition.x += vector.x;
    m_localPosition.y += vector.y;
    m_localPosition.z += vector.z;

    UpdateMatrix();
}

#if EDITOR
void Transform::CreateEngineUI()
{
	EditorUI::DragFloat3("Position", m_localPosition);
	EditorUI::DragFloat3("Rotation", m_localRotation);
	EditorUI::DragFloat3("Scale", m_localScale);

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
			break;
        }
    }
}
