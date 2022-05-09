#pragma once
#include <Engine/Managers/Events/Events.h>

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y), Event(EventType::MouseMoved) {}

	inline float GetX() const { return m_mouseX; }
	inline float GetY() const { return m_mouseY; }

private:
	float m_mouseX;
	float m_mouseY;
};

class MouseButtonEvent : public Event
{
public:
	inline int GetButton() const { return m_button; }
protected:
	MouseButtonEvent(int button, EventType eventID) : m_button(button), Event(eventID) {}
	int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button) : MouseButtonEvent(button, EventType::MouseButtonPressed) {}
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button) : MouseButtonEvent(button, EventType::MouseButtonReleased) {}
};