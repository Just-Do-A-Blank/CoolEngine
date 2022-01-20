#pragma once

class ToolBase
{
public:
	virtual void Init(ID3D11Device* pdevice);
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

protected:
	ID3D11Device* m_pdevice = nullptr;

private:

};

