#pragma once
#include "Events.h"

/// <summary>
/// 
/// Header File contains listeners
/// Methods for listeners may need 
/// 
/// Listeners are being given the information from the event manager and will be told when to execute their code when the event they have "subscribed to" is called
/// 
/// </summary>

class Observer
{
public:

	Observer() {}

	virtual ~Observer() {}

	virtual void Handle(Event* e) = 0;

};

class KeyObserver : public Observer
{
	void Handle(KeyEvent* e);

	KeyObserver() {};


};


class ListenerInherr : public Observer
{
public:
	void Handle(Event* e);

	ListenerInherr(int count) { this->count = count; }

private:
	int count;

};



class PlayerControllerObserver : public Observer
{
public:

	void KeyPressed(KeyPressedEvent* e)
	{

	}

	void KeyReleased(KeyReleasedEvent* e)
	{

	}

	void MouseButtonPressed(MouseButtonPressedEvent* e)
	{

	}

	void MouseButtonReleased(MouseButtonReleasedEvent* e)
	{

	}

	void MouseMoved(MouseMovedEvent* e)
	{

	}

	void Handle(Event* e)
	{
		switch (e->GetEventID())
		{
		case EventType::KeyPressed:
			KeyPressed((KeyPressedEvent*)e);
			break;
		case EventType::KeyReleased:
			KeyReleased((KeyReleasedEvent*)e);
			break;
		case EventType::MouseButtonPressed:
			MouseButtonPressed((MouseButtonPressedEvent*)e);
			break;
		case EventType::MouseButtonReleased:
			MouseButtonReleased((MouseButtonReleasedEvent*)e);
			break;
		case EventType::MouseMoved:
			MouseMoved((MouseMovedEvent*)e);
			break;

		}

	}
	PlayerControllerObserver(int* i) { this->m_i = i; }


private:
	int* m_i;



};