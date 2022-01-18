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

const EventType Event::GetEventID()
{
	return m_eventID;

}

void* Event::GetData()
{
	return m_data;
}