#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/KeyEvents.h"
#include "Engine/GameObjects/Gameplay/Player/PlayerController.h"

class PlayerGameObject : public CharacterGameObject
{
public:

	PlayerGameObject(string identifier, CoolUUID uuid);
	PlayerGameObject(const nlohmann::json& data, CoolUUID uuid);
	PlayerGameObject(PlayerGameObject const& other);
	virtual ~PlayerGameObject()override;

	virtual void Serialize(nlohmann::json& data) override;

    /// <summary>
    /// Handles events from the Observations
    /// </summary>
	void Handle(Event* e) override;

    /// <summary>
    /// Update loop for the gameobject
    /// </summary>
    virtual void Update() override;
    virtual void EditorUpdate() override;

#if EDITOR
    /// <summary>
    /// Shows engine UI
    /// </summary>
    virtual void CreateEngineUI() override;
#endif

protected:

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired on enter.
    /// </summary>
    virtual void OnTriggerEnter(GameObject* obj1, GameObject* obj2) override { }

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired every frame.
    /// </summary>
    virtual void OnTriggerHold(GameObject* obj1, GameObject* obj2) override {  }

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired the frame the two stop colliding.
    /// </summary>
    virtual void OnTriggerExit(GameObject* obj1, GameObject* obj2) override { }

private:
    /// <summary>
    /// Handles movement around the scene for the player
    /// </summary>
    PlayerController* m_playerController;

    /// <summary>
    /// Handles any keypresses when they are pressed (frame whilst pressed)
    /// </summary>
	//void KeyPressed(KeyPressedEvent* e);

    /// <summary>
    /// Handles any keypresses when they are released (first frame).
    /// </summary>
	//void KeyReleased(KeyReleasedEvent* e);

    /// <summary>
    /// Handles any mouse button presses when pressed (frame whilst pressed)
    /// </summary>
	//void MouseButtonPressed(MouseButtonPressedEvent* e);

    /// <summary>
    /// Handles any mouse button when they are released (first frame).
    /// </summary>
	//void MouseButtonReleased(MouseButtonReleasedEvent* e);

    /// <summary>
    /// Handles the mouse moving across the window
    /// </summary>
	//void MouseMoved(MouseMovedEvent* e);
};
