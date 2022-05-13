#pragma once
#include "Engine/Structure/Manager.h"
#include "Engine/Graphics/Vertices.h"
#include "Engine/Graphics/DDSTextureLoader.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/ConstantBuffers.h"
#include "Engine/Includes/DirectXTK/SpriteBatch.h"

#include <unordered_map>

#define QUAD_MESH_NAME L"quad"

class Mesh;

class GraphicsManager : public Manager<GraphicsManager>
{
	std::unordered_map<wstring, ID3D11ShaderResourceView*> m_textureSRVs;
	std::unordered_map<wstring, ID3D11Resource*> m_textureResources;

	std::unordered_map<wstring, ID3D11VertexShader*> m_vertexShaders;
	std::unordered_map<wstring, ID3D11PixelShader*> m_pixelShaders;

	std::unordered_map<wstring, Mesh*> m_meshes;

	std::unordered_map<wstring, std::vector<Frame>*> m_animationFrames;

	std::vector<ID3D11InputLayout*> m_inputLayouts;
	std::vector<ID3D11SamplerState*> m_samplers;

	static const int s_kNumLayers = 5;

	XMFLOAT2 m_windowDimensions = XMFLOAT2(1920, 1080);

public:
	ConstantBuffer<PerFrameCB>* m_pperFrameCB = nullptr;
	ConstantBuffer<PerInstanceCB>* m_pperInstanceCB = nullptr;
	ConstantBuffer<DebugPerInstanceCB>* m_pdebugPerInstanceCB = nullptr;
	ConstantBuffer<TextPerInstanceCB>* m_ptextPerInstanceCB = nullptr;

	void Init(ID3D11Device* pdevice, ID3D11DeviceContext* pcontext);

	bool CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);

	bool LoadTextureFromFile(wstring filename, size_t maxSize = 0, DDS_ALPHA_MODE* alphaMode = nullptr);

	bool LoadAnimationFromFile(wstring animName, size_t maxSize = 0, DDS_ALPHA_MODE* alphaMode = nullptr);

	void SetWindowDimensions(XMFLOAT2 dimensions);

	void SetHWND(HWND* hwnd);

	void RenderQuad(ID3D11ShaderResourceView* psrv, XMFLOAT3 position, XMFLOAT3 scale, float rotation, int layer);

	std::unique_ptr<DirectX::SpriteBatch>* GetSpriteBatches();

	//Getters
	ID3D11VertexShader* GetVertexShader(wstring name) const;
	ID3D11PixelShader* GetPixelShader(wstring name) const;

	ID3D11ShaderResourceView* GetShaderResourceView(wstring name) const;
	ID3D11Resource* GetResource(wstring name) const;

	Mesh* GetMesh(wstring name) const;

	SpriteAnimation GetAnimation(wstring name) const;

	int GetNumLayers();

	bool IsTextureLoaded(wstring filename);

	const XMFLOAT2& GetWindowDimensions() const;

	HWND* GetHWND();

	ID3D11Device* GetDevice();

	enum class InputLayouts
	{
		POS_TEX = 0,
		POS_TEX_COLOR,
		COUNT
	};

	ID3D11InputLayout* GetInputLayout(InputLayouts inputLayout) const;

	enum class CBOrders
	{
		PER_FRAME = 0,
		PER_INSTANCE,
		COUNT
	};

	enum class Samplers
	{
		LINEAR_CLAMP = 0,
		LINEAR_WRAP,
		COUNT
	};

	ID3D11SamplerState* GetSampler(Samplers sampler);

	void Serialize(nlohmann::json& data) override;
	void Deserialize(nlohmann::json& data) override;

private:

	//Init functions
	void CreateInputLayouts();

	void CreateQuadMesh();

	void CreateSamplers();

	void CompileDefaultShaders();

	bool CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob*& pblob);

	ID3D11Device* m_pdevice = nullptr;

	HWND* m_pHWND = nullptr;

	std::unique_ptr<DirectX::SpriteBatch> m_pBatches[GraphicsManager::s_kNumLayers] = { nullptr };
};

