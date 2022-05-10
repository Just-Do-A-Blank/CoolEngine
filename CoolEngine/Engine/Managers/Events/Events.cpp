#include "Events.h"

Event::Event(EventType eventID, void* data)
{
	this->m_data = data;
	this->m_eventID = eventID;
}

Event::Event(EventType eventID)
{
	this->m_eventID = eventID;
	this->m_data = nullptr;
}

Event::~Event()
{
	m_data = nullptr;
}

/// <summary>
/// The event ID which in this context is the broad type of the event for casting
/// </summary>
const EventType Event::GetEventID()
{
	return m_eventID;

}

/// <summary>
/// Get the data passed in when creating the event.
/// Generally not required, casting is prefered.
/// </summary>
void* Event::GetData()
{
	return m_data;
}