#pragma once
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Graphics/ConstantBuffers.h"

#include <string>

class Mesh;

class RenderableGameObject : public GameObject
{
	Mesh* m_pmesh = nullptr;

	ID3D11ShaderResourceView* m_palbedoSRV = nullptr;

	PerInstanceCB m_cb;

	ID3D11PixelShader* m_ppixelShader = nullptr;
	ID3D11VertexShader* m_pvertexShader = nullptr;

public:

	//Getters
	Mesh* GetMesh() const;

	ID3D11ShaderResourceView* GetAlbedoSRV() const;

	const PerInstanceCB& GetCB() const;

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;


	//Setters
	bool SetMesh(std::wstring meshName);
	void SetMesh(Mesh* pmesh);

	bool SetAlbedo(std::wstring albedoName);
	void SetAlbedo(ID3D11ShaderResourceView* psRV);

	void SetCB(PerInstanceCB& cb);

	bool SetVertexShader(std::wstring shaderName);
	void SetVertexShader(ID3D11VertexShader* pvertexShader);

	bool SetPixelShader(std::wstring shaderName);
	void SetPixelShader(ID3D11PixelShader* ppixelShader);

	void Render();
};

