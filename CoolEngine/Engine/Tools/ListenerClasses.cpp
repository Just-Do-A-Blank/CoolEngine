#include "ListenerClasses.h"
#include <iostream>

using namespace std;

void ListenerInherr::HandleEvent(Event* event) {
	//Converts the data a local value that can actually be used
	string* data;
	data = static_cast<string*>(event->GetData());

	switch (event->GetEventID()) {
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