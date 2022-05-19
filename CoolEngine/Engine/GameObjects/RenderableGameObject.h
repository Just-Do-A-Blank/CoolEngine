#pragma once
#include "GameObject.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Graphics/AnimationStateMachine.h"

class Mesh;
class AnimationState;

struct RenderStruct
{
	ID3D11DeviceContext* m_pcontext;
};

class RenderableGameObject : virtual public GameObject
{
public:
	RenderableGameObject();
	RenderableGameObject(RenderableGameObject const& other);
	RenderableGameObject(string identifier, CoolUUID uuid);
	RenderableGameObject(const nlohmann::json& data, CoolUUID uuid);

	virtual ~RenderableGameObject()override;

	//Getters
	const bool& IsRenderable();

	Mesh* GetMesh() const;

	SpriteAnimation GetAnimation(std::string name);

	int GetLayer() const;

	const SpriteAnimation* GetCurrentAnimation();

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

	bool AddAnimationState(string stateName, AnimationState* panimState, bool isActive = false);
	bool RemoveAnimationState(string stateName);

	void InitGraphics();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update() override;
	virtual void EditorUpdate() override;

	void PlayAnimation();
	void PauseAnimation();

	AnimationStateMachine* GetAnimationStateMachine();

	virtual void Serialize(nlohmann::json& data) override;

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

	AnimationStateMachine m_animationStateMachine;

	string m_currentAnimationName = "";

private:

#if EDITOR
	/// <summary>
	/// Creates the animation editor section of the renderable UI
	/// </summary>
	void CreateAnimationEditorUI();
#endif
};

