#pragma once


class Transform
{
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale = XMFLOAT3(1, 1, 1);

	XMFLOAT3 m_forwardVector;
	XMFLOAT3 m_upVector;
	XMFLOAT3 m_leftVector;

	//Matrices
	XMMATRIX m_scaleMatrix;
	XMMATRIX m_rotationMatrix = XMMatrixIdentity();
	XMMATRIX m_translationalMatrix;
	XMMATRIX m_worldMatrix;

	//flags

public:
	void Initialize(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);

	void UpdateMatrix();

	//Getters
	const XMFLOAT3& GetPosition()const;
	const XMFLOAT3& GetRotation()const;
	const XMFLOAT3& GetScale()const;

	const XMMATRIX& GetScaleMatrix()const;
	const XMMATRIX& GetRotationMatrix()const;
	const XMMATRIX& GetWorldMatrix()const;

	const XMFLOAT3& GetForwardVector()const;
	const XMFLOAT3& GetUpVector()const;
	const XMFLOAT3& GetLeftVector()const;

	//Setters
	void SetPosition(XMFLOAT3& position);
	void SetRotation(XMFLOAT3& rotation);
	void SetRotationMatrix(XMMATRIX& rotationMatrix);
	void SetScale(XMFLOAT3& scale);

	void SetForwardVector(XMFLOAT3& forwardVector);
	void SetUpVector(XMFLOAT3& upVector);
	void SetLeftVector(XMFLOAT3& leftVector);
};

