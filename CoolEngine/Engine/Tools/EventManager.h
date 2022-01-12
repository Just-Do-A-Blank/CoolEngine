#pragma once
#include <map>
#include <list>
#include "Events.h"
#include <vector>

///
/// To add to the event system with a listener								EventSystem::Instance()->AddClient(EVENTID::Event1, &listener);
/// To push an event into the system										EventSystem::Instance()->AddEvent(EVENTID::Event1, data); data is not needed but possible
/// In the update you need to execute the event buffer with					EventSystem::Instance()->ProcessEvents();
/// List of events stored in Event.h
/// To create new listener type new class inherrited from listener needed
/// 

class Listener
{
public:

	Listener() {}

	virtual ~Listener() {}

	virtual void HandleEvent(Event* event) = 0;

};


class EventManager
{
public:


	//Registers client to specific event
	void AddClient(EventType eventid, Listener* client);

	//Checks if the client in question is already in the system. Public because of allowing it to be checked from outside the system if wanted. Might never be called in the public domain though
	bool IsRegistered(EventType eventid, Listener* client);

	//Unregisters client from a specific event
	void RemoveClientEvent(EventType eventid, Listener* client);

	//Unregisters client from all events
	void RemoveClientAllEvents(Listener* client);

	//Sends the event to anyone registered
	void SendEvent(Event* event);

	void AddEvent(EventType eventid, void* data = 0);

	//Process All events
	void ProcessEvents();

	//Clears all events from the buffer
	void ClearBuffer();

	//Clears all m_clients from the client list
	void ClearClients();

	//Shutdown event system
	void Shutdown();

	//Returns the event system
	static EventManager* Instance();
private:

	EventManager();
	~EventManager();

	//Contains all m_clients and their subscribed events, mapped in a way that we store the events and map m_clients to those, could be the other way around.
	std::multimap<EventType, Listener*> m_clients;
	std::map<EventType, std::vector<Listener*>> m_client;

	//List of events to process
	std::list<Event> m_eventBuffer;


};