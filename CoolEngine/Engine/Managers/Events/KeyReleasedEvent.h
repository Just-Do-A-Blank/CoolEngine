#pragma once
#include "Engine/Managers/Events/KeyEvent.h"

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keyCode) : KeyEvent(keyCode, EventType::KeyReleased) {}
};