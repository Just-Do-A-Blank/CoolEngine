#pragma once
#include <Engine/Managers/Events/Events.h>

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return m_keyCode; }

protected:
	KeyEvent(int keycode, EventType eventID) : m_keyCode(keycode), Event(eventID) {}
	int m_keyCode;
};