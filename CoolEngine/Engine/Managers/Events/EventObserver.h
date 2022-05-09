#pragma once
class Event;

/// <summary>
/// 
/// Header File contains listeners
/// Methods for listeners may need 
/// 
/// Listeners are being given the information from the event manager and will be told when to execute their code when the event they have "subscribed to" is called
/// 
/// </summary>

class Observer
{
public:

	Observer() {}

	virtual ~Observer() {}

	virtual void Handle(Event* e) = 0;

};

