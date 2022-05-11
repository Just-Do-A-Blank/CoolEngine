#pragma once

#if EDITOR

class ToolBase
{
public:
	virtual void Init(ID3D11Device* pdevice);
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	const bool& HasToolExited() const;

protected:
	virtual void CreateMenuBar();

	ID3D11Device* m_pdevice = nullptr;

private:
	std::string m_previousSceneIdentifier = "TestScene";
	bool m_exitTool = false;
};

#endif