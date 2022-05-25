#include "Transform.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/EditorUI/EditorUI.h"

Transform::Transform()
{
}

Transform::Transform(Transform const& other)
{
	m_localPosition = other.m_localPosition;
	m_localRotation = other.m_localRotation;
	m_localScale = other.m_localScale;

	m_worldPosition = other.m_worldPosition;
	m_worldRotation = other.m_worldRotation;
	m_worldScale = other.m_worldScale;

	m_forwardVector = other.m_forwardVector;
	m_upVector = other.m_upVector;
	m_leftVector = other.m_leftVector;

	m_defaultForwardVector = other.m_defaultForwardVector;
	m_defaultUpVector = other.m_defaultUpVector;
	m_defaultLeftVector = other.m_defaultLeftVector;

	m_pparentTransform = other.m_pparentTransform;
	m_childrenTransformList = other.m_childrenTransformList;
	
	m_scaleMatrix = other.m_scaleMatrix;
	m_rotationMatrix = other.m_rotationMatrix;
	m_translationalMatrix = other.m_translationalMatrix;
	m_worldMatrix = other.m_worldMatrix;
}

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
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw((m_worldRotation.x / 180.0f) * XM_PI, (m_worldRotation.y / 180.0f) * XM_PI, (m_worldRotation.z / 180.0f) * XM_PI);

    XMVECTOR tempForward, tempUp, tempLeft;

    tempForward = XMVector3Transform(XMLoadFloat3(&m_defaultForwardVector), rotationMatrix);
    tempUp = XMVector3Transform(XMLoadFloat3(&m_defaultUpVector), rotationMatrix);
    tempLeft = XMVector3Transform(XMLoadFloat3(&m_defaultLeftVector), rotationMatrix);

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

void Transform::SetWorldPosition(const XMFLOAT3& position)
{
	if (m_pparentTransform == nullptr)
	{
		m_localPosition = position;
	}
	else
	{
		m_localPosition = MathHelper::Minus(position, m_pparentTransform->GetWorldPosition());
	}

	UpdateMatrix();
}

void Transform::SetLocalPosition(XMFLOAT3& position)
{
	m_localPosition = position;
}

void Transform::SetWorldRotation(XMFLOAT3& rotation)
{
	if (m_pparentTransform == nullptr)
	{
		m_localRotation = rotation;
	}
	else
	{
		m_localPosition = MathHelper::Minus(rotation, m_pparentTransform->GetWorldRotation());
	}

	UpdateMatrix();
}

void Transform::SetLocalRotation(XMFLOAT3& rotation)
{
	m_localRotation = rotation;
}

void Transform::SetRotationMatrix(XMMATRIX& rotationMatrix)
{
    m_rotationMatrix = rotationMatrix;

	UpdateMatrix();
}

void Transform::SetWorldScale(XMFLOAT3& scale)
{
	if (m_pparentTransform == nullptr)
	{
		m_localScale = scale;
	}
	else
	{
		m_localPosition = MathHelper::Minus(scale, m_pparentTransform->GetWorldScale());
	}

	UpdateMatrix();
}

void Transform::SetLocalScale(XMFLOAT3& scale)
{
	m_localScale = scale;
}

void Transform::SetForwardVector(XMFLOAT3& forwardVector)
{
	m_defaultForwardVector = forwardVector;

    m_forwardVector = forwardVector;

	UpdateMatrix();
}

void Transform::SetUpVector(XMFLOAT3& upVector)
{
	m_defaultUpVector = upVector;

    m_upVector = upVector;

	UpdateMatrix();
}

void Transform::SetLeftVector(XMFLOAT3& leftVector)
{
	m_defaultLeftVector = leftVector;

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

bool Transform::HasChildTransform(Transform* pchildTransform)
{
	for (int i = 0; i < m_childrenTransformList.size(); ++i)
	{
		if (m_childrenTransformList[i] == pchildTransform)
		{
			return true;
		}
	}
	return false;
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
