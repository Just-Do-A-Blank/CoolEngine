#include "FileIOCache.h"

FileIOCache::FileIOCache()
{
	m_VertexCache = std::unordered_map<ID3D11VertexShader*, wstring>();
	m_TextureCache = std::unordered_map<ID3D11ShaderResourceView*, wstring>();
	m_PixelCache = std::unordered_map<ID3D11PixelShader*, wstring>();
	m_AlebdoCache = std::unordered_map<ID3D11ShaderResourceView*, wstring>();
	m_XMLCache = std::unordered_map<ID3D11ShaderResourceView*, wstring>();
	m_AnimationCache = std::unordered_map<std::vector<Frame>*, string>();
	m_Map = std::vector<TileMap*>();
	m_MapData = std::vector<wstring>();
	m_ParticleSystems = std::vector<ParticleSystem*>();
	m_ParticleSample = std::vector<Particle*>();
}

FileIOCache::~FileIOCache()
{
	for (Particle* p : m_ParticleSample)
	{
		if (p != nullptr)
		{
			delete p;
		}
	}
}
