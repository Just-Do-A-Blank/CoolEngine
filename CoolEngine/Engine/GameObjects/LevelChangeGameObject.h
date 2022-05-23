#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

class LevelChangeGameObject : public TriggerableGameObject
{
public:
	LevelChangeGameObject();
	LevelChangeGameObject(string identifier, CoolUUID uuid);
	LevelChangeGameObject(const nlohmann::json& data, CoolUUID uuid);
	LevelChangeGameObject(LevelChangeGameObject const& other);
	virtual ~LevelChangeGameObject()override;

	virtual void Serialize(nlohmann::json& data) override;

	void SetSceneName(string path);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

protected:
    /// <summary>
    /// Occurs when two objects collide without collision on. Fired every frame.
    /// </summary>
    void OnTriggerHold(GameObject* obj1, GameObject* obj2);

    /// <summary>
    /// Occurs when two objects collide with collision on. Fired every frame.
    /// </summary>
    void OnCollisionHold(GameObject* obj1, GameObject* obj2) { }

    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

private:
	/// <summary>
	/// Determines if an event is a collsion event type
	/// </summary>
	bool IsEventCollision(Event* e);

	/// <summary>
	/// Triggers the events which would be inherited based on collision events
	/// </summary>
	void TriggerCollisionEvents(CollisionEvent*);

	string m_sceneName;
};

