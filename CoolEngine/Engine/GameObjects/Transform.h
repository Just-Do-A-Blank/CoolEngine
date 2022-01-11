#pragma once


class Transform
{
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;



public:
	//Getters
	const XMFLOAT3& GetPosition()const;
	const XMFLOAT3& GetRotation()const;
	const XMFLOAT3& GetScale()const;

	//Setters
	void SetPosition(XMFLOAT3& position);
	void SetRotation(XMFLOAT3& rotation);
	void SetScale(XMFLOAT3& scale);
};

