#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"
#include <MouseEvents.h>
#include <KeyPressedEvent.h>
#include <KeyReleasedEvent.h>

class PlayerGameObject : public CharacterGameObject
{
public:

	PlayerGameObject();
	PlayerGameObject(string identifier);
	~PlayerGameObject();

	void Handle(Event* e) override;

private:
	void KeyPressed(KeyPressedEvent* e);
	void KeyReleased(KeyReleasedEvent* e);
	void MouseButtonPressed(MouseButtonPressedEvent* e);
	void MouseButtonReleased(MouseButtonReleasedEvent* e);
	void MouseMoved(MouseMovedEvent* e);
};