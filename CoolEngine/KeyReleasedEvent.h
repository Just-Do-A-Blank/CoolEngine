#pragma once
#include <KeyEvent.h>

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keyCode) : KeyEvent(keyCode, EventType::KeyReleased) {}
};