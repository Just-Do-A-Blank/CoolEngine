#pragma once

//Holds the list of possible events
enum class EventType {
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved,



};

class Event {
public:

	//Constructor that sends event with data, manual deconstunction of the data required
	Event(EventType eventID, void* data);

	//Alternate constructor to just call an event proc instead of with data
	Event(EventType eventID);

	//Destructor
	~Event();

	//Getters
	const EventType GetEventID();
	void* GetData();

private:

	EventType m_eventID;
	void* m_data;
};