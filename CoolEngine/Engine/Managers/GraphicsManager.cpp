#include "GraphicsManager.h"

#include "Engine/Managers/GameManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/ResourceDefines.h"

#include <iostream>
#include <fstream>

void GraphicsManager::Init(ID3D11Device* pdevice)
{
	m_pdevice = pdevice;

	CreateQuadMesh();

	CreateInputLayouts();

	CreateSamplers();

	CompileDefaultShaders();

	m_pperFrameCB = new ConstantBuffer<PerFrameCB>(pdevice);
	m_pdebugPerInstanceCB = new ConstantBuffer<DebugPerInstanceCB>(pdevice);
	m_pperInstanceCB = new ConstantBuffer<PerInstanceCB>(pdevice);
}

bool GraphicsManager::CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{
	//Check if shader has already been compiled
	string shaderType = string(szShaderModel).substr(0, 2);
	if (shaderType == "vs" && m_vertexShaders.count(szFileName) != 0)
	{
		LOG("A vertex shader with that name already exists!");

		return false;
	}
	else if (shaderType == "ps" && m_pixelShaders.count(szFileName) != 0)
	{
		LOG("A pixel shader with that name already exists!");

		return false;
	}

	ID3DBlob* pblob = nullptr;

	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName.c_str(), nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, &pblob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return false;
	}

	if (pErrorBlob != nullptr)
	{
		pErrorBlob->Release();
	}

	//Create shader depending on shader type
	if (shaderType == "vs")
	{
		ID3D11VertexShader* pvertexShader;
		hr = m_pdevice->CreateVertexShader(pblob->GetBufferPointer(), pblob->GetBufferSize(), nullptr, &pvertexShader);

		if (FAILED(hr))
		{
			LOG("Failed to create vertex shader!");

			pblob->Release();

			return false;
		}

		m_vertexShaders[szFileName] = pvertexShader;
	}
	else if (shaderType == "ps")
	{
		ID3D11PixelShader* ppixelShader;
		hr = m_pdevice->CreatePixelShader(pblob->GetBufferPointer(), pblob->GetBufferSize(), nullptr, &ppixelShader);

		if (FAILED(hr))
		{
			LOG("Failed to create pixel shader!");

			pblob->Release();

			return false;
		}

		m_pixelShaders[szFileName] = ppixelShader;
	}
	else
	{
		LOG("That shader type isn't supported!");

		pblob->Release();

		return false;
	}

	pblob->Release();

	return true;
}

bool GraphicsManager::LoadTextureFromFile(wstring filename, size_t maxSize, DDS_ALPHA_MODE* alphaMode)
{
	if (m_textureSRVs.count(filename) != 0)
	{
		LOG("That texture has already been loaded!");

		return false;
	}

	ID3D11ShaderResourceView* psRV;

	wstring fullPath = GameManager::GetInstance()->GetWideWorkingDirectory() + L"\\" + filename;

	if (FAILED(CreateDDSTextureFromFile(m_pdevice, fullPath.c_str(), nullptr, &psRV, maxSize, alphaMode)))
	{
		LOG("Failed to load dds texture file!");

		return false;
	}


	m_textureSRVs[filename] = psRV;

	return true;
}

bool GraphicsManager::LoadAnimationFromFile(wstring animName, size_t maxSize, DDS_ALPHA_MODE* alphaMode)
{
	if (m_animationFrames.count(animName) != 0)
	{
		LOG("Tried to load an animation but one with that name already exists!");

		return false;
	}

	//Get information txt file and parse in
	ifstream file(L"Resources\\Animations\\" + animName + L"\\" + animName + L".txt", std::ios::in);

	if (file.is_open() == false)
	{
		LOG("Failed to load animation from file as couldn't open corresponding text file");

		return false;
	}

	//Get num frames and pre allocate vector memory
	int numFrames;
	file >> numFrames;

	std::vector<Frame>* pframes = new std::vector<Frame>();
	pframes->resize(numFrames);

	//Read in frame times and textures

	wstring frameName;

	for(int i = 0; i < pframes->size(); ++i)
	{
		file >> pframes->at(i).m_frameTime;

		frameName = L"Resources\\Animations\\" + animName + L"\\" + animName + to_wstring(i) + L".dds";

		if (LoadTextureFromFile(frameName, maxSize, alphaMode) == false)
		{
			LOG("Failed to load animation from file as failed to load animation frame!");

			return false;
		}

		pframes->at(i).m_ptexture = m_textureSRVs[frameName];
	}

	m_animationFrames[animName] = pframes;

	return true;
}

void GraphicsManager::SetWindowDimensions(XMFLOAT2 dimensions)
{
	m_windowDimensions = dimensions;
}

ID3D11VertexShader* GraphicsManager::GetVertexShader(wstring name) const
{
	if (m_vertexShaders.count(name) == 0)
	{
		LOG("Tried to get a vertex shader that hasn't been compiled!");

		return nullptr;
	}

	return m_vertexShaders.at(name);
}

ID3D11PixelShader* GraphicsManager::GetPixelShader(wstring name) const
{
	if (m_pixelShaders.count(name) == 0)
	{
		LOG("Tried to get a pixel shader that hasn't been compiled!");

		return nullptr;
	}

	return m_pixelShaders.at(name);
}

ID3D11ShaderResourceView* GraphicsManager::GetShaderResourceView(wstring name) const
{
	if (m_textureSRVs.count(name) == 0)
	{
		if (GetInstance()->LoadTextureFromFile(name) == false)
		{
			return nullptr;
		}
	}

	return m_textureSRVs.at(name);
}

Mesh* GraphicsManager::GetMesh(wstring name) const
{
	if (m_meshes.count(name) == 0)
	{
		LOG("Tried to get a mesh that doesn't exist!");

		return nullptr;
	}

	return m_meshes.at(name);
}

SpriteAnimation& GraphicsManager::GetAnimation(wstring name) const
{
	if (m_animationFrames.count(name) == 0)
	{
		if (GetInstance()->LoadAnimationFromFile(name) == false)
		{
			return SpriteAnimation(nullptr);
		}
	}

	return SpriteAnimation(m_animationFrames.at(name));
}

int GraphicsManager::GetNumLayers()
{
	return m_NumLayers;
}

bool GraphicsManager::IsTextureLoaded(wstring filename)
{
	return m_textureSRVs.count(filename) != 0;
}

const XMFLOAT2& GraphicsManager::GetWindowDimensions() const
{
	return m_windowDimensions;
}

ID3D11InputLayout* GraphicsManager::GetInputLayout(InputLayouts inputLayout) const
{
	return m_inputLayouts[(int)inputLayout];
}

ID3D11SamplerState* GraphicsManager::GetSampler(Samplers sampler)
{
	return m_samplers[(int)sampler];
}

void GraphicsManager::CreateInputLayouts()
{
	m_inputLayouts.resize((int)InputLayouts::COUNT);

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	//Compile dummy shader and use blob to verify the input layout
	ID3DBlob* pblob = nullptr;

	CompileShaderFromFile(POS_TEX_DUMMY_FILE_NAME, "main", "vs_4_0", pblob);

	// Create the input layout
	HRESULT hr = m_pdevice->CreateInputLayout(layout, numElements, pblob->GetBufferPointer(), pblob->GetBufferSize(), &m_inputLayouts[(int)InputLayouts::POS_TEX]);

	pblob->Release();

	if (FAILED(hr))
	{
		LOG("Failed to create input POS_TEX input layout!");
	}
}

void GraphicsManager::CreateQuadMesh()
{
	//Create default mesh for plane
	const Vertex_PosTex quadVertices[4] =
	{
		Vertex_PosTex(XMFLOAT3(-1, 1, 0), XMFLOAT2(0, 0)),
		Vertex_PosTex(XMFLOAT3(-1, -1, 0), XMFLOAT2(0, 1)),
		Vertex_PosTex(XMFLOAT3(1, -1, 0), XMFLOAT2(1, 1)),
		Vertex_PosTex(XMFLOAT3(1, 1, 0), XMFLOAT2(1, 0)),
	};

	const UINT16 quadIndices[6] =
	{
		1, 0, 3,
		1, 3, 2
	};

	ID3D11Buffer* pvertexBuffer;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex_PosTex) * _countof(quadVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = quadVertices;

	m_pdevice->CreateBuffer(&bd, &InitData, &pvertexBuffer);

	ID3D11Buffer* pindexBuffer;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT16) * _countof(quadIndices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = quadIndices;
	m_pdevice->CreateBuffer(&bd, &InitData, &pindexBuffer);

	m_meshes[QUAD_MESH_NAME] = new Mesh(pvertexBuffer, pindexBuffer, sizeof(Vertex_PosTex), 0, _countof(quadIndices));
}

void GraphicsManager::CreateSamplers()
{
	m_samplers.resize((int)Samplers::COUNT);

	//Create linear clamp sampler
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = m_pdevice->CreateSamplerState(&sampDesc, &m_samplers[(int)Samplers::LINEAR_CLAMP]);

	if (FAILED(hr))
	{
		LOG("Failed to create linear clamp sampler!");

		return;
	}

	//Create linear wrap sampler
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pdevice->CreateSamplerState(&sampDesc, &m_samplers[(int)Samplers::LINEAR_WRAP]);

	if (FAILED(hr))
	{
		LOG("Failed to create linear clamp sampler!");

		return;
	}
}

void GraphicsManager::CompileDefaultShaders()
{
	CompileShaderFromFile(DEFAULT_VERTEX_SHADER_NAME, "main", "vs_4_0");
	CompileShaderFromFile(SCREENSPACE_VERTEX_SHADER_NAME, "main", "vs_4_0");
	CompileShaderFromFile(DEFAULT_PIXEL_SHADER_NAME, "main", "ps_4_0");
}

bool GraphicsManager::CompileShaderFromFile(wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob*& pblob)
{
	//Check if shader has already been compiled
	string shaderType = string(szShaderModel).substr(0, 2);
	if (shaderType == "vs" && m_vertexShaders.count(szFileName) != 0)
	{
		LOG("A vertex shader with that name already exists!");

		return false;
	}
	else if (shaderType == "ps" && m_pixelShaders.count(szFileName) != 0)
	{
		LOG("A pixel shader with that name already exists!");

		return false;
	}

	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName.c_str(), nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, &pblob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return false;
	}

	if (pErrorBlob != nullptr)
	{
		pErrorBlob->Release();
	}

	//Create shader depending on shader type
	if (shaderType == "vs")
	{
		ID3D11VertexShader* pvertexShader;
		hr = m_pdevice->CreateVertexShader(pblob->GetBufferPointer(), pblob->GetBufferSize(), nullptr, &pvertexShader);

		if (FAILED(hr))
		{
			LOG("Failed to create vertex shader!");

			pblob->Release();

			return false;
		}

		m_vertexShaders[szFileName] = pvertexShader;
	}
	else if (shaderType == "ps")
	{
		ID3D11PixelShader* ppixelShader;
		hr = m_pdevice->CreatePixelShader(pblob->GetBufferPointer(), pblob->GetBufferSize(), nullptr, &ppixelShader);

		if (FAILED(hr))
		{
			LOG("Failed to create pixel shader!");

			pblob->Release();

			return false;
		}

		m_pixelShaders[szFileName] = ppixelShader;
	}
	else
	{
		LOG("That shader type isn't supported!");

		pblob->Release();

		return false;
	}

	return true;
}
