#pragma once
#include <map>
#include <list>
#include "Events.h"
#include <vector>
#include <functional>
#include "EventObserver.h"

///
/// To add to the event system with a listener								EventSystem::Instance()->AddClient(EVENTID::Event1, &listener);
/// To push an event into the system										EventSystem::Instance()->AddEvent(EVENTID::Event1, data); data is not needed but possible
/// In the update you need to execute the event buffer with					EventSystem::Instance()->ProcessEvents();
/// List of events stored in Event.h
/// To create new listener type new class inherrited from listener needed
/// 



class EventManager
{
public:
	
	//Registers client to specific event
	void AddClient(EventType eventid, Observer* client);

	//Checks if the client in question is already in the system. Public because of allowing it to be checked from outside the system if wanted. Might never be called in the public domain though
	bool IsRegistered(EventType eventid, Observer* client);

	//Unregisters client from a specific event
	void RemoveClientEvent(EventType eventid, Observer* client);

	//Unregisters client from all events
	void RemoveClientAllEvents(Observer* client);

	//Sends the event to anyone registered
	void SendEvent(Event* event);

	//Adds events to the event buffers
	void AddEvent(EventType eventid, void* data = 0);
	void AddEvent(Event* event);

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
	//std::multimap<EventType, Listener*> m_clients;
	//Leaving in the class listeners incase someone wants to beable to remove and add during runtime the function from the vector 
	std::map<EventType, std::vector<Observer*>> m_listenerClients;

	//List of events to process
	std::list<Event*> m_eventBuffer;





};