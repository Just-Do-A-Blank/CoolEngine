#pragma once
#include "Transform.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/ConstantBuffers.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/EditorUI/EditorUI.h"

#include <string>

class SpriteAnimation;
class Shape;

struct RenderStruct;

class GameObject
{
	friend class GameManager;
private:
	//Graphics variables
	ID3D11ShaderResourceView* m_palbedoSRV;

	ID3D11VertexShader* m_pvertexShader;
	ID3D11PixelShader* m_ppixelShader;

	Mesh* m_pmesh;

	int m_layer = 0;

	std::unordered_map<std::string, SpriteAnimation> m_animations;

	SpriteAnimation* m_pcurrentAnimation;

	string m_identifier;

	//Flags
	bool m_isRenderable = true;
	bool m_isCollidable = false;
	bool m_isTrigger = false;

	//ImGui variables
	WCHAR m_texNameBuffer[FILEPATH_BUFFER_SIZE] = DEFAULT_IMGUI_IMAGE;
	char m_animName[ANIM_NAME_SIZE];
	WCHAR m_animFilepath[FILEPATH_BUFFER_SIZE] = DEFAULT_IMGUI_ANIMATION;

	std::string m_animUpdateName = "";

	bool m_updateAnim = false;
	bool m_updateAnimName = false;

	//GameObject Setup
	void CreateRenderableGameObject();
	void CreateNonRenderableGameObject();

	void AddCollision();
	void AddTrigger();

protected:
	Transform m_transform;
	Shape* m_collider;

public:
	GameObject();
	GameObject(string identifier);

	//Getters
	const bool& IsRenderable();
	const bool& IsCollidable();
	const bool& IsTrigger();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	virtual void ShowEngineUI(ID3D11Device* pdevice);
	virtual void CreateEngineUI(ID3D11Device* pdevice);

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
	void SetIsCollidable(bool& condition);
	void SetIsTrigger(bool& condition);

	void SetLayer(int layer);

	bool AddAnimation(string animName, SpriteAnimation& anim);
	bool AddAnimation(string localAnimName, wstring animName);
	bool RemoveAnimation(string animName);
	bool OverwriteAnimation(string animName, SpriteAnimation& anim);

	void SetShape(Shape* collider);
};
