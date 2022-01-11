#include "Transform.h"

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

void Transform::SetPosition(XMFLOAT3& position)
{
    m_position = position;
}

void Transform::SetRotation(XMFLOAT3& rotation)
{
    m_rotation = rotation;
}

void Transform::SetScale(XMFLOAT3& scale)
{
    m_scale = scale;
}
