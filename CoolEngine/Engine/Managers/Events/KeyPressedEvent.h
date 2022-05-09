#pragma once
#include "Engine/Managers/Events/KeyEvent.h"

class KeyPressedEvent : public KeyEvent
{
public:
	//Consider repeat
	KeyPressedEvent(int keyCode) : KeyEvent(keyCode, EventType::KeyPressed) {}
};