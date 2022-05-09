#pragma once

//Holds the list of possible events
enum class EventType 
{
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved,
	TriggerEnter, TriggerHold, TriggerExit,
	CollisionEnter, CollisionHold, CollisionExit,
};

// Base event for obeservers.
// Particular event types maybe found in the folder such as Collision and KeyPress events
// include them if required.
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