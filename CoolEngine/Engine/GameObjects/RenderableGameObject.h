#pragma once
#include "GameObject.h"
#include "Engine/Graphics/SpriteAnimation.h"

class Mesh;

struct RenderStruct
{
	ID3D11DeviceContext* m_pcontext;
};

class RenderableGameObject : virtual public GameObject
{
public:
	RenderableGameObject();
	RenderableGameObject(string identifier, CoolUUID uuid);


	//Getters
	const bool& IsRenderable();

	Mesh* GetMesh() const;

	SpriteAnimation GetAnimation(std::string name);
	std::unordered_map<std::string, SpriteAnimation>* GetAnimations();

	int GetLayer() const;

	SpriteAnimation* GetCurrentAnimation();

	ID3D11ShaderResourceView* GetAlbedoSRV() const;

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;

	bool IsAnimated();

	//Setters
	bool SetMesh(wstring meshName);
	void SetMesh(Mesh* pmesh);

	bool SetAlbedo(wstring albedoName);
	void SetAlbedo(ID3D11ShaderResourceView* psRV);

	bool SetVertexShader(wstring shaderName);
	void SetVertexShader(ID3D11VertexShader* pvertexShader);

	bool SetPixelShader(wstring shaderName);
	void SetPixelShader(ID3D11PixelShader* ppixelShader);

	void SetIsRenderable(bool& condition);

	void SetLayer(int layer);

	bool AddAnimation(string animName, SpriteAnimation& anim);
	bool AddAnimation(string localAnimName, wstring animName);
	bool RemoveAnimation(string animName);
	bool OverwriteAnimation(string animName, SpriteAnimation& anim);


	void InitGraphics();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update() override;

	bool PlayAnimation(std::string name);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

protected:
	//Graphics variables
	ID3D11ShaderResourceView* m_palbedoSRV = nullptr;

	ID3D11VertexShader* m_pvertexShader = nullptr;
	ID3D11PixelShader* m_ppixelShader = nullptr;

	Mesh* m_pmesh = nullptr;

	int m_layer = 0;

	//Flags
	bool m_isRenderable = true;

#if EDITOR
	//ImGui variables
	wstring m_texFilepath;

	char m_createDeleteAnimName[ANIM_NAME_SIZE];
#endif

	std::unordered_map<std::string, SpriteAnimation> m_animations;

	SpriteAnimation* m_pcurrentAnimation = nullptr;
	string m_currentAnimationName = "";

private:

};

