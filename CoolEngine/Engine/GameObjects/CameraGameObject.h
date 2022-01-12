#pragma once
#include "Engine/GameObjects/GameObject.h"

class CameraGameObject : public GameObject
{
private:
    XMFLOAT4X4 m_viewMatrix;
    XMFLOAT4X4 m_projectionMatrix;

    float m_windowHeight;
    float m_windowWidth;
    float m_nearDepth;
    float m_farDepth;

public:
	CameraGameObject(string identifier);

    void Initialize(XMFLOAT3& position, XMFLOAT3& forwardVector, XMFLOAT3& upVector,
        float& windowWidth, float& windowHeight, float& nearDepth, float& farDepth);

    void CreateViewMatrix();
    void CreateProjectionMatrix();

    void ReshapeCamera(float newWindowWidth, float newWindowHeight, float
        newNearDepth, float newFarDepth);

    void Update(float deltaTime);

	//Getters
	XMFLOAT4X4 GetView() const;
	XMFLOAT4X4 GetProjection() const;
	XMFLOAT4X4 GetViewProjection() const;
};

