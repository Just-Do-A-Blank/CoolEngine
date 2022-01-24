#pragma once
#include "Engine/Structure/Singleton.h"
#include <unordered_map>
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/TileMap/TileMap/TileMap.h"

class FileIOCache : Singleton<FileIOCache>
{
public:

	FileIOCache();
	~FileIOCache();

	std::unordered_map<ID3D11VertexShader*, wstring> m_VertexCache;
	std::unordered_map<ID3D11ShaderResourceView*, wstring> m_TextureCache;
	std::unordered_map<ID3D11PixelShader*, wstring> m_PixelCache;
	std::unordered_map<ID3D11ShaderResourceView*, wstring> m_AlebdoCache;
	std::unordered_map<std::vector<Frame>*, string> m_AnimationCache;
	std::vector<TileMap*> m_Map;
};

