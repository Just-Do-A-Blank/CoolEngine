#pragma once
#include "Transform.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/ConstantBuffers.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/ResourceDefines.h"

#include <string>

class SpriteAnimation;
class Shape;

struct RenderStruct;

#define ANIM_NAME_SIZE 50

class GameObject
{
	friend class GameManager;
private:
	//Graphics variables
	ID3D11ShaderResourceView* m_palbedoSRV = nullptr;

	ID3D11VertexShader* m_pvertexShader = nullptr;
	ID3D11PixelShader* m_ppixelShader = nullptr;

	Mesh* m_pmesh = nullptr;

	int m_layer = 0;

	//ImGui variables
	wstring m_texFilepath;

	char m_createDeleteAnimName[ANIM_NAME_SIZE];

	std::unordered_map<std::string, SpriteAnimation> m_animations;

	SpriteAnimation* m_pcurrentAnimation = nullptr;
	string m_currentAnimationName = "";

	string m_identifier;

	//Flags
	bool m_isRenderable = true;

protected:
	Transform m_transform;
	Shape* m_collider = nullptr;

	virtual void CreateEngineUI(ID3D11Device* pdevice);

public:
	GameObject();
	GameObject(string identifier);
	void InitGraphics();

	//Getters
	const bool& IsRenderable();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	virtual void ShowEngineUI(ID3D11Device* pdevice);

	//Getters
	Mesh* GetMesh() const;

	SpriteAnimation& GetAnimation(std::string name);
	std::unordered_map<std::string, SpriteAnimation>* GetAnimations();

	int GetLayer() const;

	bool PlayAnimation(std::string name);

	SpriteAnimation* GetCurrentAnimation();

	ID3D11ShaderResourceView* GetAlbedoSRV() const;

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;

	Transform* GetTransform();

	const string& GetIdentifier();

	bool IsAnimated();

	Shape* GetShape();

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

	void SetShape(Shape* collider);
};
