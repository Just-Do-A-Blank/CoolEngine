#pragma once

//Holds the list of possible events
enum class EventType {
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved,
};

class Event {
public:

	////Constructor that sends event with data, manual deconstunction of the data required
	Event(EventType eventID, void* data);

	////Alternate constructor to just call an event proc instead of with data
	Event(EventType eventID);

	//Destructor
	virtual ~Event();

	//Getters

	const EventType GetEventID();
	void* GetData(); //unused but available

protected:

	EventType m_eventID;
	void* m_data;
};

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return m_keyCode; }
protected:
	KeyEvent(int keycode, EventType eventID) : m_keyCode(keycode), Event(eventID) {}
	int m_keyCode;

};

class KeyPressedEvent : public KeyEvent
{
public:
	//Consider repeat
	KeyPressedEvent(int keyCode) : KeyEvent(keyCode,EventType::KeyPressed) {}
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keyCode) : KeyEvent(keyCode,EventType::KeyReleased) {}
};

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