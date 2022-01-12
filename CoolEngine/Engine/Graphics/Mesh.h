#pragma once

class Mesh
{
	ID3D11Buffer* m_pvertexBuffer = nullptr;
	ID3D11Buffer* m_pindexBuffer = nullptr;

	UINT m_vBStride = 0;
	UINT m_vBOffset = 0;

	UINT m_indexCount = 0;

public:
	Mesh(ID3D11Buffer* pvertexBuffer, ID3D11Buffer* pindexBuffer, UINT vBStride, UINT vBOffset, UINT indexCount)
	{
		m_pvertexBuffer = pvertexBuffer;
		m_pindexBuffer = pindexBuffer;

		m_vBStride = vBStride;
		m_vBOffset = vBOffset;

		m_indexCount = indexCount;
	}

	~Mesh()
	{
		if (m_pvertexBuffer != nullptr)
		{
			m_pvertexBuffer->Release();

			m_pvertexBuffer = nullptr;
		}

		if (m_pindexBuffer != nullptr)
		{
			m_pindexBuffer->Release();

			m_pindexBuffer = nullptr;
		}
	}

	ID3D11Buffer* GetVertexBuffer() const
	{
		return m_pvertexBuffer;
	}

	ID3D11Buffer* GetIndexBuffer() const
	{
		return m_pindexBuffer;
	}

	UINT GetVBStride() const
	{
		return m_vBStride;
	}

	UINT GetVBOffset() const
	{
		return m_vBOffset;
	}

	UINT GetIndexCount() const
	{
		return m_indexCount;
	}
};