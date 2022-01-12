#include "EventObserver.h"
#include <iostream>

using namespace std;

void ListenerInherr::Handle(Event* e) {
	//Converts the data a local value that can actually be used
	string* data;
	data = static_cast<string*>(e->GetData());

	switch (e->GetEventID()) 
	{
	case EventType::KeyPressed:
		cout << count << ": Event1: " << *data << endl;
		break;
	case EventType::KeyReleased:
		cout << count << ": Event2: " << *data << endl;
		break;
	case EventType::MouseButtonPressed:
		cout << count << ": Event3: " << *data << endl;
		break;

	}
}

void KeyObserver::Handle(KeyEvent* e)
{
	int keyCode = e->GetKeyCode();

	switch (e->GetEventID())
	{
	case EventType::KeyPressed:
		switch (keyCode)
		{
			






		}


	}



}