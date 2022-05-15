#pragma once
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Structure/EditorUIComponent.h"
#include "Engine/GameObjects/UUID.h"
#include "Engine/Includes/json.hpp"

class Transform;

enum class UIComponentType
{
	BASE = 1,
	IMAGE = 2,
	TEXT = 4,
	CANVAS = 8,
	BUTTON = 16,
};

DEFINE_ENUM_FLAG_OPERATORS(UIComponentType);

class GameUIComponent : public GameObject
{
	friend FileIO;
private:
	//Graphics variables

	
	int m_layer = 0;

	//Flags
	bool m_isRenderable = true;

protected:
	ID3D11VertexShader* m_pvertexShader = nullptr;
	ID3D11PixelShader* m_ppixelShader = nullptr;	
	Mesh* m_pmesh = nullptr;

	ID3D11ShaderResourceView* m_ptexture = nullptr;
	wstring m_texFilepath = L"";

	UIComponentType m_componentType = (UIComponentType)0;

public:
	GameUIComponent(string identifier, CoolUUID uuid);
	GameUIComponent(nlohmann::json& data, CoolUUID uuid);

	void InitGraphics();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	virtual void Serialize(nlohmann::json& data);

	//Getters
	int& GetLayer();
	const bool& IsRenderable();
	Transform* GetTransform();

	const UIComponentType& GetComponentType() const;

	bool ContainsType(UIComponentType type);

	//Setters
	void SetIsRenderable(bool& condition);
	void SetLayer(const int& layer);

	void SetTexture(std::wstring wsfilepath);

#if EDITOR
	virtual void ShowEngineUI()override;
	virtual void CreateEngineUI() override;
#endif
	UIComponentType GetUIComponentType()const;

};

