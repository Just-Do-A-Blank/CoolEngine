#include "EventManager.h"
#include <Windows.h>

using namespace std;


//When removing m_clients or checks if its registered all registered listeners will be checked 
//commented out was using multimap instead of map

EventManager::EventManager()
{

}

EventManager::~EventManager()
{
	this->Shutdown();
}



//Registers client to specific event
void EventManager::AddClient(EventType eventid, Listener* client)
{

	//If the client event pair doenst exist
	if (!IsRegistered(eventid, client))
	{
		m_client[eventid].push_back(client);
		//m_clients.insert(make_pair(eventid, client));
	}
	
	else //Outputs that there is a duplicate and doesnt add it to the list of m_clients since it has no value to have 2
		OutputDebugStringA("Duplicate client eventID combo\n");

}

bool EventManager::IsRegistered(EventType eventid, Listener* client)
{
	//Assume the client isnt registered 
	bool exists = false;
	//Itterates the current list of listeners to check if there is a duplicate
	//for (auto i = m_clients.begin(); i != m_clients.end(); i++) {
	//	if (i->first == eventid) {
	//		if (i->second == client) {
	//			exists = true;
	//			break;
	//		}
	//	}
	//}

	for (auto i = 0; i < m_client[eventid].size(); ++i)
	{
		if (m_client[eventid][i] == client)
		{
			exists = true;
			break;
		}

	}



	return exists;
}



//Unregisters client from a specific event since a listener can be registered to multiple m_clients at a time it needs to be done this way
void EventManager::RemoveClientEvent(EventType eventid, Listener* client)
{
	//for (auto i = m_clients.begin(); i != m_clients.end(); i++) {
	//	if (i->first == eventid) {
	//		if (i->second == client) {
	//			//Delete the client in question. No need to keep going since its a unique and more cant exist
	//			i = m_clients.erase(i);
	//			break;
	//		}
	//	}
	//}

	for (auto i = 0; i < m_client[eventid].size(); ++i)
	{
		if (m_client[eventid][i] == client)
		{
			m_client[eventid].erase(m_client[eventid].begin() + i);
			break;
		}

	}


}

//Unregisters client from all events
void EventManager::RemoveClientAllEvents(Listener* client)
{
	//for (auto i = m_clients.begin(); i != m_clients.end(); i++) 
	//{
	//	if (i->second == client) {
	//		//Delete this client, keep going
	//		i = m_clients.erase(i);
	//	}
	//}

	//This is kind of messy but no other way to do it unless stored in a different way - store clients with their eventID in a vector (take worst of both evils), itterates each event vector to check for the client
	for (auto i = m_client.begin(); i != m_client.end(); ++i)
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
	Event newEvent(eventid, data);
	//Add to the buffer
	m_eventBuffer.push_back(newEvent);
}

//Process event buffer
void EventManager::ProcessEvents() 
{
	while (m_eventBuffer.size()) {
		SendEvent(&m_eventBuffer.front());
		//Remove the event from the buffer
		m_eventBuffer.pop_front();
	}
}

//Sends the event to the listener
void EventManager::SendEvent(Event* event) 
{
	//for (auto i = m_clients.begin(); i != m_clients.end(); i++) {
	//	if (i->first == event->GetEventID()) {
	//		i->second->HandleEvent(event);
	//	}
	//}

	for (auto i = 0; i < m_client[event->GetEventID()].size(); ++i)
	{
		m_client[event->GetEventID()][i]->HandleEvent(event);
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
	m_clients.clear();
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