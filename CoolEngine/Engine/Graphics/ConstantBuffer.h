#pragma once
template<class T>
class ConstantBuffer
{
	ID3D11Buffer* m_pbuffer;

public:
	ConstantBuffer(ID3D11Device* pdevice)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pdevice->CreateBuffer(&bufferDesc, nullptr, &m_pbuffer);
	}

	~ConstantBuffer()
	{
		m_pbuffer->Release();
	}

	void Update(T& data, ID3D11DeviceContext* pcontext)
	{
		D3D11_MAPPED_SUBRESOURCE resource;

		pcontext->Map(m_pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &data, sizeof(T));
		pcontext->Unmap(m_pbuffer, 0);
	}
};
