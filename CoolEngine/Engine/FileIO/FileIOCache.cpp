#include "FileIOCache.h"

FileIOCache::FileIOCache()
{
	m_VertexCache = std::unordered_map<ID3D11VertexShader*, wstring>();
	m_TextureCache = std::unordered_map<ID3D11ShaderResourceView*, wstring>();
	m_PixelCache = std::unordered_map<ID3D11PixelShader*, wstring>();
	m_AlebdoCache = std::unordered_map<ID3D11ShaderResourceView*, wstring>();
	m_AnimationCache = std::unordered_map<std::vector<Frame>*, string>();
	m_Map = std::vector<TileMap*>();
}

FileIOCache::~FileIOCache()
{
}
