#pragma once
#include "Transform.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/ConstantBuffers.h"

#include <string>

class GameObject
{
	friend class GameManager;
private:
	//Graphics variables
	ID3D11ShaderResourceView* m_palbedoSRV;

	ID3D11VertexShader* m_pvertexShader;
	ID3D11PixelShader* m_ppixelShader;

	Mesh* m_pmesh;

	//Flags
	bool m_isRenderable = false;
	bool m_isCollidable = false;
	bool m_isTrigger = false;

	//GameObject Setup
	void CreateRenderableGameObject();
	void CreateNonRenderableGameObject();

	void AddCollision();
	void AddTrigger();

protected:
	Transform m_transform;

public:
	const bool& IsRenderable();
	const bool& IsCollidable();
	const bool& IsTrigger();

	void Render(ID3D11DeviceContext* pcontext, ConstantBuffer<PerInstanceCB>* pconstantBuffer);

	//Getters
	Mesh* GetMesh() const;

	ID3D11ShaderResourceView* GetAlbedoSRV() const;

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;

	Transform* GetTransform();

	//Setters
	bool SetMesh(wstring meshName);
	void SetMesh(Mesh* pmesh);

	bool SetAlbedo(wstring albedoName);
	void SetAlbedo(ID3D11ShaderResourceView* psRV);

	bool SetVertexShader(wstring shaderName);
	void SetVertexShader(ID3D11VertexShader* pvertexShader);

	bool SetPixelShader(wstring shaderName);
	void SetPixelShader(ID3D11PixelShader* ppixelShader);
};