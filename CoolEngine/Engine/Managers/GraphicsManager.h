#pragma once
#include "Engine/Structure/Singleton.h"
#include "Engine/Graphics/Vertices.h"
#include "Engine/Graphics/DDSTextureLoader.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/ConstantBuffers.h"

#include <unordered_map>

#define QUAD_MESH_NAME L"quad"

class Mesh;

struct RenderStruct
{
	ID3D11DeviceContext* m_pcontext;
};

class GraphicsManager : public Singleton<GraphicsManager>
{
	std::unordered_map<wstring, ID3D11ShaderResourceView*> m_textureSRVs;

	std::unordered_map<wstring, ID3D11VertexShader*> m_vertexShaders;
	std::unordered_map<wstring, ID3D11PixelShader*> m_pixelShaders;

	std::unordered_map<wstring, Mesh*> m_meshes;

	std::unordered_map<wstring, std::vector<Frame>*> m_animationFrames;

	std::vector<ID3D11InputLayout*> m_inputLayouts;
	std::vector<ID3D11SamplerState*> m_samplers;

	int m_NumLayers = 5;

public:
	ConstantBuffer<PerFrameCB>* m_pperFrameCB = nullptr;
	ConstantBuffer<PerInstanceCB>* m_pperInstanceCB = nullptr;
	ConstantBuffer<DebugPerInstanceCB>* m_pdebugPerInstanceCB = nullptr;

	void Init(ID3D11Device* pdevice);

	bool CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);

	bool LoadTextureFromFile(wstring filename, size_t maxSize = 0, DDS_ALPHA_MODE* alphaMode = nullptr);

	bool LoadAnimationFromFile(wstring animName, size_t maxSize = 0, DDS_ALPHA_MODE* alphaMode = nullptr);

	//Getters
	ID3D11VertexShader* GetVertexShader(wstring name) const;
	ID3D11PixelShader* GetPixelShader(wstring name) const;

	ID3D11ShaderResourceView* GetShaderResourceView(wstring name) const;

	Mesh* GetMesh(wstring name) const;

	SpriteAnimation& GetAnimation(wstring name) const;

	int GetNumLayers();

	bool IsTextureLoaded(wstring filename);

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
	void CreateInputLayouts();

	void CreateQuadMesh();

	void CreateSamplers();

	void CompileDefaultShaders();

	bool CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob*& pblob);

	ID3D11Device* m_pdevice = nullptr;
};

