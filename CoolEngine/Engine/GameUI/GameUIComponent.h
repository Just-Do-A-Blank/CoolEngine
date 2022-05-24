#pragma once
#include "Engine/GameUI/GameUIPrefab.h"
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

enum class AccumulatedUIComponentType
{
	BASE = (int)UIComponentType::BASE,
	IMAGE = (BASE | (int)UIComponentType::IMAGE),
	TEXT = (BASE | (int)UIComponentType::TEXT),
	CANVAS = (BASE | (int)UIComponentType::CANVAS),
	BUTTON = (BASE | (int)UIComponentType::BUTTON),
};

DEFINE_ENUM_FLAG_OPERATORS(UIComponentType);

class GameUIComponent : public GameUIPrefab
{
	friend FileIO;
private:
	//Flags
	bool m_isRenderable = true;

protected:
	ID3D11ShaderResourceView* m_ptexture = nullptr;
	wstring m_texFilepath = L"";

	UIComponentType m_uiComponentType = (UIComponentType)0;
	int m_layer = 0;

public:
	GameUIComponent(string identifier, CoolUUID uuid);
	GameUIComponent(nlohmann::json& data, CoolUUID uuid);
	GameUIComponent(GameUIComponent const& other);

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update()override;
	virtual void EditorUpdate()override;

	virtual void Serialize(nlohmann::json& data) override;

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

protected:
	virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
	virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

private:
	void LoadLocalData(const nlohmann::json& jsonData);
	void SaveLocalData(nlohmann::json& jsonData);
};

