#pragma once

#include "Engine/Structure/EditorUIComponent.h"

class Transform : EditorUIComponent
{
	XMFLOAT3 m_localPosition;
	XMFLOAT3 m_localRotation;
	XMFLOAT3 m_localScale = XMFLOAT3(1, 1, 1);

	XMFLOAT3 m_worldPosition;
	XMFLOAT3 m_worldRotation;
	XMFLOAT3 m_worldScale;

	XMFLOAT3 m_forwardVector = XMFLOAT3(0.0f, -1.0f, 0.0f);
	XMFLOAT3 m_upVector = XMFLOAT3(0.0f, 0.0f, 1.0f);;
	XMFLOAT3 m_leftVector = XMFLOAT3(-1.0f, 0.0f, 0.0f);

	Transform* m_pparentTransform;
	vector<Transform*> m_childrenTransformList;


	//Matrices
	XMMATRIX m_scaleMatrix;
	XMMATRIX m_rotationMatrix = XMMatrixIdentity();
	XMMATRIX m_translationalMatrix;
	XMMATRIX m_worldMatrix;

	//flags

public:
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
	void SetPosition(XMFLOAT3& position);
	void SetRotation(XMFLOAT3& rotation);
	void SetRotationMatrix(XMMATRIX& rotationMatrix);
	void SetScale(XMFLOAT3& scale);

	void SetForwardVector(XMFLOAT3& forwardVector);
	void SetUpVector(XMFLOAT3& upVector);
	void SetLeftVector(XMFLOAT3& leftVector);

	void Translate(XMFLOAT3 vector);

#if EDITOR
	void CreateEngineUI() override;
#endif

	void SetParentTransform(Transform* pparentTransform);
	void AddChildTransform(Transform* pchildTransform);
	void RemoveChildTransform(Transform* pchildTransform);
};

