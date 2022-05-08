#pragma once
#include "Engine/GameObjects/GameObject.h"

//Holds the list of possible events
enum class EventType 
{
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved,
	TriggerEnter, TriggerHold, TriggerExit,
	CollisionEnter, CollisionHold, CollisionExit,
};

class Event 
{
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



class CollisionEvent : public Event
{
public:
	inline GameObject* GetGameObject(int i) { return m_objects[i]; }
protected:
	CollisionEvent(GameObject* obj1, GameObject* obj2, EventType eventID) : Event(eventID)
	{
		m_objects[0] = obj1;
		m_objects[1] = obj2;
	}
	GameObject* m_objects[2];
};

class TriggerEnterEvent : public CollisionEvent
{
public:
	TriggerEnterEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerEnter) {}
};

class TriggerHoldEvent : public CollisionEvent
{
public:
	TriggerHoldEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerHold) {}
};

class TriggerExitEvent : public CollisionEvent
{
public:
	TriggerExitEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::TriggerExit) {}
};

class CollisionEnterEvent : public CollisionEvent
{
public:
	CollisionEnterEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionEnter) {}
};

class CollisionHoldEvent : public CollisionEvent
{
public:
	CollisionHoldEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionHold) {}
};

class CollisionExitEvent : public CollisionEvent
{
public:
	CollisionExitEvent(GameObject* obj1, GameObject* obj2) : CollisionEvent(obj1, obj2, EventType::CollisionExit) {}
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