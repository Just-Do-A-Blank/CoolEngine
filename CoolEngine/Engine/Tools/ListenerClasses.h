#pragma once
#include "EventManager.h"

/// <summary>
/// 
/// Header File contains all the listeners the system will utilise
/// Methods for listeners may need 
/// 
/// Listeners are being given the information from the event manager and will be told when to execute their code when the event they have "subscribed to" is called
/// 
/// </summary>



class Listener_Player_Enemy_Collision : public Listener
{
	void HandleEvent(Event* event);

	Listener_Player_Enemy_Collision();
};




class ListenerInherr : public Listener
{
public:
	void HandleEvent(Event* event);

	ListenerInherr(int count) { this->count = count; }

private:
	int count;

};

