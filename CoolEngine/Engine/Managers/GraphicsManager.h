#pragma once
#include "Engine/Structure/Singleton.h"
#include "Engine/Graphics/Vertices.h"
#include "Engine/Graphics/DDSTextureLoader.h"

#include <unordered_map>

#define QUAD_MESH_NAME L"quad"

#define DEFAULT_VERTEX_SHADER_NAME L"Engine/Graphics/Shaders/BasicVertex.hlsl"
#define DEFAULT_PIXEL_SHADER_NAME L"Engine/Graphics/Shaders/BasicPixel.hlsl"

#define POS_TEX_DUMMY_FILE_NAME L"Engine/Graphics/Shaders/DummyPosTex.hlsl"

class Mesh;

class GraphicsManager : public Singleton<GraphicsManager>
{
	std::unordered_map<wstring, ID3D11ShaderResourceView*> m_textureSRVs;

	std::unordered_map<wstring, ID3D11VertexShader*> m_vertexShaders;
	std::unordered_map<wstring, ID3D11PixelShader*> m_pixelShaders;

	std::unordered_map<wstring, Mesh*> m_meshes;

	std::vector<ID3D11InputLayout*> m_inputLayouts;
	std::vector<ID3D11SamplerState*> m_samplers;

public:
	void Init(ID3D11Device* pdevice);

	bool CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pdevice);

	bool LoadTextureFromFile(wstring filename, ID3D11Device* pdevice, size_t maxSize = 0, DDS_ALPHA_MODE* alphaMode = nullptr);

	//Getters
	ID3D11VertexShader* GetVertexShader(wstring name) const;
	ID3D11PixelShader* GetPixelShader(wstring name) const;

	ID3D11ShaderResourceView* GetShaderResourceView(wstring name) const;

	Mesh* GetMesh(wstring name) const;

	enum class InputLayouts
	{
		POS_TEX = 0,
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

private:

	//Init functions
	void CreateInputLayouts(ID3D11Device* pdevice);

	void CreateQuadMesh(ID3D11Device* pdevice);

	void CreateSamplers(ID3D11Device* pdevice);

	void CompileDefaultShaders(ID3D11Device* pdevice);

	bool CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pdevice, ID3DBlob*& pblob);
};

