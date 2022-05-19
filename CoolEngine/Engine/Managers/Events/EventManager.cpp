#include "EventManager.h"
#include <Windows.h>

using namespace std;


//When removing m_listenerClients or checks if its registered all registered listeners will be checked 
//commented out was using multimap instead of map

EventManager::EventManager()
{

}

EventManager::~EventManager()
{
	this->Shutdown();
}



//Registers client to specific event
void EventManager::AddClient(EventType eventid, Observer* client)
{

	//If the client event pair doenst exist
	if (!IsRegistered(eventid, client))
	{
		m_listenerClients[eventid].push_back(client);
	}
	
	else //Outputs that there is a duplicate and doesnt add it to the list of m_listenerClients since it has no value to have 2
	{
		LOG("Duplicate client eventID combo");
	}
}

bool EventManager::IsRegistered(EventType eventid, Observer* client)
{
	//Assume the client isnt registered 
	bool exists = false;
	//Itterates the current list of listeners to check if there is a duplicate


	for (auto i = 0; i < m_listenerClients[eventid].size(); ++i)
	{
		if (m_listenerClients[eventid][i] == client)
		{
			exists = true;
			break;
		}

	}

	return exists;
}



//Unregisters client from a specific event since a listener can be registered to multiple m_listenerClients at a time it needs to be done this way
void EventManager::RemoveClientEvent(EventType eventid, Observer* client)
{
	for (auto i = 0; i < m_listenerClients[eventid].size(); ++i)
	{
		if (m_listenerClients[eventid][i] == client)
		{
			m_listenerClients[eventid].erase(m_listenerClients[eventid].begin() + i);
			break;
		}

	}


}

//Unregisters client from all events
void EventManager::RemoveClientAllEvents(Observer* client)
{
	//This is kind of messy but no other way to do it unless stored in a different way - store clients with their eventID in a vector (take worst of both evils), itterates each event vector to check for the client
	for (auto i = m_listenerClients.begin(); i != m_listenerClients.end(); ++i)
	{
		for (auto j = 0; j < i->second.size(); ++i)
		{
			if (i->second[j] = client)
			{
				i->second.erase(i->second.begin() + j);
			}
		}
	}

}


//Process specific event. Add to event buffer
void EventManager::AddEvent(EventType eventid, void* data)
{
	//Add to the buffer
	m_eventBuffer.push_back(new Event(eventid,data));
}

void EventManager::AddEvent(Event* event)
{
	m_eventBuffer.push_back(event);
}


//Process event buffer
void EventManager::ProcessEvents() 
{
	while (m_eventBuffer.size()) 
	{
		SendEvent(m_eventBuffer.front());
		//Remove the event from the buffer
		m_eventBuffer.pop_front();
	}
}

//Sends the event to the listener
void EventManager::SendEvent(Event* pevent) 
{
	for (auto i = 0; i < m_listenerClients[pevent->GetEventID()].size(); ++i)
	{
		m_listenerClients[pevent->GetEventID()][i]->Handle(pevent);
	}
}

//Clears the whole Events buffer
void EventManager::ClearBuffer() 
{
	m_eventBuffer.clear();
}

//Clears the whole client list
void EventManager::ClearClients() 
{
	m_listenerClients.clear();
}

//Shutdown event system
void EventManager::Shutdown() 
{
	ClearBuffer();
	ClearClients();
	
}

EventManager* EventManager::Instance()
{
	static EventManager instance;
	return &instance;
}