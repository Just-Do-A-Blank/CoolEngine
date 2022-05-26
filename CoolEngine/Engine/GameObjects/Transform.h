#pragma once

#include "Engine/Structure/EditorUIComponent.h"

#define FORWARD_DEFAULT XMFLOAT3(0.0f, 1.0f, 0.0f)
#define UP_DEFAULT XMFLOAT3(0.0f, 0.0f, 1.0f)
#define LEFT_DEFAULT XMFLOAT3(-1.0f, 0.0f, 0.0f)

class Transform : EditorUIComponent
{
	XMFLOAT3 m_localPosition;
	XMFLOAT3 m_localRotation;
	XMFLOAT3 m_localScale = XMFLOAT3(1, 1, 1);

	XMFLOAT3 m_worldPosition;
	XMFLOAT3 m_worldRotation;
	XMFLOAT3 m_worldScale;

	XMFLOAT3 m_forwardVector = FORWARD_DEFAULT;
	XMFLOAT3 m_upVector = UP_DEFAULT;
	XMFLOAT3 m_leftVector = LEFT_DEFAULT;

	XMFLOAT3 m_defaultForwardVector = FORWARD_DEFAULT;
	XMFLOAT3 m_defaultUpVector = UP_DEFAULT;
	XMFLOAT3 m_defaultLeftVector = LEFT_DEFAULT;

	Transform* m_pparentTransform;
	vector<Transform*> m_childrenTransformList;

	//Matrices
	XMMATRIX m_scaleMatrix;
	XMMATRIX m_rotationMatrix = XMMatrixIdentity();
	XMMATRIX m_translationalMatrix;
	XMMATRIX m_worldMatrix;

	//flags

public:
	Transform();
	Transform(Transform const& other);
	void Initialize(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);

	void UpdateMatrix();
	void UpdateComponentVectors();

	//Getters
	const XMFLOAT3& GetWorldPosition()const;
	const XMFLOAT3& GetWorldRotation()const;
	const XMFLOAT3& GetWorldScale()const;

	const XMFLOAT3& GetLocalPosition()const;
	const XMFLOAT3& GetLocalRotation()const;
	const XMFLOAT3& GetLocalScale()const;

	const XMMATRIX& GetScaleMatrix()const;
	const XMMATRIX& GetRotationMatrix()const;
	const XMMATRIX& GetTranslationMatrix()const;
	const XMMATRIX& GetWorldMatrix()const;

	const XMFLOAT3& GetForwardVector()const;
	const XMFLOAT3& GetUpVector()const;
	const XMFLOAT3& GetLeftVector()const;

	//Setters
	void SetWorldPosition(const XMFLOAT3& position);
	void SetLocalPosition(XMFLOAT3& position);

	void SetWorldRotation(XMFLOAT3& rotation);
	void SetLocalRotation(XMFLOAT3& rotation);

	void SetRotationMatrix(XMMATRIX& rotationMatrix);

	void SetWorldScale(XMFLOAT3& scale);
	void SetLocalScale(XMFLOAT3& scale);

	void SetForwardVector(XMFLOAT3& forwardVector);
	void SetUpVector(XMFLOAT3& upVector);
	void SetLeftVector(XMFLOAT3& leftVector);

	void Translate(XMFLOAT3 vector);

#if EDITOR
	void CreateEngineUI() override;
#endif

	void SetParentTransform(Transform* pparentTransform);
	void AddChildTransform(Transform* pchildTransform);
	bool HasChildTransform(Transform* pchildTransform);
	void RemoveChildTransform(Transform* pchildTransform);
};

