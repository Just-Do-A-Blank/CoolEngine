#pragma once
#include "Engine/Structure/Singleton.h"
#include <unordered_map>
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Physics/ParticleSystem.h"
#include "Engine/TileMap/TileMap/TileMap.h"

/// <summary>
/// A singleton designed to cache data for the FileIO cache
/// </summary>
class FileIOCache : Singleton<FileIOCache>
{
public:

	FileIOCache();
	~FileIOCache();

	/// <summary>
	/// Stores the location for vertex shaders
	/// </summary>
	std::unordered_map<ID3D11VertexShader*, wstring> m_VertexCache;

	/// <summary>
	/// Stores the location for textures
	/// </summary>
	std::unordered_map<ID3D11ShaderResourceView*, wstring> m_TextureCache;

	/// <summary>
	/// Stores the location for pixel shaders
	/// </summary>
	std::unordered_map<ID3D11PixelShader*, wstring> m_PixelCache;

	/// <summary>
	/// Stores the albedo texture location
	/// </summary>
	std::unordered_map<ID3D11ShaderResourceView*, wstring> m_AlebdoCache;

	/// <summary>
	/// Stores the animation location
	/// </summary>
	std::unordered_map<std::vector<Frame>*, string> m_AnimationCache;

	/// <summary>
	/// Stores the tile map
	/// </summary>
	std::vector<TileMap*> m_Map;

	/// <summary>
	/// Stores the locarion of map data
	/// </summary>
	std::vector<wstring> m_MapData;

	/// <summary>
	/// Temporary
	/// </summary>
	std::vector<ParticleSystem*> m_ParticleSystems;

	/// <summary>
	/// Temporary
	/// </summary>
	std::vector<Particle*> m_ParticleSample;
};

