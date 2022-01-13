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

class ExampleObserver : public Observer
{
public:
	void KeyPressed(KeyPressedEvent* e)
	{
		//Can use 'Letter' or the raw keycode for keyboard inputs.
		if (e->GetKeyCode() == 'C')
		{
			int i = 0;
		}

		if (e->GetKeyCode() == 0x43)
		{
			int i = 0;
		}

		if (e->GetKeyCode() == 0x44)
		{
			int i = 0;
		}
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
	
	//pass in object / values you want the observer to have access to for editing e.g m_playerController* controller; this->m_playerController = playerController then the observer makes all changes basedo on the events. 
	//Event processing all happens in 1 place for each object that wants access to the event classes. Controllers can be placed in their own classes if they have access to the headers for the 
	//Observed class & EventObserver.h for access to the Observer based class for inheritance 
	
	ExampleObserver(int* i) { this->m_i = i; }


private:
	int* m_i;



};