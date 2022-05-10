#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/KeyEvents.h"

class PlayerGameObject : public CharacterGameObject
{
public:

	PlayerGameObject();
	PlayerGameObject(string identifier, CoolUUID uuid);
	~PlayerGameObject();

	void Handle(Event* e) override;

protected:
    // Occurs when two objects collide with collision on. Fired on enter.
    virtual void OnCollisionEnter(GameObject* obj1, GameObject* obj2) override;

    // Occurs when two objects collide with collision on. Fired the frame the two stop colliding.
    virtual void OnCollisionExit(GameObject* obj1, GameObject* obj2) override;

    // Occurs when two objects collide with collision on. Fired every frame.
    virtual void OnCollisionHold(GameObject* obj1, GameObject* obj2) override;

    // Occurs when two objects collide without collision on. Fired on enter.
    virtual void OnTriggerEnter(GameObject* obj1, GameObject* obj2) override { LOG("TriggerableGameObject: OnTriggerEnter"); }

    // Occurs when two objects collide without collision on. Fired every frame.
    virtual void OnTriggerHold(GameObject* obj1, GameObject* obj2) override {  }

    // Occurs when two objects collide without collision on. Fired the frame the two stop colliding.
    virtual void OnTriggerExit(GameObject* obj1, GameObject* obj2) override { LOG("TriggerableGameObject: OnTriggerExit"); }

private:
	void KeyPressed(KeyPressedEvent* e);
	void KeyReleased(KeyReleasedEvent* e);
	void MouseButtonPressed(MouseButtonPressedEvent* e);
	void MouseButtonReleased(MouseButtonReleasedEvent* e);
	void MouseMoved(MouseMovedEvent* e);
};
